import { CanvasRenderingContext2D, Path2D } from "../../meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/Path2D/Path2D
    const path1 = new Path2D()
    path1.rect(10, 10, 100, 100)
    const path2 = new Path2D(path1)
    path2.moveTo(220, 60)
    path2.arc(170, 60, 50, 0, 2 * Math.PI)
    ctx.stroke(path2)
}
