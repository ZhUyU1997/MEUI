/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useEffect, useState } from "react"
import { Div, MEUI, Stack } from "@/meui"
import ReactMEUI from "@/react-meui"
import { Column, Root, Row } from "@/components"

function Digital({ fixedOffset, left, top, style, children }) {
    return (
        <Div
            style={{
                width: 50,
                height: 50,
                fontSize: 40,
                fontColor: "white",
                // backgroundColor: "red",
                textAlign: "center",
                left: fixedOffset - 25 + left,
                top: fixedOffset - 25 + top,
                ...style,
            }}
        >
            {children}
        </Div>
    )
}

function DimDigital(props) {
    return (
        <Digital
            {...props}
            style={{
                fontColor: "rgba(255,255,255,0.15)",
            }}
        ></Digital>
    )
}

function App() {
    const [count, setCount] = useState(1 * 60 * 60 + 48 * 60 + 34)
    const hour = count / 60 / 60
    const minute = (count / 60) % 60
    const second = count % 60

    useEffect(() => {
        const id = setInterval(() => setCount((count) => count + 1), 1000)
        return () => {
            clearInterval(id)
        }
    }, [])

    const stackSize = 100
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
                <Stack
                    style={{
                        width: stackSize,
                        height: stackSize,
                    }}
                >
                    <Row
                        style={{
                            height: 4,
                            width: 62 * 2,
                            borderRadius: 2,
                            backgroundColor: "#FFFFFF80",
                            left: stackSize / 2,
                            top: stackSize / 2 - 2,

                            transformOrigin: ["left", "center"],
                            transform: `rotate(-90deg) rotate(${
                                (second % 60) * (360 / 60)
                            }deg)`,
                        }}
                    ></Row>

                    <Row
                        style={{
                            height: 8,
                            width: 53.72 * 2,
                            borderRadius: 4,
                            backgroundColor: "#0BFFA7",
                            left: stackSize / 2,
                            top: stackSize / 2 - 4,
                            transformOrigin: ["left", "center"],
                            transform: `rotate(-90deg) rotate(${
                                (minute % 60) * (360 / 60)
                            }deg)`,
                        }}
                    ></Row>
                    <Row
                        style={{
                            height: 10,
                            width: 37.74 * 2,
                            borderRadius: 5,
                            backgroundColor: "#FFBB0B",
                            left: stackSize / 2,
                            top: stackSize / 2 - 5,

                            transformOrigin: ["left", "center"],
                            transform: `rotate(-90deg) rotate(${
                                (hour % 12) * (360 / 12)
                            }deg)`,
                        }}
                    ></Row>

                    <Div
                        style={{
                            width: 24,
                            height: 24,
                            borderRadius: 12,
                            backgroundColor: "#3F3F3F",

                            left: stackSize / 2 - 12,
                            top: stackSize / 2 - 12,
                        }}
                    ></Div>
                    <Digital fixedOffset={stackSize / 2} left={0} top={-160}>
                        12
                    </Digital>
                    <Digital fixedOffset={stackSize / 2} left={+130} top={0}>
                        3
                    </Digital>
                    <Digital fixedOffset={stackSize / 2} left={0} top={+160}>
                        6
                    </Digital>
                    <Digital fixedOffset={stackSize / 2} left={-130} top={0}>
                        9
                    </Digital>

                    <DimDigital
                        fixedOffset={stackSize / 2}
                        left={+85}
                        top={-150}
                    >
                        1
                    </DimDigital>
                    <DimDigital
                        fixedOffset={stackSize / 2}
                        left={+85}
                        top={+150}
                    >
                        5
                    </DimDigital>
                    <DimDigital
                        fixedOffset={stackSize / 2}
                        left={-85}
                        top={+150}
                    >
                        7
                    </DimDigital>
                    <DimDigital
                        fixedOffset={stackSize / 2}
                        left={-85}
                        top={-150}
                    >
                        11
                    </DimDigital>

                    <DimDigital
                        fixedOffset={stackSize / 2}
                        left={+125}
                        top={-75}
                    >
                        2
                    </DimDigital>
                    <DimDigital
                        fixedOffset={stackSize / 2}
                        left={+125}
                        top={+75}
                    >
                        4
                    </DimDigital>
                    <DimDigital
                        fixedOffset={stackSize / 2}
                        left={-125}
                        top={+75}
                    >
                        8
                    </DimDigital>
                    <DimDigital
                        fixedOffset={stackSize / 2}
                        left={-125}
                        top={-75}
                    >
                        10
                    </DimDigital>
                </Stack>
            </Column>
        </Root>
    )
}

const meui = new MEUI(312, 390)
ReactMEUI.render(<App />, meui)
