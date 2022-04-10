import React, { useEffect, useState } from "react"

export function Test() {
    const [dollar, setDollar] = useState(0)
    const [rmb, setRMB] = useState(0)

    useEffect(() => {
        setRMB(6.32 * dollar)
    }, [dollar])

    return (
        <Root>
            <div
                style={{
                    justifyContent: "center",
                    alignItems: "center",
                    alignContent: "center",
                    textAlign: "center",
                    fontSize: 20,
                    width: 300,
                    height: 200,
                }}
            >
                {dollar}美元 = {rmb}人民币
            </div>
            <Button
                type="primary"
                style={{ width: 200, height: 70 }}
                onClick={() => {
                    setDollar((count) => count + 1)
                }}
            >
                Click Me
            </Button>
        </Root>
    )
}
