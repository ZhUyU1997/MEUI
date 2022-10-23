import esbuild from "rollup-plugin-esbuild"
import dts from "rollup-plugin-dts"
import resolve from "@rollup/plugin-node-resolve"
import commonjs from "@rollup/plugin-commonjs"
import alias from "@rollup/plugin-alias"
import { builtinModules } from "module"
import { defineConfig } from "rollup"
import pkg from "./package.json"

const entries = {
    index: "src/index.ts",
}

const external = [
    ...builtinModules,
    ...Object.keys(pkg.dependencies || {}),
    ...Object.keys(pkg.peerDependencies || {}),
    /node_modules/,
    "meui-native",
    "os",
    "std",
]

const plugins = [
    alias({
        entries: [{ find: /^node:(.+)$/, replacement: "$1" }],
    }),
    resolve({
        preferBuiltins: true,
    }),
    commonjs(),
    esbuild({
        target: "es2020",
        jsxImportSource: "@meui/preact",
        jsx: "automatic",
    }),
]

export default defineConfig([
    {
        input: entries,
        output: {
            dir: "dist",
            format: "esm",
            entryFileNames: "[name].mjs",
            chunkFileNames: "chunk-[name].mjs",
        },
        external,
        plugins,
        onwarn,
    },
    {
        input: entries,
        output: {
            dir: "dist",
            entryFileNames: "[name].d.ts",
            format: "esm",
        },
        external,
        plugins: [dts()],
        onwarn,
    },
])

function onwarn(message) {
    if (["EMPTY_BUNDLE", "CIRCULAR_DEPENDENCY"].includes(message.code)) return
    console.error(message)
}
