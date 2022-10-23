/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D } from "@meui/core"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/shadowOffsetY

    ctx.shadowColor = "black"
    ctx.shadowOffsetY = 10
    ctx.shadowBlur = 10

    ctx.fillStyle = "green"
    ctx.fillRect(10, 10, 100, 100)
}
