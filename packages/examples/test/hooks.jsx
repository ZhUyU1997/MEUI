/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useEffect, useState } from "preact/compat"
import { View } from "@meui-js/preact"
import { Root, Button } from "@meui-js/components"
export function Test() {
    const [dollar, setDollar] = useState(0)
    const [rmb, setRMB] = useState(0)

    useEffect(() => {
        setRMB(6.32 * dollar)
    }, [dollar])

    return (
        <Root>
            <View
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
            </View>
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
