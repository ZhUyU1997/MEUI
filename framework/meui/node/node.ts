/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { Element } from "./element"

export abstract class Node {
    childNodes: Node[] = []
    parentNode: Element | null = null
    text = ""
    abstract nodeType: NodeType
    private _previousSibling: Node | null = null
    private _nextSibling: Node | null = null

    constructor() {}

    get firstChild() {
        return this.childNodes[0]
    }

    get lastChild() {
        return this.childNodes[this.childNodes.length - 1]
    }

    get nextSibling(): Node | null {
        return this._nextSibling
    }

    get previousSibling(): Node | null {
        return this._previousSibling
    }

    appendChild(child: Node) {
        this.insertBefore(child, null)
    }

    insertBefore(child: Node, ref: Node | null) {
        child.remove()
        let index = this.childNodes.length

        if (ref) {
            const i = this.childNodes.indexOf(ref)
            index = i == -1 ? index : i
        }

        this.childNodes.splice(index, 0, child)
        child._previousSibling = this.childNodes[index - 1]
        child._nextSibling = this.childNodes[index + 1]

        child.parentNode = this as unknown as Element
        return child
    }

    removeChild(child: Node) {
        const index = this.childNodes.indexOf(child)

        if (index !== -1) {
            this.childNodes.splice(index, 1)
            child._previousSibling = null
            child._nextSibling = null
            child.parentNode = null
        }
    }

    remove() {
        this.parentNode?.removeChild(this)
    }
}

export enum NodeType {
    ELEMENT_NODE = 1,
    TEXT_NODE = 3,
}
