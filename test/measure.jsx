import React, { useEffect } from "react"
import { Root } from "@/components"
import { Div } from "@/meui"

export function Test() {
    return (
        <Root
            style={{
                flexDirection: "row",
                flexWrap: "wrap",
            }}
        >
            <Div
                style={{
                    backgroundColor: "red",
                }}
            >
                3213144444444444
            </Div>
        </Root>
    )
}
