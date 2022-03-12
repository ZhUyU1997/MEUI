import "./polyfill"
import React, { useState, useEffect, useLayoutEffect } from "react"
import ReactMEUI from "./ReactMEUI"
import Button from "./button"
import { MEUI } from "./meui"
import path from "./path"

function Root({ children, style, ...props }) {
    return <div style={{
        backgroundColor: "#FFFFFFFF",
        width: "100%",
        height: "100%",
        ...style
    }}>{children}</div>
}

function Center({ children, style, ...props }) {
    return <div style={{
        justifyContent: "center",
        alignItems: "center",
        alignContent: "center",
        ...style
    }}
        {...props}
    >{children}</div>
}

function Row({ children, style, ...props }) {
    return <div style={{
        justifyContent: "center",
        alignItems: "center",
        alignContent: "center",
        flexDirection: "row",
        ...style
    }}
        {...props}
    >{children}</div>
}

function Column({ children, style, ...props }) {
    return <div style={{
        justifyContent: "center",
        alignItems: "center",
        alignContent: "center",
        flexDirection: "column",
        ...style
    }}
        {...props}
    >{children}</div>
}

function MaterialDesignIcon({ width = 50, height = 50, icon, color = "black" }) {
    return <Center
        style={{
            textAlign: "center",
            fontSize: Math.min(width, height),
            width: width,
            height: height,
            fontFamily: "MaterialDesignIcons",
            fontColor: color,
        }}>{icon}</Center>
}

function TestHooks() {
    const [dollar, setDollar] = useState(0)
    const [rmb, setRMB] = useState(0)

    useEffect(() => {
        setRMB(6.32 * dollar)
    }, [dollar])

    return <Root>
        <div
            style={{
                justifyContent: "center",
                alignItems: "center",
                alignContent: "center",
                textAlign: "center",
                fontSize: 20,
                width: 300,
                height: 200,
            }}>
            {dollar}美元 = {rmb}人民币
        </div>
        <Button
            type="primary"
            style={{ width: 200, height: 70 }}
            onClick={() => {
                setDollar(count => count + 1)
            }}
        >Click Me</Button>
    </Root>
}

import { readdir, stat, S_IFMT, S_IFDIR } from "os"
import * as os from "os"

function PathBar({ fullPath, onChange }) {
    const pathArray = fullPath.split(path.sep)
    const { root } = path.parse(fullPath)
    pathArray[0] = root

    return <Row
        style={{
            flexDirection: "row",
            width: "100%",
            height: 50,
            minHeight:50,
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
            }}>{pathArray.map(
                (name, index) => {
                    return <>
                        {index === 0 ? null : <MaterialDesignIcon width={20} height={40} icon={"\u{F035F}"}></MaterialDesignIcon>}
                        <Column
                            style={{
                                padding: [0, 10, 0, 10],
                                width: index === 0 ? 30 : name.length * 18,
                                fontSize: 18,
                                height: "100%",
                                HOVER: {
                                    backgroundColor: "white",
                                }
                            }}
                            onClick={() => {
                                onChange?.(path.join(...pathArray.slice(0, index + 1)))
                            }}
                        >{name}</Column>
                    </>
                }
            )}</Row>
    </Row>
}

function TestDir() {
    const [curPath, setCurPath] = useState(() => process.cwd())
    const [dirent, setDirent] = useState([])

    useLayoutEffect(() => {
        const [d, err] = readdir(curPath)
        setDirent(
            d.sort().filter(name => name !== ".").map((name) => {
                const attr = stat(path.join(curPath, name))[0]
                return {
                    name,
                    attribute: attr,
                    isDir: ((attr.mode & S_IFMT) === S_IFDIR)
                }
            }).sort((a, b) => {
                let va = a.isDir
                let vb = b.isDir
                return vb - va
            })
        )
    }, [curPath])

    return <Root style={{
        flexDirection: "column",
    }}>
        <PathBar fullPath={curPath} onChange={(newPath)=>{
            setCurPath(newPath)
        }}/>
        <div style={{
            alignItems: "flex-start",
            alignContent: "flex-start",
            flexDirection: "row",
            flexWrap: "wrap",
            width: "100%",
            flexGrow: 1,
        }}>
            {
                dirent.map(({ name, attribute, isDir }, index) => {
                    return <Column
                        key={name}
                        style={{
                            textAlign: "center",
                            fontSize: 20,
                            width: 100,
                            height: 120,
                            HOVER: {
                                backgroundColor: "rgba(0, 177, 255, 0.1)",
                            }
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
                            }}>
                            {name}
                        </Center>
                    </Column>
                })
            }
        </div>
    </Root >
}

const meui = new MEUI(1000, 800)

ReactMEUI.render(<TestDir />, meui)