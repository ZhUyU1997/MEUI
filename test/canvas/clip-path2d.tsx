import { CanvasRenderingContext2D, Path2D } from "../../meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    const path = new Path2D()
    path.arc(0, 100, 75, 0, Math.PI * 2, false)
    ctx.clip(path)
    ctx.fillRect(0, 0, 100, 100)
}
