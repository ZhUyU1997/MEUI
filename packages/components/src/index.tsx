/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React from "react"
import { View, DivElement, MeuiElementAttribule, MeuiStyle } from "@/meui"
import { getIconHex, IconsKey } from "./icons"

interface RootProps extends MeuiElementAttribule {
    style?: MeuiStyle
}

export const Root: React.FC<RootProps> = ({ children, style, ...props }) => {
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

interface CenterProps extends MeuiElementAttribule {
    children?: React.ReactNode
    style?: MeuiStyle
}

export const Center = React.forwardRef<DivElement, CenterProps>(
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

interface RowProps extends MeuiElementAttribule {
    children?: React.ReactNode
    style?: MeuiStyle
}

export const Row: React.FC<RowProps> = ({ children, style, ...props }) => {
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

interface ColumnProps extends MeuiElementAttribule {
    children?: React.ReactNode
}

export const Column: React.FC<ColumnProps> = ({
    children,
    style,
    ...props
}) => {
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

interface IconProps extends MeuiElementAttribule {
    width?: number
    height?: number
    icon: IconsKey | (string & {})
    color?: MeuiStyle["fontColor"]
}

export const MaterialDesignIcon: React.FC<IconProps> = ({
    width = 50,
    height = 50,
    icon = "\u{F0027}",
    color = "black",
    style,
    ...props
}) => {
    return (
        <Center
            style={{
                textAlign: "center",
                fontSize: Math.min(width, height),
                width: width,
                height: height,
                flexShrink: 0,
                fontFamily: "MaterialDesignIcons",
                fontColor: color,
                ...style,
            }}
            {...props}
        >
            {getIconHex(icon)}
        </Center>
    )
}

export { default as Button } from "./button"
