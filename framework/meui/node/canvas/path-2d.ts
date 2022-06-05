import { Path2D as NativePath2D } from "NativeMEUI"
import { Matrix2D } from "../../style"

export interface CanvasPath {
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
    // addPath(path: Path2D, transform?: DOMMatrix2DInit): void {}
    addPath(path: Path2D, transform?: Matrix2D): void {
        this.native.addPath(path, transform)
    }
}
