export { Fragment } from "preact"
import {
    ComponentType,
    ComponentChild,
    ComponentChildren,
    VNode,
    Attributes,
} from "preact"
import { JSXInternal } from "../src/jsx"

export function jsx(
    type: string,
    props: Record<string, any> & { children?: ComponentChild },
    key?: string
): VNode<any>
export function jsx<P>(
    type: ComponentType<P>,
    props: Attributes & P & { children?: ComponentChild },
    key?: string
): VNode<any>

export function jsxs(
    type: string,
    props: Record<string, any> & { children?: ComponentChild[] },
    key?: string
): VNode<any>
export function jsxs<P>(
    type: ComponentType<P>,
    props: Attributes & P & { children?: ComponentChild[] },
    key?: string
): VNode<any>

export function jsxDEV(
    type: string,
    props: Record<string, any> & { children?: ComponentChildren },
    key?: string
): VNode<any>
export function jsxDEV<P>(
    type: ComponentType<P>,
    props: Attributes & P & { children?: ComponentChildren },
    key?: string
): VNode<any>

export { JSXInternal as JSX }
