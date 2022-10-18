/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { Globals, createStringInterpolator, colors } from "@react-spring/shared"
import { createHost } from "@react-spring/animated"
import { primitives } from "./primitives"
import { WithAnimated } from "./animated"
import { AnimatedStyle } from "./AnimatedStyle"

Globals.assign({
    createStringInterpolator,
    colors,
})

const host = createHost(primitives, {
    applyAnimatedValues(instance, props) {
        // console.log("applyAnimatedValues", JSON.stringify(props, null, 4))
        const { style } = props
        if (style) instance.setStyle(style)
    },
    createAnimatedStyle: (style) => new AnimatedStyle(style),
})

export const animated = host.animated as WithAnimated
export { animated as a }

export * from "./animated"
export * from "@react-spring/core"
