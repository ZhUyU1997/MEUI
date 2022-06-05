import { CanvasRenderingContext2D, Path2D } from "@/meui"

export default function Test(ctx: CanvasRenderingContext2D) {
    // Create a radial gradient
    // The inner circle is at x=110, y=90, with radius=30
    // The outer circle is at x=100, y=100, with radius=70
    const gradient = ctx.createRadialGradient(110, 90, 30, 100, 100, 70)

    // Add three color stops
    gradient.addColorStop(0, "pink")
    gradient.addColorStop(0.9, "white")
    gradient.addColorStop(1, "green")

    // Set the fill style and draw a rectangle
    ctx.fillStyle = gradient
    ctx.fillRect(20, 20, 160, 160)
}
