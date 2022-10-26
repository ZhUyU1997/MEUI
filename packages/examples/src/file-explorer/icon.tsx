/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { MaterialDesignIcon } from "@meui-js/components"
import { IconsKey } from "@meui-js/components"
import { MeuiStyle } from "@meui-js/core"

export function Icon({
    icon,
    backgroundColor,
    style,
    onClick,
}: {
    icon: IconsKey | string
    backgroundColor?: string
    style?: MeuiStyle
    onClick?: (o: any) => void
}) {
    return (
        <MaterialDesignIcon
            icon={icon}
            color="rgba(0,0,0, 0.50)"
            style={{
                padding: [10, 10, 10, 10],
                marginRight: 5,
                marginLeft: 5,
                borderRadius: 10,
                fontSize: 18,
                backgroundColor,
                fontColor: "black",
                HOVER: {
                    backgroundColor: "rgba(0,0,0, 0.05)",
                },
                ...style,
            }}
            onClick={onClick}
        ></MaterialDesignIcon>
    )
}
