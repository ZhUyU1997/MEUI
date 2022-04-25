import { CanvasRenderingContext2D } from "@/meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/arcTo

    ctx.beginPath()
    ctx.moveTo(150, 20)
    ctx.arcTo(150, 100, 50, 20, 30)
    ctx.stroke()

    ctx.fillStyle = "blue"
    // base point
    ctx.fillRect(150, 20, 10, 10)

    ctx.fillStyle = "red"
    // control point one
    ctx.fillRect(150, 100, 10, 10)
    // control point two
    ctx.fillRect(50, 20, 10, 10)
    //
    ctx.setLineDash([5, 5])
    ctx.moveTo(150, 20)
    ctx.lineTo(150, 100)
    ctx.lineTo(50, 20)
    ctx.stroke()
    ctx.beginPath()
    ctx.arc(120, 38, 30, 0, 2 * Math.PI)
    ctx.stroke()
}
