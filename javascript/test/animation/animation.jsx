import React, { useState, useEffect, useLayoutEffect, useRef } from "react"
import ReactMEUI from "../../ReactMEUI"
import Button from "../../button"
import { MEUI } from "../../meui"
import { Root, Column, Center, MaterialDesignIcon } from "../../components"
import { animated, useSpring } from "../../react-sping-meui"

function App() {
    const styles = useSpring({
        loop: true,
        from: { rotateZ: 0 },
        to: { rotateZ: 180 },
    })

    return (
        <animated.div
            style={{
                width: 80,
                height: 80,
                backgroundColor: '#46e891',
                borderRadius: 16,
                ...styles,
            }}
        />
    )
}

export function Main() {
    const meui = new MEUI(390, 844)
    ReactMEUI.render(<div>
        <App />
    </div>, meui)
}