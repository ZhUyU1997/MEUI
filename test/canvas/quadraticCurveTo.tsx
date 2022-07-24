/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D } from "@/meui"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/quadraticCurveTo

    // Quadratic Bézier curve
    ctx.beginPath()
    ctx.moveTo(50, 20)
    ctx.quadraticCurveTo(230, 30, 50, 100)
    ctx.stroke()

    // Start and end points
    ctx.fillStyle = "blue"
    ctx.beginPath()
    ctx.arc(50, 20, 5, 0, 2 * Math.PI) // Start point
    ctx.arc(50, 100, 5, 0, 2 * Math.PI) // End point
    ctx.fill()

    // Control point
    ctx.fillStyle = "red"
    ctx.beginPath()
    ctx.arc(230, 30, 5, 0, 2 * Math.PI)
    ctx.fill()
}
