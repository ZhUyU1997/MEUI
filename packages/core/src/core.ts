/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { NativeMEUI, BOX_STATE, MeuiMouseRawEvent } from "meui-native"
import * as os from "os"
import * as std from "std"
import { KeyMap } from "./keymap"

import {
    Element,
    CustomEvent,
    MeuiFocusEvent,
    MeuiKeyboardEvent,
    MeuiMouseEvent,
    MeuiWheelEvent,
} from "./node"

import { CanvasElement, DivElement, StackElement, Text } from "./node"
import type { MeuiStyle } from "./style"

const FPS = 60

export function createElement(type = "View", style: MeuiStyle = {}): Element {
    if (type === "View") return new DivElement(style)
    else if (type === "Stack") return new StackElement(style)
    else if (type === "Canvas") return new CanvasElement(style)
    return new DivElement(style)
}

export class MEUI {
    private nativeMEUI: NativeMEUI
    private root: Element
    private mouseX: number
    private mouseY: number
    private mouseHit: Element | null
    private focusElement: Element | null
    private intervalId: any
    private width: number
    private height: number
    onunload: () => void
    constructor(width: number, height: number) {
        this.nativeMEUI = new NativeMEUI(width, height)
        this.width = width
        this.height = height

        this.root = createElement("View", {
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
        this.focusElement = null
        this.onunload = () => {}

        if (os.setReadHandler)
            os.setReadHandler(this.getConnectNumber(), this.onEvent.bind(this))
        else os.setEventHandler(this.onEvent.bind(this))

        this.intervalId = setInterval(() => this.onFrameTick(), 1000.0 / FPS)
    }

    private diffPath(
        oldPath: Element[],
        newPath: Element[]
    ): [Element[], Element[]] {
        const length =
            oldPath.length < newPath.length ? oldPath.length : newPath.length
        let i = 0
        for (i = 0; i < length; i++) {
            if (oldPath[i] !== newPath[i]) break
        }

        return [oldPath.slice(i), newPath.slice(i)]
    }

    onExit() {
        this.onunload?.()

        if (os.setReadHandler) os.setReadHandler(this.getConnectNumber(), null)
        else os.setEventHandler(null)

        clearInterval(this.intervalId)

        this.mouseHit = null
        this.focusElement = null

        std.gc()

        // setTimeout(() => {
        //     // std.exit(1)
        // }, 1000)
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
            let focusElement = this.focusElement
            if (
                event.type &&
                ["mousedown", "mouseup", "mousemove"].includes(event.type)
            ) {
                const mouseEvent = event as MeuiMouseRawEvent
                this.mouseX = mouseEvent.x
                this.mouseY = mouseEvent.y

                box = this.searchNode(this.mouseX, this.mouseY)

                if (box !== this.mouseHit) {
                    const [oldPath, newPath] = this.diffPath(
                        this.mouseHit?.getPath() ?? [],
                        box?.getPath() ?? []
                    )
                    oldPath.forEach((item) => {
                        item.setState(BOX_STATE.DEFAULT)
                    })

                    newPath.forEach((item) => {
                        item.setState(BOX_STATE.HOVER)
                    })
                }

                if (event.type === "mousedown") {
                    box?.getPath()
                        .reverse()
                        .forEach((item) => {
                            if (item.focusable) {
                                focusElement = item
                                item.setState(BOX_STATE.FOCUS)
                            }
                        })
                }
            } else if (event.type === "mousewheel") {
                box = this.searchNode(this.mouseX, this.mouseY)

                if (box !== this.mouseHit) {
                    const [oldPath, newPath] = this.diffPath(
                        this.mouseHit?.getPath() ?? [],
                        box?.getPath() ?? []
                    )
                    oldPath.forEach((item) => {
                        item.setState(BOX_STATE.DEFAULT)
                    })

                    newPath.forEach((item) => {
                        item.setState(BOX_STATE.HOVER)
                    })
                }
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

                    if (event.deltaY !== 0) {
                        const needScrollBox = box
                            .getPath()
                            .reverse()
                            .find((box) => {
                                if (box.scrollHeight > box.clientHeight) {
                                    if (event.deltaY > 0)
                                        return (
                                            box.scrollTop + box.clientHeight <
                                            box.scrollHeight
                                        )
                                    else return box.scrollTop > 0
                                }
                                return false
                            })

                        if (needScrollBox) {
                            needScrollBox.scrollTop += event.deltaY
                            needScrollBox.dispatchEvent(
                                new CustomEvent("scroll")
                            )
                        }
                    }
                } else if (event.type === "keyup" || event.type === "keydown") {
                    focusElement?.dispatchEvent(
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

                if (event.type === "mouseup" && event.button === 0) {
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

            if (focusElement !== this.focusElement) {
                if ("mousedown" === event.type) {
                    if (this.focusElement) {
                        this.focusElement.dispatchEvent(
                            new MeuiFocusEvent("focusout")
                        )
                    }

                    if (focusElement) {
                        focusElement.dispatchEvent(
                            new MeuiFocusEvent("focusin")
                        )
                    }
                }
                this.focusElement = focusElement
            }
        }
    }
    onFrameTick() {
        ;(window as any).triggerAnimationFrame()
        this.update()
    }

    registerCallback() {}
    flush() {
        this.nativeMEUI.flush()
    }
    update() {
        // The layout or position may change even if the mouse is not moving
        const hit = this.searchNode(this.mouseX, this.mouseY)

        if (this.mouseHit !== hit) {
            // TODO: if keep the mouse down, we should set the element to "active" state
            const [oldPath, newPath] = this.diffPath(
                this.mouseHit?.getPath() ?? [],
                hit?.getPath() ?? []
            )
            oldPath.forEach((item) => {
                item.setState(BOX_STATE.DEFAULT)
            })

            newPath.forEach((item) => {
                item.setState(BOX_STATE.HOVER)
            })

            this.mouseHit = hit
        }

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

    searchNode(x: number, y: number): Element | null {
        if (x < 0 || y <= 0 || x >= this.width || y >= this.height) return null

        const path = this.root.search(x, y)

        let target = this.root
        for (const i of path) {
            if (i === -1) break
            target = target.children[i] as Element
        }
        return path.length > 0 ? target : null
    }
}
