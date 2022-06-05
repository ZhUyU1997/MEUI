import { CanvasRenderingContext2D } from "@/meui"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/fillRect

    ctx.fillStyle = "green"
    ctx.fillRect(20, 10, 150, 100)
}
