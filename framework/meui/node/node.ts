import { Element } from "./element"

export abstract class Node {
    childNodes: Node[]
    parentNode: Element | null
    text = ""
    abstract nodeType: NodeType

    constructor() {
        this.childNodes = []
        this.parentNode = null
    }

    get firstChild() {
        return this.childNodes[0]
    }
    get lastChild() {
        return this.childNodes[this.childNodes.length - 1]
    }
    get nextSibling(): Node | null {
        if (!this.parentNode) return null
        const index = this.parentNode.childNodes.indexOf(this)
        return this.parentNode.childNodes[index + 1]
    }

    get previousSibling(): Node | null {
        if (!this.parentNode) return null
        const index = this.parentNode.childNodes.indexOf(this)
        return this.parentNode.childNodes[index - 1]
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
        child.parentNode = this as unknown as Element
        return child
    }

    removeChild(child: Node) {
        const index = this.childNodes.indexOf(child)

        if (index !== -1) {
            this.childNodes.splice(index, 1)
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