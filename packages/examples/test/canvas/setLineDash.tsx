/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D } from "@meui-js/core"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/setLineDash

    // Dashed line
    ctx.beginPath()
    ctx.setLineDash([5, 15])
    ctx.moveTo(0, 50)
    ctx.lineTo(300, 50)
    ctx.stroke()

    // Solid line
    ctx.beginPath()
    ctx.setLineDash([])
    ctx.moveTo(0, 100)
    ctx.lineTo(300, 100)
    ctx.stroke()
}
