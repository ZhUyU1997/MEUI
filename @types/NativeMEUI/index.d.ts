/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

declare module "NativeMEUI" {
    export interface MeuiKeyboardRawEvent {
        type: "keydown" | "keyup"
        keyCode: number
        key: string
        altKey: boolean
        ctrlKey: boolean
        shiftKey: boolean
    }

    export interface MeuiMouseRawEvent {
        type: "mousedown" | "mouseup" | "mousemove"
        x: number
        y: number
        button: number
    }
    export interface MeuiWheelRawEvent {
        type: "mousewheel"
        deltaX: number
        deltaY: number
        deltaZ: number
        button: number
    }

    export interface MeuiUnloadRawEvent {
        type: "unload"
    }

    export type MeuiRawEvent =
        | MeuiKeyboardRawEvent
        | MeuiMouseRawEvent
        | MeuiWheelRawEvent
        | MeuiUnloadRawEvent

    export class NativeMEUI {
        constructor(width: number, height: number)
        render(root: Box): void
        flush(): void
        update(): void
        debug(): void
        addFontFace(fontFamily: string, path: string): void
        getConnectNumber(): number
        pending(): number
        nextEvent(): MeuiRawEvent | null
    }
    export class Box {
        constructor(type: string)
        getStyle(state: UI_STATE): Record<string, any>
        setStyle(style: Record<string, any>, state: UI_STATE): void
        getChildCount(): number
        addChild(child: Box): void
        insertChild(child: Box, index: number): void
        removeChild(child: Box): void
        setState(state: UI_STATE): void
        getState(): UI_STATE
        hit(x: number, y: number): boolean
        search(x: number, y: number): number[]
        toClient(x: number, y: number): [number, number]
        toOffset(x: number, y: number): [number, number]
        get scrollLeft(): number
        set scrollLeft(value: number)
        get scrollTop(): number
        set scrollTop(value: number)
        get scrollWidth(): number
        set scrollWidth(value: number)
        get scrollHeight(): number
        set scrollHeight(value: number)
        get clientWidth(): number
        set clientWidth(value: number)
        get clientHeight(): number
        set clientHeight(value: number)
    }
    export const BOX_STATE: {
        DEFAULT: 0
        FOCUS: 1
        ACTIVE: 2
        HOVER: 3
        SELECT: 4
        DISABLE: 5
        CHECK: 6
    }

    export type UI_STATE = typeof BOX_STATE[keyof typeof BOX_STATE]

    export class Path2D {
        constructor(path?: Path2D | string)

        arc(
            x: number,
            y: number,
            radius: number,
            startAngle: number,
            endAngle: number,
            counterclockwise?: boolean
        ): void
        arcTo(
            x1: number,
            y1: number,
            x2: number,
            y2: number,
            radius: number
        ): void
        bezierCurveTo(
            cp1x: number,
            cp1y: number,
            cp2x: number,
            cp2y: number,
            x: number,
            y: number
        ): void
        closePath(): void
        ellipse(
            x: number,
            y: number,
            radiusX: number,
            radiusY: number,
            rotation: number,
            startAngle: number,
            endAngle: number,
            counterclockwise?: boolean
        ): void
        lineTo(x: number, y: number): void
        moveTo(x: number, y: number): void
        quadraticCurveTo(cpx: number, cpy: number, x: number, y: number): void
        rect(x: number, y: number, w: number, h: number): void
        addPath(
            path: Path2D,
            transform?: [number, number, number, number, number, number]
        ): void
    }

    export class CanvasGradient {
        constructor(
            type: "linear",
            x1: number,
            y1: number,
            x2: number,
            y2: number
        )
        constructor(
            type: "radial",
            cx: number,
            cy: number,
            cr: number,
            fx: number,
            fy: number,
            fr: number
        )

        addColorStop(
            offset: number,
            r: number,
            g: number,
            b: number,
            a: number
        ): void
    }

