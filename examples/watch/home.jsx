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

function rect_invalid(rect) {
    return rect.w < 0.0 || rect.h < 0.0
}

function rect_intersect(rect, source) {
    if (rect_invalid(source))
        return {
            ...rect,
        }

    if (rect_invalid(rect)) {
        return {
            ...source,
        }
    }

    const l = Math.max(rect.x, source.x)
    const t = Math.max(rect.y, source.y)
    const r = Math.min(rect.x + rect.w, source.x + source.w)
    const b = Math.min(rect.y + rect.h, source.y + source.h)

    return { x: l, y: t, w: r - l, h: b - t }
}

const SCREEN_WIDTH = 312
const SCREEN_HEIGHT = 390

const ICON_SIZE = 75
const ICON_MAGIN = 10

function CalcSize3(x, y) {
    if (
        x * x + y * y >
        10 * (ICON_SIZE + ICON_MAGIN) * (ICON_SIZE + ICON_MAGIN)
    )
        return [0, 0, 0]
    // if ( > (7 * (ICON_SIZE + ICON_MAGIN) * (ICON_SIZE + ICON_MAGIN))) return [0, 0, 0]
    const R = (1.4 * SCREEN_WIDTH) / 2.0

    function Convert(x, y) {
        const D = R

        const a_h = Math.atan2(x, D)
        const a_v = Math.atan2(Math.sqrt(D * D + x * x), -y)

        const x1 = R * Math.sin(a_v) * Math.cos(a_h)
        const y1 = R * Math.sin(a_h) * Math.sin(a_v)
        const z1 = R * Math.cos(a_v)

        return [x1, y1, z1]
    }

    const [x1, y1, z1] = Convert(x, y)
    const [, y_0, z_0] = Convert(x - ICON_SIZE / 2, y - ICON_SIZE / 2)
    const [, y_1, z_1] = Convert(x + ICON_SIZE / 2, y + ICON_SIZE / 2)
    const [, y_2, z_2] = Convert(x + ICON_SIZE / 2, y - ICON_SIZE / 2)
    const [, y_3, z_3] = Convert(x - ICON_SIZE / 2, y + ICON_SIZE / 2)
    const a =
        Math.min(
            Math.abs(y_1 - y_0),
            Math.abs(z_1 - z_0),
            Math.abs(y_3 - y_2),
            Math.abs(z_2 - z_3)
        ) / ICON_SIZE
    const x2 = y1
    const y2 = -z1
    return [x2, y2, a]
}

function App() {
    const [icons] = useState(() =>
        Array(12 * 12)
            .fill(null)
            .map((_, index) => {
                let row = Math.floor(index / 12)
                let col = Math.floor(index % 12)
                const odd = Math.floor(row % 2) == 1
                row = ((row - 6) * Math.sqrt(3)) / 2
                col = col - 3 + (odd ? 0.5 : 0)
                // const dis = Math.sqrt(3) / 2
                return {
                    color: `rgb(${Math.floor(
                        255 - Math.sqrt(row * row + col * col) * 80
                    )},${Math.floor(
                        Math.sqrt(row * row + col * col) * 80
                    )},${Math.floor(Math.sqrt(row * row + col * col) * 80)})`,
                    row,
                    col,
                }
            })
    )

    const [offset, setOffset] = useState({
        x: 0,
        y: 0,
    })
    return (
        <Root>
            <Div
                style={{
                    backgroundColor: "black",
                    borderRadius: 40,
                    width: "100%",
                    overflow: "hidden",
                }}
                onMouseMove={(e) => {
                    setOffset({
                        x: e.screenX - SCREEN_WIDTH / 2,
                        y: e.screenY - SCREEN_HEIGHT / 2,
                    })
                }}
            >
                <Stack>
                    {icons.map((item, index) => {
                        const x = offset.x + item.col * (ICON_SIZE + ICON_MAGIN)
                        const y = offset.y + item.row * (ICON_SIZE + ICON_MAGIN)
                        const [x1, y1, scale] = CalcSize3(x, y)

                        const x2 = x1 + SCREEN_WIDTH / 2
                        const y2 = y1 + SCREEN_HEIGHT / 2

                        return (
                            <Div
                                key={`${item.row} ${item.col}`}
                                style={{
                                    width: ICON_SIZE,
                                    height: ICON_SIZE,
                                    left: x2 - ICON_SIZE / 2,
                                    top: y2 - ICON_SIZE / 2,
                                    transform: `scale(${scale})`,
                                }}
                            >
                                <Div
                                    key={`${item.row} ${item.col}`}
                                    style={{
                                        width: ICON_SIZE,
                                        height: ICON_SIZE,
                                        borderRadius: ICON_SIZE / 2,
                                        backgroundColor: item.color,
                                        // transform:`scale(${size})`
                                    }}
                                >
                                    {/* {size} */}
                                    {/* {`${item.row} ${item.col}`} */}
                                </Div>
                            </Div>
                        )
                    })}
                </Stack>
            </Div>
        </Root>
    )
}

const meui = new MEUI(312, 390)
ReactMEUI.render(<App />, meui)
