import React, { useEffect, useRef } from "react"
import { Canvas, Div } from "@/meui"
import ReactMEUI from "@/react-meui"

import { MEUI } from "@/meui"
import { Root } from "@/components"

// @ts-ignore
import jsnes from "jsnes"
import path from "@/path"
import * as std from "std"
import { CanvasElement, COLOR_FORMAT, ImageData } from "@/meui/canvas"

const SCREEN_WIDTH = 256
const SCREEN_HEIGHT = 240

let framebuffer_u32: Uint32Array | null = null
const nes = new jsnes.NES({
    onFrame: (framebuffer_24: Uint32Array) => {
        framebuffer_u32 = framebuffer_24
    },
    onStatusUpdate: console.log,
    emulateSound: false,
    preferredFrameRate: 30,
})

function readFileToBuffer(file: string) {
    const f = std.open(file, "rb")
    f.seek(0, std.SEEK_END)
    const size = f.tell()
    f.seek(0, std.SEEK_SET)
    const buf = new Uint8Array(size)
    const ret = f.read(buf.buffer, 0, size)
    f.close()

    return buf.buffer
}

function nes_init(nesfile: string) {
    nes.loadROM(readFileToBuffer(nesfile))
}

function App() {
    const ref = useRef<CanvasElement>(null)
    useEffect(() => {
        nes_init(path.join(process.cwd(), "res/nes/mario.nes"))
        if (!ref.current) return
        ref.current.width = SCREEN_WIDTH
        ref.current.height = SCREEN_HEIGHT
        const ctx = ref.current.getContext("2d")

        function onAnimationFrame() {
            window.requestAnimationFrame(onAnimationFrame)
            nes.frame()
            if (framebuffer_u32 && ref.current && ctx) {
                ctx.putImageData(
                    new ImageData(
                        new Uint8ClampedArray(framebuffer_u32.buffer),
                        SCREEN_WIDTH,
                        SCREEN_HEIGHT
                    ),
                    0,
                    0,
                    0,
                    0,
                    SCREEN_WIDTH,
                    SCREEN_HEIGHT,
                    COLOR_FORMAT.COLOR_BGR
                )
            }
        }
        const id = requestAnimationFrame(onAnimationFrame)
        return () => cancelAnimationFrame(id)
    }, [])
    return (
        <Root style={{ backgroundColor: "red" }}>
            <Canvas ref={ref}></Canvas>
        </Root>
    )
}

const meui = new MEUI(SCREEN_WIDTH, SCREEN_HEIGHT)
ReactMEUI.render(<App />, meui)
