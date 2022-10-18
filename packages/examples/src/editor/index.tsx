/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useEffect, useRef } from "react"
import { MEUI } from "@/meui"
import ReactMEUI from "@/react-meui"

import { Root } from "@/components"
import TextArea, { TextAreaHandle } from "@/components/text-area"

function App() {
    const ref = useRef<TextAreaHandle>(null)
    return (
        <Root
            style={{
                flexDirection: "column",
            }}
        >
            <TextArea
                ref={ref}
                style={{
                    border: [1, 1, 1, 1],
                    borderColor: "black",
                    width: 300,
                    height: 100,
                    fontSize: 30,
                }}
            ></TextArea>

            <TextArea
                ref={ref}
                multiline={false}
                style={{
                    margin: [10, 0, 0, 0],
                    border: [1, 1, 1, 1],
                    borderColor: "black",
                    width: 300,
                    height: 100,
                    fontSize: 30,
                }}
            ></TextArea>

            <TextArea
                ref={ref}
                type="password"
                multiline={false}
                style={{
                    margin: [10, 0, 0, 0],
                    border: [1, 1, 1, 1],
                    borderColor: "black",
                    width: 300,
                    height: 100,
                    fontSize: 30,
                }}
            ></TextArea>

            <TextArea
                ref={ref}
                type="password"
                placeholder="Input something"
                multiline={false}
                style={{
                    margin: [10, 0, 0, 0],
                    border: [1, 1, 1, 1],
                    borderColor: "black",
                    width: 300,
                    height: 100,
                    fontSize: 30,
                }}
            ></TextArea>
        </Root>
    )
}

const meui = new MEUI(1920, 1080)
ReactMEUI.render(<App />, meui)
