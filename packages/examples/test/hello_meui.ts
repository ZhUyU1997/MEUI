/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { MEUI, DivElement } from "@meui-js/core"

const meui = new MEUI(320, 170, "RGB565")

const root = meui.getRoot()

const rootDiv = new DivElement({
    backgroundColor: "#FFFFFFFF",
    width: "100%",
    height: "100%",
    justifyContent: "center",
    alignItems: "center",
})

root.appendChild(rootDiv)

const item0 = new DivElement({
    textAlign: "center",
    width: 120,
    height: 120,
    backgroundColor: "#46e891",
    borderRadius: 16,
    // zIndex: 1,
})
rootDiv.appendChild(item0)

async function animation() {
    let deg = 0
    for (;;) {
        // console.log(`rotate(${deg}deg)`)

        deg += 10
        deg %= 360
        item0.setStyle({
            transform: `rotate(${deg}deg)`,
        })
        // meui.update()
        await new Promise((resolve) => setTimeout(resolve, 24))
    }
}

animation()

setTimeout(() => {
    console.log("screenshot")
    // meui.screenshot("./output.png")
}, 1000)
