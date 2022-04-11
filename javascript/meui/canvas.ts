import { Canvas as NativeCanvas } from "NativeMEUI"
import { Box } from "./box"
import { MeuiStyle } from "./style"

export class ImageData {
    data: Uint8ClampedArray
    width: number
    height: number
    constructor(...args: any[]) {
        if (args.length < 2) {
            throw new TypeError(`
              Failed to construct 'ImageData': 2 arguments required, but only ${args.length} present.
            `)
        }
        if (args.length > 2) {
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

class CanvasRenderingContext2D {
    contextType: string
    contextAttributes?: Record<string, string>
    canvas: CanvasElement
    constructor(
        canvas: CanvasElement,
        contextType: string,
        contextAttributes?: Record<string, string>
    ) {
        this.canvas = canvas
        this.contextType = contextType
        this.contextAttributes = contextAttributes
    }
    putImageData(
        imageData: ImageData,
        dx: number,
        dy: number,
        dirtyX?: number,
        dirtyY?: number,
        dirtyWidth?: number,
        dirtyHeight?: number
    ) {
        console.log(dx, dy, dirtyWidth, dirtyHeight)
        NativeCanvas.putImage.call(
            this.canvas.nativeBox,
            imageData.data.buffer,
            dx,
            dy,
            dirtyX ?? 0,
            dirtyY ?? 0,
            dirtyWidth ?? imageData.width,
            dirtyHeight ?? imageData.height,
            imageData.width,
            imageData.height
        )
    }
}
export class CanvasElement extends Box {
    private context: CanvasRenderingContext2D | null = null
    private _context2d: CanvasRenderingContext2D | null = null
    constructor(style: MeuiStyle) {
        super("canvas", style)
    }

    get width() {
        return NativeCanvas.getWidth.call(this.nativeBox)
    }

    set width(val) {
        NativeCanvas.setWidth.call(this.nativeBox, val)
    }

    get height() {
        return NativeCanvas.getHeight.call(this.nativeBox)
    }

    set height(val) {
        NativeCanvas.setHeight.call(this.nativeBox, val)
    }

    getContext(
        contextType: string,
        contextAttributes?: Record<string, string>
    ) {
        if (contextType == "2d") {
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
