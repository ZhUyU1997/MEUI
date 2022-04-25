import { CanvasRenderingContext2D } from "@/meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/resetTransform
    ctx.rotate((45 * Math.PI) / 180)
    ctx.fillRect(70, 0, 100, 30)

    // reset current transformation matrix to the identity matrix
    ctx.resetTransform()
}
