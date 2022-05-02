import { Box } from "./box"
import { MeuiStyle } from "./style"

export class DivElement extends Box {
    constructor(style?: MeuiStyle) {
        super("div", style)
    }
}
