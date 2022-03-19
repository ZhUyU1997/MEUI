import React from "react"

export function Root({ children, style, ...props }) {
    return <div style={{
        backgroundColor: "#FFFFFFFF",
        width: "100%",
        height: "100%",
        ...style
    }}>{children}</div>
}

export const Center = React.forwardRef(function ({ children, style, ...props }, ref) {
    return <div ref={ref} style={{
        justifyContent: "center",
        alignItems: "center",
        alignContent: "center",
        ...style
    }}
        {...props}
    >{children}</div>
})

export function Row({ children, style, ...props }) {
    return <div style={{
        justifyContent: "center",
        alignItems: "center",
        alignContent: "center",
        flexDirection: "row",
        ...style
    }}
        {...props}
    >{children}</div>
}

export function Column({ children, style, ...props }) {
    return <div style={{
        justifyContent: "center",
        alignItems: "center",
        alignContent: "center",
        flexDirection: "column",
        ...style
    }}
        {...props}
    >{children}</div>
}

export function MaterialDesignIcon({ width = 50, height = 50, icon, color = "black", style }) {
    return <Center
        style={{
            textAlign: "center",
            fontSize: Math.min(width, height),
            width: width,
            height: height,
            flexShrink: 0,
            fontFamily: "MaterialDesignIcons",
            fontColor: color,
            ...style,
        }}>{icon}</Center>
}