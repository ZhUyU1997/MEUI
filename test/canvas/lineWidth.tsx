/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D } from "@/meui"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/lineWidth

    ctx.lineWidth = 15

    ctx.beginPath()
    ctx.moveTo(20, 20)
    ctx.lineTo(130, 130)
    ctx.rect(40, 40, 70, 70)
    ctx.stroke()
}
