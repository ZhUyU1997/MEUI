import { MeuiStyle } from "@meui-js/core"
import { MeuiElementProps } from "@meui-js/preact"
import { getIconHex, IconsKey } from "./icons-key"
import Center from "./center"

interface IconProps extends MeuiElementProps {
    width?: number
    height?: number
    icon: IconsKey | (string & {})
    color?: MeuiStyle["fontColor"]
}

export default function MaterialDesignIcon({
    width = 50,
    height = 50,
    icon = "\u{F0027}",
    color = "black",
    style,
    ...props
}: IconProps) {
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
