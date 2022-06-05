import React, { PropsWithChildren, RefAttributes } from "react"

import {
    CustomEvent,
    MeuiFocusEvent,
    MeuiKeyboardEvent,
    MeuiMouseEvent,
    MeuiWheelEvent,
} from "./box"

import { CanvasElement } from "./canvas"
import { DivElement } from "./div"
import { StackElement } from "./stack"
import type { MeuiStyle } from "./style"

export * from "./core"

export interface MeuiElementProps {
    focusable?: boolean
    style?: MeuiStyle
    onClick?: (ev: CustomEvent) => any
    onMouseUp?: (ev: MeuiMouseEvent) => any
    onMouseDown?: (ev: MeuiMouseEvent) => any
    onKeyDown?: (ev: MeuiKeyboardEvent) => any
    onKeyUp?: (ev: MeuiKeyboardEvent) => any
    onMouseMove?: (ev: MeuiMouseEvent) => any
    onMouseWheel?: (ev: MeuiWheelEvent) => any
    onFocusIn?: (ev: MeuiFocusEvent) => any
    onFocusOut?: (ev: MeuiFocusEvent) => any
    onScroll?: (ev: CustomEvent) => any
}

interface MeuiCanvasElementProps extends MeuiElementProps {
    width?: number
    height?: number
    fit?: boolean
}

export type MeuiElementAttribule = PropsWithChildren<MeuiElementProps>
export type MeuiCanvasElementAttribule =
    PropsWithChildren<MeuiCanvasElementProps>

declare global {
    // eslint-disable-next-line @typescript-eslint/no-namespace
    namespace JSX {
        interface IntrinsicElements {
            Div: MeuiElementAttribule & RefAttributes<DivElement>
            Canvas: MeuiCanvasElementAttribule & RefAttributes<CanvasElement>
            Stack: MeuiElementAttribule & RefAttributes<StackElement>
        }
    }
}

export interface MeuiElements {
    Div: JSX.IntrinsicElements["Div"]
    Stack: JSX.IntrinsicElements["Stack"]
    Canvas: JSX.IntrinsicElements["Canvas"]
}
