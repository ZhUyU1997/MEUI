/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D } from "@meui/core"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/textBaseline

    const baselines = [
        "top",
        "hanging",
        "middle",
        "alphabetic",
        "ideographic",
        "bottom",
    ]
    ctx.font = "36px serif"
    ctx.strokeStyle = "red"

    baselines.forEach(function (baseline, index) {
        ctx.textBaseline = baseline as CanvasTextBaseline
        const y = 75 + index * 75
        ctx.beginPath()
        ctx.moveTo(0, y + 0.5)
        ctx.lineTo(550, y + 0.5)
        ctx.stroke()
        ctx.fillText("┃ぬabc中国kⅧ (" + baseline + ")", 0, y)
    })
}
