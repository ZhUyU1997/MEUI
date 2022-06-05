import { CanvasRenderingContext2D } from "@/meui"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/setTransform

    ctx.setTransform(1, 1, 0, 1, 0, 0)
    ctx.fillRect(0, 0, 100, 100)
}
