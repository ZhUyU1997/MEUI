/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useState } from "react"
import { View, MEUI } from "@/meui"
import ReactMEUI from "@/react-meui"

import { Center, Column, MaterialDesignIcon, Root, Row } from "@/components"
import TextArea from "@/components/text-area"
import { MeuiStyle } from "@/meui"
import { evaluate } from "mathjs"

function Screen({ log, style }: { log: string; style?: MeuiStyle }) {
    return (
        <Center
            style={{
                width: "100%",
                height: 135,
                backgroundColor: "#b2c2b3",
                flexDirection: "column",
                marginTop: 20,
                marginBottom: 20,
                ...style,
            }}
        >
            <View
                style={{
                    width: "100%",
                    height: 75,
                    fontSize: 40,
                    fontColor: "black",
                }}
            >
                {log}
            </View>
            <View
                style={{
                    width: "100%",
                    height: 75,
                    fontSize: 40,
                    textAlign: "center-right",
                }}
            ></View>
        </Center>
    )
}

function Key({
    children,
    keyClick,
    keyLog,
    math,
}: {
    children: React.ReactNode
    keyClick?: (keyLog: string, math: string) => void
    keyLog?: string
    math?: string
}) {
    return (
        <View
            style={{
                borderRadius: 12.5,
                fontColor: "white",
                backgroundColor: "#202742",
                fontSize: 20,
                width: 49 + 12,
                height: 39 + 2,
                padding: [1, 6, 6, 1],
                margin: [14, 8, 14, 8],
                textAlign: "center",
            }}
            onClick={() => {
                if (keyLog && math) keyClick?.(keyLog, math)
            }}
        >
            {children}
        </View>
    )
}

function BigKey({
    children,
    keyClick,
    keyLog,
    math,
    style,
}: {
    children: React.ReactNode
    keyClick?: (keyLog: string, math: string) => void
    keyLog?: string
    math?: string
    style?: MeuiStyle
}) {
    return (
        <View
            style={{
                borderRadius: [10, 10, 18, 18],
                fontColor: "white",
                backgroundColor: "#888fa1",
                fontSize: 30,
                width: 68 + 12,
                height: 48 + 2,
                padding: [1, 6, 6, 1],
                margin: [10, 6, 10, 6],
                textAlign: "center",
                ...style,
            }}
            onClick={() => {
                if (keyLog && math) keyClick?.(keyLog, math)
            }}
        >
            {children}
        </View>
    )
}

function KeyBoard({
    keyClick,
}: {
    keyClick: (keyLog: string, math: string) => void
}) {
    return (
        <Column>
            <Row
                style={{
                    justifyContent: "space-between",
                    width: "100%",
                }}
            >
                <Key>X^-1</Key>
                <Key>sqrt</Key>
                <Key>X^2</Key>
                <Key>^</Key>
                <Key>log</Key>
                <Key>ln</Key>
            </Row>
            <Row
                style={{
                    justifyContent: "space-between",
                    width: "100%",
                }}
            >
                <Key>{"(-)"}</Key>
                <Key>x^3</Key>
                <Key>hyp</Key>
                <Key>sin</Key>
                <Key>cos</Key>
                <Key>tan</Key>
            </Row>
            <Row
                style={{
                    justifyContent: "space-between",
                    width: "100%",
                }}
            >
                <Key>RCL</Key>
                <Key>ENG</Key>
                <Key>{"("}</Key>
                <Key>{")"}</Key>
                <Key>,</Key>
                <Key>M+</Key>
            </Row>
            <Row
                style={{
                    justifyContent: "space-between",
                    width: "100%",
                }}
            >
                <BigKey keyLog="1" math="number" keyClick={keyClick}>
                    1
                </BigKey>
                <BigKey keyLog="2" math="number" keyClick={keyClick}>
                    2
                </BigKey>
                <BigKey keyLog="3" math="number" keyClick={keyClick}>
                    3
                </BigKey>
                <BigKey
                    style={{
                        backgroundColor: "#995b7a",
                    }}
                >
                    DEL
                </BigKey>
                <BigKey
                    style={{
                        backgroundColor: "#995b7a",
                    }}
                >
                    AC
                </BigKey>
            </Row>
            <Row
                style={{
                    justifyContent: "space-between",
                    width: "100%",
                }}
            >
                <BigKey>2</BigKey>
                <BigKey>5</BigKey>
                <BigKey>6</BigKey>
                <BigKey>×</BigKey>
                <BigKey>÷</BigKey>
            </Row>
            <Row
                style={{
                    justifyContent: "space-between",
                    width: "100%",
                }}
            >
                <BigKey>7</BigKey>
                <BigKey>8</BigKey>
                <BigKey>9</BigKey>
                <BigKey keyLog="+" math="sum" keyClick={keyClick}>
                    +
                </BigKey>
                <BigKey>-</BigKey>
            </Row>
            <Row
                style={{
                    justifyContent: "space-between",
                    width: "100%",
                }}
            >
                <BigKey>0</BigKey>
                <BigKey>.</BigKey>
                <BigKey>EXP</BigKey>
                <BigKey>Ans</BigKey>
                <BigKey keyLog="=" math="equals" keyClick={keyClick}>
                    =
                </BigKey>
            </Row>
        </Column>
    )
}
function App() {
    const [log, setLog] = useState("")
    const keyClick = (keyLog: string, math: string) => {
        console.log(keyLog, math)

        if (math === "equals") {
            setLog(evaluate(log))
        }

        if (math.match(/trig|log|number|comma|prnths|ans|sqrt|exponent/)) {
            setLog((prev) => prev + keyLog)
        }

        if (math.match(/sum|sub|multiply|divide|power|sqr|inv/)) {
            setLog((prev) => prev + keyLog)
        }
    }
    return (
        <Root
            style={{
                flexDirection: "column",
                justifyContent: "center",
                alignItems: "center",
            }}
        >
            <Column
                style={{
                    justifyContent: "flex-start",
                    alignItems: "flex-start",
                    borderRadius: 10,
                    backgroundColor: "#4b5568",
                    width: "100%",
                    height: "100%",
                    padding: 21,
                }}
            >
                <View style={{ fontSize: 30, height: 45, fontColor: "white" }}>
                    Pasio
                </View>
                <View style={{ fontSize: 30, height: 70, fontColor: "white" }}>
                    unleash the mathematician within
                </View>
                <Screen log={log}></Screen>
                <KeyBoard keyClick={keyClick}></KeyBoard>
            </Column>
        </Root>
    )
}

const meui = new MEUI(463 + 21 * 2, 743 + 21 * 2)
ReactMEUI.render(<App />, meui)
