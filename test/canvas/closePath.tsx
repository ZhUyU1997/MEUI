import { CanvasRenderingContext2D } from "../../meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/closePath

    ctx.beginPath()
    ctx.moveTo(20, 20)
    ctx.lineTo(200, 20)
    ctx.lineTo(120, 120)
    ctx.closePath() // draws last line of the triangle
    ctx.stroke()
}
