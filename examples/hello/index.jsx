/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useState } from "react"
import { MEUI } from "@/meui"
import ReactMEUI from "@/react-meui"

import { animated, useSpring } from "@/react-sping-meui"

function App() {
    const styles = useSpring({
        loop: true,
        from: { rotateZ: 0 },
        to: { rotateZ: 360 },
    })

    const [text, setText] = useState("Hello MEUI!")

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
            onClick={() => {
                setText("Hello MEUI!\nHello React!")
            }}
        >
            {text}
        </animated.Div>
    )
}

const meui = new MEUI(300, 300)
ReactMEUI.render(<App />, meui)
