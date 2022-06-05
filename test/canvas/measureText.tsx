import { CanvasRenderingContext2D } from "@/meui"

export default function Test(ctx: CanvasRenderingContext2D) {
    ctx.font = "36px serif"
    ctx.strokeStyle = "red"
    const y = 100
    ctx.fillText("┃ぬabc中国kⅧ ", 0, y)

    const m = ctx.measureText("┃ぬabc中国kⅧ ")

    ctx.beginPath()
    ctx.moveTo(0, y)
    ctx.lineTo(m.width, y)
    ctx.stroke()

    ctx.moveTo(0, y - m.fontBoundingBoxAscent)
    ctx.lineTo(m.width, y - m.fontBoundingBoxAscent)
    ctx.stroke()

    ctx.moveTo(0, y + m.fontBoundingBoxDescent)
    ctx.lineTo(m.width, y + m.fontBoundingBoxDescent)
    ctx.stroke()

    ctx.moveTo(0, y - m.actualBoundingBoxAscent)
    ctx.lineTo(m.width, y - m.actualBoundingBoxAscent)
    ctx.stroke()

    ctx.moveTo(0, y + m.actualBoundingBoxDescent)
    ctx.lineTo(m.width, y + m.actualBoundingBoxDescent)
    ctx.stroke()

    ctx.moveTo(m.actualBoundingBoxRight, y - m.fontBoundingBoxAscent)
    ctx.lineTo(m.actualBoundingBoxRight, y + m.fontBoundingBoxDescent)
    ctx.stroke()
}
