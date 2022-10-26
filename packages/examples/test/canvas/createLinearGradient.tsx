/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D, Path2D } from "@meui-js/core"

export default function Test(ctx: CanvasRenderingContext2D) {
    // Create a linear gradient
    // The start gradient point is at x=20, y=0
    // The end gradient point is at x=220, y=0
    const gradient = ctx.createLinearGradient(20, 0, 220, 0)

    // Add three color stops
    gradient.addColorStop(0, "green")
    gradient.addColorStop(0.5, "cyan")
    gradient.addColorStop(1, "green")

    // Set the fill style and draw a rectangle
    ctx.fillStyle = gradient
    ctx.fillRect(20, 20, 200, 100)
}
