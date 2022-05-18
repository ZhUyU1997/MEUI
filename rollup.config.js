
import commonjs from '@rollup/plugin-commonjs'
import resolve from '@rollup/plugin-node-resolve'
import { babel } from '@rollup/plugin-babel'
import typescript from 'rollup-plugin-typescript2'
import alias from '@rollup/plugin-alias'
import inject from '@rollup/plugin-inject'
import { terser } from "rollup-plugin-terser"
import replace from '@rollup/plugin-replace'
import path from 'path'
import execute from "./plugin/execute"

const NODE_ENV = process.env.NODE_ENV ?? "development"
const IS_DEV = NODE_ENV === "development"

export default commandLineArgs => {
	return {
		output: {
			format: 'esm',
			dir: "dist",
		},
		external: ['NativeMEUI', 'os', 'std'],
		plugins: [
			alias({
				entries: {
					'@': 'framework'
				}
			}),
			resolve({
				// browser: true,
				extensions: ['.mjs', '.js', '.json', '.node', '.jsx']
			}),
			commonjs({ include: 'node_modules/**', sourceMap: true }),
			babel({
				// presets: ['@babel/preset-react'],
				babelHelpers: 'bundled', exclude: ['node_modules/**']
			}),
			typescript(),
			replace({
				'process.env.NODE_ENV': JSON.stringify(NODE_ENV),
			}),

			inject({
				setTimeout: [path.resolve('framework/polyfill.js'), "setTimeout"],
				clearTimeout: [path.resolve('framework/polyfill.js'), "clearTimeout"],
				setInterval: [path.resolve('framework/polyfill.js'), "setInterval"],
				clearInterval: [path.resolve('framework/polyfill.js'), "clearInterval"],
				requestAnimationFrame: [path.resolve('framework/polyfill.js'), "requestAnimationFrame"],
				cancelAnimationFrame: [path.resolve('framework/polyfill.js'), "cancelAnimationFrame"],
				triggerAnimationFrame: [path.resolve('framework/polyfill.js'), "triggerAnimationFrame"],
				window: [path.resolve('framework/polyfill.js'), "window"],
				process: [path.resolve('framework/polyfill.js'), "process"],
			}),
			IS_DEV ? execute("./meui dist/index.js") : undefined,
			IS_DEV ? undefined : terser(),
		]
	}
}