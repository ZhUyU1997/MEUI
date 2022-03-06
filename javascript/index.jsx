import "./polyfill"
import React, { useState } from "react"
import ReactMEUI from "./ReactMEUI"
import Button from "./button"
import { MEUI } from "./meui"
function Root({ children, ...props }) {
    return <div style={{
        backgroundColor: "#FFFFFFFF",
        justifyContent: "center",
        alignItems: "center",
        alignContent: "center",
        flexDirection: "column",
        width: "100%",
        height: "100%",
    }}>{children}</div>
}

function TestHooks() {
    const [count, setCount] = useState(0)

    return <Root>
        <div
            style={{
                justifyContent: "center",
                alignItems: "center",
                alignContent: "center",
                textAlign: "center",
                fontSize: 20,
                width: 300,
                height: 200,
            }}>
            You clicked below {count} times
        </div>
        <Button
            type="primary"
            style={{ width: 200, height: 70 }}
            onMouseDown={() => {
                console.log("onMouseDown")
                setCount(count => count + 1)
            }}
        >Click Me</Button>
    </Root>
}

const meui = new MEUI(300, 300)

ReactMEUI.render(<TestHooks />, meui)
