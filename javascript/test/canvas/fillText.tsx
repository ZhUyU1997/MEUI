import { CanvasRenderingContext2D } from "../../meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/fillText

    ctx.font = "48px serif"
    ctx.fillText("Hello world", 50, 100)
    ctx.fillText("Hello world", 50, 200, 100)
}
