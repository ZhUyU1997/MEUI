import { BOX_STATE, Box as NativeBox, UI_STATE } from "NativeMEUI"
import * as colorString from "color-string"
import { MeuiStyle, parseTransform } from "./style"

interface MeuiEventMap {
    mousedown: MeuiMouseEvent
    mouseup: MeuiMouseEvent
    mousemove: MeuiMouseEvent
    mouseout: MeuiMouseEvent
    mouseover: MeuiMouseEvent
    mousewheel: MeuiWheelEvent
    keydown: MeuiKeyboardEvent
    keyup: MeuiKeyboardEvent
    click: MeuiMouseEvent
    focusin: MeuiFocusEvent
    focusout: MeuiFocusEvent
}

interface EventInit {
    bubbles?: boolean
    cancelable?: boolean
    // composed?: boolean;
}

interface MouseEventInit {
    button?: number
    buttons?: number
    clientX?: number
    clientY?: number
    movementX?: number
    movementY?: number
    screenX?: number
    screenY?: number
}

interface WheelEventInit extends MouseEventInit {
    deltaMode?: number
    deltaX?: number
    deltaY?: number
    deltaZ?: number
}
export class MeuiEvent implements MeuiEvent {
    readonly bubbles: boolean
    // cancelBubble: boolean;
    readonly cancelable: boolean

    readonly type: string
    target: Box | null
    currentTarget: Box | null

    constructor(type: string, eventInitDict: EventInit = {}) {
        this.type = type
        this.bubbles = eventInitDict.bubbles ?? false
        this.cancelable = eventInitDict.cancelable ?? false
        this.target = null
        this.currentTarget = null
    }

    stopPropagation() {}
}

export class MeuiMouseEvent extends MeuiEvent {
    readonly button: number
    readonly buttons: number
    readonly clientX: number
    readonly clientY: number
    readonly offsetX: number
    readonly offsetY: number
    readonly screenX: number
    readonly screenY: number
    readonly x: number
    readonly y: number
    constructor(type: string, eventInitDict: MouseEventInit = {}) {
        super(type)
        this.button = eventInitDict.button ?? 0
        this.buttons = eventInitDict.buttons ?? 0
        this.clientX = eventInitDict.clientX ?? 0
        this.clientY = eventInitDict.clientY ?? 0
        this.screenX = eventInitDict.screenX ?? 0
        this.screenY = eventInitDict.screenY ?? 0
        this.x = eventInitDict.clientX ?? 0
        this.y = eventInitDict.clientY ?? 0
        // TODO:
        this.offsetX = 0
        this.offsetY = 0
    }
}

export class MeuiWheelEvent extends MeuiMouseEvent {
    deltaX: number
    deltaY: number
    deltaZ: number

    constructor(type: string, eventInitDict: WheelEventInit = {}) {
        super(type, eventInitDict)
        this.deltaX = eventInitDict.deltaX ?? 0.0
        this.deltaY = eventInitDict.deltaY ?? 0.0
        this.deltaZ = eventInitDict.deltaZ ?? 0.0
    }
}

interface EventModifierInit extends UIEventInit {
    altKey?: boolean
    ctrlKey?: boolean
    metaKey?: boolean
    modifierAltGraph?: boolean
    modifierCapsLock?: boolean
    modifierFn?: boolean
    modifierFnLock?: boolean
    modifierHyper?: boolean
    modifierNumLock?: boolean
    modifierScrollLock?: boolean
    modifierSuper?: boolean
    modifierSymbol?: boolean
    modifierSymbolLock?: boolean
    shiftKey?: boolean
}

interface KeyboardEventInit extends EventModifierInit {
    /** @deprecated */
    charCode?: number
    code?: string
    isComposing?: boolean
    key?: string
    /** @deprecated */
    keyCode?: number
    location?: number
    repeat?: boolean
}
/** KeyboardEvent objects describe a user interaction with the keyboard; each event describes a single interaction between the user and a key (or combination of a key with modifier keys) on the keyboard. */
export class MeuiKeyboardEvent extends MeuiEvent {
    readonly altKey: boolean
    readonly ctrlKey: boolean
    readonly key: string
    /** @deprecated */
    readonly keyCode: number
    readonly shiftKey: boolean
    readonly DOM_KEY_LOCATION_LEFT: number = 0x00
    readonly DOM_KEY_LOCATION_NUMPAD: number = 0x01
    readonly DOM_KEY_LOCATION_RIGHT: number = 0x02
    readonly DOM_KEY_LOCATION_STANDARD: number = 0x03
    constructor(type: string, eventInitDict: KeyboardEventInit = {}) {
        super(type)
        this.altKey = eventInitDict.altKey ?? false
        this.ctrlKey = eventInitDict.ctrlKey ?? false
        this.shiftKey = eventInitDict.shiftKey ?? false
        this.key = eventInitDict.key ?? ""
        this.keyCode = eventInitDict.keyCode ?? 0
    }
}

interface FocusEventInit {
    relatedTarget?: Box | null
}

