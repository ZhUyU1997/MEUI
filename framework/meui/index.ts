import { NativeMEUI, BOX_STATE, MeuiMouseRawEvent } from "NativeMEUI"
import * as os from "os"
import * as std from "std"

import React, { PropsWithChildren, RefAttributes } from "react"
import {
    Box,
    CustomEvent,
    MeuiKeyboardEvent,
    MeuiMouseEvent,
    MeuiWheelEvent,
} from "./box"
import { CanvasElement } from "./canvas"
import { DivElement } from "./div"
import { StackElement } from "./stack"
import type { MeuiStyle } from "./style"
import { KeyMap } from "./keymap"

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

interface MeuiElementProps {
    style?: MeuiStyle
    onClick?: (ev: CustomEvent) => any
    onMouseUp?: (ev: MeuiMouseEvent) => any
    onMouseDown?: (ev: MeuiMouseEvent) => any
    onKeyDown?: (ev: MeuiKeyboardEvent) => any
    onKeyUp?: (ev: MeuiKeyboardEvent) => any
    onMouseMove?: (ev: MeuiMouseEvent) => any
    onMouseWheel?: (ev: MeuiWheelEvent) => any
    onScroll?: (ev: CustomEvent) => any
}

interface MeuiCanvasElementProps extends MeuiElementProps {
    width?: number
    height?: number
    fit?: boolean
}

export type MeuiElementAttribule = PropsWithChildren<MeuiElementProps>
export type MeuiCanvasElementAttribule =
    PropsWithChildren<MeuiCanvasElementProps>

declare global {
    // eslint-disable-next-line @typescript-eslint/no-namespace
    namespace JSX {
        interface IntrinsicElements {
            Div: MeuiElementAttribule & RefAttributes<DivElement>
            Canvas: MeuiCanvasElementAttribule & RefAttributes<CanvasElement>
            Stack: MeuiElementAttribule & RefAttributes<StackElement>
        }
    }
}

export interface MeuiElements {
    Div: JSX.IntrinsicElements["Div"]
    Stack: JSX.IntrinsicElements["Canvas"]
    Canvas: JSX.IntrinsicElements["Stack"]
}

export class MEUI {
    private nativeMEUI: NativeMEUI
    private root: DivElement | StackElement | CanvasElement
    private mouseX: number
    private mouseY: number
    private mouseHit: Box | null
    onunload: () => void
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
        this.onunload = () => {}
        os.setReadHandler(this.getConnectNumber(), this.onEvent.bind(this))
        setInterval(() => this.onFrameTick(), 1000.0 / FPS)
    }

    onExit() {
        this.onunload?.()
        std.gc()
        std.exit(0)
    }
    onEvent() {
        const eventList = []
        while (this.pending() > 0) {
            const event = this.nextEvent()
            if (!event) continue

            eventList.push(event)
        }

        for (const event of eventList) {
            let box = this.mouseHit

            if (
                event.type &&
                ["mousedown", "mouseup", "mousemove"].includes(event.type)
            ) {
                const mouseEvent = event as MeuiMouseRawEvent
                this.mouseX = mouseEvent.x
                this.mouseY = mouseEvent.y

                this.mouseHit?.getPath().forEach((item) => {
                    item.setState(BOX_STATE.DEFAULT)
                })
                box = this.searchNode(this.mouseX, this.mouseY)

                box?.getPath().forEach((item) => {
                    item.setState(BOX_STATE.HOVER)
                })
            } else if (event.type === "mousewheel") {
                this.mouseHit?.getPath().forEach((item) => {
                    item.setState(BOX_STATE.DEFAULT)
                })
                box = this.searchNode(this.mouseX, this.mouseY)

                box?.getPath().forEach((item) => {
                    item.setState(BOX_STATE.HOVER)
                })
            } else if (event.type === "unload") {
                this.onExit()
            }

            if (box) {
                if (event.type === "mousedown") {
                    box.setState(BOX_STATE.ACTIVE)
                }
                if (
                    event.type &&
                    ["mousedown", "mouseup", "mousemove"].includes(event.type)
                ) {
                    const mouseEvent = event as MeuiMouseRawEvent

                    box.dispatchEvent(
                        new MeuiMouseEvent(event.type, {
                            screenX: mouseEvent.x,
                            screenY: mouseEvent.y,
                            button: mouseEvent.button,
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
                } else if (event.type === "keyup" || event.type === "keydown") {
                    box.dispatchEvent(
                        new MeuiKeyboardEvent(event.type, {
                            altKey: event.altKey,
                            ctrlKey: event.ctrlKey,
                            shiftKey: event.shiftKey,
                            key:
                                event.key in KeyMap
                                    ? (KeyMap as any)[event.key]
                                    : event.key,
                            keyCode: event.keyCode,
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

            if (box !== this.mouseHit) {
                if ("mousemove" === event.type) {
                    const mouseEvent = event as MeuiMouseRawEvent
                    this.mouseX = mouseEvent.x
                    this.mouseY = mouseEvent.y

                    if (this.mouseHit) {
                        this.mouseHit.dispatchEvent(
                            new MeuiMouseEvent("mouseout", {
                                screenX: mouseEvent.x,
                                screenY: mouseEvent.y,
                                button: mouseEvent.button,
                            })
                        )
                    }

                    if (box) {
                        box.dispatchEvent(
                            new MeuiMouseEvent("mouseover", {
                                screenX: mouseEvent.x,
                                screenY: mouseEvent.y,
                                button: mouseEvent.button,
                            })
                        )
                    }
                }
                this.mouseHit = box
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
        this.mouseHit?.getPath().forEach((item) => {
            item.setState(BOX_STATE.DEFAULT)
        })

        this.mouseHit = this.searchNode(this.mouseX, this.mouseY)

        this.mouseHit?.getPath().forEach((item) => {
            item.setState(BOX_STATE.HOVER)
        })

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

    searchNode(x: number, y: number): Box | null {
        const path = this.root.search(x, y)

        let target = this.root

        for (const i of path) {
            if (i === -1) break
            target = target.children[i]
        }
        return target
    }
}

export { default as ReactMEUI } from "./ReactMEUI"
