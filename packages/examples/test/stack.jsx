/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React from "preact/compat"
import { View, Stack } from "@meui/preact"
import { createRoot } from "@meui/preact"
import { Root } from "@meui/components"

function App() {
    return (
        <Root style={{}}>
            <Stack
                style={{
                    width: 390,
                    height: 844,
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
                        top: 10,
                        left: 10,
                    }}
                ></View>
                <View
                    style={{
                        width: 100,
                        height: 100,
                        backgroundColor: "red",
                        top: 20,
                        left: 20,
                    }}
                ></View>
                <View
                    style={{
                        width: 100,
                        height: 100,
                        backgroundColor: "green",
                        top: 30,
                        left: 30,
                    }}
                ></View>
                <View
                    style={{
                        width: 100,
                        height: 100,
                        backgroundColor: "red",
                        top: 40,
                        left: 40,
                    }}
                ></View>
                <View
                    style={{
                        width: 100,
                        height: 100,
                        backgroundColor: "red",
                        right: 10,
                    }}
                ></View>

                <View
                    style={{
                        width: 100,
                        height: 100,
                        backgroundColor: "red",
                        bottom: 10,
                    }}
                ></View>
            </Stack>
        </Root>
    )
}

createRoot(390, 844).render(<App />)
