import { createMEUI } from "./create"
import { useEffect, useRef } from "react"
export default function MEUI({ input }) {
    const canvas = useRef(null)
    const destory = useRef(null)

    useEffect(() => {
        const init = async () => {
            if (input && input.length > 0) {
                destory.current = await createMEUI({
                    canvas: canvas.current,
                    input,
                })
                return destory
            }
        }
        init()
        return () => {
            destory.current?.()
        }
    }, [input])
    return (
        <canvas
            ref={canvas}
            id="canvas"
            onContextMenu={(e) => {
                e.preventDefault()
            }}
            tabIndex={-1}
            style={{
                maxWidth: "100%",
                maxHeight: "100%",
            }}
        ></canvas>
    )
}
