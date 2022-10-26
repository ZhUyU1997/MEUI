/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useState } from "preact/compat"
import { View } from "@meui-js/preact"
import { createRoot } from "@meui-js/preact"
import { Root, Button } from "@meui-js/components"
import { parseTransform } from "@meui-js/core"

console.log(parseTransform(" scale( 1.2 ,1.2)  scale(1.3)  scale(1.4)  "))
console.log(
    parseTransform(" scaleX( 1.2) scaleY(1.2)  scale(1.3)  scale(1.4)  ")
)
console.log(parseTransform("rotate(0)"))
console.log(parseTransform("rotate(1turn)"))
console.log(parseTransform("rotate(3.14rad)"))
console.log(parseTransform("rotate(100grad)"))
console.log(parseTransform("rotate(90deg)"))
console.log(parseTransform("rotate(-90deg)"))

const transformArr = [
    "matrix(1.0, 2.0, 3.0, 4.0, 5.0, 6.0)",
    "translate(120, 120)",
    "scale(2, 0.5)",
    "rotate(0.5turn)",
    "skew(30deg, 20deg)",
    "scale(0.5) translate(-100,-100)",
]

function App() {
    const [index, setIndex] = useState(0)
    return (
        <Root
            style={{
                justifyContent: "center",
                alignItems: "center",
                flexDirection: "column",
            }}
        >
            <View
                style={{
                    width: "100%",
                    height: 100,
                    flexGrow: 1,
                    justifyContent: "center",
                    alignItems: "center",
                    flexDirection: "column",
                }}
            >
                <View
                    style={{
                        width: 200,
                        height: 200,
                        backgroundColor: "red",

                        transform: transformArr[index],
                    }}
                >
                    AAAAAAAAAABBBBBBBBBBBBBBB
                </View>
            </View>

            <Button
                style={{
                    width: 100,
                    height: 50,
                    margin: [30, 30, 30, 30],
                }}
                onClick={() => {
                    setIndex((index + 1) % transformArr.length)
                }}
            >
                Touch
            </Button>
        </Root>
    )
}

createRoot(390, 844).render(<App />)
