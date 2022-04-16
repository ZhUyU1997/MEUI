import { CanvasRenderingContext2D } from "../../meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/transform

    ctx.setTransform(1, 0.2, 0.8, 1, 0, 0)
    ctx.fillRect(25, 25, 50, 50)

    const storedTransform = ctx.getTransform()
    console.log(storedTransform)

    ctx.setTransform(storedTransform)
    ctx.beginPath()
    ctx.arc(150, 50, 50, 0, 2 * Math.PI)
    ctx.fill()
}
