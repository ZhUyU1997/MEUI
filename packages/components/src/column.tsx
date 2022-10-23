/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { View, MeuiElementProps } from "@meui/preact"

export default function Column({
    children,
    style,
    ...props
}: MeuiElementProps) {
    return (
        <View
            style={{
                justifyContent: "center",
                alignItems: "center",
                alignContent: "center",
                flexDirection: "column",
                ...style,
            }}
            {...props}
        >
            {children}
        </View>
    )
}
