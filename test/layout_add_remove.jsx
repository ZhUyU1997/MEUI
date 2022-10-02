/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useState } from "react"
import { Div, Stack } from "@/meui"
import ReactMEUI from "@/react-meui"

import { MEUI } from "@/meui"
import { Root } from "@/components"

function App() {
    const [items, setItems] = useState(
        Array(49)
            .fill(0)
            .map((_, index) => ({ key: index, show: true }))
    )
    return (
        <Root
            style={{
                flexWrap: "wrap",
            }}
        >
            {items.map(({ show, key }, index) => {
                return show ? (
                    index == 20 ? (
                        <Div
                            key={key}
                            style={{
                                margin: 5,
                                width: 100,
                                height: 100,
                                transform: "rotate(45deg)",
                                zIndex: 0,
                                backgroundColor: "red",
                            }}
                        >
                            {key}
                        </Div>
                    ) : (
                        <Div
                            key={key}
                            style={{
                                margin: 5,
                                width: 100,
                                height: 100,
                                backgroundColor: "green",
                            }}
                            onClick={() => {
                                const newItems = [...items]
                                newItems[key].show = false
                                setItems(newItems)
                            }}
                        >
                            {key}
                        </Div>
                    )
                ) : null
            })}
        </Root>
    )
}

const meui = new MEUI(800, 800)
ReactMEUI.render(<App />, meui)
