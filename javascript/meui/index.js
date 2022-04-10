import { NativeMEUI, BOX_STATE } from "NativeMEUI"
import * as os from "os"
import { Box } from "./box"
import { Canvas } from "./canvas"

const FPS = 60
const mainLoop = (onTick, delay = 1000.0 / FPS) => {
    setInterval(onTick, delay)
}

export function createBox(type = "div", style) {
    if (type === "div" || type === "stack") return new Box(type, style)
    else if (type === "canvas") return new Canvas(style)
}

export class MEUI {
    constructor(width, height) {
        this.nativeMEUI = new NativeMEUI(width, height)
        this.root = createBox("div", {
            justifyContent: "center",
            alignItems: "center",
            alignContent: "center",
            width: "100%",
            height: "100%",
        })

        this.addFontFace(
            "Droid-Sans-Fallback",
            "res/font/Droid-Sans-Fallback.ttf"
        )
        this.addFontFace(
            "MaterialDesignIcons",
            "res/font/MaterialDesignIconsDesktop.ttf"
        )

        this.nativeMEUI.render(this.root.getNativeObject())
        this.mouseX = 0
        this.mouseY = 0

        os.setReadHandler(this.getConnectNumber(), () => {
            while (this.pending() > 0) {
                const event = this.nextEvent()
                if (!event) continue
                let box = null

                if (
                    ["mousedown", "mouseup", "mousemove"].includes(event.type)
                ) {
                    box = this.searchNode(event.x, event.y, (hit, box) => {
                        box.setState(hit ? BOX_STATE.HOVER : BOX_STATE.DEFAULT)
                    })
                    this.mouseX = event.x
                    this.mouseY = event.y
                } else {
                    box = this.searchNode(
                        this.mouseX,
                        this.mouseY,
                        (hit, box) => {
                            box.setState(
                                hit ? BOX_STATE.HOVER : BOX_STATE.DEFAULT
                            )
                        }
                    )
                }

                if (box) {
                    if (event.type === "mousedown") {
                        box.setState(BOX_STATE.ACTIVE)
                    }

                    box.dispatchEvent(event)

                    if (
                        event.type === "mouseup" &&
                        event.button === 0 &&
                        box.getState() === BOX_STATE.HOVER
                    ) {
                        box.dispatchEvent({ type: "click" })
                    }
                }
            }
        })

        mainLoop(() => this.onFrameTick())
    }

    onFrameTick() {
        window.triggerAnimationFrame()
        this.update()
    }

    registerCallback() {}
    flush() {
        this.nativeMEUI.flush()
    }
    update() {
        this.nativeMEUI.update()
    }
    debug() {
        this.nativeMEUI.debug()
    }
    addFontFace(fontFamily, fileName) {
        this.nativeMEUI.addFontFace(fontFamily, fileName)
    }
    getConnectNumber() {
        return this.nativeMEUI.getConnectNumber()
    }

    getRoot() {
        return this.root
    }
    pending() {
        return this.nativeMEUI.pending()
    }
    nextEvent() {
        return this.nativeMEUI.nextEvent()
    }
    _searchNode(node, x, y, callback) {
        let target = null
        if (node.hit(x, y)) {
            target = node
        }

        if (callback) callback(target === node, node)

        for (const item of node.children) {
            const ret = this._searchNode(item, x, y, callback)
            target = ret ? ret : target
        }
        return target
    }

    searchNode(x, y, callback) {
        return this._searchNode(this.root, x, y, callback)
    }
}

export { default as ReactMEUI } from "./ReactMEUI"
