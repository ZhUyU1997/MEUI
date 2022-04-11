import React, { useEffect, useRef } from "react"
import { Canvas, MEUI, ReactMEUI } from "../../meui"
import { Root } from "../../components"
import { CanvasElement, ImageData } from "../../meui/canvas"

function App() {
    const ref = useRef<CanvasElement>(null)

    useEffect(() => {
        if (!ref.current) return
        ref.current.width = 500
        ref.current.height = 500
        const ctx = ref.current.getContext("2d")
        if (!ctx) return

        function ColorRect(color: number, width: number, height: number) {
            return new ImageData(
                new Uint8ClampedArray(
                    new Uint32Array(width * height).fill(color).buffer
                ),
                width,
                height
            )
        }
        ctx.putImageData(ColorRect(0xff00ff00, 100, 100), 50, 50, 0, 0)
        ctx.putImageData(ColorRect(0xff0000ff, 100, 100), 50, 150, 50, 50)
        ctx.putImageData(
            ColorRect(0xffff0000, 200, 200),
            50,
            200,
            50,
            50,
            100,
            50
        )
    }, [])
    return (
        <Root style={{ backgroundColor: "white" }}>
            <Canvas ref={ref}></Canvas>
        </Root>
    )
}

export function Main() {
    const meui = new MEUI(500, 500)
    ReactMEUI.render(<App />, meui)
}
