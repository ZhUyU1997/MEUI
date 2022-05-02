import React, { useState, useEffect, useLayoutEffect, useRef } from "react"
import { readdir, stat, S_IFMT, S_IFDIR } from "os"
import * as os from "os"
import * as std from "std"

import { Div, MEUI, ReactMEUI } from "@/meui"
import path from "@/path"

import {
    useTransition,
    useSpring,
    useChain,
    config,
    animated,
    useSpringRef,
} from "@/react-sping-meui"

import { Center, Row, Column, Root, MaterialDesignIcon } from "@/components"

function Icon({ icon, backgroundColor, style }) {
    return (
        <MaterialDesignIcon
            icon={icon}
            color="rgba(0,0,0, 0.50)"
            style={{
                padding: [10, 10, 10, 10],
                marginRight: 5,
                marginLeft: 5,
                borderRadius: 10,
                fontSize: 18,
                height: "100%",
                width: 50,
                backgroundColor,
                fontColor: "black",
                HOVER: {
                    backgroundColor: "rgba(0,0,0, 0.05)",
                },
                ...style,
            }}
        ></MaterialDesignIcon>
    )
}

function TopBar({ fullPath, onChange }) {
    const pathArray = fullPath.split(path.sep).filter((item) => {
        return typeof item === "string" && item !== ""
    })

    console.log(fullPath, pathArray, pathArray.length)
    return (
        <Row
            style={{
                flexDirection: "row",
                width: "100%",
                height: 60,
                minHeight: 60,
                padding: [5, 5, 5, 5],
            }}
        >
            <Icon
                icon={"\u{F004D}"}
                backgroundColor="rgba(0,0,0, 0.04)"
                style={{
                    marginLeft: 5,
                    marginRight: undefined,

                    borderRadius: [10, 0, 0, 10],
                    HOVER: {
                        backgroundColor: "rgba(0,0,0, 0.09)",
                    },
                }}
            ></Icon>
            <Icon
                icon={"\u{F0054}"}
                backgroundColor="rgba(0,0,0, 0.04)"
                style={{
                    marginRight: 5,
                    marginLeft: undefined,
                    borderRadius: [0, 10, 10, 0],
                    HOVER: {
                        backgroundColor: "rgba(0,0,0, 0.09)",
                    },
                }}
            ></Icon>

            <Icon icon={"\u{F02DC}"} backgroundColor="rgba(0,0,0, 0.03)"></Icon>
            <Row
                style={{
                    padding: [0, 5, 0, 5],
                    borderRadius: [20, 20, 20, 20],
                    width: "100%",
                    height: 45,
                    flexGrow: 1,
                    justifyContent: "flex-start",
                }}
            >
                {pathArray.map((name, index) => {
                    return (
                        <Column
                            key={index}
                            style={{
                                padding: [10, 10, 10, 10],
                                margin: [0, 5, 0, 5],
                                fontSize: 18,
                                height: "100%",
                                borderRadius: 10,
                                backgroundColor:
                                    index === pathArray.length - 1
                                        ? "rgba(0,129,255,1)"
                                        : "rgba(0,0,0, 0.03)",
                                fontColor:
                                    index === pathArray.length - 1
                                        ? "white"
                                        : "black",
                                HOVER: {
                                    backgroundColor:
                                        index === pathArray.length - 1
                                            ? "rgba(0,129,255,0.8)"
                                            : "rgba(0,0,0, 0.05)",
                                },
                            }}
                            onClick={() => {
                                onChange?.(
                                    path.join(
                                        "/",
                                        ...pathArray.slice(0, index + 1)
                                    )
                                )
                            }}
                        >
                            {name}
                        </Column>
                    )
                })}
            </Row>
            <Icon icon={"\u{F021E}"}></Icon>
            <Icon icon={"\u{F0570}"}></Icon>
            <Icon icon={"\u{F0572}"}></Icon>
            <Icon icon={"\u{F0BAB}"}></Icon>
            <Icon icon={"\u{F05B0}"}></Icon>
            <Icon icon={"\u{F05AF}"}></Icon>
            <Icon icon={"\u{F05AD}"}></Icon>
        </Row>
    )
}

function FileExplorer() {
    const [curPath, setCurPath] = useState(() => process.cwd())
    const [dirent, setDirent] = useState([])
    // console.log(JSON.stringify(dirent))
    useLayoutEffect(() => {
        const [d, err] = readdir(curPath)
        console.log(curPath)
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
                borderRadius: 20,
            }}
        >
            <TopBar
                fullPath={curPath}
                onChange={(newPath) => {
                    setCurPath(newPath)
                }}
            />
            <Div
                key={curPath}
                style={{
                    alignItems: "flex-start",
                    alignContent: "flex-start",
                    flexDirection: "row",
                    flexWrap: "wrap",
                    width: "100%",
                    // TODO clip scroll area
                    borderRadius: 20,

                    flexGrow: 1,
                    overflow: "scroll",
                    backgroundColor: "#F8F8F8",
                }}
            >
                {dirent.map(({ name, attribute, isDir }) => {
                    return (
                        <Div
                            key={path.join(curPath, name)}
                            style={{
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
                            }}
                            onClick={() => {
                                let nextPath = path.join(curPath, name)
                                const [, err] = readdir(nextPath)

                                if (err === 0) {
                                    setCurPath(nextPath)
                                }
                            }}
                        >
                            <Div
                                style={{
                                    width: 70,
                                    height: 70,
                                    contentImage: isDir
                                        ? "res/icon/folder.svg"
                                        : undefined,
                                    backgroundColor: isDir
                                        ? undefined
                                        : "green",
                                }}
                            />
                            <Center
                                style={{
                                    textAlign: "center",
                                    fontSize: 15,
                                    width: 90,
                                    height: 30,
                                }}
                            >
                                {name}
                            </Center>
                        </Div>
                    )
                })}
            </Div>
        </Root>
    )
}

const meui = new MEUI(1200, 800)
ReactMEUI.render(<FileExplorer />, meui)
