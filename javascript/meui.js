import { NativeMEUI, BOX_STATE, createBox } from "MEUI"
import * as os from "os"
import * as colorString from "color-string"

export class Box {
    constructor(style) {
        this.nativeBox = createBox()
        this.children = []
        this.parent = null
        this.eventListeners = {}

        if (style) {
            this.setStyle(style, BOX_STATE.DEFAULT)
        }
    }

    getStyle(state) {
        return this.nativeBox.getStyle(state)
    }
    _setStyle(style, state) {
        const colorProperty = ["borderColor", "backgroundColor", "fontColor"]

        for (const p of colorProperty) {
            if (p in style) {
                let [r, g, b, a] = colorString.get.rgb(style[p])
                style[p] = [r / 255.0, g / 255.0, b / 255.0, a]
            }
        }

        this.nativeBox.setStyle(style, state)
    }
    setStyle(style) {
        let defaultStyle = {}
        for (let [k, v] of Object.entries(style)) {
            if (k in BOX_STATE) {
                let otherStyle = {};
                for (let [key, val] of Object.entries(v)) {
                    otherStyle[key] = val
                }
                this._setStyle(otherStyle, BOX_STATE[k])
            }
            else {
                defaultStyle[k] = v
            }
        }

        this._setStyle(defaultStyle, BOX_STATE.DEFAULT)
    }

    addChild(child) {
        console.log("addChild")
        child.parent = this
        this.nativeBox.addChild(child.nativeBox);
        this.children.push(child)
    }

    removeChild(child) {
        const index = this.children.indexOf(child)

        if (index !== -1) {
            this.nativeBox.removeChild(child.nativeBox);
            this.children.splice(index, 1)
        }
    }

    getState() {
        return this.nativeBox.getState()
    }

    setState(state) {
        this.nativeBox.setState(state)
    }

    hit(x, y) {
        return this.nativeBox.hit(x, y)
    }

    getNativeObject() {
        return this.nativeBox
    }

    addEventListener(type, listener, useCapture = false) {
        if (!(type in this.eventListeners))
            this.eventListeners[type] = []
        this.eventListeners[type].push({ listener, useCapture })
    }

    removeEventListener(type, listener, useCapture) {
        if ((!listener) || (!type))
            return
        if (!(type in this.eventListeners))
            return

        const listenerArray = this.eventListeners[type]
        for (const index in listenerArray) {
            if (listenerArray[index].listener === listener
                && listenerArray[index].useCapture === useCapture) {
                listenerArray.splice(index, 1)
                return
            }
        }
    }

    tryHandleEvent(event, capture) {
        if (event.type in this.eventListeners) {
            for (const { listener, useCapture } of this.eventListeners[event.type]) {
                if (capture == useCapture) listener(event)
            }
        }
    }

    capturingPhase(node, event) {
        if (node.parent)
            this.capturingPhase(node.parent, event)
        event.currentTarget = node
        node.tryHandleEvent(event, true)
    }

    bubblingPhase(node, event) {
        while (true) {
            event.currentTarget = node
            node.tryHandleEvent(event, false)

            if (!node.parent)
                break;
            node = node.parent
        }
    }

    dispatchEvent(event) {
        event.target = this
        this.capturingPhase(this, event)
        this.bubblingPhase(this, event)
    }
}

const FPS = 60
const mainLoop = async (onTick, delay = 1000 / FPS) => {
    const nativeTick = () => new Promise(resolve => {
        onTick()
        setTimeout(resolve, delay)
    })
    // eslint-disable-next-line no-constant-condition
    while (true) await nativeTick()
}

export class MEUI {
    constructor(width, height) {
        this.nativeMEUI = new NativeMEUI(width, height)
        this.root = new Box({
            justifyContent: "center",
            alignItems: "center",
            alignContent: "center",
            width: "100%",
            height: "100%",
        })

        this.addFontFace("res/font/Droid-Sans-Fallback.ttf")

        this.nativeMEUI.render(this.root.getNativeObject())

        os.setReadHandler(this.getConnectNumber(), () => {
            while (this.pending() > 0) {
                const event = this.nextEvent();
                if (!event) continue
                const box = this.searchNode(event.x, event.y, (hit, box) => {
                    box.setState(hit ? BOX_STATE.HOVER : BOX_STATE.DEFAULT)
                    if (event.type === "mousedown" && hit) {
                        box.setState(BOX_STATE.ACTIVE)
                    }
                })

                if (box) {
                    box.dispatchEvent(event)

                    if (event.type === "mouseup" && box.getState() === BOX_STATE.HOVER) {
                        box.dispatchEvent({ type: "click" })
                    }
                }
            }
            this.update()
        })

        mainLoop(() => this.onFrameTick())
    }

    onFrameTick() {
        this.update()
    }

    registerCallback() {

    }
    flush() {
        this.nativeMEUI.flush()
    }
    update() {
        this.nativeMEUI.update()
    }
    debug() {
        this.nativeMEUI.debug()
    }
    addFontFace(fileName) {
        this.nativeMEUI.addFontFace(fileName)
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

        if (callback)
            callback(target === node, node);

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