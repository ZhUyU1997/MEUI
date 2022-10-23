/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D, ImageData } from "@meui/core"

export default function Test(ctx: CanvasRenderingContext2D) {
    function ColorRect(color: number, width: number, height: number) {
        return new ImageData(
            new Uint8ClampedArray(
                new Uint32Array(width * height).fill(color).buffer
            ),
            width,
            height
        )
    }
    ctx.putImageData(ColorRect(0xff00ff00, 100, 100), 50, 50, 0, 0, 100, 100)
    ctx.putImageData(ColorRect(0xff0000ff, 100, 100), 50, 150, 50, 50, 100, 100)
    ctx.putImageData(ColorRect(0xffff0000, 200, 200), 50, 200, 50, 50, 100, 50)
}
