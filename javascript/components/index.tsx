import React from "react"
import { Div } from "../meui"
import { DivElement } from "../meui/div"
import { MeuiStyle } from "../meui/style"

type RootProps = {
    children?: React.ReactNode
    style?: MeuiStyle
}

export const Root: React.FC<RootProps> = ({ children, style, ...props }) => {
    return (
        <Div
            style={{
                backgroundColor: "#FFFFFFFF",
                width: "100%",
                height: "100%",
                ...style,
            }}
        >
            {children}
        </Div>
    )
}

type CenterProps = {
    children?: React.ReactNode
    style?: MeuiStyle
}

export const Center = React.forwardRef<DivElement, CenterProps>(
    ({ children, style, ...props }, ref) => {
        return (
            <Div
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
            </Div>
        )
    }
)

Center.displayName = "Center"

type RowProps = {
    children?: React.ReactNode
    style?: MeuiStyle
}

export const Row: React.FC<RootProps> = ({ children, style, ...props }) => {
    return (
        <Div
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
        </Div>
    )
}

type ColumnProps = {
    children?: React.ReactNode
    style?: MeuiStyle
}

export const Column: React.FC<ColumnProps> = ({
    children,
    style,
    ...props
}) => {
    return (
        <Div
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
        </Div>
    )
}

type IconProps = {
    width: number
    height: number
    icon: string
    color: MeuiStyle["fontColor"]
    style: MeuiStyle
}

export const MaterialDesignIcon: React.FC<IconProps> = ({
    width = 50,
    height = 50,
    icon,
    color = "black",
    style,
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
        >
            {icon}
        </Center>
    )
}

export { default as Button } from "./button"
