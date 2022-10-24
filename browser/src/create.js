import createModule from "/gen/meui.js"
import Wasm from "/gen/meui.wasm?url"
// import Data from '/gen/meui.data?url'
import Droid from "/res/font/Droid-Sans-Fallback.ttf?url"
import MaterialDesignIconsDesktop from "/res/font/MaterialDesignIconsDesktop.ttf?url"

async function FetchFile(path) {
    const response = await fetch(path)
    const buffer = await response.arrayBuffer()
    return new Uint8Array(buffer)
}

export async function createMEUI({ canvas, input }) {
    if (!canvas) throw new Error("canvas == null")
    if (input == null) throw new Error("input == null")
    const Module = await createModule({
        canvas: canvas,
        locateFile: (path) => {
            if (path === "meui.wasm") return Wasm
            // if (path === "meui.data")
            //     return Data
            return null
        },
        keyboardListeningElement: canvas,
    })
    const { FS, callMain, abort } = Module

    FS.writeFile("index.js", input)
    FS.mkdir("/res")
    FS.mkdir("/res/font")
    FS.writeFile("res/font/Droid-Sans-Fallback.ttf", await FetchFile(Droid))
    FS.writeFile(
        "res/font/MaterialDesignIconsDesktop.ttf",
        await FetchFile(MaterialDesignIconsDesktop)
    )

    const js_cancel_main_loop = Module.cwrap("js_cancel_main_loop")
    callMain(["index.js"])

    return () => {
        js_cancel_main_loop()
    }
}
