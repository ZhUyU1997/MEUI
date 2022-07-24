/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D } from "@/meui"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/font

    ctx.font = "bold 48px serif"
    ctx.strokeText("Hello world", 50, 100)
}
