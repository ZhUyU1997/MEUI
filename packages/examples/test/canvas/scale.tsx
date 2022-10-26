/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D } from "@meui-js/core"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/scale

    // Scaled rectangle
    ctx.scale(9, 3)
    ctx.fillStyle = "red"
    ctx.fillRect(10, 10, 8, 20)

    // Reset current transformation matrix to the identity matrix
    ctx.setTransform(1, 0, 0, 1, 0, 0)

    // Non-scaled rectangle
    ctx.fillStyle = "gray"
    ctx.fillRect(10, 10, 8, 20)
}
