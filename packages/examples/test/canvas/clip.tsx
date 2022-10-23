/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D } from "@meui/core"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/clip

    // Create clipping region
    ctx.arc(100, 100, 75, 0, Math.PI * 2, false)
    ctx.clip()

    ctx.fillRect(0, 0, 100, 100)
}
