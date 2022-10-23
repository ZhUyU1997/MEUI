/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { View, MeuiElementProps } from "@meui/preact"

export default function Root({ children, style, ...props }: MeuiElementProps) {
    return (
        <View
            style={{
                backgroundColor: "#FFFFFFFF",
                width: "100%",
                height: "100%",
                ...style,
            }}
            {...props}
        >
            {children}
        </View>
    )
}
