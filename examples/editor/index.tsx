import React from "react"
import { MEUI, ReactMEUI } from "@/meui"
import { Root } from "@/components"
import TextArea from "@/components/text-area"

function App() {
    return (
        <Root
            style={{
                flexDirection: "column",
            }}
        >
            <TextArea
                style={{
                    width: 1920,
                    height: 1080,
                }}
            ></TextArea>
        </Root>
    )
}

const meui = new MEUI(1920, 1080)
ReactMEUI.render(<App />, meui)
