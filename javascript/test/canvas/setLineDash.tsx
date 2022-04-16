import { CanvasRenderingContext2D } from "../../meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/setLineDash

    // Dashed line
    ctx.beginPath()
    ctx.setLineDash([5, 15])
    ctx.moveTo(0, 50)
    ctx.lineTo(300, 50)
    ctx.stroke()

    // Solid line
    ctx.beginPath()
    ctx.setLineDash([])
    ctx.moveTo(0, 100)
    ctx.lineTo(300, 100)
    ctx.stroke()
}
