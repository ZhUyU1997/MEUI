/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { View, MeuiElementProps } from "@meui-js/preact"

export default function Row({ children, style, ...props }: MeuiElementProps) {
    return (
        <View
            style={{
                justifyContent: "center",
                alignItems: "center",
                alignContent: "center",
                flexDirection: "row",
                ...style,
            }}
            {...props}
        >
            {children}
        </View>
    )
}
