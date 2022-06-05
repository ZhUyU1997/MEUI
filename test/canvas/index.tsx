import React, { useEffect, useRef, useState } from "react"
import { Canvas, Div, MEUI } from "@/meui"
import ReactMEUI from "@/react-meui"

import { Root, Button } from "@/components"
import { CanvasElement } from "@/meui/canvas"

const Test: any = {
    arc: () => import("./arc"),
    arcTo: () => import("./arcTo"),
    beiginPath: () => import("./beiginPath"),
    bezierCurveTo: () => import("./bezierCurveTo"),
    createRadialGradient: () => import("./createRadialGradient"),
    createLinearGradient: () => import("./createLinearGradient"),
    clearRect: () => import("./clearRect"),
    "clip-path2d": () => import("./clip-path2d"),
    clip: () => import("./clip"),
    closePath: () => import("./closePath"),
    ellipse: () => import("./ellipse"),
    fill: () => import("./fill"),
    fillRect: () => import("./fillRect"),
    fillText: () => import("./fillText"),
    font: () => import("./font"),
    getImageData: () => import("./getImageData"),
    getTransform: () => import("./getTransform"),
    lineCap: () => import("./lineCap"),
    lineDashOffset: () => import("./lineDashOffset"),
    lineJoin: () => import("./lineJoin"),
    lineWidth: () => import("./lineWidth"),
    measureText: () => import("./measureText"),
    "path2d-svg": () => import("./path2d-svg"),
    path2d: () => import("./path2d"),
    putImageData: () => import("./putImageData"),
    quadraticCurveTo: () => import("./quadraticCurveTo"),
    resetTransform: () => import("./resetTransform"),
    rotate: () => import("./rotate"),
    save: () => import("./save"),
    scale: () => import("./scale"),
    setLineDash: () => import("./setLineDash"),
    setTransform: () => import("./setTransform"),
    shadowBlur: () => import("./shadowBlur"),
    shadowColor: () => import("./shadowColor"),
    shadowOffsetX: () => import("./shadowOffsetX"),
    shadowOffsetY: () => import("./shadowOffsetY"),
    strokeRect: () => import("./strokeRect"),
    strokeText: () => import("./strokeText"),
    textAlign: () => import("./textAlign"),
    textBaseline: () => import("./textBaseline"),
    transform: () => import("./transform"),
    translate: () => import("./translate"),
}

function App() {
    const ref = useRef<CanvasElement>(null)
    const [testName, setTestName] = useState("arc")
    useEffect(() => {
        if (!ref.current) return
        ref.current.width = 500
        ref.current.height = 500
    }, [])

    useEffect(() => {
        if (!ref.current) return
        const ctx = ref.current.getContext("2d")
        if (!ctx) return
        ctx.clearRect(0, 0, 500, 500)
        ctx.resetTransform()

        ctx.globalAlpha = 1.0
        ctx.globalCompositeOperation = "source-over"
        ctx.imageSmoothingEnabled = true
        ctx.imageSmoothingQuality = "medium"
        ctx.direction = "inherit"
        ctx.font = "10px sans-serif"
        ctx.textAlign = "start"
        ctx.textBaseline = "alphabetic"

        ctx.shadowBlur = 0
        ctx.shadowColor = "#00000000"
        ctx.shadowOffsetX = 0
        ctx.shadowOffsetY = 0
        ctx.lineCap = "butt"
        ctx.lineDashOffset = 0.0
        ctx.lineJoin = "miter"
        ctx.lineWidth = 1.0
        ctx.miterLimit = 10
        ctx.setLineDash([])

        ctx.fillStyle = "#000000"
        ctx.strokeStyle = "#000000"

        ctx.beginPath()
        Test[testName]()
            .then((mod: any) => {
                mod.default(ctx)
            })
            .catch((err: any) => {
                console.log(err)
            })
    }, [testName])
    return (
        <Root style={{ backgroundColor: "white" }}>
            <Div
                style={{
                    width: 600,
                    height: "100%",
                    // flexDirection: "column",
                    overflow: "scroll",
                    flexWrap: "wrap",
                }}
            >
                {Object.entries(Test).map(([key]) => (
                    <Button
                        key={key}
                        style={{
                            width: 200,
                            minHeight: 50,
                            maxHeight: 50,
                            flexShrink: 0,
                        }}
                        onClick={() => {
                            setTestName(key)
                        }}
                    >
                        {key}
                    </Button>
                ))}
            </Div>
            <Canvas
                ref={ref}
                style={{
                    width: 500,
                    height: 500,
                    backgroundColor: "yellow",
                }}
            ></Canvas>
        </Root>
    )
}

const meui = new MEUI(1100, 500)
ReactMEUI.render(<App />, meui)
