import { Property } from "csstype"
import { BOX_STATE } from "NativeMEUI"
import valueParser from "postcss-value-parser"

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

export type Matrix2D = [number, number, number, number, number, number]
function matrix_multiply(_a: Matrix2D, _b: Matrix2D): Matrix2D {
    let a, b
    {
        const [m00, m10, m01, m11, m02, m12] = _a
        a = { m00, m10, m01, m11, m02, m12 }
    }

    {
        const [m00, m10, m01, m11, m02, m12] = _b
        b = { m00, m10, m01, m11, m02, m12 }
    }

    const m00 = a.m00 * b.m00 + a.m10 * b.m01
    const m10 = a.m00 * b.m10 + a.m10 * b.m11
    const m01 = a.m01 * b.m00 + a.m11 * b.m01
    const m11 = a.m01 * b.m10 + a.m11 * b.m11
    const m02 = a.m02 * b.m00 + a.m12 * b.m01 + b.m02
    const m12 = a.m02 * b.m10 + a.m12 * b.m11 + b.m12

    return [m00, m10, m01, m11, m02, m12]
}

const Transform = {
    identity(): Matrix2D {
        return [1.0, 0.0, 0.0, 1.0, 0.0, 0.0]
    },
    translate(m: Matrix2D, x: number, y: number): Matrix2D {
        return matrix_multiply(m, [1.0, 0.0, 0.0, 1.0, x, y])
    },
    translateX(m: Matrix2D, x: number): Matrix2D {
        return matrix_multiply(m, [1.0, 0.0, 0.0, 1.0, x, 0])
    },
    translateY(m: Matrix2D, y: number): Matrix2D {
        return matrix_multiply(m, [1.0, 0.0, 0.0, 1.0, 0, y])
    },
    rotate(m: Matrix2D, radians: number): Matrix2D {
        const c = Math.cos(radians)
        const s = Math.sin(radians)
        return matrix_multiply(m, [c, s, -s, c, 0.0, 0.0])
    },
    scale(m: Matrix2D, x: number, y: number): Matrix2D {
        return matrix_multiply(m, [x, 0.0, 0.0, y, 0.0, 0.0])
    },
    scaleX(m: Matrix2D, x: number): Matrix2D {
        return matrix_multiply(m, [x, 0.0, 0.0, 1, 0.0, 0.0])
    },
    scaleY(m: Matrix2D, y: number): Matrix2D {
        return matrix_multiply(m, [1, 0.0, 0.0, y, 0.0, 0.0])
    },
    skew(m: Matrix2D, x: number, y: number): Matrix2D {
        return matrix_multiply(m, [
            1.0,
            Math.tan(y),
            Math.tan(x),
            1.0,
            0.0,
            0.0,
        ])
    },
    skewX(m: Matrix2D, x: number) {
        return matrix_multiply(m, [1.0, 0, Math.tan(x), 1.0, 0.0, 0.0])
    },
    skewY(m: Matrix2D, y: number) {
        return matrix_multiply(m, [1.0, Math.tan(y), 0.0, 1.0, 0.0, 0.0])
    },
}

export function parseLength(value: string): number {
    if (value.endsWith("px")) {
        const num = parseFloat(value.slice(0, -"px".length))
        return num
    } else {
        const num = parseFloat(value)
        return num
    }
}

export function parseAngle(value: string) {
    if (value.endsWith("deg")) {
        const num = parseFloat(value.slice(0, -"deg".length))
        return num * (Math.PI / 180)
    } else if (value.endsWith("grad")) {
        const num = parseFloat(value.slice(0, -"grad".length))
        return num * (Math.PI / 200)
    } else if (value.endsWith("rad")) {
        const num = parseFloat(value.slice(0, -"rad".length))
        return num
    } else if (value.endsWith("turn")) {
        const num = parseFloat(value.slice(0, -"turn".length))
        return num * (2 * Math.PI)
    } else {
        const num = parseFloat(value)

        if (num !== 0.0) {
            throw new Error("should be zero")
        }
        return num
    }
}

