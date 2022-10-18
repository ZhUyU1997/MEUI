/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useState } from "react"
import { View, MEUI, Stack } from "@/meui"
import ReactMEUI from "@/react-meui"
import { Column, Root, Row } from "@/components"

function App() {
    return (
        <Root>
            <Column
                style={{
                    backgroundColor: "black",
                    borderRadius: 40,
                    width: "100%",
                    justifyContent: "center",
                    alignItems: "center",
                }}
            >
                <Row
                    style={{
                        height: 120 * 2,
                    }}
                >
                    <Stack
                        style={{
                            height: 120 * 2,
                            width: 80 * 2,
                        }}
                    >
                        <Column
                            style={{
                                width: "100%",
                                justifyContent: "center",
                                alignItems: "center",
                            }}
                        >
                            <View
                                style={{
                                    fontColor: "#00D1FF",
                                    fontSize: 100,
                                    // fontSize: 140,
                                    // lineHeight: "60px",
                                    textAlign: "center",
                                }}
                            >
                                0933
                            </View>
                        </Column>
                        <View
                            style={{
                                fontColor: "white",
                                fontSize: 20,
                                right: -30,
                                top: "50%",
                            }}
                        >
                            Am
                        </View>
                    </Stack>
                </Row>
                <Row style={{ fontColor: "white", fontSize: 20 }}>
                    21 June, Monday
                </Row>
            </Column>
        </Root>
    )
}

const meui = new MEUI(312, 390)
ReactMEUI.render(<App />, meui)
