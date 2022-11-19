/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { MEUI, DivElement } from "@meui-js/core"

const meui = new MEUI(100, 100)

const root = meui.getRoot()

const rootDiv = new DivElement({
    backgroundColor: "red",
    width: "100%",
    height: "100%",
    justifyContent: "center",
    alignItems: "center",
    flexWrap: "wrap",
    overflow: "scroll",
    backgroundImage: "res/img/file.svg",
    border: 10,
    borderColor: "green",
})

root.appendChild(rootDiv)

setTimeout(() => {
    meui.screenshot("./output.png")
}, 1000)
