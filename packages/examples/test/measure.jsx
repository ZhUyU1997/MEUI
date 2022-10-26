/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, { useEffect } from "preact/compat"
import { Root } from "@meui-js/components"
import { View } from "@meui-js/preact"

export function Test() {
    return (
        <Root
            style={{
                flexDirection: "row",
                flexWrap: "wrap",
            }}
        >
            <View
                style={{
                    backgroundColor: "red",
                }}
            >
                3213144444444444
            </View>
        </Root>
    )
}
