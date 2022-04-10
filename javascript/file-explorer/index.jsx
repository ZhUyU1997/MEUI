import React, { useState, useEffect, useLayoutEffect, useRef } from "react"
import { readdir, stat, S_IFMT, S_IFDIR } from "os"
import * as os from "os"
import { MEUI, ReactMEUI } from "../meui"
import path from "../path"

import {
    useTransition,
    useSpring,
    useChain,
    config,
    animated,
    useSpringRef,
} from "../react-sping-meui"

import { Center, Row, Column, Root, MaterialDesignIcon } from "../components"
function PathBar({ fullPath, onChange }) {
    const pathArray = fullPath.split(path.sep)
    const { root } = path.parse(fullPath)
    pathArray[0] = root

    return (
        <Row
            style={{
                flexDirection: "row",
                width: "100%",
                height: 50,
                minHeight: 50,
                padding: [5, 5, 5, 5],
            }}
        >
            <Row
                style={{
                    padding: [0, 10, 0, 10],
                    borderRadius: [20, 20, 20, 20],
                    backgroundColor: "rgba(0, 0, 0, 0.04)",
                    width: "100%",
                    height: 40,
                    flexGrow: 1,
                    justifyContent: "flex-start",
                    HOVER: {
                        backgroundColor: "rgba(0, 0, 20, 0.1)",
                    },
                }}
            >
                {pathArray.map((name, index) => {
                    return (
                        <>
                            {index === 0 ? null : (
                                <MaterialDesignIcon
                                    width={20}
                                    height={40}
                                    icon={"\u{F035F}"}
                                ></MaterialDesignIcon>
                            )}
                            <Column
                                style={{
                                    padding: [0, 10, 0, 10],
                                    fontSize: 18,
                                    height: "100%",
                                    HOVER: {
                                        backgroundColor: "white",
                                    },
                                }}
                                onClick={() => {
                                    onChange?.(
                                        path.join(
                                            ...pathArray.slice(0, index + 1)
                                        )
                                    )
                                }}
                            >
                                {name}
                            </Column>
                        </>
                    )
                })}
            </Row>
        </Row>
    )
}

function FileExplorer() {
    const [curPath, setCurPath] = useState(() => process.cwd())
    const [dirent, setDirent] = useState([])

    const transition = useTransition(dirent, {
        trail: 18,
        from: { opacity: 0, scale: 0 },
        enter: { opacity: 1, scale: 1 },
    })

    useLayoutEffect(() => {
        const [d, err] = readdir(curPath)
        setDirent(
            d
                .sort()
                .filter((name) => name !== ".")
                .map((name) => {
                    const attr = stat(path.join(curPath, name))[0]
                    return {
                        name,
                        attribute: attr,
                        isDir: (attr.mode & S_IFMT) === S_IFDIR,
                    }
                })
                .sort((a, b) => {
                    let va = a.isDir
                    let vb = b.isDir
                    return vb - va
                })
        )
    }, [curPath])

    return (
        <Root
            style={{
                flexDirection: "column",
            }}
        >
            <PathBar
                fullPath={curPath}
                onChange={(newPath) => {
                    setCurPath(newPath)
                }}
            />
            <div
                style={{
                    alignItems: "flex-start",
                    alignContent: "flex-start",
                    flexDirection: "row",
                    flexWrap: "wrap",
                    width: "100%",
                    flexGrow: 1,
                    overflow: "scroll",
                }}
            >
                {transition((style, { name, attribute, isDir }) => {
                    return (
                        <animated.div
                            key={name}
                            style={{
                                justifyContent: "center",
                                alignItems: "center",
                                alignContent: "center",
                                flexDirection: "column",
                                textAlign: "center",
                                fontSize: 20,
                                width: 100,
                                height: 120,
                                HOVER: {
                                    backgroundColor: "rgba(0, 177, 255, 0.1)",
                                },
                                ...style,
                            }}
                            onClick={() => {
                                let nextPath = path.join(curPath, name)
                                const [, err] = readdir(nextPath)

                                if (err === 0) {
                                    setCurPath(nextPath)
                                }
                            }}
                        >
                            <MaterialDesignIcon
                                width={100}
                                height={70}
                                color={isDir ? "#ffbf00" : "black"}
                                icon={isDir ? "\u{F024B}" : "\u{F0214}"}
                            />
                            <Center
                                style={{
                                    textAlign: "center",
                                    fontSize: 20,
                                    width: 90,
                                    height: 30,
                                }}
                            >
                                {name}
                            </Center>
                        </animated.div>
                    )
                })}
            </div>
        </Root>
    )
}

export function Main() {
    const meui = new MEUI(1200, 800)
    ReactMEUI.render(<FileExplorer />, meui)
}
