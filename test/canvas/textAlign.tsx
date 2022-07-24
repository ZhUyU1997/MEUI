/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D } from "@/meui"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://www.w3schools.com/tags/canvas_textalign.asp

    // Create a red line in position 150
    ctx.strokeStyle = "red"
    ctx.moveTo(150, 20)
    ctx.lineTo(150, 170)
    ctx.stroke()

    ctx.font = "15px Arial"
    // Show the different textAlign values
    ctx.textAlign = "start"
    ctx.fillText("textAlign=start", 150, 60)
    ctx.textAlign = "end"
    ctx.fillText("textAlign=end", 150, 80)
    ctx.textAlign = "left"
    ctx.fillText("textAlign=left", 150, 100)
    ctx.textAlign = "center"
    ctx.fillText("textAlign=center", 150, 120)
    ctx.textAlign = "right"
    ctx.fillText("textAlign=right", 150, 140)
}
