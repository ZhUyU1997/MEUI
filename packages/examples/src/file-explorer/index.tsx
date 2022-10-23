/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import * as os from "os"
import * as std from "std"
import { readdir, stat, S_IFMT, S_IFDIR } from "os"
import path from "@meui/path"

import { useState, useLayoutEffect } from "preact/compat"
import { createRoot, View, Stack } from "@meui/preact"

import { Center, Row, Column, Root, MaterialDesignIcon } from "@meui/components"
import { MeuiStyle } from "@meui/core"
import { Setting } from "./setting"
import { Icon } from "./icon"
import { ComponentChildren } from "preact"

function MenuItem({
    children,
    onClick,
}: {
    children: ComponentChildren
    onClick?: () => void
}) {
    return (
        <Row
            style={{
                width: "100%",
                height: 37,
                textAlign: "center-left",
                fontSize: 18,
                paddingLeft: 35,
                paddingRight: 35,
                HOVER: {
                    backgroundColor: "#0080FD",
                },
            }}
            onClick={onClick}
        >
            {children}
        </Row>
    )
}
function Divider() {
    return (
        <Row
            style={{
                width: "100%",
                height: 2,
                backgroundColor: "rgba(0,0,0, 0.05)",
            }}
        ></Row>
    )
}

function SettingMenu({ onClickSetting }: { onClickSetting: () => void }) {
    const [open, setOpen] = useState(false)
    return (
        <Stack
            style={{
                width: 100,
                height: "100%",
            }}
        >
            <Icon
                icon={"\u{F0BAB}"}
                style={{}}
                onClick={() => {
                    setOpen((prev) => !prev)
                }}
            ></Icon>

            {open ? (
                <Column
                    style={{
                        zIndex: 1,
                        top: 50,
                        width: 200,
                        backgroundColor: "#F7F7F8",
                        borderRadius: 10,
                        border: [1, 1, 1, 1],
                        borderColor: "#C1C1C1",
                        paddingTop: 10,
                        paddingBottom: 10,
                    }}
                    onClick={() => {
                        setOpen(false)
                    }}
                >
                    <MenuItem>新建窗口</MenuItem>
                    <Divider></Divider>
                    <MenuItem>连接服务器</MenuItem>
                    <MenuItem>设置共享密码</MenuItem>
                    <MenuItem onClick={onClickSetting}>设置</MenuItem>
                    <MenuItem>主题</MenuItem>
                    <Divider></Divider>
                    <MenuItem>帮助</MenuItem>
                    <MenuItem>反馈</MenuItem>
                    <MenuItem>关于</MenuItem>
                    <MenuItem>退出</MenuItem>
                </Column>
            ) : null}
        </Stack>
    )
}
function TopBar({
    fullPath,
    onChange,
    onChangeViewMode,
    onClickSetting,
}: {
    fullPath: string
    onChange?: (newPath: string) => void
    onChangeViewMode?: (mode: string) => void
    onClickSetting: () => void
}) {
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
            <Icon
                icon={"\u{F0570}"}
                onClick={() => {
                    onChangeViewMode?.("icon")
                }}
            ></Icon>
            <Icon
                icon={"\u{F0572}"}
                onClick={() => {
                    onChangeViewMode?.("item")
                }}
            ></Icon>
            <SettingMenu onClickSetting={onClickSetting}></SettingMenu>
            <Icon icon={"\u{F05B0}"}></Icon>
            <Icon icon={"\u{F05AF}"}></Icon>
            <Icon icon={"\u{F05AD}"}></Icon>
        </Row>
    )
}

function SideItem({
    icon,
    style,
    children,
}: {
    icon: string
    style?: MeuiStyle
    children: string
}) {
    return (
        <Row
            style={{
                width: "100%",
                justifyContent: "flex-start",
                borderRadius: 10,
                HOVER: {
                    backgroundColor: "rgba(0,0,0, 0.05)",
                },
                ...style,
            }}
        >
            <MaterialDesignIcon
                icon={icon}
                color="#444444"
                style={{
                    marginRight: 5,
                    marginLeft: 5,
                    borderRadius: 10,
                    fontSize: 18,
                    height: 40,
                    width: 30,
                    fontColor: "black",
                }}
            ></MaterialDesignIcon>
            <Row style={{ fontSize: 18, fontColor: "#444444" }}>{children}</Row>
        </Row>
    )
}

