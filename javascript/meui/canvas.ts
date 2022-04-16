import { Canvas as NativeCanvas, Path2D as NativePath2D } from "NativeMEUI"
import { Box, Matrix2D } from "./box"
import { MeuiStyle } from "./style"
import * as colorString from "color-string"

export enum COLOR_FORMAT {
    COLOR_RGBA = 0,
    COLOR_RGB = 1,
    COLOR_BGRA = 2,
    COLOR_BGR = 3,
}

/** The underlying pixel data of an area of a <canvas> element. It is created using the ImageData() constructor or creator methods on the CanvasRenderingContext2D object associated with a canvas: createImageData() and getImageData(). It can also be used to set a part of the canvas by using putImageData(). */
export class ImageData {
    /** Returns the one-dimensional array containing the data in RGBA order, as integers in the range 0 to 255. */
    readonly data: Uint8ClampedArray
    /** Returns the actual dimensions of the data in the ImageData object, in pixels. */
    readonly height: number
    /** Returns the actual dimensions of the data in the ImageData object, in pixels. */
    readonly width: number

    constructor(sw: number, sh: number, settings?: ImageDataSettings)
    constructor(
        data: Uint8ClampedArray,
        sw: number,
        sh?: number,
        settings?: ImageDataSettings
    )
    constructor(...args: any[]) {
        if (args.length < 2) {
            throw new TypeError(`
              Failed to construct 'ImageData': 2 arguments required, but only ${args.length} present.
            `)
        }
        if (args.length > 2 && typeof args[0] !== "number") {
            this.data = args.shift()

            if (!(this.data instanceof Uint8ClampedArray)) {
                throw new TypeError(`
                Failed to construct 'ImageData': parameter 1 is not of type 'Uint8ClampedArray'
              `)
            }

            if (this.data.length !== 4 * args[0] * args[1]) {
                throw new Error(`
                Failed to construct 'ImageData': The input data byte length is not a multiple of (4 * width * height)
              `)
            }
        }

        this.width = args[0]
        this.height = args[1]

        this.data ??= new Uint8ClampedArray(
            new ArrayBuffer(4 * args[0] * args[1])
        )
    }
}

interface CanvasCompositing {
    globalAlpha: number
    globalCompositeOperation: GlobalCompositeOperation
}

interface CanvasDrawImage {
    drawImage(image: CanvasImageSource, dx: number, dy: number): void
    drawImage(
        image: CanvasImageSource,
        dx: number,
        dy: number,
        dw: number,
        dh: number
    ): void
    drawImage(
        image: CanvasImageSource,
        sx: number,
        sy: number,
        sw: number,
        sh: number,
        dx: number,
        dy: number,
        dw: number,
        dh: number
    ): void
}

/** This Canvas 2D API interface is used to declare a path that can then be used on a CanvasRenderingContext2D object. The path methods of the CanvasRenderingContext2D interface are also present on this interface, which gives you the convenience of being able to retain and replay your path whenever desired. */
export class Path2D implements CanvasPath {
    native: NativePath2D
    constructor(path?: Path2D | string) {
        if (!path) {
            this.native = new NativePath2D()
        } else if (path instanceof Path2D) {
            this.native = new NativePath2D((path as Path2D).native)
        } else if (typeof path === "string") {
            this.native = new NativePath2D(path)
        } else {
            throw new Error("Invalid paramter type")
        }
    }
    arc(
        x: number,
        y: number,
        radius: number,
        startAngle: number,
        endAngle: number,
        counterclockwise?: boolean
    ): void {
        this.native.arc(x, y, radius, startAngle, endAngle, counterclockwise)
    }
    arcTo(
        x1: number,
        y1: number,
        x2: number,
        y2: number,
        radius: number
    ): void {
        this.native.arcTo(x1, y1, x2, y2, radius)
    }
    bezierCurveTo(
        cp1x: number,
        cp1y: number,
        cp2x: number,
        cp2y: number,
        x: number,
        y: number
    ): void {
        this.native.bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y)
    }
    closePath(): void {
        this.native.closePath()
    }
    ellipse(
        x: number,
        y: number,
        radiusX: number,
        radiusY: number,
        rotation: number,
        startAngle: number,
        endAngle: number,
        counterclockwise?: boolean
    ): void {
        this.native.ellipse(
            x,
            y,
            radiusX,
            radiusY,
            rotation,
            startAngle,
            endAngle,
            counterclockwise
        )
    }
    lineTo(x: number, y: number): void {
        this.native.lineTo(x, y)
    }
    moveTo(x: number, y: number): void {
        this.native.moveTo(x, y)
    }
    quadraticCurveTo(cpx: number, cpy: number, x: number, y: number): void {
        this.native.quadraticCurveTo(cpx, cpy, x, y)
    }
    rect(x: number, y: number, w: number, h: number): void {
        this.native.rect(x, y, w, h)
    }
    /** Adds to the path the path given by the argument. */
    addPath(path: Path2D, transform?: DOMMatrix2DInit): void {}
}

