/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React from "react"
import { Div } from "@/meui"
import ReactMEUI from "@/react-meui"

import { MEUI } from "@/meui"
import { Root } from "@/components"

function App() {
    return (
        <Root style={{}}>
            <stack
                style={{
                    width: 390,
                    height: 844,
                }}
            >
                <Div
                    style={{
                        width: 100,
                        height: 100,
                        backgroundColor: "red",
                    }}
                ></Div>
                <Div
                    style={{
                        width: 100,
                        height: 100,
                        backgroundColor: "green",
                        top: 10,
                        left: 10,
                    }}
                ></Div>
                <Div
                    style={{
                        width: 100,
                        height: 100,
                        backgroundColor: "red",
                        top: 20,
                        left: 20,
                    }}
                ></Div>
                <Div
                    style={{
                        width: 100,
                        height: 100,
                        backgroundColor: "green",
                        top: 30,
                        left: 30,
                    }}
                ></Div>
                <Div
                    style={{
                        width: 100,
                        height: 100,
                        backgroundColor: "red",
                        top: 40,
                        left: 40,
                    }}
                ></Div>
                <Div
                    style={{
                        width: 100,
                        height: 100,
                        backgroundColor: "red",
                        right: 10,
                    }}
                ></Div>

                <Div
                    style={{
                        width: 100,
                        height: 100,
                        backgroundColor: "red",
                        bottom: 10,
                    }}
                ></Div>
            </stack>
        </Root>
    )
}

const meui = new MEUI(390, 844)
ReactMEUI.render(<App />, meui)
