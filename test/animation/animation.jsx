/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React from "react"
import { MEUI } from "@/meui"
import ReactMEUI from "@/react-meui"

import { animated, useSpring } from "@/react-sping-meui"

function App() {
    const styles = useSpring({
        loop: true,
        from: { rotateZ: 0 },
        to: { rotateZ: 180 },
    })

    return (
        <animated.View
            style={{
                width: 80,
                height: 80,
                backgroundColor: "#46e891",
                borderRadius: 16,
                ...styles,
            }}
        />
    )
}

const meui = new MEUI(300, 300)
ReactMEUI.render(<App />, meui)
