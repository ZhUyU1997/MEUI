import { CanvasRenderingContext2D } from "../../meui/canvas"

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
