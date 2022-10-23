/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { build } from "esbuild"
import { spawn } from "child_process"
import kill from "tree-kill"
import path from "path"

const isDev = process.env.NODE_ENV === "development"

let childProcess = null

export function execute(command, args) {
    return {
        name: "execute",
        setup(build) {
            if (!isDev) return
            build.onEnd((result) => {
                console.log(`build ended with ${result.errors.length} errors`)
                if (childProcess) {
                    kill(childProcess.pid, "SIGKILL")
                }
                console.log("[execute]", command)
                childProcess = spawn(command, args, {
                    shell: true,
                    stdio: "inherit",
                    env: process.env,
                })

                console.log("spawn", command, args, childProcess.pid)
            })
        },
    }
}

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
    entryPoints: [process.argv[2] ?? "./src/hello/index.jsx"],
    // minify: true,
    minify: !isDev,
    // bundle: false,
    bundle: true,
    // resolveExtensions: ['.mjs', '.cjs'],
    external: ["meui-native", "os", "std"],
    // preserveSymlinks: true,
    target: "es2020",
    outfile: process.argv[3] ?? "dist/index.js",
    format: "esm",
    inject: ["../../framework/polyfill.js"],
    watch: isDev,
    jsxImportSource: "@meui/preact",
    jsx: "transform",
    plugins: [preactCompatPlugin()],
})
