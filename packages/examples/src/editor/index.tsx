/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useEffect, useRef } from "preact/compat"
import { MEUI } from "@meui-js/core"
import { createRoot } from "@meui-js/preact"

import { Root } from "@meui-js/components"
import { TextArea } from "@meui-js/components"
import type { TextAreaHandle } from "@meui-js/components"

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

createRoot(1920, 1080).render(<App />)
