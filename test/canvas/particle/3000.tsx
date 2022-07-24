/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useEffect, useRef } from "react"
import { Canvas, MEUI } from "@/meui"
import ReactMEUI from "@/react-meui"

import { Root } from "@/components"
import { CanvasElement } from "@/meui"

const ROWS = 100
const COLS = 300
const MARGIN = 100
const SPACING = 3

const SCREEN_WIDTH = COLS * SPACING + MARGIN * 2
const SCREEN_HEIGHT = ROWS * SPACING + MARGIN * 2

const NUM_PARTICLES = ROWS * COLS
const THICKNESS = Math.pow(80, 2)
const COLOR = 0xffffffff
const DRAG = 0.95
const EASE = 0.25

class Particle {
    vx: number
    vy: number
    x: number
    y: number
    ox: number
    oy: number
    constructor(x: number, y: number) {
        this.x = this.ox = x
        this.y = this.oy = y
        this.vx = 0
        this.vy = 0
    }
}

function App() {
    const ref = useRef<CanvasElement>(null)

    useEffect(() => {
        const canvas = ref.current
        if (!canvas) return
        canvas.width = 500
        canvas.height = 500
        const ctx = canvas.getContext("2d")
        if (!ctx) return

        canvas.width = SCREEN_WIDTH
        canvas.height = SCREEN_HEIGHT

        const list: Particle[] = []

        let mx = 0
        let my = 0

        for (let i = 0; i < NUM_PARTICLES; i++) {
            list[i] = new Particle(
                MARGIN + SPACING * (i % COLS),
                MARGIN + SPACING * Math.floor(i / COLS)
            )
        }

        ref.current.addEventListener("mousemove", function (e) {
            // bounds = container.getBoundingClientRect()
            mx = e.clientX
            my = e.clientY
            // console.log(mx, my)
        })

        let tog = true

        function step() {
            requestAnimationFrame(step)

            if (!ctx) return

            if ((tog = !tog)) {
                for (let i = 0; i < NUM_PARTICLES; i++) {
                    const p = list[i]
                    const dx = mx - p.x
                    const dy = my - p.y
                    const d = dx * dx + dy * dy
                    const f = -THICKNESS / d

                    if (d < THICKNESS) {
                        const t = Math.atan2(dy, dx)
                        p.vx += f * Math.cos(t)
                        p.vy += f * Math.sin(t)
                    }

                    p.x += (p.vx *= DRAG) + (p.ox - p.x) * EASE
                    p.y += (p.vy *= DRAG) + (p.oy - p.y) * EASE
                }
            } else {
                const data = ctx.createImageData(SCREEN_WIDTH, SCREEN_HEIGHT)
                const buf = new Uint32Array(data.data.buffer)

                for (let i = 0; i < NUM_PARTICLES; i++) {
                    const p = list[i]
                    const n: number =
                        Math.round(p.x) + Math.round(p.y) * SCREEN_WIDTH

                    if (n < SCREEN_WIDTH * SCREEN_HEIGHT && n >= 0) {
                        buf[n] = COLOR
                    }
                }

                ctx.putImageData(data, 0, 0)
            }
        }
        requestAnimationFrame(step)
    }, [])
    return (
        <Root style={{ backgroundColor: "black" }}>
            <Canvas ref={ref}></Canvas>
        </Root>
    )
}

const meui = new MEUI(SCREEN_WIDTH, SCREEN_HEIGHT)
ReactMEUI.render(<App />, meui)
