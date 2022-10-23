/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useState } from "preact/compat"
import { View } from "@meui/preact"
import { createRoot } from "@meui/preact"
import { Root } from "@meui/components"

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
                        <View
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
                        </View>
                    ) : (
                        <View
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
                        </View>
                    )
                ) : null
            })}
        </Root>
    )
}

createRoot(800, 800).render(<App />)
