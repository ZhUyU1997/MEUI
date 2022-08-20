/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { render } from "preact"
import "@/meui/node/dom"

export default {
    render: (reactElement, meui, callback) => {
        render(reactElement, meui.getRoot())
    },
}
