/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { useState } from "preact/compat"
import { createRoot, View } from "@meui-js/preact"

import { animated, useSpring } from "@meui-js/spring"
import { Root, TextArea } from "@meui-js/components"

function EditableItem({ name, children, onChange }) {
    return (
        <View style={{ width: "100%", height: 50, margin: 10 }}>
            <View
                style={{
                    fontColor: "#8C92A4",
                    fontSize: 20,
                    width: 200,
                }}
            >
                {name}
            </View>
            <View
                style={{
                    flexGrow: 1,
                    height: 50,
                    fontColor: "#8C92A4",
                    fontSize: 20,
                    borderRadius: 7,
                    backgroundColor: "#373C4B",
                    alignItems: "center",
                    padding: 10,
                }}
            >
                <TextArea
                    style={{
                        height: 30,
                        fontColor: "#8C92A4",
                        fontSize: 20,
                        width: "100%",
                    }}
                    onChange={onChange}
                >
                    {children}
                </TextArea>
            </View>
        </View>
    )
}
function Editor({ option, setOption }) {
    return (
        <View
            style={{
                backgroundColor: "#181C20",
                width: 300,
                // height: "100%",
                padding: 20,
                flexDirection: "column",
                borderRadius: 20,
                zIndex: 0,
            }}
        >
            {Object.entries(option).map(([k, v]) => (
                <EditableItem
                    key={k}
                    name={k}
                    onChange={(text) => {
                        try {
                            const num = parseFloat(text)

                            if (Number.isNaN(num) === false) {
                                setOption((prev) => ({
                                    ...prev,
                                    [k]: num,
                                }))
                            }
                        } catch (error) {
                            console.log("exception")
                        }
                    }}
                >
                    {v.toString()}
                </EditableItem>
            ))}
        </View>
    )
}

function App() {
    const [option, setOption] = useState({
        mass: 1,
        tension: 170,
        friction: 26,
        precision: 0.01,
        velocity: 0,
    })
    const [styles, api] = useSpring(() => ({
        transform: "scale(1.0)",
    }))

    return (
        <Root>
            <View
                style={{
                    // width: "100%",
                    flexGrow: 1,
                    height: "100%",
                    backgroundColor: "white",
                    justifyContent: "center",
                    alignItems: "center",
                }}
            >
                <animated.View
                    style={{
                        width: 120,
                        height: 120,
                        backgroundColor: "#46e891",
                        borderRadius: 16,
                        textAlign: "center",
                        zIndex: 0,
                        ...styles,
                    }}
                    onMouseOver={() => {
                        api.start({
                            transform: "scale(1.5)",
                            config: option,
                        })
                    }}
                    onMouseOut={() => {
                        api.start({
                            transform: "scale(1.0)",
                            config: option,
                        })
                    }}
                ></animated.View>
            </View>
            <Editor option={option} setOption={setOption}></Editor>
        </Root>
    )
}

createRoot(1000, 500).render(<App />)
