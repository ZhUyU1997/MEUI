/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { useState } from "preact/compat"
import { View, Stack } from "@meui-js/preact"
import { createRoot } from "@meui-js/preact"
import { Root } from "@meui-js/components"

const SCREEN_WIDTH = 396
const SCREEN_HEIGHT = 484

// 三角转直角
function PointTo([x, y]) {
    return [x + y / 2, (y * Math.sqrt(3)) / 2]
}
// 直角转三角
function PointFrom([x, y]) {
    return [x - y / Math.sqrt(3), (2 * y) / Math.sqrt(3)]
}

let PointMap = [
    [
        [0, 0],
        [0, 0, 51],
    ],
    [
        [-1, 0],
        [-111, 0, 45],
    ],
    [
        [0, -1],
        [-53, -99, 45],
    ],
    [
        [-1, -1],
        [-150, -91, 38],
    ],
    [
        [1, -2],
        [0, -185, 38],
    ],
    [
        [0, -2],
        [-105, -180, 35],
    ],
    [
        [-2, 0],
        [-173, 0, 10],
    ],
    [
        [1, -3],
        [-61, -223, 10],
    ],
    [
        [-1, -2],
        [-169, -167, 10],
    ],
    [
        [-3, 0],
        [-183, 0, -10],
    ],
    [
        [2, -4],
        [0, -243, -10],
    ],
    [
        [-2, -1],
        [-183, -80, -10],
    ],
    [
        [-2, -2],
        [-183, -130, -10],
    ],
    [
        [-1, -3],
        [-175, -207, -10],
    ],
    [
        [0, -3],
        [-135, -210, -10],
    ],
    [
        [1, -4],
        [-91, -243, -10],
    ],
]

PointMap = PointMap.reduce((previous, [from, [x, y, size]]) => {
    previous.push([from, [x, y, size]])

    if (x !== 0 || y !== 0) {
        const to1 = PointTo(from)

        if (y !== 0) {
            const from1 = PointFrom([to1[0], -to1[1]])

            from1[0] = Math.round(from1[0])
            from1[1] = Math.round(from1[1])
            previous.push([from1, [x, -y, size]])
        }
        if (x !== 0) {
            const from1 = PointFrom([-to1[0], to1[1]])
            from1[0] = Math.round(from1[0])
            from1[1] = Math.round(from1[1])
            previous.push([from1, [-x, y, size]])
        }

        if (x !== 0 && y !== 0) {
            const from1 = PointFrom([-to1[0], -to1[1]])
            from1[0] = Math.round(from1[0])
            from1[1] = Math.round(from1[1])
            previous.push([from1, [-x, -y, size]])
        }
    }
    return previous
}, [])

function IsNeighbor(p1, p2) {
    const dx = p1[0] - p2[0]
    const dy = p1[1] - p2[1]
    const [x, y] = PointTo([dx, dy])
    return Math.ceil(Math.sqrt(x * x + y * y)) === 1
}

function ScanTriangle() {
    const map = []

    PointMap.forEach(([from], index) => {
        const x = from[0] + 10
        const y = from[1] + 10

        if (!map[x]) map[x] = []

        map[x][y] = {
            visited: false,
            from: [x, y],
            index,
        }
    })

    const dirs = [
        [1, 0],
        [-1, 0],
        [0, -1],
        [0, 1],
        [-1, 1],
        [1, -1],
    ]

    const queue = []
    const start = map[10][10]
    start.visited = true

    queue.push({
        from: start.from,
        parent: null,
        current: start,
    })

    const TriangleRecored = {}
    while (queue.length > 0) {
        const item = queue.shift()
        const from = item.from
        const parent = item.parent
        const current = item.current

        for (const [dx, dy] of dirs) {
            const x = from[0]
            const y = from[1]
            if (map[x + dx]) {
                const next = map[x + dx][y + dy]
                if (next) {
                    if (next.visited === false) {
                        next.visited = true
                        queue.push({
                            from: next.from,
                            parent: current,
                            current: next,
                        })
                    }

                    if (parent && IsNeighbor(parent.from, next.from)) {
                        const ps = [
                            parent.index,
                            current.index,
                            next.index,
                        ].sort()

                        TriangleRecored[ps.toString()] =
                            TriangleRecored[ps.toString()] ?? ps
                    }
                }
            }
        }
    }

    const result = {}
    Object.values(TriangleRecored).forEach((ps) => {
        const minx = Math.min(...ps.map((index) => PointMap[index][0][0]))
        const maxx = Math.max(...ps.map((index) => PointMap[index][0][0]))
        const miny = Math.min(...ps.map((index) => PointMap[index][0][1]))
        const maxy = Math.max(...ps.map((index) => PointMap[index][0][1]))

        const composite = [
            [minx, maxx, miny, maxy],
            [minx, minx, miny, maxy],
            [maxx, maxx, miny, maxy],
            [minx, maxx, miny, miny],
            [minx, maxx, maxy, maxy],
            [minx, minx, maxy, maxy],
            [maxx, maxx, maxy, maxy],
            [minx, minx, miny, miny],
            [maxx, maxx, miny, miny],
        ]

        for (const range of composite) {
            const key = `${range[0]}-${range[1]}-${range[2]}-${range[3]}`
            if (key in result) result[key].push(ps)
            else result[key] = [ps]
        }
    })
    return result
}

