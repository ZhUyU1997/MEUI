/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React from "preact/compat"
import { View, MeuiElementProps } from "@meui/preact"
import { DivElement } from "@meui/core"

const Center = React.forwardRef<DivElement, MeuiElementProps>(
    ({ children, style, ...props }, ref) => {
        return (
            <View
                ref={ref}
                style={{
                    justifyContent: "center",
                    alignItems: "center",
                    alignContent: "center",
                    ...style,
                }}
                {...props}
            >
                {children}
            </View>
        )
    }
)

Center.displayName = "Center"

export default Center
