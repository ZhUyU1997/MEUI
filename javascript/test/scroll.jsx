import React, { useEffect } from "react"

function Test() {
    const ref = useRef()
    useEffect(() => {
        setTimeout(() => {
            meui.debug()
        }, 1000)
    }, [])

    useEffect(() => {
        console.log("ref", ref.current)
    }, [])
    return (
        <div
            ref={ref}
            style={{
                backgroundColor: "#FFFFFFFF",
                width: 500,
                height: 400,
                padding: [20, 20, 20, 20],
                border: [20, 20, 20, 20],
                borderColor: "blue",
                margin: [20, 20, 20, 20],
                // width: 1000,
                // height: 100,
                flexDirection: "row",
                flexWrap: "wrap",
                overflow: "scroll",
            }}
        >
            {Array(100)
                .fill(0)
                .map(() => {
                    return (
                        <div
                            style={{
                                textAlign: "center",
                                fontSize: 20,
                                width: Math.round(Math.random() * 100),
                                height: Math.round(Math.random() * 100),
                                backgroundColor: `rgba(${Math.round(
                                    Math.random() * 255
                                )}, ${Math.round(
                                    Math.random() * 255
                                )}, ${Math.round(Math.random() * 255)}, 1)`,
                            }}
                        ></div>
                    )
                })}
        </div>
    )
}