function Side() {
    return (
        <Column
            style={{
                justifyContent: "flex-start",
                alignItems: "flex-start",
                width: 250,
                flexGrow: 0,
                flexShrink: 0,
                padding: [0, 8, 0, 8],
                height: "100%",
            }}
        >
            <SideItem icon="history">最近使用</SideItem>
            <SideItem icon="home">主目录</SideItem>
            <SideItem icon="desktop-mac-dashboard">桌面</SideItem>
            <SideItem icon="video">视频</SideItem>
            <SideItem icon="music">音乐</SideItem>
            <SideItem icon="image">图片</SideItem>
            <SideItem icon="file-document">文档</SideItem>
            <SideItem icon="download">下载</SideItem>
            <SideItem icon="recycle">回收站</SideItem>
            <Divider />
            <SideItem icon="desktop-classic">计算机</SideItem>
            <SideItem icon="harddisk">系统盘</SideItem>
            <Divider />
            <SideItem icon="folder-network">网络邻居</SideItem>
            <Divider />
        </Column>
    )
}

interface DirentEntry {
    name: string
    attribute: os.stat_return_type
    isDir: boolean
}

function IconView({
    curPath,
    dirent,
    setCurPath,
}: {
    curPath: string
    dirent: DirentEntry[]
    setCurPath: (newPath: string) => void
}) {
    return (
        <View
            style={{
                alignItems: "flex-start",
                alignContent: "flex-start",
                justifyContent: "flex-start",
                flexDirection: "row",
                flexWrap: "wrap",
                flexGrow: 1,
                height: "100%",
                overflow: "scroll",
                backgroundColor: "#F8F8F8",
                borderBottomRightRadius: 20,
            }}
        >
            {dirent.map(({ name, attribute, isDir }) => {
                return (
                    <View
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
                            const nextPath = path.join(curPath, name)
                            const [, err] = readdir(nextPath)

                            if (err === 0) {
                                setCurPath(nextPath)
                            }
                        }}
                    >
                        <View
                            style={{
                                width: 70,
                                height: 70,
                                contentImage: isDir
                                    ? "res/icon/folder.svg"
                                    : undefined,
                                backgroundColor: isDir ? undefined : "green",
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
                    </View>
                )
            })}
        </View>
    )
}

function formatBytes(bytes: number, decimals = 2) {
    if (bytes === 0) return "0 Bytes"

    const k = 1024
    const dm = decimals < 0 ? 0 : decimals
    const sizes = ["Bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"]

    const i = Math.floor(Math.log(bytes) / Math.log(k))

    return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + " " + sizes[i]
}

