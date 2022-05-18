import { build } from "esbuild"
import { spawn } from 'child_process'
import kill from "tree-kill"

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

build({
    entryPoints: [process.argv[2] ?? 'examples/hello/index.jsx'],
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
    plugins: [execute("./meui dist/index.js")]
})