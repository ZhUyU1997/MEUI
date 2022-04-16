import React, { useEffect, useRef } from "react"
import { Canvas, MEUI, ReactMEUI } from "../../meui"
import { Root } from "../../components"
import { CanvasElement } from "../../meui/canvas"
import Test from "./getImageData"

function App() {
    const ref = useRef<CanvasElement>(null)

    useEffect(() => {
        if (!ref.current) return
        ref.current.width = 500
        ref.current.height = 500
        const ctx = ref.current.getContext("2d")
        if (!ctx) return

        Test(ctx)
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
