import { Globals, createStringInterpolator, colors } from "@react-spring/shared"
import { createHost } from "@react-spring/animated"
import { primitives } from "./primitives"
import { WithAnimated } from "./animated"
import { AnimatedStyle } from "./AnimatedStyle"
import { batchedUpdates } from "../meui/ReactMEUI"

Globals.assign({
    batchedUpdates: batchedUpdates,
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
