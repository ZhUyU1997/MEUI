// export type Primitives = 'div' | 'stack'
import React, { ElementType } from "react"
import { MeuiElements } from "../meui"
import { MeuiStyle } from "../meui/style"

export type Primitives = {
    [P in keyof MeuiElements]: MeuiElements[P] extends ElementType ? P : never
}[keyof MeuiElements]

export const primitives: Primitives[] = ["Div", "Canvas", "Stack"]
