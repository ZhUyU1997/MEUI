import { Box } from "./box"
import { MeuiStyle } from "./style"

export class Text extends Box {
    constructor(text: string) {
        super("Div", {
            width: "auto",
            text: text,
        })
    }

    set data(value: string) {
        this.setStyle({ text: value.toString() })
    }
}
