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
        <Root style={{ justifyContent: "center", alignItems: "center" }}>
            <Div
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
            </Div>
            <Div
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
            </Div>
            <Div
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
            </Div>
        </Root>
    )
}

const meui = new MEUI(600, 600)
ReactMEUI.render(<App />, meui)
