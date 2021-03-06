import { CanvasRenderingContext2D } from "@/meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/lineDashOffset

    ctx.setLineDash([4, 16])
    ctx.lineDashOffset = 2

    ctx.beginPath()
    ctx.moveTo(0, 100)
    ctx.lineTo(400, 100)
    ctx.stroke()
}
