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
            const preactmeui = path.join(
                process.cwd(),
                "packages",
                "preact",
                "index.js"
            )

            build.onResolve({ filter: /^(@\/react-meui|react)$/ }, (args) => {
                if (args.path === "react") {
                    return { path: preact }
                } else args.path === "@/react-meui"
                {
                    return { path: preactmeui }
                }
            })
        },
    }
}

build({
    entryPoints: [process.argv[2] ?? "examples/hello/index.jsx"],
    // minify: true,
    minify: !isDev,
    // bundle: false,
    bundle: true,
    external: ["meui-native", "os", "std"],
    target: "es2020",
    outfile: process.argv[3] ?? "dist/index.js",
    format: "esm",
    inject: ["framework/polyfill.js"],
    watch: isDev
        ? {
              onRebuild(error, result) {
                  if (error) console.error("watch build failed:")
                  else console.log("watch build succeeded:")
              },
          }
        : false,
    plugins: [
        preactCompatPlugin(),
        execute(path.resolve("./meui"), [path.resolve("./dist/index.js")]),
    ],
})
