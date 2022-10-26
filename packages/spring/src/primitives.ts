/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { ElementType, PropsWithChildren, RefAttributes } from "react"
import { CanvasElement, DivElement, StackElement } from "@meui-js/core"
import { MeuiCanvasElementProps, MeuiElementProps } from "@meui-js/preact"

export type MeuiElementAttribule = PropsWithChildren<MeuiElementProps>
export type MeuiCanvasElementAttribule =
    PropsWithChildren<MeuiCanvasElementProps>

declare global {
    // eslint-disable-next-line @typescript-eslint/no-namespace
    namespace JSX {
        interface IntrinsicElements {
            View: MeuiElementAttribule & RefAttributes<DivElement>
            Canvas: MeuiCanvasElementAttribule & RefAttributes<CanvasElement>
            Stack: MeuiElementAttribule & RefAttributes<StackElement>
        }
    }
}

export interface MeuiElements {
    View: JSX.IntrinsicElements["View"]
    Stack: JSX.IntrinsicElements["Stack"]
    Canvas: JSX.IntrinsicElements["Canvas"]
}

export type Primitives = {
    [P in keyof MeuiElements]: P extends ElementType ? P : never
}[keyof MeuiElements]

export const primitives: Primitives[] = ["View", "Canvas", "Stack"]
