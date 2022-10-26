import { useEffect, useState, useCallback, useRef } from "react"
import { getTime, getVersion } from "./util"
import Editor from "@monaco-editor/react"
import { build, initialize } from "esbuild-wasm"
import WASM from "esbuild-wasm/esbuild.wasm?url"
import { useAsync } from "@react-hookz/web"
import MEUI from "./MEUI"
import Loading from "./Loading"

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

const codeRaw = `import { createRoot, View } from "@meui-js/preact"

function App() {
    return (
        <View
            style={{
                width: 120,
                height: 120,
                backgroundColor: "#46e891",
                borderRadius: 16,
                textAlign: "center",
            }}
        >
            Hello MEUI!
        </View>
    )
}

createRoot(300, 300).render(<App />)`

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

let gIsReady = false
function useESBuild() {
    const [state, actions] = useAsync(async (code) => {
        if (gIsReady === false) {
            await initialize({
                wasmURL: WASM,
            })
            gIsReady = true
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
            tsconfig: `{
                    "compilerOptions": {
                        "target": "ESNext",
                        "useDefineForClassFields": true,
                        "lib": ["DOM", "DOM.Iterable", "ESNext"],
                        "allowJs": false,
                        "skipLibCheck": true,
                        "esModuleInterop": false,
                        "allowSyntheticDefaultImports": true,
                        "strict": true,
                        "forceConsistentCasingInFileNames": true,
                        "module": "ESNext",
                        "moduleResolution": "Node",
                        "resolveJsonModule": true,
                        "isolatedModules": true,
                        "noEmit": true,
                        "jsx": "react-jsx",
                        "jsxImportSource": "@meui-js/preact"
                    },
                    "include": ["@meui", "./index.tsx"]
                }`,
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
    })

    const execute = useCallback(
        (code) => {
            if (state.status !== "loading" || state.status == "not-executed") {
                actions.reset()
                actions.execute(code)
            }
        },
        [actions, state.status]
    )

    return [state, execute]
}

function Alert({ status, style }) {
    const colorMap = {
        loading: {
            color: "rgb(30, 70, 32)",
            borderColor: "rgb(76, 175, 80)",
        },
        success: {
            color: "rgb(30, 70, 32)",
            borderColor: "rgb(76, 175, 80)",
        },
        error: {
            color: "rgb(95, 33, 32)",
            borderColor: "rgb(239, 83, 80)",
        },
        "not-executed": {
            color: "rgb(30, 70, 32)",
            borderColor: "rgb(76, 175, 80)",
        },
    }
    return (
        <div
            style={{
                borderRadius: 4,
                marginBottom: 10,
                paddingLeft: 16,
                paddingTop: 6,
                paddingRight: 16,
                paddingBottom: 6,
                borderStyle: "solid",
                borderWidth: 1,
                fontSize: "15pt",
                ...colorMap[status],
                ...style,
            }}
        >
            {status}
        </div>
    )
}

import { useWindowSize } from "@react-hookz/web"
import NiceModal from "@ebay/nice-modal-react"
import { useRegisterSW } from "virtual:pwa-register/react"
import { UpdatePopup } from "./Popup"

function useUpdatePrompt() {
    /// https://vite-plugin-pwa.netlify.app/frameworks/react.html#prompt-for-update
    const {
        offlineReady: [offlineReady, setOfflineReady],
        needRefresh: [needRefresh, setNeedRefresh],
        updateServiceWorker,
    } = useRegisterSW({
        onRegistered(r) {
            // eslint-disable-next-line prefer-template
            console.log("SW Registered: " + r)
        },
        onRegisterError(error) {
            console.log("SW registration error", error)
        },
    })

    useEffect(() => {
        if (needRefresh) {
            NiceModal.show(UpdatePopup).then(() => {
                updateServiceWorker(true)
            })
        }
    }, [needRefresh, updateServiceWorker])
}

export default function App() {
    useUpdatePrompt()
    const [state, execute] = useESBuild()
    const { width, height } = useWindowSize()
    const isPortrait = width < height
    console.log(width, height)

    useEffect(() => {
        execute(codeRaw)
        // eslint-disable-next-line react-hooks/exhaustive-deps
    }, [])
    return (
        <div
            style={{
                display: "flex",
                width: "100%",
                height: "100%",
                flexDirection: "column",
                justifyContent: "center",
                alignItems: "center",
                backgroundColor: "#F3F6F9",
                paddingLeft: isPortrait ? 20 : "10%",
                paddingRight: isPortrait ? 20 : "10%",
                paddingTop: 20,
                boxSizing: "border-box",
            }}
        >
            <div
                style={{
                    width: "min(100%, 1500px)",
                    display: "flex",
                    flexDirection: "column",
                    justifyContent: "center",
                    alignItems: "center",
                    flexGrow: 1,
                }}
            >
                <Alert
                    status={state.status}
                    style={{
                        alignSelf: "flex-start",
                    }}
                ></Alert>
                <div
                    style={{
                        width: "100%",
                        flexGrow: 1,
                        borderRadius: 4,
                        overflow: "hidden",
                        backgroundColor: "white",
                        boxSizing: "border-box",
                        display: "flex",
                        flexDirection: isPortrait ? "column" : "row",
                        padding: 10,
                        boxShadow:
                            "0px 2px 1px -1px rgb(0 0 0 / 20%), 0px 1px 1px 0px rgb(0 0 0 / 14%), 0px 1px 3px 0px rgb(0 0 0 / 12%)",
                    }}
                >
                    <Editor
                        language="javascript"
                        value={codeRaw}
                        loading={
                            <Loading
                                style={{
                                    backgroundColor: "#c0c0c0",
                                }}
                            ></Loading>
                        }
                        onChange={(value) => {
                            execute(value)
                        }}
                        {...{
                            height: isPortrait ? "50%" : "100%",
                            width: isPortrait ? "100%" : "50%",
                            options: {
                                lineNumbers: isPortrait ? "off" : "on",
                                scrollBeyondLastLine: false,
                                minimap: { enabled: false },
                                fontSize: isPortrait ? "10pt" : "20pt",
                                contextmenu: false,
                                automaticLayout: true,
                            },
                        }}
                    ></Editor>
                    <div
                        style={{
                            height: isPortrait ? "50%" : "100%",
                            width: isPortrait ? "100%" : "50%",
                            display: "flex",
                            justifyContent: "center",
                            alignItems: "center",
                            backgroundColor: "#606060",
                        }}
                    >
                        <MEUI input={state.result}></MEUI>
                    </div>
                </div>
            </div>

            <div
                style={{
                    marginTop: 10,
                    marginBottom: 10,
                    fontSize: "14pt",
                    height: "30pt",
                    color: "#3E5060",
                }}
            >
                MEUI {getVersion()} {getTime()}
            </div>
        </div>
    )
}
