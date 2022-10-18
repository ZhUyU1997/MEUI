/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

// export type Primitives = 'div' | 'stack'
import { ElementType } from "react"
import { MeuiElements } from "@/meui"

export type Primitives = {
    [P in keyof MeuiElements]: P extends ElementType ? P : never
}[keyof MeuiElements]

export const primitives: Primitives[] = ["View", "Canvas", "Stack"]
