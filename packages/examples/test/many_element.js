/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { MEUI, DivElement } from "@meui-js/core"

const meui = new MEUI(1000, 1000)

const root = meui.getRoot()

const rootDiv = new DivElement({
    backgroundColor: "#FFFFFFFF",
    width: "100%",
    height: "100%",
    justifyContent: "center",
    alignItems: "center",
    flexWrap: "wrap",
    overflow: "scroll",
})

root.appendChild(rootDiv)

for (let i = 0; i < 1788; i++) {
    const item0 = new DivElement({
        justifyContent: "center",
        alignItems: "center",
        alignContent: "center",
        flexDirection: "column",
        textAlign: "center",
        fontSize: 20,
        width: 120,
        height: 150,
        borderRadius: 20,
        margin: [15, 15, 15, 15],
        backgroundColor: "white",
        HOVER: {
            backgroundColor: "rgba(0, 177, 255, 0.1)",
        },
    })
    const item0_0 = new DivElement({
        width: 70,
        height: 70,
        contentImage: "res/icon/folder.svg",
    })
    const item0_1 = new DivElement({
        textAlign: "center",
        fontSize: 15,
        width: 90,
        height: 30,
    })
    item0_1.textContent = "test"
    rootDiv.appendChild(item0)
    item0.appendChild(item0_0)
    item0.appendChild(item0_1)
}
