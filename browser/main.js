import createModule from "./meui.js";
import input from './test.js?raw'
import Droid from '/res/font/Droid-Sans-Fallback.ttf?url'
import MaterialDesignIconsDesktop from '/res/font/MaterialDesignIconsDesktop.ttf?url'
import Wasm from './meui.wasm?url'

async function FetchFile(path) {
    const response = await fetch(path)
    const buffer = await response.arrayBuffer()
    return new Uint8Array(buffer)
}
async function Main() {
    const Module = await createModule({
        canvas: document.getElementById('canvas'),
        locateFile: (path) => {
            if (path === "meui.wasm")
                return Wasm
            return null
        }
    })
    const { FS, callMain } = Module

    FS.writeFile('index.js', input)
    FS.mkdir('/res');
    FS.mkdir('/res/font');
    FS.writeFile('res/font/Droid-Sans-Fallback.ttf', await FetchFile(Droid))
    FS.writeFile('res/font/MaterialDesignIconsDesktop.ttf', await FetchFile(MaterialDesignIconsDesktop))

    callMain(['index.js'])
}
Main()
