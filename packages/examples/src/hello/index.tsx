/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { createRoot } from "@meui-js/preact"
import { useState } from "preact/compat"
import { animated, useSpring } from "@meui-js/spring"

function App() {
    const styles = useSpring({
        loop: true,
        from: { rotateZ: 0 },
        to: { rotateZ: 360 },
    })

    const [text, setText] = useState("Hello MEUI!")

    return (
        <animated.View
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
        </animated.View>
    )
}

createRoot(300, 300).render(<App />)
