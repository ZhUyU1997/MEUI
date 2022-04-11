import { Box } from "./box"
import { MeuiStyle } from "./style"

export class StackElement extends Box {
    constructor(style: MeuiStyle) {
        super("stack", style)
    }
}
