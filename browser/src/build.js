import { useCallback } from "react"
import { build, initialize } from "esbuild-wasm"
import WASM from "esbuild-wasm/esbuild.wasm?url"
import { useAsync, useDebouncedCallback } from "@react-hookz/web"
import * as Path from "pathe"
import MEUI_PREACT_RAW from "/node_modules/@meui-js/preact/dist/index.mjs?raw"
import MEUI_CORE_RAW from "/node_modules/@meui-js/core/dist/index.mjs?raw"
import PREACT_RAW from "/node_modules/preact/dist/preact.module.js?raw"
import MEUI_PREACT_JSX_RAW from "/node_modules/@meui-js/preact/jsx-runtime/index.js?raw"
import PREACT_JSX_RAW from "/node_modules/preact/jsx-runtime/dist/jsxRuntime.module.js?raw"
import POLYFILL_RAW from "/gen/polyfill.js?raw"

const module2PathMap = {
    "@meui-js/preact": {
        path: "/node_modules/@meui-js/preact/dist/index.mjs",
        content: MEUI_PREACT_RAW,
    },
    "@meui-js/core": {
        path: "/node_modules/@meui-js/core/dist/index.mjs",
        content: MEUI_CORE_RAW,
    },
    preact: {
        path: "/node_modules/preact/dist/preact.module.js",
        content: PREACT_RAW,
    },
    "@meui-js/preact/jsx-runtime": {
        path: "/node_modules/@meui-js/preact/jsx-runtime/index.js",
        content: MEUI_PREACT_JSX_RAW,
    },
    "preact/jsx-runtime": {
        path: "/node_modules/preact/jsx-runtime/dist/jsxRuntime.module.js",
        content: PREACT_JSX_RAW,
    },
    "polyfill.js": {
        path: "/polyfill.js",
        content: POLYFILL_RAW,
    },
}

async function GetFileContent(path) {
    const response = await fetch(path)
    const text = await response.text()
    return text
}

function onResolveCallback(args) {
    if (args.kind === "entry-point") {
        return { path: Path.join("/", args.path) }
    }
    if (args.kind === "import-statement") {
        if (["meui-native", "os", "std"].includes(args.path))
            return { external: true }

        return { path: module2PathMap[args.path].path }
    }
    throw Error("not resolvable")
}

async function onLoadCallback(args) {
    let contents = null

    const result = Object.values(module2PathMap).find(
        ({ path }) => path === args.path
    )

    if (result) {
        contents = result.content
    } else {
        console.log("Not found path:", args.path)
        return
    }

    const extname = Path.extname(args.path)

    const loader =
        extname === ".ts"
            ? "ts"
            : extname === ".tsx"
            ? "tsx"
            : extname === ".js"
            ? "js"
            : extname === ".jsx"
            ? "jsx"
            : "default"
    return { contents, loader }
}

const tsConfig = {
    compilerOptions: {
        target: "ESNext",
        useDefineForClassFields: true,
        lib: ["DOM", "DOM.Iterable", "ESNext"],
        allowJs: false,
        skipLibCheck: true,
        esModuleInterop: false,
        allowSyntheticDefaultImports: true,
        strict: true,
        forceConsistentCasingInFileNames: true,
        module: "ESNext",
        moduleResolution: "Node",
        resolveJsonModule: true,
        isolatedModules: true,
        noEmit: true,
        jsx: "react-jsx",
        jsxImportSource: "@meui-js/preact",
    },
    include: ["@meui-js", "./index.tsx"],
}

function MakeQuerablePromise(promise) {
    // Don't modify any promise that has been already modified.
    if (promise.isFulfilled) return promise

    // Set initial state
    var isPending = true
    var isRejected = false
    var isFulfilled = false

    // Observe the promise, saving the fulfillment in a closure scope.
    var result = promise.then(
        function (v) {
            console.log("isFulfilled")
            isFulfilled = true
            isPending = false
            return v
        },
        function (e) {
            console.log("isRejected")

            isRejected = true
            isPending = false
            throw e
        }
    )

    result.isFulfilled = function () {
        return isFulfilled
    }
    result.isPending = function () {
        return isPending
    }
    result.isRejected = function () {
        return isRejected
    }
    return result
}

const esbuildInitPromise = MakeQuerablePromise(
    initialize({
        wasmURL: WASM,
    })
)

async function Build(code) {
    if (esbuildInitPromise.isFulfilled() === false) {
        await esbuildInitPromise
    }
    console.log("start to exec esbuild")

    const config = {
        entryPoints: ["/index.tsx"],
        bundle: true,
        write: false,
        external: ["meui-native", "os", "std"],
        target: "es2020",
        format: "esm",
        jsxImportSource: "@meui-js/preact",
        jsx: "automatic",
        tsconfig: JSON.stringify(tsConfig),
        // inject: ["/polyfill.js"],
        plugins: [
            {
                name: "esbuild-wasm-meui",
                setup(build) {
                    build.onResolve({ filter: /.*/ }, (args) =>
                        onResolveCallback(args)
                    )
                    build.onLoad({ filter: /.*/ }, async (args) => {
                        if (args.path === "/index.tsx") {
                            const contents = `import "polyfill.js"\n${code}`
                            return { contents, loader: "tsx" }
                        }
                        return await onLoadCallback(args)
                    })
                },
            },
        ],
    }
    let result = null
    try {
        result = await build(config)
    } catch (error) {
        console.log(error)
    }

    const contents = result.outputFiles[0].contents
    const output = new TextDecoder().decode(contents)
    return output
}

export default function useESBuild() {
    const [state, actions] = useAsync(async (code) => Build(code))
    const execute = useDebouncedCallback(
        (code) => {
            if (state.status !== "loading" || state.status == "not-executed") {
                actions.reset()
                actions.execute(code)
            }
        },
        [actions, state.status],
        500
    )

    return [state, execute]
}
