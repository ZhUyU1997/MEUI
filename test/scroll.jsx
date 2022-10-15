/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useEffect, useRef } from "react"
import { View } from "@/meui"

function Test() {
    const ref = useRef()

    useEffect(() => {
        console.log("ref", ref.current)
    }, [])
    return (
        <View
            ref={ref}
            style={{
                backgroundColor: "#FFFFFFFF",
                width: 500,
                height: 400,
                padding: [20, 20, 20, 20],
                border: [20, 20, 20, 20],
                borderColor: "blue",
                margin: [20, 20, 20, 20],
                // width: 1000,
                // height: 100,
                flexDirection: "row",
                flexWrap: "wrap",
                overflow: "scroll",
            }}
        >
            {Array(100)
                .fill(0)
                .map((index) => {
                    return (
                        <View
                            key={index}
                            style={{
                                textAlign: "center",
                                fontSize: 20,
                                width: Math.round(Math.random() * 100),
                                height: Math.round(Math.random() * 100),
                                backgroundColor: `rgba(${Math.round(
                                    Math.random() * 255
                                )}, ${Math.round(
                                    Math.random() * 255
                                )}, ${Math.round(Math.random() * 255)}, 1)`,
                            }}
                        ></View>
                    )
                })}
        </View>
    )
}
