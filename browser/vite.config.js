import { defineConfig } from "vite"
import { VitePWA } from "vite-plugin-pwa"

// https://vitejs.dev/config/
export default defineConfig({
    base: "/MEUI/",
    define: {
        __APP_VERSION__: JSON.stringify("v0.0.1"),
        __APP_BUILD_TIME__: Date.now(),
    },
    plugins: [
        VitePWA({
            workbox: {
                globPatterns: ["**/*.{js,css,html,wasm,ttf,data}"],
                maximumFileSizeToCacheInBytes: 10000000,
            },
            registerType: "autoUpdate",
        }),
    ],
})
