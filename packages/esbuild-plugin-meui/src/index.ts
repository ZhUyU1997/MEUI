import { Plugin, PluginBuild } from "esbuild"
import { createRequire } from "module"

const require = createRequire(import.meta.url)
const polyfillPath = require.resolve("../polyfill")

export default function meuiPlugin(): Plugin {
    return {
        name: "meui-plugin",
        setup(build: PluginBuild) {
            const options = build.initialOptions
            const {
                inject: defaultInject = [],
                external: defaultExternal = [],
            } = options
            options.inject = [polyfillPath, ...defaultInject]
            options.external = ["meui-native", "os", "std", ...defaultExternal]
        },
    }
}
