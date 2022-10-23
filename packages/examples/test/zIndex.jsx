/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React from "preact/compat"
import { View } from "@meui/preact"
import { createRoot } from "@meui/preact"
import { Root } from "@meui/components"

function App() {
    return (
        <Root style={{ justifyContent: "center", alignItems: "center" }}>
            <View
                style={{
                    width: 100,
                    height: 100,
                    backgroundColor: "red",
                    textAlign: "center",
                    transform: " rotate(20deg) translateX(-20px) scale(1.2)",
                    HOVER: {
                        border: 10,
                        borderColor: "black",
                    },
                    // zIndex: 3,
                }}
            >
                zindex: default
            </View>
            <View
                style={{
                    width: 100,
                    height: 100,
                    backgroundColor: "green",
                    textAlign: "center",
                    transform: " rotate(20deg) translateX(-20px) scale(1.2)",
                    zIndex: 2,
                    HOVER: {
                        border: 10,
                        borderColor: "black",
                    },
                }}
            >
                zindex: 2
            </View>
            <View
                style={{
                    width: 100,
                    height: 100,
                    backgroundColor: "blue",
                    textAlign: "center",
                    transform: " rotate(20deg) translateX(-20px) scale(1.5)",
                    zIndex: 1,
                    HOVER: {
                        border: 10,
                        borderColor: "black",
                    },
                }}
            >
                zindex: 1
            </View>
        </Root>
    )
}

createRoot(600, 600).render(<App />)