function ItemView({
    curPath,
    dirent,
    setCurPath,
}: {
    curPath: string
    dirent: DirentEntry[]
    setCurPath: (newPath: string) => void
}) {
    return (
        <View
            style={{
                alignItems: "flex-start",
                alignContent: "flex-start",
                justifyContent: "flex-start",
                flexDirection: "column",
                flexGrow: 1,
                width: "100%",
                minHeight: "100%",
                overflow: "scroll",
                backgroundColor: "#F8F8F8",
                borderBottomRightRadius: 20,
                padding: [10, 10, 10, 10],
            }}
        >
            {dirent.map(({ name, attribute, isDir }) => {
                return (
                    <View
                        key={path.join(curPath, name)}
                        style={{
                            justifyContent: "flex-start",
                            alignItems: "center",
                            alignContent: "center",
                            flexDirection: "row",
                            textAlign: "center",
                            fontSize: 20,
                            width: "100%",
                            minHeight: 40,
                            borderRadius: 10,
                            backgroundColor: "white",
                            marginTop: 5,
                            marginBottom: 5,
                            HOVER: {
                                backgroundColor: "rgba(0, 177, 255, 0.1)",
                            },
                        }}
                        onClick={() => {
                            const nextPath = path.join(curPath, name)
                            const [, err] = readdir(nextPath)

                            if (err === 0) {
                                setCurPath(nextPath)
                            }
                        }}
                    >
                        <View
                            style={{
                                width: 30,
                                height: 30,
                                marginLeft: 10,
                                marginRight: 10,

                                contentImage: isDir
                                    ? "res/icon/folder.svg"
                                    : undefined,
                                backgroundColor: isDir ? undefined : "green",
                            }}
                        />
                        <Center
                            style={{
                                textAlign: "center-left",
                                fontSize: 15,
                                flexGrow: 1,
                                height: 30,
                            }}
                        >
                            {name}
                        </Center>
                        <Center
                            style={{
                                textAlign: "center-left",
                                fontSize: 15,
                                fontColor: "#8A91A1",
                                width: 200,
                                height: 30,
                            }}
                        >
                            {new Date(attribute.mtime).toLocaleDateString(
                                "zh-CN"
                            ) +
                                " " +
                                new Date(attribute.mtime).toLocaleTimeString(
                                    "zh-CN"
                                )}
                        </Center>
                        <Center
                            style={{
                                textAlign: "center-left",
                                fontSize: 15,
                                fontColor: "#8A91A1",
                                width: 150,
                                height: 30,
                            }}
                        >
                            {isDir ? "-" : formatBytes(attribute.size)}
                        </Center>
                        <Center
                            style={{
                                textAlign: "center-left",
                                fontSize: 15,
                                fontColor: "#8A91A1",
                                width: 150,
                                height: 30,
                            }}
                        >
                            {isDir ? "目录" : "文档"}
                        </Center>
                    </View>
                )
            })}
        </View>
    )
}

function FileExplorer() {
    // const [curPath, setCurPath] = useState(() => process.cwd())
    const [curPath, setCurPath] = useState(() => "/bin")

    const [dirent, setDirent] = useState<DirentEntry[]>([])
    const [viewMode, setViewMode] = useState<string>("icon")
    const [showSetting, setShowSetting] = useState<boolean>(false)
    // console.log(JSON.stringify(dirent))
    useLayoutEffect(() => {
        const [d, err] = readdir(curPath)
        console.log(curPath)
        setDirent(
            d
                .sort()
                .filter((name) => name !== ".")
                .map((name) => {
                    // Provide the default value because stat fails on 32bit mode.
                    // Reference to: http://www.mjr19.org.uk/sw/inodes64.html
                    const attr = stat(path.join(curPath, name))[0] ?? {
                        dev: 0,
                        ino: 0,
                        mode: 0,
                        nlink: 0,
                        uid: 0,
                        gid: 0,
                        rdev: 0,
                        size: 0,
                        block: 0,
                        atime: 0,
                        mtime: 0,
                        ctime: 0,
                    }
                    return {
                        name,
                        attribute: attr,
                        isDir: (attr.mode & S_IFMT) === S_IFDIR,
                    }
                })
                .sort((a, b) => {
                    const va = a.isDir
                    const vb = b.isDir
                    return (vb ? 1 : 0) - (va ? 1 : 0)
                })
        )
    }, [curPath])

    return (
        <Stack
            style={{
                width: "100%",
                height: "100%",
            }}
        >
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
                    onChangeViewMode={setViewMode}
                    onClickSetting={() => {
                        setShowSetting(true)
                    }}
                />
                <Row
                    style={{
                        width: "100%",
                        flexGrow: 1,
                    }}
                >
                    <Side></Side>
                    {viewMode == "icon" ? (
                        <IconView
                            curPath={curPath}
                            dirent={dirent}
                            setCurPath={setCurPath}
                        ></IconView>
                    ) : (
                        <ItemView
                            curPath={curPath}
                            dirent={dirent}
                            setCurPath={setCurPath}
                        ></ItemView>
                    )}
                </Row>
            </Root>
            {showSetting ? (
                <Setting
                    onClose={() => {
                        setShowSetting(false)
                    }}
                ></Setting>
            ) : null}
        </Stack>
    )
}

createRoot(1920, 1080).render(<FileExplorer />)