interface CanvasDrawPath {
    beginPath(): void
    clip(fillRule?: CanvasFillRule): void
    clip(path: Path2D, fillRule?: CanvasFillRule): void
    fill(fillRule?: CanvasFillRule): void
    fill(path: Path2D, fillRule?: CanvasFillRule): void
    isPointInPath(x: number, y: number, fillRule?: CanvasFillRule): boolean
    isPointInPath(
        path: Path2D,
        x: number,
        y: number,
        fillRule?: CanvasFillRule
    ): boolean
    isPointInStroke(x: number, y: number): boolean
    isPointInStroke(path: Path2D, x: number, y: number): boolean
    stroke(): void
    stroke(path: Path2D): void
}

interface CanvasFillStrokeStyles {
    fillStyle: string | CanvasGradient | CanvasPattern
    strokeStyle: string | CanvasGradient | CanvasPattern
    createConicGradient(
        startAngle: number,
        x: number,
        y: number
    ): CanvasGradient
    createLinearGradient(
        x0: number,
        y0: number,
        x1: number,
        y1: number
    ): CanvasGradient
    createPattern(
        image: CanvasImageSource,
        repetition: string | null
    ): CanvasPattern | null
    createRadialGradient(
        x0: number,
        y0: number,
        r0: number,
        x1: number,
        y1: number,
        r1: number
    ): CanvasGradient
}

interface CanvasFilters {
    filter: string
}

/** An opaque object describing a gradient. It is returned by the methods CanvasRenderingContext2D.createLinearGradient() or CanvasRenderingContext2D.createRadialGradient(). */
class CanvasGradient {
    constructor() {}
    /**
     * Adds a color stop with the given color to the gradient at the given offset. 0.0 is the offset at one end of the gradient, 1.0 is the offset at the other end.
     *
     * Throws an "IndexSizeError" DOMException if the offset is out of range. Throws a "SyntaxError" DOMException if the color cannot be parsed.
     */
    addColorStop(offset: number, color: string): void {}
}

interface CanvasImageData {
    createImageData(
        sw: number,
        sh: number,
        settings?: ImageDataSettings
    ): ImageData
    createImageData(imagedata: ImageData): ImageData
    getImageData(
        sx: number,
        sy: number,
        sw: number,
        sh: number,
        settings?: ImageDataSettings
    ): ImageData
    putImageData(imagedata: ImageData, dx: number, dy: number): void
    putImageData(
        imagedata: ImageData,
        dx: number,
        dy: number,
        dirtyX: number,
        dirtyY: number,
        dirtyWidth: number,
        dirtyHeight: number
    ): void
}

interface CanvasImageSmoothing {
    imageSmoothingEnabled: boolean
    imageSmoothingQuality: ImageSmoothingQuality
}

interface CanvasPath {
    arc(
        x: number,
        y: number,
        radius: number,
        startAngle: number,
        endAngle: number,
        counterclockwise?: boolean
    ): void
    arcTo(x1: number, y1: number, x2: number, y2: number, radius: number): void
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
}

interface CanvasPathDrawingStyles {
    lineCap: CanvasLineCap
    lineDashOffset: number
    lineJoin: CanvasLineJoin
    lineWidth: number
    miterLimit: number
    getLineDash(): number[]
    setLineDash(segments: number[]): void
}

/** An opaque object describing a pattern, based on an image, a canvas, or a video, created by the CanvasRenderingContext2D.createPattern() method. */
class CanvasPattern {
    /** Sets the transformation matrix that will be used when rendering the pattern during a fill or stroke painting operation. */
    constructor() {}
    setTransform(transform?: DOMMatrix2DInit): void {}
}