export class MeuiFocusEvent extends MeuiEvent {
    readonly relatedTarget: Box | null
    constructor(type: string, eventInitDict: FocusEventInit = {}) {
        super(type)
        this.relatedTarget = eventInitDict.relatedTarget ?? null
    }
}

export class CustomEvent extends MeuiEvent {
    constructor(type: string) {
        super(type)
    }
}

type EventListener<K extends string> = K extends keyof MeuiEventMap
    ? (ev: MeuiEventMap[K]) => void
    : (ev: MeuiEvent) => void

type EventRecord<K extends string> = Array<{
    useCapture: boolean
    listener: EventListener<K>
}>

type EventCollection = {
    [K in string]: EventRecord<K>
}

export class Box {
    nativeBox: NativeBox
    childNodes: Box[]
    parentNode: Box | null
    eventListeners: EventCollection
    text: string
    focusable = false
    style: any
    constructor(type = "Div", style?: MeuiStyle) {
        this.nativeBox = new NativeBox(type)
        this.childNodes = []
        this.parentNode = null
        this.eventListeners = {} as EventCollection
        this.text = ""

        if (style) {
            this.setStyle(style)
        }
        this.addEventListener("mousewheel", (e: MeuiWheelEvent) => {
            this.scrollTop += e.deltaY
            this.dispatchEvent(new CustomEvent("scroll"))
        })

        this.style = new Proxy(
            {
                setProperty: (
                    key: keyof MeuiStyle,
                    value: MeuiStyle[keyof MeuiStyle]
                ) => {
                    this.setStyle({ [key]: value })
                },
            },
            {
                // get(obj, prop) {
                //     console.log(obj, prop)
                //     return ""
                // },
                set: (obj, prop, value) => {
                    this.setStyle({ [prop]: value })
                    return true
                },
            }
        )
    }

    getStyle(state: UI_STATE) {
        return this.nativeBox.getStyle(state)
    }
    private _setStyle(style: MeuiStyle, state: UI_STATE) {
        const nativeStyle: Record<string, any> = style
        if (style.borderColor) {
            const [r, g, b, a] = colorString.get.rgb(style.borderColor) ?? [
                0, 0, 0, 0,
            ]
            nativeStyle.borderColor = [r / 255.0, g / 255.0, b / 255.0, a]
        }
        if (style.backgroundColor) {
            const [r, g, b, a] = colorString.get.rgb(style.backgroundColor) ?? [
                0, 0, 0, 0,
            ]
            nativeStyle.backgroundColor = [r / 255.0, g / 255.0, b / 255.0, a]
        }
        if (style.fontColor) {
            const [r, g, b, a] = colorString.get.rgb(style.fontColor) ?? [
                0, 0, 0, 0,
            ]
            nativeStyle.fontColor = [r / 255.0, g / 255.0, b / 255.0, a]
        }

        if (style.transform) {
            nativeStyle.transform = parseTransform(style.transform)
        }

        if (style.borderRadius) {
            const v = style.borderRadius
            if (typeof style.borderRadius === "number") {
                nativeStyle.borderRadius = [v, v, v, v]
            }
        }
        if (style.margin) {
            const v = style.margin
            if (typeof style.margin === "number") {
                nativeStyle.margin = [v, v, v, v]
            }
        }

        if (style.border) {
            const v = style.border
            if (typeof style.border === "number") {
                nativeStyle.border = [v, v, v, v]
            }
        }

        if (style.padding) {
            const v = style.padding
            if (typeof style.padding === "number") {
                nativeStyle.padding = [v, v, v, v]
            }
        }

        this.nativeBox.setStyle(nativeStyle, state)
    }
    setStyle(style: MeuiStyle) {
        const defaultStyle: any = {}
        for (const [k, v] of Object.entries(style)) {
            if (k in BOX_STATE) {
                const otherStyle: any = {}
                for (const [key, val] of Object.entries(v)) {
                    otherStyle[key] = val
                }
                this._setStyle(
                    otherStyle,
                    BOX_STATE[k as keyof typeof BOX_STATE]
                )
            } else {
                defaultStyle[k] = v
            }
        }

        this._setStyle(defaultStyle, BOX_STATE.DEFAULT)
    }

    checkConsistency() {
        if (this.childNodes.length !== this.nativeBox.getChildCount()) {
            throw new Error("Failed to pass consistency check")
        }
    }

    remove() {
        this.parentNode?.removeChild(this)
    }

    addChild(child: Box) {
        if (typeof child === "string" || typeof child === "number") {
            this.text += child
            this.setStyle({ text: this.text })
        } else {
            child.remove()
            this.nativeBox.addChild(child.nativeBox)
            this.childNodes.push(child)
            child.parentNode = this
            this.checkConsistency()
        }
    }

    appendChild(child: Box) {
        this.addChild(child)
    }

    insertChild(child: Box, index: number) {
        child.remove()

        this.nativeBox.insertChild(child.nativeBox, index)
        this.childNodes.splice(index, 0, child)
        child.parentNode = this
        this.checkConsistency()
    }

