/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasGradient as NativeCanvasGradient } from "NativeMEUI"
import * as colorString from "color-string"

/** An opaque object describing a gradient. It is returned by the methods CanvasRenderingContext2D.createLinearGradient() or CanvasRenderingContext2D.createRadialGradient(). */
export class CanvasGradient {
    public native: NativeCanvasGradient
    constructor(startAngle: number, x: number, y: number)
    constructor(x0: number, y0: number, x1: number, y1: number)
    constructor(
        x0: number,
        y0: number,
        r0: number,
        x1: number,
        y1: number,
        r1: number
    )
    constructor(...args: any[]) {
        if (args.length === 3) {
            const [startAngle, x, y] = args
            throw new Error("Not support ConicGradient")
        } else if (args.length === 4) {
            const [x0, y0, x1, y1] = args
            this.native = new NativeCanvasGradient("linear", x0, y0, x1, y1)
        } else if (args.length === 6) {
            const [x0, y0, r0, x1, y1, r1] = args
            this.native = new NativeCanvasGradient(
                "radial",
                x0,
                y0,
                r0,
                x1,
                y1,
                r1
            )
        } else {
            throw new Error("Invalid arguments count")
        }
    }
    /**
     * Adds a color stop with the given color to the gradient at the given offset. 0.0 is the offset at one end of the gradient, 1.0 is the offset at the other end.
     *
     * Throws an "IndexSizeError" DOMException if the offset is out of range. Throws a "SyntaxError" DOMException if the color cannot be parsed.
     */
    addColorStop(offset: number, color: string): void {
        const [r, g, b, a] = colorString.get.rgb(color)
        this.native.addColorStop(offset, r / 255.0, g / 255.0, b / 255.0, a)
    }
}
