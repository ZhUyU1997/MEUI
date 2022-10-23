// Users who only use Preact for SSR might not specify "dom" in their lib in tsconfig.json
/// <reference lib="dom" />

import { CanvasElement, DivElement, StackElement } from "@meui/core"
import type { MeuiElementProps, MeuiCanvasElementProps } from "./props"

import {
    ClassAttributes,
    Component,
    ComponentChild,
    ComponentType,
    FunctionComponent,
    PreactDOMAttributes,
    VNode,
} from "preact"

type Defaultize<Props, Defaults> =
    // Distribute over unions
    Props extends any // Make any properties included in Default optional
        ? Partial<Pick<Props, Extract<keyof Props, keyof Defaults>>> & // Include the remaining properties from Props
              Pick<Props, Exclude<keyof Props, keyof Defaults>>
        : never

export namespace JSXInternal {
    export type LibraryManagedAttributes<Component, Props> = Component extends {
        defaultProps: infer Defaults
    }
        ? Defaultize<Props, Defaults>
        : Props

    export interface IntrinsicAttributes {
        key?: any
    }

    export type ElementType<P = any> =
        | {
              [K in keyof IntrinsicElements]: P extends IntrinsicElements[K]
                  ? K
                  : never
          }[keyof IntrinsicElements]
        | ComponentType<P>
    export interface Element extends VNode<any> {}
    export type ElementClass = Component<any, any> | FunctionComponent<any>

    export interface ElementAttributesProperty {
        props: any
    }

    export interface ElementChildrenAttribute {
        children: any
    }

    export type DOMCSSProperties = {
        [key in keyof Omit<
            CSSStyleDeclaration,
            | "item"
            | "setProperty"
            | "removeProperty"
            | "getPropertyValue"
            | "getPropertyPriority"
        >]?: string | number | null | undefined
    }
    export type AllCSSProperties = {
        [key: string]: string | number | null | undefined
    }
    export interface CSSProperties extends AllCSSProperties, DOMCSSProperties {
        cssText?: string | null
    }

    export type TargetedEvent<
        Target extends EventTarget = EventTarget,
        TypedEvent extends Event = Event
    > = Omit<TypedEvent, "currentTarget"> & {
        readonly currentTarget: Target
    }

    export interface EventHandler<E extends TargetedEvent> {
        /**
         * The `this` keyword always points to the DOM element the event handler
         * was invoked on. See: https://developer.mozilla.org/en-US/docs/Web/Guide/Events/Event_handlers#Event_handlers_parameters_this_binding_and_the_return_value
         */
        (this: never, event: E): void
    }

    export interface IntrinsicElements {
        view: MeuiElementProps & ClassAttributes<DivElement>

        View: MeuiElementProps & ClassAttributes<DivElement>
        Canvas: MeuiCanvasElementProps & ClassAttributes<CanvasElement>
        Stack: MeuiElementProps & ClassAttributes<StackElement>
    }
}
