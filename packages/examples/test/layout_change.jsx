/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useState } from "preact/compat"
import { View, Stack } from "@meui-js/preact"
import { createRoot } from "@meui-js/preact"

import { MEUI } from "@meui-js/core"
import { Root } from "@meui-js/components"

function App() {
    const [direction, setDirection] = useState(undefined)
    return (
        <Root
            style={{
                flexDirection: direction,
            }}
            onClick={() => {
                setDirection(direction ? undefined : "column")
            }}
        >
            <View
                style={{
                    width: 100,
                    height: 100,
                    backgroundColor: "red",
                }}
            ></View>
            <View
                style={{
                    width: 100,
                    height: 100,
                    backgroundColor: "green",
                }}
            ></View>
        </Root>
    )
}

createRoot(390, 844).render(<App />)
