import { CanvasRenderingContext2D } from "../../meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/strokeRect

    ctx.strokeStyle = "green"
    ctx.strokeRect(20, 10, 160, 100)
}
