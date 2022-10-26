/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D } from "@meui-js/core"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/shadowColor

    ctx.shadowColor = "black"
    ctx.shadowOffsetY = 10
    ctx.shadowOffsetX = 10

    ctx.fillStyle = "green"
    ctx.fillRect(10, 10, 100, 100)
}
