/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useState } from "react"
import { View, Stack } from "@/meui"
import ReactMEUI from "@/react-meui"

import { MEUI } from "@/meui"
import { Root } from "@/components"

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

const meui = new MEUI(390, 844)
ReactMEUI.render(<App />, meui)
