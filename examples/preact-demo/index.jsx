import { h, render } from "preact"
import React, { useEffect, useState } from "preact/compat"

// Tells babel to use h for JSX. It's better to configure this globally.
// See https://babeljs.io/docs/en/babel-plugin-transform-react-jsx#usage
// In tsconfig you can specify this with the jsxFactory
/** @jsx h */

// create our tree and append it to document.body:

import { Div, MEUI, createElement } from "@/meui"
import { Text } from "@/meui"

const meui = new MEUI(300, 300)

globalThis.document = {
    createElement(nodeType, option) {
        return createElement(nodeType)
    },
    createTextNode(data) {
        return new Text(data)
    },
}

function Main() {
    return (
        <Div
            style={{
                width: "100%",
                height: "100%",
                backgroundColor: "white",
                fontSize: 50,
                // fontColor:"black"
            }}
            onClick={() => {
                console.log("click")
            }}
        >
            Hello MEUI
        </Div>
    )
}

const style_name = [
    "primary",
    "secondary",
    "success",
    "info",
    "warning",
    "danger",
    "light",
    "dark",
]
const style_color = [
    [
        ["#536de6ff", "#ffffffff", "#00000000"],
        ["#3a57e2ff", "#ffffffff", "#00000000"],
        ["#2647e0ff", "#ffffffff", "#536de660"],
    ],
    [
        ["#6c757dff", "#ffffffff", "#00000000"],
        ["#5a6268ff", "#ffffffff", "#00000000"],
        ["#545b62ff", "#ffffffff", "#6c757d60"],
    ],
    [
        ["#10c469ff", "#ffffffff", "#00000000"],
        ["#0da156ff", "#ffffffff", "#00000000"],
        ["#0c9550ff", "#ffffffff", "#10c46960"],
    ],
    [
        ["#35b8e0ff", "#ffffffff", "#00000000"],
        ["#20a6cfff", "#ffffffff", "#00000000"],
        ["#1e9dc4ff", "#ffffffff", "#35b8e060"],
    ],
    [
        ["#f9c851ff", "#ffffffff", "#00000000"],
        ["#f8bc2cff", "#ffffffff", "#00000000"],
        ["#f7b820ff", "#ffffffff", "#f9c85160"],
    ],
    [
        ["#ff5b5bff", "#ffffffff", "#00000000"],
        ["#ff3535ff", "#ffffffff", "#00000000"],
        ["#ff2828ff", "#ffffffff", "#ff5b5b60"],
    ],
    [
        ["#eef2f7ff", "#323a46ff", "#00000000"],
        ["#d4deebff", "#323a46ff", "#00000000"],
        ["#cbd7e7ff", "#323a46ff", "#eef2f760"],
    ],
    [
        ["#323a46ff", "#ffffffff", "#00000000"],
        ["#222830ff", "#ffffffff", "#00000000"],
        ["#1d2128ff", "#ffffffff", "#323a4660"],
    ],
]

export default function Button({
    type = "primary",
    children,
    style,
    ...props
}) {
    const index = style_name.indexOf(type)
    const list = style_color[index]
    return (
        <Div
            style={{
                marginBottom: 10,
                border: [1, 1, 1, 1],
                borderRadius: [20, 20, 20, 20],
                backgroundColor: list[0][0],
                fontColor: list[0][1],
                borderColor: list[0][2],
                textAlign: "center",
                HOVER: {
                    backgroundColor: list[1][0],
                    fontColor: list[1][1],
                    borderColor: list[1][2],
                },
                ACTIVE: {
                    backgroundColor: list[2][0],
                    fontColor: list[2][1],
                    borderColor: list[2][2],
                },
                ...style,
            }}
            {...props}
        >
            {children}
        </Div>
    )
}

const Root = ({ children, style, ...props }) => {
    return (
        <Div
            style={{
                backgroundColor: "#FFFFFFFF",
                width: "100%",
                height: "100%",
                ...style,
            }}
            {...props}
        >
            {children}
        </Div>
    )
}

export function Test() {
    const [dollar, setDollar] = useState(0)
    const [rmb, setRMB] = useState(0)

    useEffect(() => {
        setRMB(6.32 * dollar)
    }, [dollar])

    return (
        <Root
            style={{
                flexDirection: "column",
                alignItems: "center",
                justifyContent: "center",
            }}
        >
            <Div
                style={{
                    justifyContent: "center",
                    alignItems: "center",
                    alignContent: "center",
                    textAlign: "center",
                    fontSize: 20,
                    width: 300,
                    height: 200,
                }}
            >
                {dollar}美元 = {rmb}人民币
            </Div>
            <Button
                type="primary"
                style={{
                    width: 200,
                    height: 70,
                }}
                onClick={() => {
                    setDollar((count) => {
                        return count + 1
                    })
                }}
            >
                Click Me
            </Button>
        </Root>
    )
}

render(<Test />, meui.getRoot())
