import { CanvasRenderingContext2D } from "@/meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/getImageData

    ctx.rect(10, 10, 100, 100)
    ctx.fill()

    const data = ctx.getImageData(50, 50, 100, 100)
    console.log(data.width)
    console.log(data.height)
    ctx.putImageData(data, 200, 10)
}