interface CanvasRect {
    clearRect(x: number, y: number, w: number, h: number): void
    fillRect(x: number, y: number, w: number, h: number): void
    strokeRect(x: number, y: number, w: number, h: number): void
}

interface CanvasShadowStyles {
    shadowBlur: number
    shadowColor: string
    shadowOffsetX: number
    shadowOffsetY: number
}

interface CanvasState {
    restore(): void
    save(): void
}

interface CanvasText {
    fillText(text: string, x: number, y: number, maxWidth?: number): void
    measureText(text: string): TextMetrics
    strokeText(text: string, x: number, y: number, maxWidth?: number): void
}

interface CanvasTextDrawingStyles {
    direction: CanvasDirection
    font: string
    textAlign: CanvasTextAlign
    textBaseline: CanvasTextBaseline
}

interface CanvasTransform {
    // getTransform(): DOMMatrix
    // Not follow standard
    getTransform(): Matrix2D
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
    // setTransform(transform?: DOMMatrix2DInit): void
    // Not follow standard
    setTransform(transform?: Matrix2D): void

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

interface CanvasUserInterface {
    drawFocusIfNeeded(element: Element): void
    drawFocusIfNeeded(path: Path2D, element: Element): void
}

export class CanvasRenderingContext2D
    implements
        CanvasDrawImage,
        CanvasTransform,
        CanvasGradient,
        CanvasText,
        CanvasShadowStyles,
        CanvasPathDrawingStyles,
        CanvasRect,
        CanvasFillStrokeStyles,
        CanvasDrawPath,
        CanvasPath,
        CanvasImageData
{
    contextType: string
    contextAttributes?: Record<string, string>
    canvas: CanvasElement

    _fillStyle: string | CanvasGradient | CanvasPattern
    _strokeStyle: string | CanvasGradient | CanvasPattern

    private _beginPath: () => void
    private _closePath: () => void

    private _moveTo: (x: number, y: number) => void
    private _lineTo: (x: number, y: number) => void
    private _bezierCurveTo: (
        cp1x: number,
        cp1y: number,
        cp2x: number,
        cp2y: number,
        x: number,
        y: number
    ) => void
    private _quadraticCurveTo: (
        cpx: number,
        cpy: number,
        x: number,
        y: number
    ) => void

    private _setStrokeStyle: (
        r: number,
        g: number,
        b: number,
        a: number
    ) => void
    private _setFillStyle: (r: number, g: number, b: number, a: number) => void
    private _strokeRect: (x: number, y: number, w: number, h: number) => void
    private _fillRect: (x: number, y: number, w: number, h: number) => void
    private _clearRect: (x: number, y: number, w: number, h: number) => void

    private _rect: (x: number, y: number, w: number, h: number) => void

    private _ellipse: (
        x: number,
        y: number,
        radiusX: number,
        radiusY: number,
        rotation: number,
        startAngle: number,
        endAngle: number,
        counterclockwise?:
            | boolean
            | undefined /** The underlying pixel data of an area of a <canvas> element. It is created using the ImageData() constructor or creator methods on the CanvasRenderingContext2D object associated with a canvas: createImageData() and getImageData(). It can also be used to set a part of the canvas by using putImageData(). */
    ) => void

    private _arc: (
        x: number,
        y: number,
        radius: number,
        startAngle: number,
        endAngle: number,
        counterclockwise?: boolean | undefined
    ) => void
    private _arcTo: (
        x1: number,
        y1: number,
        x2: number,
        y2: number,
        radius: number
    ) => void
    private _setLineCap: (lineCap: "butt" | "round" | "square") => void
    private _setLineJoin: (lineJoin: "round" | "bevel" | "miter") => void
    private _setLineWidth: (lineWidth: number) => void
    private _setMiterLimit: (miterLimit: number) => void
    private _setLineDash: (lineDashOffset: number, segments: number[]) => void

    private _lineCap: CanvasLineCap
    private _lineDashOffset: number
    private _lineJoin: CanvasLineJoin
    private _lineWidth: number
    private _miterLimit: number
    private _lineDashSegments: number[]

    shadowBlur: number
    shadowColor: string
    shadowOffsetX: number
    shadowOffsetY: number
    private _stroke: { (): void; (path: NativePath2D): void }
    private _fill: {
        (fillRule?: "evenodd" | "nonzero" | undefined): void
        (path: NativePath2D, fillRule?: "evenodd" | "nonzero" | undefined): void
    }
    private _clip: {
        (fillRule?: "evenodd" | "nonzero" | undefined): void
        (path: NativePath2D, fillRule?: "evenodd" | "nonzero" | undefined): void
    }

    private _scale: (x: number, y: number) => void
    private _rotate: (angle: number) => void
    private _translate: (x: number, y: number) => void
    private _transform: (
        a: number,
        b: number,
        c: number,
        d: number,
        e: number,
        f: number
    ) => void
    private _setTransform: {
        (a: number, b: number, c: number, d: number, e: number, f: number): void
        (
            transform?:
                | [number, number, number, number, number, number]
                | undefined
        ): void
        (a?: any, b?: any, c?: any, d?: any, e?: any, f?: any): void
    }
    private _resetTransform: () => void
    private _getTransform: () => [
        number,
        number,
        number,
        number,
        number,
        number
    ]
    private _putImage: (
        buffer: ArrayBuffer,
        dx: number,
        dy: number,
        dirtyX: number,
        dirtyY: number,
        dirtyWidth: number,
        dirtyHeight: number,
        width: number,
        height: number,
        format: COLOR_FORMAT
    ) => void
    private _getImage: (
        sx: number,
        sy: number,
        sw: number,
        sh: number,
        settings?: ImageDataSettings | undefined
    ) => ArrayBuffer

    constructor(
        canvas: CanvasElement,
        contextType: string,
        contextAttributes?: Record<string, string>
    ) {
        this.canvas = canvas
        this.contextType = contextType
        this.contextAttributes = contextAttributes

        this.shadowBlur = 0
        this.shadowColor = "#00000000"
        this.shadowOffsetX = 0
        this.shadowOffsetY = 0
        this._lineCap = "butt"
        this._lineDashOffset = 0.0
        this._lineJoin = "miter"
        this._lineWidth = 1.0
        this._miterLimit = 10
        this._lineDashSegments = []

        this._fillStyle = "#000000"
        this._strokeStyle = "#000000"

        this._putImage = NativeCanvas.putImage.bind(this.canvas.nativeBox)
        this._getImage = NativeCanvas.getImage.bind(this.canvas.nativeBox)

        this._beginPath = NativeCanvas.beginPath.bind(this.canvas.nativeBox)
        this._closePath = NativeCanvas.closePath.bind(this.canvas.nativeBox)

        this._scale = NativeCanvas.scale.bind(this.canvas.nativeBox)
        this._rotate = NativeCanvas.rotate.bind(this.canvas.nativeBox)
        this._translate = NativeCanvas.translate.bind(this.canvas.nativeBox)
        this._transform = NativeCanvas.transform.bind(this.canvas.nativeBox)
        this._setTransform = NativeCanvas.setTransform.bind(
            this.canvas.nativeBox
        )
        this._resetTransform = NativeCanvas.resetTransform.bind(
            this.canvas.nativeBox
        )
        this._getTransform = NativeCanvas.getTransform.bind(
            this.canvas.nativeBox
        )

        this._ellipse = NativeCanvas.ellipse.bind(this.canvas.nativeBox)

        this._arc = NativeCanvas.arc.bind(this.canvas.nativeBox)
        this._arcTo = NativeCanvas.arcTo.bind(this.canvas.nativeBox)

        this._lineTo = NativeCanvas.lineTo.bind(this.canvas.nativeBox)
        this._moveTo = NativeCanvas.moveTo.bind(this.canvas.nativeBox)
        this._bezierCurveTo = NativeCanvas.bezierCurveTo.bind(
            this.canvas.nativeBox
        )
        this._quadraticCurveTo = NativeCanvas.quadraticCurveTo.bind(
            this.canvas.nativeBox
        )
        this._rect = NativeCanvas.rect.bind(this.canvas.nativeBox)

        this._stroke = NativeCanvas.stroke.bind(this.canvas.nativeBox)
        this._fill = NativeCanvas.fill.bind(this.canvas.nativeBox)
        this._clip = NativeCanvas.clip.bind(this.canvas.nativeBox)

        this._strokeRect = NativeCanvas.strokeRect.bind(this.canvas.nativeBox)
        this._fillRect = NativeCanvas.fillRect.bind(this.canvas.nativeBox)
        this._clearRect = NativeCanvas.clearRect.bind(this.canvas.nativeBox)

        this._setStrokeStyle = NativeCanvas.setStrokeStyle.bind(
            this.canvas.nativeBox
        )
        this._setFillStyle = NativeCanvas.setFillStyle.bind(
            this.canvas.nativeBox
        )

        this._setLineCap = NativeCanvas.setLineCap.bind(this.canvas.nativeBox)
        this._setLineJoin = NativeCanvas.setLineJoin.bind(this.canvas.nativeBox)
        this._setLineWidth = NativeCanvas.setLineWidth.bind(
            this.canvas.nativeBox
        )
        this._setMiterLimit = NativeCanvas.setMiterLimit.bind(
            this.canvas.nativeBox
        )
        this._setLineDash = NativeCanvas.setLineDash.bind(this.canvas.nativeBox)

        this.updateStrokeStyle()
        this.updateFillStyle()

        this.lineCap = this._lineCap
        this.lineDashOffset = this._lineDashOffset
        this.lineJoin = this._lineJoin
        this.lineWidth = this._lineWidth
        this.miterLimit = this._miterLimit
    }
    drawImage(image: CanvasImageSource, dx: number, dy: number): void
    drawImage(
        image: CanvasImageSource,
        dx: number,
        dy: number,
        dw: number,
        dh: number
    ): void
    drawImage(
        image: CanvasImageSource,
        sx: number,
        sy: number,
        sw: number,
        sh: number,
        dx: number,
        dy: number,
        dw: number,
        dh: number
    ): void
    drawImage(
        image: any,
        sx: any,
        sy: any,
        sw?: any,
        sh?: any,
        dx?: any,
        dy?: any,
        dw?: any,
        dh?: any
    ): void {
        throw new Error("Method not implemented.")
    }
    addColorStop(offset: number, color: string): void {
        throw new Error("Method not implemented.")
    }
    getTransform(): Matrix2D {
        return this._getTransform()
    }
    resetTransform(): void {
        this._resetTransform()
    }
    rotate(angle: number): void {
        this._rotate(angle)
    }
    scale(x: number, y: number): void {
        this._scale(x, y)
    }
    setTransform(
        a: number,
        b: number,
        c: number,
        d: number,
        e: number,
        f: number
    ): void
    setTransform(transform?: Matrix2D): void
    setTransform(a?: any, b?: any, c?: any, d?: any, e?: any, f?: any): void {
        if (typeof a === "number") {
            this._setTransform(a, b, c, d, e, f)
            return
        } else if (Array.isArray(a)) {
            const transform = a as Matrix2D
            this._setTransform(transform)
            return
        }
        throw new Error("Method not implemented.")
    }
    transform(
        a: number,
        b: number,
        c: number,
        d: number,
        e: number,
        f: number
    ): void {
        this._transform(a, b, c, d, e, f)
    }
    translate(x: number, y: number): void {
        this._translate(x, y)
    }

    fillText(text: string, x: number, y: number, maxWidth?: number): void {
        throw new Error("Method not implemented.")
    }
    measureText(text: string): TextMetrics {
        throw new Error("Method not implemented.")
    }
    strokeText(text: string, x: number, y: number, maxWidth?: number): void {
        throw new Error("Method not implemented.")
    }

    getLineDash(): number[] {
        return this._lineDashSegments
    }
    setLineDash(segments: number[]): void {
        this._lineDashSegments = segments
        this._setLineDash(this._lineDashOffset, segments)
    }
    private updateStrokeStyle() {
        const [r, g, b, a] = colorString.get.rgb(
            this._strokeStyle as string
        ) ?? [0, 0, 0, 1]
        this._setStrokeStyle(r / 255.0, g / 255.0, b / 255.0, a)
    }

    private updateFillStyle() {
        const [r, g, b, a] = colorString.get.rgb(this._fillStyle as string) ?? [
            0, 0, 0, 1,
        ]
        this._setFillStyle(r / 255.0, g / 255.0, b / 255.0, a)
    }

    get lineCap(): CanvasLineCap {
        return this._lineCap
    }
    set lineCap(val: CanvasLineCap) {
        this._lineCap = val
        this._setLineCap(val)
    }

    get lineDashOffset(): number {
        return this._lineDashOffset
    }
    set lineDashOffset(val: number) {
        this._lineDashOffset = val
        this._setLineDash(val, this._lineDashSegments)
    }

    get lineJoin(): CanvasLineJoin {
        return this._lineJoin
    }
    set lineJoin(val: CanvasLineJoin) {
        this._lineJoin = val
        this._setLineJoin(val)
    }

    get lineWidth(): number {
        return this._lineWidth
    }
    set lineWidth(val: number) {
        this._lineWidth = val
        this._setLineWidth(val)
    }
    get miterLimit(): number {
        return this._miterLimit
    }
    set miterLimit(val: number) {
        this._miterLimit = val
        this._setMiterLimit(val)
    }
    get strokeStyle(): string | CanvasGradient | CanvasPattern {
        return this._strokeStyle
    }
    set strokeStyle(val: string | CanvasGradient | CanvasPattern) {
        this._strokeStyle = val
        this.updateStrokeStyle()
    }

    get fillStyle(): string | CanvasGradient | CanvasPattern {
        return this._fillStyle
    }
    set fillStyle(val: string | CanvasGradient | CanvasPattern) {
        this._fillStyle = val
        this.updateFillStyle()
    }

    clearRect(x: number, y: number, w: number, h: number): void {
        this._clearRect(x, y, w, h)
    }
    fillRect(x: number, y: number, w: number, h: number): void {
        this._fillRect(x, y, w, h)
    }
    strokeRect(x: number, y: number, w: number, h: number): void {
        this._strokeRect(x, y, w, h)
    }

    createConicGradient(
        startAngle: number,
        x: number,
        y: number
    ): CanvasGradient {
        throw new Error("Method not implemented.")
    }
    createLinearGradient(
        x0: number,
        y0: number,
        x1: number,
        y1: number
    ): CanvasGradient {
        throw new Error("Method not implemented.")
    }
    createPattern(
        image: CanvasImageSource,
        repetition: string | null
    ): CanvasPattern | null {
        throw new Error("Method not implemented.")
    }
    createRadialGradient(
        x0: number,
        y0: number,
        r0: number,
        x1: number,
        y1: number,
        r1: number
    ): CanvasGradient {
        throw new Error("Method not implemented.")
    }
    beginPath(): void {
        this._beginPath()
    }

    clip(fillRule?: CanvasFillRule): void
    clip(path: Path2D, fillRule?: CanvasFillRule): void
    clip(path?: any, fillRule?: any): void {
        if (!path) {
            this._clip()
            return
        } else if (typeof path === "string") {
            const fillRule = path

            if (fillRule === "evenodd" || fillRule === "nonzero") {
                this._clip(fillRule)
                return
            } else throw new Error(`fillRule: ${fillRule} is not valid value`)
        } else if (path instanceof Path2D) {
            this._clip(path.native)
            return
        } else if (fillRule) {
            this._clip(path.native, fillRule)
            return
        }
        throw new Error("Method not implemented.")
    }
    fill(fillRule?: CanvasFillRule): void
    fill(path: Path2D, fillRule?: CanvasFillRule): void
    fill(path?: any, fillRule?: any): void {
        if (!path) {
            this._fill()
            return
        } else if (typeof path === "string") {
            const fillRule = path

            if (fillRule === "evenodd" || fillRule === "nonzero") {
                this._fill(fillRule)
                return
            } else throw new Error(`fillRule: ${fillRule} is not valid value`)
        } else if (path instanceof Path2D) {
            this._fill(path.native)
            return
        } else if (fillRule) {
            this._fill(path.native, fillRule)
            return
        }
        throw new Error("Method not implemented.")
    }
    isPointInPath(x: number, y: number, fillRule?: CanvasFillRule): boolean
    isPointInPath(
        path: Path2D,
        x: number,
        y: number,
        fillRule?: CanvasFillRule
    ): boolean
    isPointInPath(path: any, x: any, y?: any, fillRule?: any): boolean {
        throw new Error("Method not implemented.")
    }
    isPointInStroke(x: number, y: number): boolean
    isPointInStroke(path: Path2D, x: number, y: number): boolean
    isPointInStroke(path: any, x: any, y?: any): boolean {
        throw new Error("Method not implemented.")
    }
    stroke(): void
    stroke(path: Path2D): void
    stroke(path?: any): void {
        if (!path) {
            this._stroke()
            return
        } else if (path instanceof Path2D) {
            this._stroke(path.native)
            return
        }
        throw new Error("Method not implemented.")
    }

    arc(
        x: number,
        y: number,
        radius: number,
        startAngle: number,
        endAngle: number,
        counterclockwise?: boolean
    ): void {
        this._arc(x, y, radius, startAngle, endAngle, counterclockwise)
    }
    arcTo(
        x1: number,
        y1: number,
        x2: number,
        y2: number,
        radius: number
    ): void {
        this._arcTo(x1, y1, x2, y2, radius)
    }
    bezierCurveTo(
        cp1x: number,
        cp1y: number,
        cp2x: number,
        cp2y: number,
        x: number,
        y: number
    ): void {
        this._bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y)
    }
    closePath(): void {
        this._closePath()
    }
    ellipse(
        x: number,
        y: number,
        radiusX: number,
        radiusY: number,
        rotation: number,
        startAngle: number,
        endAngle: number,
        counterclockwise?: boolean
    ): void {
        this._ellipse(
            x,
            y,
            radiusX,
            radiusY,
            rotation,
            startAngle,
            endAngle,
            counterclockwise
        )
    }
    lineTo(x: number, y: number): void {
        this._lineTo(x, y)
    }
    moveTo(x: number, y: number): void {
        this._moveTo(x, y)
    }
    quadraticCurveTo(cpx: number, cpy: number, x: number, y: number): void {
        this._quadraticCurveTo(cpx, cpy, x, y)
    }
    rect(x: number, y: number, w: number, h: number): void {
        this._rect(x, y, w, h)
    }