    insertBefore(child: Box, beforeChild: Box) {
        child.remove()
        const index = this.childNodes.indexOf(beforeChild)
        if (index !== -1) {
            this.insertChild(child, index)
            this.checkConsistency()
        }
    }
    removeChild(child: Box) {
        const index = this.childNodes.indexOf(child)

        if (index !== -1) {
            this.nativeBox.removeChild(child.nativeBox)
            this.childNodes.splice(index, 1)
            child.parentNode = null
            this.checkConsistency()
        }
    }

    getState() {
        return this.nativeBox.getState()
    }

    setState(state: UI_STATE) {
        this.nativeBox.setState(state)
    }

    hit(x: number, y: number) {
        return this.nativeBox.hit(x, y)
    }

    search(x: number, y: number) {
        return this.nativeBox.search(x, y)
    }

    getNativeObject() {
        return this.nativeBox
    }

    addEventListener<K extends string>(
        type: K,
        listener: EventListener<K>,
        useCapture = false
    ) {
        if (!listener) return

        const listenerArray =
            this.eventListeners[type] ?? (this.eventListeners[type] = [])
        listenerArray.push({ listener, useCapture })
    }

    removeEventListener<K extends string>(
        type: K,
        listener: EventListener<K>,
        useCapture = false
    ) {
        if (!listener || !type) return
        if (!this.eventListeners[type]) return

        const listenerArray = this.eventListeners[type]
        if (!listenerArray) return

        const index = listenerArray.findIndex(
            (value) =>
                value.listener === listener && value.useCapture === useCapture
        )

        if (index !== -1) {
            listenerArray.splice(index, 1)
            return
        }
    }

    toClient(x: number, y: number) {
        return this.nativeBox.toClient(x, y)
    }

    toOffset(x: number, y: number) {
        return this.nativeBox.toOffset(x, y)
    }

    tryHandleEvent(event: MeuiEvent, capture: boolean) {
        const listenerArray = this.eventListeners[event.type]
        if (!listenerArray) return

        for (const { listener, useCapture } of listenerArray) {
            if (["mouseup", "mousedown", "mousemove"].includes(event.type)) {
                const mouseEvent = event as MeuiMouseEvent
                const [clientX, clientY] = this.toClient(
                    mouseEvent.screenX,
                    mouseEvent.screenY
                )
                Object.defineProperty(mouseEvent, "clientX", {
                    ...Object.getOwnPropertyDescriptor(mouseEvent, "clientX"),
                    value: clientX,
                })
                Object.defineProperty(mouseEvent, "clientY", {
                    ...Object.getOwnPropertyDescriptor(mouseEvent, "clientY"),
                    value: clientY,
                })

                const [offsetX, offsetY] = this.toOffset(
                    mouseEvent.screenX,
                    mouseEvent.screenY
                )
                Object.defineProperty(mouseEvent, "offsetX", {
                    ...Object.getOwnPropertyDescriptor(mouseEvent, "offsetX"),
                    value: offsetX,
                })
                Object.defineProperty(mouseEvent, "offsetY", {
                    ...Object.getOwnPropertyDescriptor(mouseEvent, "offsetY"),
                    value: offsetY,
                })
            }
            if (capture == useCapture) {
                listener.call(this, event)
            }
        }
    }

    capturingPhase(node: Box, event: MeuiEvent) {
        if (node.parentNode) this.capturingPhase(node.parentNode, event)
        event.currentTarget = node
        node.tryHandleEvent(event, true)
    }

    bubblingPhase(node: Box, event: MeuiEvent) {
        // eslint-disable-next-line no-constant-condition
        while (true) {
            event.currentTarget = node
            node.tryHandleEvent(event, false)

            if (!node.parentNode) break
            node = node.parentNode
        }
    }

    dispatchEvent(event: MeuiEvent) {
        event.target = this
        this.capturingPhase(this, event)
        this.bubblingPhase(this, event)
    }

    getPath(): Box[] {
        const path: Box[] = []
        // eslint-disable-next-line @typescript-eslint/no-this-alias
        let box: Box | null = this
        while (box) {
            path.unshift(box)
            box = box.parentNode
        }
        return path
    }

    get scrollWidth() {
        return this.nativeBox.scrollWidth
    }

    get scrollHeight() {
        return this.nativeBox.scrollHeight
    }

    get clientWidth() {
        return this.nativeBox.clientWidth
    }

    get clientHeight() {
        return this.nativeBox.clientHeight
    }

    get scrollTopMax() {
        return this.nativeBox.scrollHeight - this.nativeBox.clientHeight
    }

    get scrollLeftMax() {
        return this.nativeBox.scrollWidth - this.nativeBox.clientWidth
    }

    get scrollTop() {
        return this.nativeBox.scrollTop
    }

    set scrollTop(val) {
        this.nativeBox.scrollTop = val
    }

    get scrollLeft() {
        return this.nativeBox.scrollLeft
    }

    set scrollLeft(val) {
        this.nativeBox.scrollLeft = val
    }
}
