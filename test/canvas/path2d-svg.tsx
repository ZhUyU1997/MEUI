import { CanvasRenderingContext2D, Path2D } from "@/meui"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/Path2D/Path2D

    const p = new Path2D("M10 10 h 80 v 80 h -80 Z")
    ctx.fill(p)
}
