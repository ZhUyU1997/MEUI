/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { Element } from "./element"
import { MeuiStyle } from "../style"

export class StackElement extends Element {
    constructor(style?: MeuiStyle) {
        super("stack", style)
    }
}
