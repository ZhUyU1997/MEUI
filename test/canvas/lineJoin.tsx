import { CanvasRenderingContext2D } from "@/meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/lineJoin

    ctx.lineWidth = 10
    ctx.lineJoin = "round"
    ctx.beginPath()
    ctx.moveTo(0, 0)
    ctx.lineTo(200, 100)
    ctx.lineTo(300, 0)
    ctx.stroke()
}
