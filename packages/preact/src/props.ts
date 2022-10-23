import {
    MeuiFocusEvent,
    MeuiKeyboardEvent,
    MeuiMouseEvent,
    MeuiWheelEvent,
} from "@meui/core"
import { MeuiStyle } from "@meui/core"
import { ComponentChild } from "preact"

export interface MeuiElementProps {
    focusable?: boolean
    style?: MeuiStyle
    onClick?: (ev: CustomEvent) => any
    onMouseUp?: (ev: MeuiMouseEvent) => any
    onMouseDown?: (ev: MeuiMouseEvent) => any
    onMouseOver?: (ev: MeuiMouseEvent) => any
    onMouseOut?: (ev: MeuiMouseEvent) => any
    onKeyDown?: (ev: MeuiKeyboardEvent) => any
    onKeyUp?: (ev: MeuiKeyboardEvent) => any
    onMouseMove?: (ev: MeuiMouseEvent) => any
    onMouseWheel?: (ev: MeuiWheelEvent) => any
    onFocusIn?: (ev: MeuiFocusEvent) => any
    onFocusOut?: (ev: MeuiFocusEvent) => any
    onScroll?: (ev: CustomEvent) => any
    children?: ComponentChild
}

export interface MeuiCanvasElementProps extends MeuiElementProps {
    width?: number
    height?: number
    fit?: boolean
}
