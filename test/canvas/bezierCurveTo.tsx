import { CanvasRenderingContext2D } from "@/meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/bezierCurveTo

    ctx.beginPath()
    ctx.moveTo(50, 20)
    ctx.bezierCurveTo(230, 30, 150, 60, 50, 100)
    ctx.stroke()

    ctx.fillStyle = "blue"
    // start point
    ctx.fillRect(50, 20, 10, 10)
    // end point
    ctx.fillRect(50, 100, 10, 10)

    ctx.fillStyle = "red"
    // control point one
    ctx.fillRect(230, 30, 10, 10)
    // control point two
    ctx.fillRect(150, 70, 10, 10)
}
