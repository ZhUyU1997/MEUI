import { Property } from "csstype"
import { BOX_STATE } from "NativeMEUI"

// eslint-disable-next-line @typescript-eslint/ban-types
type Length<T = (string & {}) | 0> = number | "auto" | T

export interface MeuiStyle {
    borderRadius?: [number, number, number, number]
    borderColor?: string
    backgroundColor?: Property.BackgroundColor
    fontColor?: Property.Color
    text?: string
    fontSize?: number
    fontFamily?: string
    textAlign?:
        | "center-left"
        | "center-right"
        | "center"
        | "top-left"
        | "top-right"
        | "top-center"
        | "bottom-left"
        | "bottom-right"
        | "bottom-center"
    backgroundImage?: string
    contentImage?: string
    transform?: string
    transformOrigin?:
        | [number, number]
        | ["left" | "center" | "right", "top" | "center" | "bottom"]
    flexWrap?: "nowrap" | "wrap" | "wrap-reverse"
    flexDirection?: "row" | "column" | "row-reverse" | "column-reverse"
    alignItems?: "stretch" | "flex-start" | "center" | "flex-end" | "baseline"
    alignSelf?:
        | "auto"
        | "stretch"
        | "flex-start"
        | "center"
        | "flex-end"
        | "baseline"
    alignContent?:
        | "stretch"
        | "flex-start"
        | "center"
        | "flex-end"
        | "space-between"
        | "space-around"
    justifyContent?:
        | "flex-start"
        | "center"
        | "flex-end"
        | "space-between"
        | "space-around"
    margin?: [number, number, number, number]
    border?: [number, number, number, number]
    padding?: [number, number, number, number]
    overflow?: "visible" | "hidden" | "scroll" | "auto"

    flexBasis?: number
    flexGrow?: number
    flexShrink?: number
    width?: Length
    height?: Length
    minWidth?: Length
    minHeight?: Length
    maxWidth?: Length
    maxHeight?: Length
    marginLeft?: Length
    marginTop?: Length
    marginBottom?: Length
    marginRight?: Length
    marginStart?: Length
    marginEnd?: Length
    paddingLeft?: Length
    paddingTop?: Length
    paddingBottom?: Length
    paddingRight?: Length
    paddingStart?: Length
    paddingEnd?: Length
    borderLeft?: number
    borderTop?: number
    borderBottom?: number
    borderRight?: number
    borderStart?: number
    borderEnd?: number
    borderTopLeftRadius?: number
    borderTopRightRadius?: number
    borderBottomRightRadius?: number
    borderBottomLeftRadius?: number
    left?: Length
    right?: Length
    top?: Length
    bottom?: Length
    zIndex?: number
    FOCUS?: MeuiStyle
    ACTIVE?: MeuiStyle
    HOVER?: MeuiStyle
    SELECT?: MeuiStyle
    DISABLE?: MeuiStyle
    CHECK?: MeuiStyle
}

export type MeuiStyleKey = keyof Omit<MeuiStyle, keyof typeof BOX_STATE>
export type MeuiStyleValue = MeuiStyle[MeuiStyleKey]
