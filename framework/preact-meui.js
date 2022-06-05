import { h, render } from "preact"

import { createBox } from "@/meui/core"
import { Text } from "@/meui/text"

globalThis.document = {
    createElement(nodeType, option) {
        return createBox(nodeType)
    },
    createTextNode(data) {
        return new Text(data)
    },
}

export default {
    render: (reactElement, meui, callback) => {
        render(reactElement, meui.getRoot())
    },
}