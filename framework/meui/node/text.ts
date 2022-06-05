import { Node, NodeType } from "./node"

export class Text extends Node {
    nodeType = NodeType.TEXT_NODE
    constructor(text: string) {
        super()
        this.text = text
    }

    set data(value: string) {
        this.text = value
        this.parentNode?.updateText()
    }
}
