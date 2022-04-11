declare module "NativeMEUI" {
    export interface MeuiMouseDownRawEvent {
        type: "mousedown"
        x: number
        y: number
        button: number
    }
    export interface MeuiMouseUpRawEvent {
        type: "mouseup"
        x: number
        y: number
        button: number
    }
    export interface MeuiMouseMoveRawEvent {
        type: "mousemove"
        x: number
        y: number
        button: number
    }
    export interface MeuiMouseWheelRawEvent {
        type: "mousewheel"
        deltaX: number
        deltaY: number
        deltaZ: number
        button: number
    }

    export type MeuiRawEvent =
        | MeuiMouseDownRawEvent
        | MeuiMouseUpRawEvent
        | MeuiMouseMoveRawEvent
        | MeuiMouseWheelRawEvent

    export class NativeMEUI {
        constructor(width: number, height: number)
        render(root: Box): void
        flush(): void
        update(): void
        debug(): void
        addFontFace(fontFamily: string, path: string): void
        getConnectNumber(): number
        pending(): number
        nextEvent(): MeuiRawEvent | null
    }
    export class Box {
        constructor(type: string)
        getStyle(state: UI_STATE): Record<string, any>
        setStyle(style: Record<string, any>, state: UI_STATE): void
        addChild(child: Box): void
        insertChild(child: Box, index: number): void
        removeChild(child: Box): void
        setState(state: UI_STATE): void
        getState(): UI_STATE
        hit(x: number, y: number): boolean
        get scrollLeft(): number
        set scrollLeft(value: number)
        get scrollTop(): number
        set scrollTop(value: number)
        get scrollWidth(): number
        set scrollWidth(value: number)
        get scrollHeight(): number
        set scrollHeight(value: number)
        get clientWidth(): number
        set clientWidth(value: number)
        get clientHeight(): number
        set clientHeight(value: number)
    }
    export const BOX_STATE: {
        DEFAULT: 0
        FOCUS: 1
        ACTIVE: 2
        HOVER: 3
        SELECT: 4
        DISABLE: 5
        CHECK: 6
    }

    export type UI_STATE = typeof BOX_STATE[keyof typeof BOX_STATE]

    export const Canvas: {
        putImage(
            buffer: ArrayBuffer,
            dx: number,
            dy: number,
            dirtyX: number,
            dirtyY: number,
            dirtyWidth: number,
            dirtyHeight: number,
            width: number,
            height: number,
        ): void
        getWidth(): number
        setWidth(width: number): void
        getHeight(): number
        setHeight(height: number): void
    }
}
