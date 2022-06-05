import { build } from "esbuild"
import { spawn } from 'child_process'
import kill from "tree-kill"
import path from "path"

let childProcess = null

export function execute(command) {
    return {
        name: 'execute',
        setup(build) {
            build.onEnd(result => {
                console.log(`build ended with ${result.errors.length} errors`)
                if (childProcess) {
                    kill(childProcess.pid, 'SIGKILL')
                }
                console.log("[execute]", command)
                childProcess = spawn(command, {
                    shell: true,
                    stdio: 'inherit',
                    env: process.env
                })

                console.log("spawn", command, childProcess.pid)
            })
        },
    }
}

function preactCompatPlugin() {
    return {
        name: "preact-compat",
        setup(build) {
            const preact = path.join(process.cwd(), "node_modules", "preact", "compat", "dist", "compat.module.js");
            const preactmeui = path.join(process.cwd(), "framework", "preact-meui.js");

            build.onResolve({ filter: /^(@\/react-meui|react)$/ }, args => {
                if (args.path === "react") {
                    return { path: preact };
                }
                else (args.path === "@/react-meui")
                {
                    return { path: preactmeui };
                }
            });
        }
    }
}

build({
    entryPoints: [process.argv[2] ?? 'examples/hello/index.jsx'],
    // minify: true,
    minify: process.env.NODE_ENV === 'production',
    // bundle: false,
    bundle: true,
    external: ['NativeMEUI', 'os', 'std'],
    target: "es2020",
    outfile: 'dist/index.js',
    format: "esm",
    inject: ["framework/polyfill.js"],
    watch: {
        onRebuild(error, result) {
            if (error) console.error('watch build failed:')
            else console.log('watch build succeeded:')
        },
    },
    plugins: [preactCompatPlugin(), execute("./meui dist/index.js")]
})