    /** The dimensions of a piece of text in the canvas, as created by the CanvasRenderingContext2D.measureText() method. */
    export interface TextMetrics {
        /** Returns the measurement described below. */
        readonly actualBoundingBoxAscent: number
        /** Returns the measurement described below. */
        readonly actualBoundingBoxDescent: number
        /** Returns the measurement described below. */
        readonly actualBoundingBoxLeft: number
        /** Returns the measurement described below. */
        readonly actualBoundingBoxRight: number
        /** Returns the measurement described below. */
        readonly fontBoundingBoxAscent: number
        /** Returns the measurement described below. */
        readonly fontBoundingBoxDescent: number
        /** Returns the measurement described below. */
        readonly width: number
    }

    export const Canvas: {
        restore(): void
        save(): void

        getImage(
            sx: number,
            sy: number,
            sw: number,
            sh: number,
            settings?: ImageDataSettings
        ): ArrayBuffer
        putImage(
            buffer: ArrayBuffer,
            dx: number,
            dy: number,
            dirtyX: number,
            dirtyY: number,
            dirtyWidth: number,
            dirtyHeight: number,
            width: number,
            height: number,
            format?: number
        ): void
        getWidth(): number
        setWidth(width: number): void
        getHeight(): number
        setHeight(height: number): void
        getFit(): boolean
        setFit(fit: boolean): void

        beginPath(): void
        closePath(): void

        fillText(text: string, x: number, y: number, maxWidth?: number): void
        strokeText(text: string, x: number, y: number, maxWidth?: number): void
        measureText(text: string): TextMetrics

        setFontSize(fontSize: number): void
        setFontFamily(fontFamily: string): void
        setDirection(dir: "inherit" | "ltr" | "rtl"): void
        // setFont(font: string): void
        setTextAlign(
            textAlign: "center" | "end" | "left" | "right" | "start"
        ): void
        setTextBaseline(
            textBaseline:
                | "alphabetic"
                | "bottom"
                | "hanging"
                | "ideographic"
                | "middle"
                | "top"
        ): void

        arc(
            x: number,
            y: number,
            radius: number,
            startAngle: number,
            endAngle: number,
            counterclockwise?: boolean
        ): void
        arcTo(
            x1: number,
            y1: number,
            x2: number,
            y2: number,
            radius: number
        ): void
        bezierCurveTo(
            cp1x: number,
            cp1y: number,
            cp2x: number,
            cp2y: number,
            x: number,
            y: number
        ): void
        ellipse(
            x: number,
            y: number,
            radiusX: number,
            radiusY: number,
            rotation: number,
            startAngle: number,
            endAngle: number,
            counterclockwise?: boolean
        ): void
        lineTo(x: number, y: number): void
        moveTo(x: number, y: number): void

        quadraticCurveTo(cpx: number, cpy: number, x: number, y: number): void
        rect(x: number, y: number, w: number, h: number): void

        stroke(): void
        stroke(path: Path2D): void

        fill(fillRule?: "evenodd" | "nonzero"): void
        fill(path: Path2D, fillRule?: "evenodd" | "nonzero"): void
        clip(fillRule?: "evenodd" | "nonzero"): void
        clip(path: Path2D, fillRule?: "evenodd" | "nonzero"): void

        clearRect(x: number, y: number, w: number, h: number): void
        strokeRect(x: number, y: number, w: number, h: number): void
        fillRect(x: number, y: number, w: number, h: number): void

        setStrokeStyle(r: number, g: number, b: number, a: number): void
        setStrokeStyle(strokeStyle: CanvasGradient): void

        setFillStyle(r: number, g: number, b: number, a: number): void
        setFillStyle(strokeStyle: CanvasGradient): void

        setLineCap(lineCap: "butt" | "round" | "square"): void
        setLineJoin(lineJoin: "bevel" | "miter" | "round"): void
        setMiterLimit(miterLimit: number): void
        setLineWidth(lineWidth: number): void
        setLineDash(lineDashOffset: number, segments: number[]): void

        getTransform(): [number, number, number, number, number, number]
        resetTransform(): void
        rotate(angle: number): void
        scale(x: number, y: number): void
        setTransform(
            a: number,
            b: number,
            c: number,
            d: number,
            e: number,
            f: number
        ): void
        setTransform(
            transform?: [number, number, number, number, number, number]
        ): void
        setTransform(a?: any, b?: any, c?: any, d?: any, e?: any, f?: any): void
        transform(
            a: number,
            b: number,
            c: number,
            d: number,
            e: number,
            f: number
        ): void
        translate(x: number, y: number): void
    }
}