export function parseTransform(value: string) {
    return valueParser(value)
        .nodes.filter((node) => node.type === "function")
        .reduce((previousValue: Matrix2D, currentValue: any) => {
            switch (currentValue.value) {
                case "matrix": {
                    const valueNode = currentValue.nodes.filter(
                        (node: valueParser.Node) => node.type === "word"
                    )
                    const v = valueNode.map((node: valueParser.Node) =>
                        parseFloat(node.value)
                    )

                    return matrix_multiply(previousValue, v)
                }
                case "scale": {
                    const valueNode = currentValue.nodes.filter(
                        (node: valueParser.Node) => node.type === "word"
                    )
                    if (valueNode.length === 1) {
                        const v = parseFloat(valueNode[0].value)
                        return Transform.scale(previousValue, v, v)
                    } else {
                        return Transform.scale(
                            previousValue,
                            parseFloat(valueNode[0].value),
                            parseFloat(valueNode[1].value)
                        )
                    }
                }
                case "scaleX": {
                    const valueNode = currentValue.nodes.filter(
                        (node: valueParser.Node) => node.type === "word"
                    )
                    const v = parseFloat(valueNode[0].value)
                    return Transform.scaleX(previousValue, v)
                }
                case "scaleY": {
                    const valueNode = currentValue.nodes.filter(
                        (node: valueParser.Node) => node.type === "word"
                    )
                    const v = parseFloat(valueNode[0].value)
                    return Transform.scaleY(previousValue, v)
                }
                case "rotate":
                case "rotateZ": {
                    const valueNode = currentValue.nodes.filter(
                        (node: valueParser.Node) => node.type === "word"
                    )
                    const v = parseAngle(valueNode[0].value)
                    return Transform.rotate(previousValue, v)
                }
                case "skew": {
                    const valueNode = currentValue.nodes.filter(
                        (node: valueParser.Node) => node.type === "word"
                    )
                    const v0 = parseAngle(valueNode[0].value)
                    const v1 = parseAngle(valueNode[1].value)

                    return Transform.skew(previousValue, v0, v1)
                }
                case "skewX": {
                    const valueNode = currentValue.nodes.filter(
                        (node: valueParser.Node) => node.type === "word"
                    )
                    const v = parseAngle(valueNode[0].value)

                    return Transform.skewX(previousValue, v)
                }
                case "skewY": {
                    const valueNode = currentValue.nodes.filter(
                        (node: valueParser.Node) => node.type === "word"
                    )
                    const v = parseAngle(valueNode[0].value)

                    return Transform.skewY(previousValue, v)
                }
                case "translate": {
                    const valueNode = currentValue.nodes.filter(
                        (node: valueParser.Node) => node.type === "word"
                    )
                    const v0 = parseLength(valueNode[0].value)
                    const v1 = parseLength(valueNode[1].value)

                    return Transform.translate(previousValue, v0, v1)
                }
                case "translateX": {
                    const valueNode = currentValue.nodes.filter(
                        (node: valueParser.Node) => node.type === "word"
                    )
                    const v = parseLength(valueNode[0].value)

                    return Transform.translateX(previousValue, v)
                }
                case "translateY": {
                    const valueNode = currentValue.nodes.filter(
                        (node: valueParser.Node) => node.type === "word"
                    )
                    const v = parseLength(valueNode[0].value)

                    return Transform.translateY(previousValue, v)
                }
            }

            return previousValue
        }, Transform.identity())
}

export function parseFont(value: string) {
    const item = valueParser(value).nodes.filter((node) =>
        ["function", "word"].includes(node.type)
    )

    if (item.length === 3) {
        item.shift()
    }

    if (item.length !== 2) throw new Error("Not supported format " + value)

    const fontSize = parseLength(item[0].value)

    const fontFamily = item[1].value
    return {
        //TODO:
        fontSize,
        fontFamily,
    }
}
