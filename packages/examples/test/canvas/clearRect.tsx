/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { CanvasRenderingContext2D } from "@meui-js/core"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/clearRect

    // 绘制黄色背景
    ctx.beginPath()
    ctx.fillStyle = "#ff6"
    ctx.fillRect(0, 0, 500, 500)

    // 绘制蓝色三角形
    ctx.beginPath()
    ctx.fillStyle = "blue"
    ctx.moveTo(20, 20)
    ctx.lineTo(180, 20)
    ctx.lineTo(130, 130)
    ctx.closePath()
    ctx.fill()

    // 清除一部分画布
    ctx.clearRect(10, 10, 120, 100)
}
