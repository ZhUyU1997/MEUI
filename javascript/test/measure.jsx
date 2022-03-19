import React, { useEffect } from "react"

export function Test() {
    useEffect(() => {
        meui.debug()
    }, [])

    return <Root style={{
        flexDirection: "row",
        flexWrap: "wrap",
    }}>
        <div style={{
            backgroundColor: "red",

        }}>3213144444444444</div>
    </Root >
}
