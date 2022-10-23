/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { useState } from "preact/compat"
import { Button } from "@meui/components"
import { View } from "@meui/preact"
import { createRoot } from "@meui/preact"

import {
    useTransition,
    useSpring,
    useChain,
    config,
    animated,
    useSpringRef,
} from "@meui/spring"

const data = [
    {
        name: "Rare Wind",
        description: "#a8edea → #fed6e3",
        css: "#a8edea",
        height: 200,
    },
    {
        name: "Saint Petersburg",
        description: "#f5f7fa → #c3cfe2",
        css: "#f5f7fa",
        height: 400,
    },
    {
        name: "Deep Blue",
        description: "#e0c3fc → #8ec5fc",
        css: "#e0c3fc",
        height: 400,
    },
    {
        name: "Ripe Malinka",
        description: "#f093fb → #f5576c",
        css: "#f093fb",
        height: 400,
    },
    {
        name: "Perfect White",
        description: "#fdfbfb → #ebedee",
        css: "#fdfbfb",
        height: 400,
    },
    {
        name: "Near Moon",
        description: "#5ee7df → #b490ca",
        css: "#5ee7df",
        height: 400,
    },
    {
        name: "Wild Apple",
        description: "#d299c2 → #fef9d7",
        css: "#d299c2",
        height: 200,
    },
    {
        name: "Ladoga Bottom",
        description: "#ebc0fd → #d9ded8",
        css: "#ebc0fd",
        height: 400,
    },
    {
        name: "Sunny Morning",
        description: "#f6d365 → #fda085",
        css: "#f6d365",
        height: 200,
    },
    {
        name: "Lemon Gate",
        description: "#96fbc4 → #f9f586",
        css: "#96fbc4",
        height: 400,
    },
    {
        name: "Salt Mountain",
        description: " #FFFEFF → #D7FFFE",
        css: "#FFFEFF",
        height: 200,
    },
    {
        name: "New York",
        description: " #fff1eb → #ace0f9",
        css: "#fff1eb",
        height: 400,
    },
    {
        name: "Soft Grass",
        description: " #c1dfc4 → #deecdd",
        css: "#c1dfc4",
        height: 400,
    },
    {
        name: "Japan Blush",
        description: " #ddd6f3 → #faaca8",
        css: "#ddd6f3",
        height: 200,
    },
]
function App() {
    const [open, set] = useState(false)

    const springApi = useSpringRef()
    const { size, ...rest } = useSpring({
        ref: springApi,
        config: config.stiff,
        from: { size: "20%", backgroundColor: "hotpink" },
        to: {
            size: open ? "100%" : "20%",
            backgroundColor: open ? "white" : "hotpink",
        },
    })
    const transApi = useSpringRef()
    const transition = useTransition(open ? data : [], {
        ref: transApi,
        trail: 400 / data.length,
        from: { opacity: 0, scale: 0 },
        enter: { opacity: 1, scale: 1 },
        leave: { opacity: 0, scale: 0 },
    })

    // This will orchestrate the two animations above, comment the last arg and it creates a sequence
    useChain(open ? [springApi, transApi] : [transApi, springApi], [
        0,
        open ? 0.1 : 0.6,
    ])

    return (
        <View
            style={{
                width: "100%",
                height: "100%",
                backgroundColor: "lightblue",
                padding: [20, 20, 20, 20],
                alignItems: "center",
                justifyContent: "center",
            }}
        >
            <animated.View
                style={{
                    backgroundColor: "white",
                    ...rest,
                    width: size,
                    height: size,
                    padding: [25, 25, 25, 25],
                    borderRadius: 5,
                    flexWrap: "wrap",
                }}
                onClick={() => set((open) => !open)}
            >
                {transition((style, item) => (
                    <animated.View
                        style={{
                            width: 190,
                            height: 43,
                            backgroundColor: "white",
                            borderRadius: 5,
                            margin: [25, 25, 25, 25],
                            ...style,
                            backgroundColor: item.css,
                        }}
                    />
                ))}
            </animated.View>
        </View>
    )
}

createRoot(1060, 340).render(<App />)
