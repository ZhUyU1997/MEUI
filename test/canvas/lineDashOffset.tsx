/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D } from "@/meui"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/lineDashOffset

    ctx.setLineDash([4, 16])
    ctx.lineDashOffset = 2

    ctx.beginPath()
    ctx.moveTo(0, 100)
    ctx.lineTo(400, 100)
    ctx.stroke()
}
