import React, { useState, useEffect, useLayoutEffect, useRef } from "react"
import { MEUI, ReactMEUI } from "../meui"
import { Root, Column, Center, MaterialDesignIcon } from "../components"

function DropDownList({ option = [], style }) {
    return (
        <div
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
            {option.map((text) => {
                return (
                    <Center
                        key={text}
                        style={{
                            width: 163,
                            height: 36,
                            marginTop: 4,
                            marginBottom: 4,
                        }}
                    >
                        <div
                            style={{
                                fontColor: "#000000",
                                fontSize: 16,
                                width: 105,
                            }}
                        >
                            {text}
                        </div>
                        <div style={{ marginLeft: 8, width: 18 }}></div>
                    </Center>
                )
            })}
        </div>
    )
}
function DropDown({ option = [], style }) {
    const [open, setOpen] = useState(false)
    return (
        <stack
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
                <div
                    style={{
                        fontColor: "#5533FF",
                        fontSize: 16,
                        width: 105,
                    }}
                >
                    Pick an option
                </div>
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
        </stack>
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

export function Main() {
    const meui = new MEUI(248, 360)
    ReactMEUI.render(<App />, meui)
}
