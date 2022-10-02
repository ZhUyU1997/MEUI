/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useState } from "react"
import { MEUI, Div } from "@/meui"
import ReactMEUI from "@/react-meui"

import { animated, useSpring, config } from "@/react-sping-meui"
import { Root } from "@/components"

function App() {
    const styles = useSpring({
        loop: true,
        from: { rotateZ: 0, scale: 1.0 },
        to: [
            { rotateZ: 45, scale: 2 },
            { rotateZ: 90, scale: 1.0 },
            { rotateZ: 135, scale: 2 },
            { rotateZ: 180, scale: 1.0 },
            { rotateZ: 225, scale: 2 },
            { rotateZ: 270, scale: 1.0 },
            { rotateZ: 315, scale: 2 },
            { rotateZ: 360, scale: 1.0 },
        ],
        config: {
            // mass: 10000,
            // friction: 100,
        },
    })

    return (
        <Root
            style={{
                justifyContent: "center",
                alignItems: "center",
                flexWrap: "wrap",
            }}
        >
            {new Array(56).fill(0).map((item, index) => {
                return (
                    <animated.Div
                        key={index}
                        style={
                            index == 12
                                ? {
                                      width: 120,
                                      height: 120,
                                      backgroundColor: "red",
                                      borderRadius: 16,
                                      textAlign: "center",
                                      zIndex: 1,
                                      ...styles,
                                  }
                                : {
                                      width: 120,
                                      height: 120,
                                      backgroundColor: "#46e891",
                                      borderRadius: 16,
                                      textAlign: "center",
                                  }
                        }
                    >
                        {index == 12 ? (
                            <Div
                                style={{
                                    width: 20,
                                    height: 20,
                                    backgroundColor: "blue",
                                    borderRadius: 10,
                                    textAlign: "center",
                                    zIndex: 1,
                                }}
                            ></Div>
                        ) : null}
                    </animated.Div>
                )
            })}
        </Root>
    )
}

const meui = new MEUI(1000, 1000)
ReactMEUI.render(<App />, meui)
