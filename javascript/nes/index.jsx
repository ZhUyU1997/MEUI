import React, { useState, useEffect, useLayoutEffect, useRef } from "react"
import ReactMEUI from "../ReactMEUI"
import Button from "../button"
import { MEUI } from "../meui"
import { Root, Column, Center, MaterialDesignIcon } from "../components"
import jsnes from "jsnes"
import path from "../path"
import * as std from "std"

const SCREEN_WIDTH = 256;
const SCREEN_HEIGHT = 240;
const FRAMEBUFFER_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;


let framebuffer_u8, framebuffer_u32;
const nes = new jsnes.NES({
    onFrame: (framebuffer_24) => {
        framebuffer_u32 = framebuffer_24
    },
    onStatusUpdate: console.log,
    emulateSound: false,
    preferredFrameRate: 30,
});

function readFileToBuffer(file) {
    let f = std.open(file, "rb")
    f.seek(0, std.SEEK_END);
    let size = f.tell();
    f.seek(0, std.SEEK_SET);
    let buf = new Uint8Array(size);
    let ret = f.read(buf.buffer, 0, size);
    f.close()

    return buf.buffer
}

function nes_init(nesfile) {
    // Allocate framebuffer array.
    var buffer = new ArrayBuffer(FRAMEBUFFER_SIZE * 4);
    framebuffer_u8 = new Uint8ClampedArray(buffer);
    framebuffer_u32 = new Uint32Array(buffer);

    nes.loadROM(readFileToBuffer(nesfile))
}


function App() {
    const ref = useRef()
    useEffect(() => {
        nes_init(path.join(process.cwd(), "res/nes/mario.nes"))
        ref.current.width = SCREEN_WIDTH
        ref.current.height = SCREEN_HEIGHT

        function onAnimationFrame() {
            window.requestAnimationFrame(onAnimationFrame);
            nes.frame()
            // console.log(nes.getFPS())
            ref.current.putImage(framebuffer_u32.buffer, SCREEN_WIDTH, SCREEN_HEIGHT)
        }
        window.requestAnimationFrame(onAnimationFrame);
        return () => cancelAnimationFrame(onAnimationFrame);
    }, [])
    return <Root style={{ backgroundColor: "red" }}>
        <canvas ref={ref}></canvas>
    </Root >
}

export function Main() {
    const meui = new MEUI(SCREEN_WIDTH, SCREEN_HEIGHT)
    ReactMEUI.render(<App />, meui)
}