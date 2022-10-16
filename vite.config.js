import { defineConfig } from 'vite'
import { VitePWA } from 'vite-plugin-pwa'

// https://vitejs.dev/config/
export default defineConfig({
    base: '/MEUI/',
    plugins: [
        VitePWA({
            workbox: {
                globPatterns: ['**/*.{js,css,html,wasm,ttf}'],
                maximumFileSizeToCacheInBytes: 10000000,
            },
            registerType: 'autoUpdate',
        })
    ]
})