// export type Primitives = 'div' | 'stack'
import { ElementType } from "react"
import { MeuiElements } from "../meui"

export type Primitives = {
    [P in keyof MeuiElements]: P extends ElementType ? P : never
}[keyof MeuiElements]

export const primitives: Primitives[] = ["Div", "Canvas", "Stack"]
