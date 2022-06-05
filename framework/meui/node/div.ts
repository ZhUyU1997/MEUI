import { Element } from "./element"
import { MeuiStyle } from "../style"

export class DivElement extends Element {
    constructor(style?: MeuiStyle) {
        super("div", style)
    }
}
