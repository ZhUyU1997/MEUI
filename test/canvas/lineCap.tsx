import { CanvasRenderingContext2D } from "@/meui"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/lineCap

    ctx.beginPath()
    ctx.moveTo(0, 0)
    ctx.lineWidth = 15
    ctx.lineCap = "round"
    ctx.lineTo(100, 100)
    ctx.stroke()
}
