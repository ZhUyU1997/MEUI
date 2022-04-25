
import commonjs from '@rollup/plugin-commonjs'
import resolve from '@rollup/plugin-node-resolve'
import { babel } from '@rollup/plugin-babel'
import typescript from 'rollup-plugin-typescript2'
import alias from '@rollup/plugin-alias'
import inject from '@rollup/plugin-inject'
import path from 'path'

export default commandLineArgs => {
	const inputBase = commandLineArgs.input || 'examples/hello/index.jsx'
	return {
		input: [inputBase],
		output: {
			format: 'esm',
			dir: "dist",
		},
		external: ['NativeMEUI', 'os'],
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
		]
	}
}