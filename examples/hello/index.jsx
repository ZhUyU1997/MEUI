import React from "react"
import { MEUI } from "@/meui"
import ReactMEUI from "@/react-meui"

import { animated, useSpring } from "@/react-sping-meui"

function App() {
    const styles = useSpring({
        loop: true,
        from: { rotateZ: 0 },
        to: { rotateZ: 360 },
    })

    return (
        <animated.Div
            style={{
                width: 120,
                height: 120,
                backgroundColor: "#46e891",
                borderRadius: 16,
                textAlign: "center",
                ...styles,
            }}
        >
            Hello MEUI!
        </animated.Div>
    )
}

const meui = new MEUI(300, 300)
ReactMEUI.render(<App />, meui)
