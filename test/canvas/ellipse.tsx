import { CanvasRenderingContext2D } from "@/meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/ellipse

    ctx.setLineDash([])
    ctx.beginPath()
    ctx.ellipse(100, 100, 50, 75, (45 * Math.PI) / 180, 0, 2 * Math.PI) //倾斜45°角
    ctx.stroke()
    ctx.setLineDash([5])
    ctx.moveTo(0, 200)
    ctx.lineTo(200, 0)
    ctx.stroke()
}
