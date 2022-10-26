/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { Node, NodeType } from "./node"

export class Text extends Node {
    nodeType = NodeType.TEXT_NODE
    text: string
    constructor(text: string) {
        super()
        this.text = text
    }

    set data(value: string) {
        this.text = value
        this.parentNode?.updateText()
    }
}
