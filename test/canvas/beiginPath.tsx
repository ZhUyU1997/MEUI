import { CanvasRenderingContext2D } from "@/meui"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/beginPath

    // First path
    ctx.beginPath()
    ctx.strokeStyle = "blue"
    ctx.moveTo(20, 20)
    ctx.lineTo(200, 20)
    ctx.stroke()

    // Second path
    ctx.beginPath()
    ctx.strokeStyle = "green"
    ctx.moveTo(20, 20)
    ctx.lineTo(120, 120)
    ctx.stroke()
}
