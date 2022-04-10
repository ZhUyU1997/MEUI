import { Canvas as NativeCanvas } from "NativeMEUI"
import { Box } from "./box"

export class Canvas extends Box {
    constructor(style) {
        super("canvas", style)
    }

    putImage(buf, width, height) {
        NativeCanvas.putImage.call(this.nativeBox, buf, width, height)
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
}
