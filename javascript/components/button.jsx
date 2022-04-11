import React from "react"
import { Div } from "../meui"

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
