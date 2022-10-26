/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { createElement } from "../"
import { Text } from "./text"

globalThis.document = {
    createElement(tagName: string) {
        return createElement(tagName)
    },
    createTextNode(data: string) {
        return new Text(data)
    },
} as any