const Triangle = ScanTriangle()

const ICON_SIZE = 51 * 2

function GetFactor([x0, y0], [x1, y1], [x2, y2], [x3, y3]) {
    var divisor = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3)
    var a = ((y2 - y3) * (x0 - x3) + (x3 - x2) * (y0 - y3)) / divisor
    var b = ((y3 - y1) * (x0 - x3) + (x1 - x3) * (y0 - y3)) / divisor
    var c = 1 - a - b

    return [a, b, c]
}

function pointInTriangle(p, p1, p2, p3) {
    const [a, b, c] = GetFactor(p, p1, p2, p3)
    return a >= 0 && a <= 1 && b >= 0 && b <= 1 && c >= 0 && c <= 1
}

function GetTriangle(x, y) {
    const p = PointFrom([x, y])
    const minx = Math.floor(p[0])
    const maxx = Math.ceil(p[0])
    const miny = Math.floor(p[1])
    const maxy = Math.ceil(p[1])

    const key = `${minx}-${maxx}-${miny}-${maxy}`

    if (!(key in Triangle)) return

    for (const [i1, i2, i3] of Triangle[key]) {
        const p1 = PointMap[i1][0]
        const p2 = PointMap[i2][0]
        const p3 = PointMap[i3][0]

        const ret = pointInTriangle(p, p1, p2, p3)
        if (ret) {
            return [
                [i1, i2, i3],
                GetFactor([x, y], PointTo(p1), PointTo(p2), PointTo(p3)),
            ]
        }
    }
}

function GetValue([s1, s2, s3], [a, b, c]) {
    return a * s1 + b * s2 + c * s3
}

function Calc(x, y) {
    const p = PointFrom([x, y])
    if (p[0] > 4 || p[0] < -4 || p[1] > 4 || p[1] < -4)
        return {
            size: 0,
            x: 0,
            y: 0,
        }

    const result = GetTriangle(x, y)
    if (result) {
        const [index, factor] = result
        const p1 = PointMap[index[0]][1]
        const p2 = PointMap[index[1]][1]
        const p3 = PointMap[index[2]][1]

        const x = GetValue([p1[0], p2[0], p3[0]], factor)
        const y = GetValue([p1[1], p2[1], p3[1]], factor)
        const size = GetValue([p1[2], p2[2], p3[2]], factor)

        return {
            size,
            x,
            y,
        }
    }
    return {
        size: 0,
        x: 0,
        y: 0,
    }
}

function App() {
    const [icons] = useState(() =>
        Array(12 * 12)
            .fill(null)
            .map((_, index) => {
                let row = Math.floor(index / 12)
                let col = Math.floor(index % 12)
                const odd = Math.floor(row % 2) == 1
                row = ((row - 6) * Math.sqrt(3)) / 2
                col = col - 3 + (odd ? 0.5 : 0)
                // const dis = Math.sqrt(3) / 2
                return {
                    color: `rgb(${Math.floor(
                        255 - Math.sqrt(row * row + col * col) * 80
                    )},${Math.floor(
                        Math.sqrt(row * row + col * col) * 80
                    )},${Math.floor(Math.sqrt(row * row + col * col) * 80)})`,
                    row,
                    col,
                }
            })
    )

    const [offset, setOffset] = useState({
        x: 0,
        y: 0,
    })

    const [start, setStart] = useState({
        offsetX: 0,
        offsetY: 0,
        x: 0,
        y: 0,
    })

    const [touching, setTouching] = useState(false)
    return (
        <Root>
            <View
                style={{
                    backgroundColor: "black",
                    borderRadius: 40,
                    width: "100%",
                    overflow: "hidden",
                }}
                onMouseDown={(e) => {
                    setTouching(true)
                    setStart({
                        x: e.screenX,
                        y: e.screenY,
                        offsetX: offset.x,
                        offsetY: offset.y,
                    })
                }}
                onMouseUp={(e) => {
                    setTouching(false)
                }}
                onMouseMove={(e) => {
                    if (touching) {
                        setOffset(({ x, y }) => ({
                            x: start.offsetX + (e.screenX - start.x),
                            y: start.offsetY + (e.screenY - start.y),
                        }))
                    }
                }}
            >
                <Stack>
                    {icons.map((item, index) => {
                        const { size, x, y } = Calc(
                            item.col + offset.x / 100,
                            item.row + offset.y / 100
                        )

                        const scale = (size * 2) / ICON_SIZE

                        return scale <= 0 ? null : (
                            <View
                                key={`${item.row} ${item.col}`}
                                style={{
                                    width: ICON_SIZE,
                                    height: ICON_SIZE,
                                    left: x - ICON_SIZE / 2 + SCREEN_WIDTH / 2,
                                    top: y - ICON_SIZE / 2 + SCREEN_HEIGHT / 2,
                                    transform: `scale(${scale})`,

                                    borderRadius: ICON_SIZE / 2,
                                    backgroundColor: item.color,
                                    fontSize: 90,
                                }}
                            >
                                {index}
                            </View>
                        )
                    })}
                </Stack>
            </View>
        </Root>
    )
}

createRoot(396, 484).render(<App />)
