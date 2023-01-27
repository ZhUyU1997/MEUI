/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

export const View = "View"
export const Stack = "Stack"
export const Canvas = "Canvas"
export type { MeuiCanvasElementProps, MeuiElementProps } from "./props"

import { MEUI } from "@meui-js/core"
import { ColorFormat } from "meui-native"
import { VNode } from "preact"
import { render as _render } from "preact"

export function createRoot(
    width: number,
    height: number,
    format: ColorFormat = "ARGB8888"
) {
    const meui = new MEUI(width, height, format)

    function unmount() {
        _render(null as any, meui.getRoot() as any)
    }
    return {
        render(element: VNode<any>) {
            _render(element, meui.getRoot() as any)
            meui.onunload = unmount
        },
        unmount,
    }
}
