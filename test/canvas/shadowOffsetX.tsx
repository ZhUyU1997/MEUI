import { CanvasRenderingContext2D } from "@/meui"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/shadowOffsetX

    ctx.shadowColor = "black"
    ctx.shadowOffsetX = 10
    ctx.shadowBlur = 10

    ctx.fillStyle = "green"
    ctx.fillRect(10, 10, 100, 100)
}
