import { NativeMEUI, BOX_STATE } from "NativeMEUI"
import * as os from "os"
import React, { PropsWithChildren, RefAttributes } from "react"
import { Box, CustomEvent, MeuiMouseEvent, MeuiWheelEvent } from "./box"
import { CanvasElement } from "./canvas"
import { DivElement } from "./div"
import { StackElement } from "./stack"
import type { MeuiStyle } from "./style"

const FPS = 60

export function createBox(type = "Div", style: MeuiStyle = {}) {
    if (type === "Div") return new DivElement(style)
    else if (type === "Stack") return new StackElement(style)
    else if (type === "Canvas") return new CanvasElement(style)
    return new DivElement(style)
}
export const Div = "Div"
export const Stack = "Stack"
export const Canvas = "Canvas"

type ComponentProps = {
    style?: MeuiStyle
    onClick?: (ev: CustomEvent) => any
    onMouseUp?: (ev: CustomEvent) => any
    onMouseDown?: (ev: CustomEvent) => any
    onMouseMove?: (ev: CustomEvent) => any
    onMouseWheel?: (ev: CustomEvent) => any
    onScroll?: (ev: CustomEvent) => any
}

declare global {
    // eslint-disable-next-line @typescript-eslint/no-namespace
    namespace JSX {
        interface IntrinsicElements {
            Div: PropsWithChildren<ComponentProps & RefAttributes<DivElement>>
            Canvas: PropsWithChildren<
                ComponentProps & RefAttributes<CanvasElement>
            >
            Stack: PropsWithChildren<
                ComponentProps & RefAttributes<StackElement>
            >
        }
    }
}

export interface MeuiElements {
    Div: JSX.IntrinsicElements["Div"]
    Stack: JSX.IntrinsicElements["Canvas"]
    Canvas: JSX.IntrinsicElements["Stack"]
}

export class MEUI {
    nativeMEUI: NativeMEUI
    root: DivElement | StackElement | CanvasElement
    mouseX: number
    mouseY: number
    mouseHit: Box | null
    constructor(width: number, height: number) {
        this.nativeMEUI = new NativeMEUI(width, height)
        this.root = createBox("Div", {
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
        this.mouseX = -1
        this.mouseY = -1
        this.mouseHit = null
        os.setReadHandler(this.getConnectNumber(), this.onEvent.bind(this))
        setInterval(() => this.onFrameTick(), 1000.0 / FPS)
    }

    onEvent() {
        while (this.pending() > 0) {
            const event = this.nextEvent()
            if (!event) continue

            let box = this.mouseHit

            if (
                event.type === "mousedown" ||
                event.type === "mouseup" ||
                event.type === "mousemove"
            ) {
                if (box && (this.mouseX != event.x || this.mouseY != event.y)) {
                    box.getPath().forEach((item) => {
                        item.setState(BOX_STATE.DEFAULT)
                    })
                }

                this.mouseHit = box = this.searchNode(event.x, event.y)

                box?.getPath().forEach((item) => {
                    item.setState(BOX_STATE.HOVER)
                })

                this.mouseX = event.x
                this.mouseY = event.y
            }

            if (box) {
                if (event.type === "mousedown") {
                    box.setState(BOX_STATE.ACTIVE)
                }
                if (
                    event.type === "mousedown" ||
                    event.type === "mouseup" ||
                    event.type === "mousemove"
                ) {
                    box.dispatchEvent(
                        new MeuiMouseEvent(event.type, {
                            clientX: event.x,
                            clientY: event.y,
                            button: event.button,
                        })
                    )
                } else if (event.type === "mousewheel") {
                    box.dispatchEvent(
                        new MeuiWheelEvent(event.type, {
                            deltaX: event.deltaX,
                            deltaY: event.deltaY,
                            deltaZ: event.deltaZ,
                        })
                    )
                }

                if (
                    event.type === "mouseup" &&
                    event.button === 0 &&
                    box.getState() === BOX_STATE.HOVER
                ) {
                    box.dispatchEvent(new CustomEvent("click"))
                }
            }
        }
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
    addFontFace(fontFamily: string, fileName: string) {
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

    private _searchNode(node: Box, x: number, y: number): Box | null {
        let target = null
        const len = node.children.length
        for (let i = len - 1; i >= 0; i--) {
            const ret = this._searchNode(node.children[i], x, y)
            if (ret) return ret
        }

        if (node.hit(x, y)) {
            target = node
        }

        return target
    }

    searchNode(x: number, y: number): Box | null {
        return this._searchNode(this.root, x, y)
    }
}

export { default as ReactMEUI } from "./ReactMEUI"
