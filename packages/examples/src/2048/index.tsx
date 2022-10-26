/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { useState } from "preact/compat"
import { createRoot, View } from "@meui-js/preact"
import { Column, Root, Row } from "@meui-js/components"
import { createGame, Direction, Tile } from "2048-engine"
import { MeuiKeyboardEvent } from "@meui-js/core"

function getColor(val: number) {
    let color = "#ffffff"
    switch (val) {
        case 2:
            color = "#F6CED8"
            break
        case 4:
            color = "#F7BE81"
            break
        case 8:
            color = "#F3F781"
            break
        case 16:
            color = "#BEF781"
            break
        case 32:
            color = "#81F7D8"
            break
        case 64:
            color = "#58D3F7"
            break
        case 128:
            color = "#FA58F4"
            break
        case 256:
            color = "#A901DB"
            break
        case 512:
            color = "#01DF3A"
            break
        case 1024:
            color = "#D7DF01"
            break
        case 2048:
            color = "#D7DF01"
            break
        default:
            color = "#ffffff"
    }
    return color
}

function Tile({ value }: { value: Tile | null }) {
    return (
        <View
            focusable={true}
            style={{
                width: 90,
                height: 90,
                margin: 5,
                borderRadius: 10,
                backgroundColor: getColor(value === null ? 1 : value.value),
                fontSize: 40,
                textAlign: "center",
            }}
        >
            {value === null ? "" : value.value}
        </View>
    )
}

function App() {
    const [, forceUpdate] = useState({})
    const [game] = useState(() => createGame())
    return (
        <Root
            style={{
                flexDirection: "column",
                justifyContent: "center",
                alignItems: "center",
                backgroundColor: "#f0f0f0",
            }}
            focusable={true}
            onKeyDown={(e: MeuiKeyboardEvent) => {
                if (e.key == "ArrowUp") {
                    // up arrow
                    game.move(Direction.UP)
                } else if (e.key == "ArrowDown") {
                    // down arrow
                    game.move(Direction.DOWN)
                } else if (e.key == "ArrowLeft") {
                    // left arrow
                    game.move(Direction.LEFT)
                } else if (e.key == "ArrowRight") {
                    // right arrow
                    game.move(Direction.RIGHT)
                }
                forceUpdate({})
            }}
        >
            <Column
                style={{
                    justifyContent: "flex-start",
                    alignItems: "flex-start",
                    borderRadius: 10,
                    width: "100%",
                    height: "100%",
                }}
            >
                <Row>
                    <Tile value={game.currentState.board[0][0]}></Tile>
                    <Tile value={game.currentState.board[0][1]}></Tile>
                    <Tile value={game.currentState.board[0][2]}></Tile>
                    <Tile value={game.currentState.board[0][3]}></Tile>
                </Row>
                <Row>
                    <Tile value={game.currentState.board[1][0]}></Tile>
                    <Tile value={game.currentState.board[1][1]}></Tile>
                    <Tile value={game.currentState.board[1][2]}></Tile>
                    <Tile value={game.currentState.board[1][3]}></Tile>
                </Row>
                <Row>
                    <Tile value={game.currentState.board[2][0]}></Tile>
                    <Tile value={game.currentState.board[2][1]}></Tile>
                    <Tile value={game.currentState.board[2][2]}></Tile>
                    <Tile value={game.currentState.board[2][3]}></Tile>
                </Row>
                <Row>
                    <Tile value={game.currentState.board[3][0]}></Tile>
                    <Tile value={game.currentState.board[3][1]}></Tile>
                    <Tile value={game.currentState.board[3][2]}></Tile>
                    <Tile value={game.currentState.board[3][3]}></Tile>
                </Row>
            </Column>
        </Root>
    )
}

createRoot(400, 400).render(<App />)
