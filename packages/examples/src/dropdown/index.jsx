/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { useState } from "preact/compat"
import { createRoot, View, Stack } from "@meui/preact"
import { Root, Center, MaterialDesignIcon } from "@meui/components"

function DropDownList({ option = [], style }) {
    return (
        <View
            style={{
                width: 163,
                backgroundColor: "white",
                borderColor: "#00000019",
                borderRadius: [8, 8, 8, 8],
                padding: [8, 0, 8, 0],
                flexDirection: "column",
                zIndex: 1,
                ...style,
            }}
        >
            {option.map((text, index) => {
                return (
                    <Center
                        key={text + index}
                        style={{
                            width: 163,
                            height: 36,
                            marginTop: 4,
                            marginBottom: 4,
                        }}
                    >
                        <View
                            style={{
                                fontColor: "#000000",
                                fontSize: 16,
                                width: 105,
                            }}
                        >
                            {text}
                        </View>
                        <View style={{ marginLeft: 8, width: 18 }}></View>
                    </Center>
                )
            })}
        </View>
    )
}
function DropDown({ option = [], style }) {
    const [open, setOpen] = useState(false)
    return (
        <Stack
            style={{
                width: 163,
                height: 36,
                ...style,
            }}
        >
            <Center
                style={{
                    width: 163,
                    height: 36,
                    backgroundColor: "white",
                    borderColor: "#00000019",
                    borderRadius: [8, 8, 8, 8],
                }}
                onClick={() => {
                    setOpen((prev) => !prev)
                }}
            >
                <View
                    style={{
                        fontColor: "#5533FF",
                        fontSize: 16,
                        width: 105,
                    }}
                >
                    Pick an option
                </View>
                <MaterialDesignIcon
                    width={18}
                    height={18}
                    icon={open ? "\u{F0143}" : "\u{F0140}"}
                    color={"#7257FF"}
                    style={{ marginLeft: 8 }}
                ></MaterialDesignIcon>
            </Center>

            {open ? (
                <DropDownList
                    option={option}
                    style={{
                        top: 36 + 8,
                    }}
                />
            ) : null}
        </Stack>
    )
}
function App() {
    return (
        <Root
            style={{
                flexDirection: "column",
                flexWrap: "wrap",
                justifyContent: "center",
                alignItems: "center",
                backgroundColor: "#E5E5E5",
            }}
        >
            <DropDown
                option={[
                    "Karol Wojtyła",
                    "John Paul II",
                    "John Paul II",
                    "Polish Pope",
                ]}
                style={{}}
            ></DropDown>
            <DropDown
                option={[
                    "Karol Wojtyła",
                    "John Paul II",
                    "John Paul II",
                    "Polish Pope",
                ]}
                style={{ marginTop: 20 }}
            ></DropDown>
        </Root>
    )
}

createRoot(248, 360).render(<App />)
