import { createElement } from "@/meui"
import { Text } from "./text"

globalThis.document = {
    createElement(tagName: string) {
        return createElement(tagName)
    },
    createTextNode(data: string) {
        return new Text(data)
    },
} as any
