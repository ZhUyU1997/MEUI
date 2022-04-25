import { CanvasRenderingContext2D } from "@/meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/shadowBlur

    ctx.shadowColor = "black"
    ctx.shadowBlur = 10

    ctx.fillStyle = "white"
    ctx.fillRect(10, 10, 100, 100)
}
