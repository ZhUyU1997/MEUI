import { build } from "esbuild"
import meuiPlugin from "@meui-js/esbuild-plugin-meui"
import path from "path"

const isDev = process.env.NODE_ENV === "development"

function preactCompatPlugin() {
    return {
        name: "preact-compat",
        setup(build) {
            const preact = path.join(
                process.cwd(),
                "node_modules",
                "preact",
                "compat",
                "dist",
                "compat.module.js"
            )

            build.onResolve({ filter: /^react$/ }, (args) => {
                if (args.path === "react") {
                    return { path: preact }
                }
            })
        },
    }
}

build({
    entryPoints: [process.argv[2] ?? "./src/index.tsx"],
    minify: !isDev,
    bundle: true,
    target: "es2020",
    outfile: process.argv[3] ?? "dist/index.js",
    format: "esm",
    watch: isDev,
    jsxImportSource: "@meui-js/preact",
    jsx: "transform",
    plugins: [meuiPlugin(), preactCompatPlugin()],
})
