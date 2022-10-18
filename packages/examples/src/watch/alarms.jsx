/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useEffect, useState } from "react"
import { View, MEUI, Stack } from "@/meui"
import ReactMEUI from "@/react-meui"
import { Root } from "@/components"

const SCREEN_WIDTH = 396
const SCREEN_HEIGHT = 484

function List() {}

function ListItem({ style, ...props }) {
    return (
        <View
            style={{
                width: "100%",
                backgroundColor: "#242424",
                borderRadius: 18,
                paddingTop: 12,
                paddingBottom: 12,
                paddingLeft: 20,
                paddingRight: 20,
                justifyContet: "center",
                alignItems: "center",
                ...style,
            }}
            {...props}
        ></View>
    )
}

function AlarmsItem({ mainText, subText, initStatus }) {
    const [isOn, setIsOn] = useState(initStatus)
    return (
        <ListItem
            style={{
                height: 108,
                minHeight: 108,
            }}
            onClick={() => {
                setIsOn((prev) => !prev)
            }}
        >
            <View
                style={{
                    width: 124 * 2,
                    flexDirection: "column",
                }}
            >
                <View
                    style={{
                        height: 24 * 2,
                        fontSize: 20 * 2,
                        fontColor: isOn ? "white" : "rgba(255,255,255,0.4)",
                    }}
                >
                    {mainText}
                </View>
                <View
                    style={{
                        height: 20 * 2,
                        fontSize: 15 * 2,
                        fontColor: isOn ? "white" : "rgba(255,255,255,0.4)",
                    }}
                >
                    {subText}
                </View>
            </View>
            <View
                style={{
                    width: 72,
                    height: 44,
                    borderRadius: 22,
                    backgroundColor: isOn ? "#04DE71" : "#9BA0AA",
                    padding: 2,
                    justifyContent: isOn ? "flex-end" : "flex-start",
                }}
            >
                <View
                    style={{
                        width: 40,
                        height: 40,
                        borderRadius: 20,
                        backgroundColor: "white",
                        padding: 2,
                    }}
                ></View>
            </View>
        </ListItem>
    )
}
function App() {
    return (
        <Root>
            <View
                style={{
                    backgroundColor: "black",
                    borderRadius: 40,
                    width: "100%",
                    flexDirection: "column",
                    overflow: "scroll",
                    rowGap: 10,
                    padding: 20,
                }}
            >
                <AlarmsItem
                    mainText="9:00AM"
                    subText="Alarm"
                    initStatus={true}
                ></AlarmsItem>
                <AlarmsItem
                    mainText="10:00AM"
                    subText="Alarm"
                    initStatus={false}
                ></AlarmsItem>
                <AlarmsItem
                    mainText="11:30AM"
                    subText="Alarm"
                    initStatus={false}
                ></AlarmsItem>
                <AlarmsItem
                    mainText="12:00AM"
                    subText="Alarm"
                    initStatus={false}
                ></AlarmsItem>
                <AlarmsItem
                    mainText="12:30AM"
                    subText="Alarm"
                    initStatus={false}
                ></AlarmsItem>
            </View>
        </Root>
    )
}

const meui = new MEUI(396, 484)
ReactMEUI.render(<App />, meui)
