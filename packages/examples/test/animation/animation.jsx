/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React from "preact/compat"
import { createRoot } from "@meui-js/preact"
import { animated, useSpring } from "@meui-js/spring"

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

createRoot(300, 300).render(<App />)