    putImageData(
        imagedata: ImageData,
        dx: number,
        dy: number,
        format?: COLOR_FORMAT
    ): void
    putImageData(
        imagedata: ImageData,
        dx: number,
        dy: number,
        dirtyX: number,
        dirtyY: number,
        dirtyWidth: number,
        dirtyHeight: number,
        format?: COLOR_FORMAT
    ): void
    putImageData(
        imagedata: ImageData,
        dx: number,
        dy: number,
        dirtyX?: any,
        dirtyY?: any,
        dirtyWidth?: any,
        dirtyHeight?: any,
        format?: COLOR_FORMAT
    ): void {
        this._putImage(
            imagedata.data.buffer,
            dx,
            dy,
            dirtyX ?? 0,
            dirtyY ?? 0,
            dirtyWidth ?? imagedata.width,
            dirtyHeight ?? imagedata.height,
            imagedata.width,
            imagedata.height,
            format ?? COLOR_FORMAT.COLOR_RGBA
        )
    }

    createImageData(
        sw: number,
        sh: number,
        settings?: ImageDataSettings
    ): ImageData
    createImageData(imagedata: ImageData): ImageData
    createImageData(sw: any, sh?: any, settings?: any): ImageData {
        if (typeof sw === "number") {
            return new ImageData(sw, sh, settings)
        } else {
            const imagedata: ImageData = sw
            return new ImageData(imagedata.width, imagedata.height)
        }
    }
    getImageData(
        sx: number,
        sy: number,
        sw: number,
        sh: number,
        settings?: ImageDataSettings
    ): ImageData {
        const buffer = this._getImage(sx, sy, sw, sh)
        return new ImageData(new Uint8ClampedArray(buffer), sw, sh, settings)
    }
}
export class CanvasElement extends Box {
    private context: CanvasRenderingContext2D | null = null
    private _context2d: CanvasRenderingContext2D | null = null
    private getWidth: () => number
    private setWidth: (width: number) => void
    private setHeight: (height: number) => void
    private getHeight: () => number
    constructor(style: MeuiStyle) {
        super("canvas", style)
        this.getWidth = NativeCanvas.getWidth.bind(this.nativeBox)
        this.setWidth = NativeCanvas.setWidth.bind(this.nativeBox)
        this.getHeight = NativeCanvas.getHeight.bind(this.nativeBox)
        this.setHeight = NativeCanvas.setHeight.bind(this.nativeBox)
    }

    get width() {
        return this.getWidth()
    }

    set width(val) {
        this.setWidth(val)
    }

    get height() {
        return this.getHeight()
    }

    set height(val) {
        this.setHeight(val)
    }

    getContext(
        contextType: string,
        contextAttributes?: Record<string, string>
    ) {
        if (contextType === "2d") {
            this._context2d ??= new CanvasRenderingContext2D(
                this,
                contextType,
                contextAttributes
            )

            this.context = this._context2d
            return this._context2d
        }
    }
}
