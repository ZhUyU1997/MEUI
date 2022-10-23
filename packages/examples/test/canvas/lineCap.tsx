/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D } from "@meui/core"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/lineCap

    ctx.beginPath()
    ctx.moveTo(0, 0)
    ctx.lineWidth = 15
    ctx.lineCap = "round"
    ctx.lineTo(100, 100)
    ctx.stroke()
}
