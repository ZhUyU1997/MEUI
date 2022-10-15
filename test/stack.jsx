/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React from "react"
import { View, Stack } from "@/meui"
import ReactMEUI from "@/react-meui"

import { MEUI } from "@/meui"
import { Root } from "@/components"

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

const meui = new MEUI(390, 844)
ReactMEUI.render(<App />, meui)
