import React, { useState, useEffect, useLayoutEffect, useRef } from "react"
import { ReactMEUI } from "../meui"
import { MEUI } from "../meui"
import { Root, Column, Center, MaterialDesignIcon } from "../components"

function Circle({ radius, style, children }) {
    return (
        <div
            style={{
                width: radius * 2,
                height: radius * 2,
                minWidth: radius * 2,
                minHeight: radius * 2,
                justifyContent: "center",
                alignItems: "center",
                borderRadius: [radius, radius, radius, radius],
                ...style,
            }}
        >
            {children}
        </div>
    )
}

function Message({ isSelf, text, time }) {
    const dot = (
        <div
            style={{
                alignItems: "flex-end",
            }}
        >
            <Circle
                radius={5}
                style={{
                    backgroundColor: isSelf
                        ? "rgb(37,211,102)"
                        : "rgb(210,210,210)",
                }}
            ></Circle>
        </div>
    )

    const timehint = (
        <div
            style={{
                flexShrink: 0,
                margin: [4, 15, 0, 15],
                fontSize: 12,
                fontColor: "#333333",
            }}
        >
            {time}
        </div>
    )
    return (
        <div
            style={{
                width: "100%",
                flexDirection: "column",
                alignItems: isSelf ? "flex-end" : "flex-start",
                padding: [7, 0, 7, 0],
                flexShrink: 0,
            }}
        >
            <div
                style={{
                    width: "100%",
                    flexShrink: 0,
                    justifyContent: isSelf ? "flex-end" : "flex-start",
                }}
            >
                {isSelf ? null : dot}
                <div
                    style={{
                        // minWidth: 50,
                        minHeight: 40,
                        maxWidth: 305,
                        flexShrink: 0,
                        borderRadius: [20, 20, 20, 20],
                        padding: [5, 20, 5, 20],
                        backgroundColor: isSelf
                            ? "rgb(37,211,102)"
                            : "rgb(210,210,210)",
                        fontColor: isSelf ? "white" : "#303030",
                        fontSize: 18,
                    }}
                >
                    {text}
                </div>
                {!isSelf ? null : dot}
            </div>
            {time ? timehint : null}
        </div>
    )
}

const data = [
    {
        isSelf: false,
        text: "Hey There!",
    },
    {
        isSelf: false,
        text: "How are you?",
    },
    {
        isSelf: false,
        text: "How was your day?",
        time: "Today, 8.30pm",
    },
    {
        isSelf: true,
        text: "Hello!",
        time: "Today, 8.33pm",
    },
    {
        isSelf: true,
        text: "I am fine and how are you?",
    },
    {
        isSelf: true,
        text: "Today was great!!!",
        time: "Today, 8.34pm",
    },
    {
        isSelf: false,
        text: "I am doing well, Can you meet tomorrow?",
        time: "Today, 8.33pm",
    },
    {
        isSelf: true,
        text: "Yes Sure!",
    },
    {
        isSelf: true,
        text: "At what time?",
        time: "Today, 8.58pm",
    },
]

function Whatsapp() {
    const [message, setMessage] = useState(data)

    const ref = useRef()

    useEffect(() => {
        ref.current.scrollTop = ref.current.scrollTopMax
    }, [message])
    return (
        <Root
            style={{
                width: 390,
                height: 844,
                flexDirection: "column",
                flexWrap: "wrap",
            }}
        >
            <Center
                style={{
                    padding: [40, 19, 23, 19],
                    height: 115,
                    flexShrink: 0,
                }}
            >
                <Circle
                    radius={52 / 2}
                    style={{
                        backgroundColor: "black",
                        marginRight: 11,
                        flexShrink: 0,
                    }}
                ></Circle>
                <Column style={{ flexGrow: 1 }}>
                    <div
                        style={{
                            width: "100%",
                            textAlign: "center-left",
                            fontColor: "#303030",
                            fontSize: 25,
                        }}
                    >
                        Anil
                    </div>
                    <div
                        style={{
                            width: "100%",
                            textAlign: "center-left",
                            fontColor: "#303030",
                            fontSize: 13,
                        }}
                    >
                        Online - Last seen, 2.02pm
                    </div>
                </Column>
                <MaterialDesignIcon
                    width={30}
                    height={30}
                    icon={"\u{F03F2}"}
                    color="rgb(37,211,102)"
                    style={{ marginLeft: 12 }}
                ></MaterialDesignIcon>
                <MaterialDesignIcon
                    width={30}
                    height={30}
                    icon={"\u{F0567}"}
                    color="rgb(37,211,102)"
                    style={{ marginLeft: 12 }}
                ></MaterialDesignIcon>
                <MaterialDesignIcon
                    width={30}
                    height={30}
                    icon={"\u{F01D8}"}
                    color="rgb(37,211,102)"
                    style={{ marginLeft: 12 }}
                ></MaterialDesignIcon>
            </Center>
            <Center
                ref={ref}
                style={{
                    width: "100%",
                    height: 100,
                    flexGrow: 1,
                    flexDirection: "column",
                    justifyContent: "flex-start",
                    overflow: "scroll",
                    padding: [0, 18, 0, 18],
                    // backgroundColor:"red",
                    // flexWrap: "wrap",
                }}
            >
                {message.map((item) => {
                    return (
                        <Message
                            isSelf={item.isSelf}
                            text={item.text}
                            time={item.time}
                        ></Message>
                    )
                })}
            </Center>
            <Center
                style={{
                    flexShrink: 0,
                    marginBottom: 13,
                }}
            >
                <Center
                    style={{
                        padding: [12, 12, 12, 12],
                        borderRadius: [25, 25, 25, 25],
                        width: 308,
                        height: 49,
                        backgroundColor: "rgba(239,246,252, 87%)",
                    }}
                    onClick={() => {
                        setMessage((prev) => {
                            return [
                                ...prev,
                                {
                                    isSelf: true,
                                    text: "At what time?",
                                },
                            ]
                        })
                    }}
                >
                    <MaterialDesignIcon
                        width={24}
                        height={24}
                        icon={"\u{F0785}"}
                        color="#333333"
                        style={{ marginRight: 8 }}
                    ></MaterialDesignIcon>
                    <div
                        style={{
                            flexGrow: 1,
                            fontColor: "#303030",
                            fontSize: 13,
                        }}
                    >
                        Type your message here
                    </div>
                    <MaterialDesignIcon
                        width={24}
                        height={24}
                        icon={"\u{F0100}"}
                        color="#333333"
                        style={{ marginLeft: 15, marginRight: 8 }}
                    ></MaterialDesignIcon>
                    <MaterialDesignIcon
                        width={24}
                        height={24}
                        icon={"\u{F0066}"}
                        color="#333333"
                        style={{ marginLeft: 8 }}
                    ></MaterialDesignIcon>
                </Center>
                <Circle
                    radius={49 / 2}
                    style={{
                        backgroundColor: "rgb(37,211,102)",
                        marginLeft: 8,
                    }}
                >
                    <MaterialDesignIcon
                        width={30}
                        height={30}
                        icon={"\u{F036C}"}
                        color="white"
                    ></MaterialDesignIcon>
                </Circle>
            </Center>
            <Center
                style={{
                    height: 30,
                    flexShrink: 0,
                }}
            >
                <div
                    style={{
                        width: 179,
                        height: 4,
                        backgroundColor: "rgba(169,169,169,87%)",
                    }}
                ></div>
            </Center>
        </Root>
    )
}

export function Main() {
    const meui = new MEUI(390, 844)
    ReactMEUI.render(
        <div>
            <Whatsapp />
        </div>,
        meui
    )
}
