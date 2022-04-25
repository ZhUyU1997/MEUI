
import commonjs from '@rollup/plugin-commonjs'
import resolve from '@rollup/plugin-node-resolve'
import { babel } from '@rollup/plugin-babel'
import typescript from 'rollup-plugin-typescript2'

export default {
	input: 'framework/index.jsx',
	output: {
		format: 'esm',
		dir: "dist",
		entryFileNames: 'app.js'
	},
	external: ['NativeMEUI', 'os', 'renderer'],
	plugins: [
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
	]
}