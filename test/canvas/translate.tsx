import { CanvasRenderingContext2D } from "@/meui"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/translate

    ctx.translate(50, 50)
    ctx.fillRect(0, 0, 100, 100)

    // reset current transformation matrix to the identity matrix
    ctx.setTransform(1, 0, 0, 1, 0, 0)
}
