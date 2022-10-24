import { defineConfig } from "vite"
import { VitePWA } from "vite-plugin-pwa"
import react from "@vitejs/plugin-react"

// https://vitejs.dev/config/
export default defineConfig({
    base: "/MEUI/",
    define: {
        __APP_VERSION__: JSON.stringify("v0.0.2"),
        __APP_BUILD_TIME__: Date.now(),
    },
    plugins: [
        react(),
        VitePWA({
            workbox: {
                globPatterns: ["**/*.{js,mjs,css,html,wasm,ttf,data}"],
                maximumFileSizeToCacheInBytes: 20000000,
            },
        }),
    ],
})
