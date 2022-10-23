/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { useEffect, useState } from "preact/compat"
import { createRoot, View } from "@meui/preact"
import { Root } from "@meui/components"

function App() {
    const [time, setTime] = useState(0)
    const [status, setStatus] = useState("init")

    useEffect(() => {
        if (status === "running") {
            const id = setInterval(() => {
                setTime((prev) => prev + 10)
            }, 10)

            return () => {
                clearInterval(id)
            }
        }

        if (status === "init") setTime(0)
    }, [status])
    return (
        <Root>
            <View
                style={{
                    backgroundColor: "black",
                    borderRadius: 40,
                    width: "100%",
                    flexDirection: "column",
                    padding: 20,
                }}
            >
                <View
                    style={{
                        fontColor: "white",
                        fontSize: 100,
                        textAlign: "center",
                        flexGrow: 1,
                    }}
                >
                    {(time / 1000).toFixed(0).padStart(2, "0")}:
                    {(time % 1000).toString().padStart(3, "0").slice(0, 2)}
                </View>
                <View
                    style={{
                        width: "100%",
                        padding: 12,
                        columnGap: 6,
                    }}
                >
                    <View
                        style={{
                            width: 168,
                            height: 88,
                            borderRadius: [44, 8, 8, 44],
                            backgroundColor: "#48494B",
                            fontColor: "white",
                            fontSize: 30,
                            textAlign: "center",
                        }}
                        onClick={() => {
                            setStatus("init")
                            setTime(0)
                        }}
                    >
                        Cancel
                    </View>
                    <View
                        style={{
                            width: 168,
                            height: 88,
                            borderRadius: [8, 44, 44, 8],
                            backgroundColor: "#FF9500",
                            fontColor: "black",
                            fontSize: 30,
                            textAlign: "center",
                        }}
                        onClick={() => {
                            if (status == "running") setStatus("pause")
                            else setStatus("running")
                        }}
                    >
                        {status == "running" ? "Pause" : "Start"}
                    </View>
                </View>
            </View>
        </Root>
    )
}

createRoot(396, 484).render(<App />)
