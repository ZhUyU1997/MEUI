import { NativeMEUI, Box, BOX_STATE } from "meui-native"

const meui = new NativeMEUI(320, 170, "RGB565")
const root = new Box("Div")

root.setStyle(
    {
        justifyContent: "center",
        alignItems: "center",
        alignContent: "center",
        width: "100%",
        height: "100%",
    },
    BOX_STATE.DEFAULT
)

const child = new Box("Div")

child.setStyle(
    {
        width: 120,
        height: 120,
        backgroundColor: [0x46 / 255.0, 0xe9 / 255.0, 0x91 / 255.0, 1],
        borderRadius: [16, 16, 16, 16],
    },
    BOX_STATE.DEFAULT
)

root.addChild(child)
meui.render(root)

while (true) meui.update()
