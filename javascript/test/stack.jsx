import React, { useState, useEffect, useLayoutEffect, useRef } from "react"
import ReactMEUI from "../ReactMEUI"
import Button from "../button"
import { MEUI } from "../meui"
import { Root, Column, Center, MaterialDesignIcon } from "../components"

function App() {
    return <Root style={{
    }}>
        <stack
            style={{
                width: 390,
                height: 844,
            }}
        >
            <div style={{
                width: 100,
                height: 100,
                backgroundColor: "red",
            }}></div>
            <div style={{
                width: 100,
                height: 100,
                backgroundColor: "green",
                top: 10,
                left: 10,
            }}></div>
            <div style={{
                width: 100,
                height: 100,
                backgroundColor: "red",
                top: 20,
                left: 20,
            }}></div>
            <div style={{
                width: 100,
                height: 100,
                backgroundColor: "green",
                top: 30,
                left: 30,
            }}></div>
            <div style={{
                width: 100,
                height: 100,
                backgroundColor: "red",
                top: 40,
                left: 40,
            }}></div>
            <div style={{
                width: 100,
                height: 100,
                backgroundColor: "red",
                right: 10,
            }}></div>

            <div style={{
                width: 100,
                height: 100,
                backgroundColor: "red",
                bottom: 10,
            }}></div>
        </stack>
    </Root >
}

export function Main() {
    const meui = new MEUI(390, 844)
    ReactMEUI.render(<div>
        <App />
    </div>, meui)
}