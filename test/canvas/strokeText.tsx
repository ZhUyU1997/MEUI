import { CanvasRenderingContext2D } from "@/meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/strokeText
    ctx.font = "48px serif"
    ctx.strokeText("Hello world", 50, 100)
}
