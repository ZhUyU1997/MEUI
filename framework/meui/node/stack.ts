import { Element } from "./element"
import { MeuiStyle } from "../style"

export class StackElement extends Element {
    constructor(style?: MeuiStyle) {
        super("stack", style)
    }
}
