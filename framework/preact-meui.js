import { render } from "preact"
import "@/meui/node/dom"

export default {
    render: (reactElement, meui, callback) => {
        render(reactElement, meui.getRoot())
    },
}