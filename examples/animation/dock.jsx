/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useEffect, useState } from "react"
import { View, MEUI } from "@/meui"
import ReactMEUI from "@/react-meui"

import { animated, useSpring, config } from "@/react-sping-meui"
import { Root } from "@/components"

function App() {
    const [open, setOpen] = useState(false)

    const [styles, api] = useSpring(() => ({
        transform: open
            ? "scale(1.0) translateY(0px)"
            : "scale(0.1) translateY(400px)",
        config: {
            friction: 100,
        },
    }))

    useEffect(() => {
        api.start({
            to: async (next) => {
                // await next({
                //     transform: "scale(0.1) translateY(400px)",
                //     config: { tension: 310, friction: 20 }
                // })

                await next({
                    transform: open
                        ? "scale(1.0) translateY(0px)"
                        : "scale(0.1) translateY(400px)",
                    config: { tension: 310, friction: 20 },
                })
            },
        })
    }, [api, open])

    return (
        <Root
            style={{
                flexDirection: "column",
            }}
        >
            <View
                style={{
                    width: "100%",
                    flexGrow: 1,
                }}
            >
                <animated.View
                    style={{
                        width: "100%",
                        flexGrow: 1,
                        backgroundColor: "#46e891",
                        borderRadius: 16,
                        zIndex: 0,
                        ...styles,
                    }}
                ></animated.View>
            </View>
            <View
                style={{
                    width: "content",
                    height: 150,
                    justifyContent: "center",
                    alignItems: "center",
                    padding: 10,
                }}
            >
                <View
                    style={{
                        width: 120,
                        height: 120,
                        backgroundColor: "red",
                        borderRadius: 16,
                        textAlign: "center",
                        zIndex: 1,
                        HOVER: {
                            border: 10,
                            borderColor: "red",
                        },
                        FOCUS: {
                            border: 10,
                            borderColor: "red",
                        },
                        ACTIVE: {
                            border: 10,
                            borderColor: "red",
                        },
                    }}
                    onClick={() => {
                        setOpen((prev) => !prev)
                    }}
                ></View>
            </View>
        </Root>
    )
}

const meui = new MEUI(1000, 800)
ReactMEUI.render(<App />, meui)
