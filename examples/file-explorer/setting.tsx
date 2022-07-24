/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { Column, Root, Row } from "@/components"
import { Stack } from "@/meui"
import { MeuiStyle } from "@/meui"
import React, { useState } from "react"
import { Icon } from "./icon"

function Title({ children }: { children: string }) {
    return (
        <Row
            style={{
                fontSize: 22,
                textAlign: "center-left",
                width: 180,
                height: 65,
            }}
        >
            {children}
        </Row>
    )
}

function SubTitle({ children }: { children: string }) {
    return (
        <Row
            style={{
                fontSize: 20,
                textAlign: "center-left",
                fontColor: "#4D4D4D",
                width: 180,
                height: 60,
            }}
        >
            {children}
        </Row>
    )
}

function MenuItem({
    children,
    isSelect,
    onClick,
}: {
    children: React.ReactNode
    isSelect: boolean
    onClick: () => void
}) {
    return (
        <Row
            style={{
                padding: [0, 10, 0, 10],
                HOVER: {
                    backgroundColor: "#0080FD",
                },
            }}
        >
            <Row
                style={{
                    width: 35,
                }}
            >
                {isSelect ? <Icon icon="check"></Icon> : null}
            </Row>
            <Row
                style={{
                    width: "100%",
                    height: 37,
                    textAlign: "center-left",
                    fontSize: 18,
                }}
                onClick={onClick}
            >
                {children}
            </Row>
        </Row>
    )
}

function DropDown({ option }: { option: string[] }) {
    const [open, setOpen] = useState<boolean>(false)
    const [selectIndex, setSelectIndex] = useState<number>(0)
    const menu = (
        <Column
            style={{
                zIndex: 1,
                width: "100%",
                backgroundColor: "#F7F7F8",
                borderRadius: 10,
                border: [1, 1, 1, 1],
                borderColor: "#C1C1C1",
                paddingTop: 10,
                paddingBottom: 10,
            }}
        >
            {option.map((item, index) => {
                return (
                    <MenuItem
                        key={item}
                        isSelect={selectIndex === index}
                        onClick={() => {
                            setSelectIndex(index)
                            setOpen(false)
                        }}
                    >
                        {item}
                    </MenuItem>
                )
            })}
        </Column>
    )
    return (
        <Stack style={{ height: 40, width: 250, flexGrow: 0, flexShrink: 0 }}>
            <Row
                style={{
                    fontSize: 18,
                    borderRadius: 10,
                    border: [1, 1, 1, 1],
                    borderColor: "#DADADA",
                    backgroundColor: "#E6E6E6",
                    textAlign: "center-left",
                    fontColor: "#4D4D4D",
                    padding: [0, 10, 0, 10],
                    height: 40,
                    width: "100%",
                    HOVER: {
                        backgroundColor: "#E0E0E0",
                    },
                }}
                onClick={() => {
                    setOpen((prev) => !prev)
                }}
            >
                {option[selectIndex]}
            </Row>
            {open ? menu : null}
        </Stack>
    )
}

function Option({
    title,
    option,
    style,
}: {
    title: string
    option: string[]
    style?: MeuiStyle
}) {
    return (
        <Row
            style={{
                borderRadius: 10,
                backgroundColor: "#F8F8F8",
                height: 55,
                width: "100%",
                padding: [0, 10, 0, 10],
                justifyContent: "flex-start",
                ...style,
            }}
        >
            <Row
                style={{
                    fontSize: 18,
                    textAlign: "center-left",
                    fontColor: "#4D4D4D",
                    width: 200,
                    flexGrow: 1,
                    height: 40,
                }}
            >
                {title}
            </Row>
            <DropDown option={option}></DropDown>
        </Row>
    )
}

function Checkbox({
    title,
    style,
    initState = false,
}: {
    title: string
    initState?: boolean
    style?: MeuiStyle
}) {
    const [checked, setChecked] = useState<boolean>(initState)
    return (
        <Row
            style={{
                borderRadius: 10,
                backgroundColor: "#F8F8F8",
                height: 55,
                width: "100%",
                padding: [0, 10, 0, 10],
                justifyContent: "flex-start",
                ...style,
            }}
        >
            <Icon
                icon={checked ? "checkbox-marked" : "checkbox-blank-outline"}
                style={{
                    padding: undefined,
                    width: 20,
                    height: 20,
                    marginLeft: 0,
                    fontSize: 20,
                    fontColor: checked ? "#0080FD" : "black",
                }}
                onClick={() => {
                    setChecked((prev) => !prev)
                }}
            ></Icon>
            <Row
                style={{
                    fontSize: 18,
                    textAlign: "center-left",
                    fontColor: "#4D4D4D",
                    width: 200,
                    flexGrow: 1,
                    height: 40,
                }}
            >
                {title}
            </Row>
        </Row>
    )
}

function MenuTitle({ children }: { children: string }) {
    return (
        <Row
            style={{
                fontSize: 22,
                textAlign: "center-left",
                width: 180,
                borderRadius: 10,
                height: 55,
                padding: [10, 10, 10, 10],
                HOVER: {
                    fontColor: "white",
                    backgroundColor: "#0081FF",
                },
            }}
        >
            {children}
        </Row>
    )
}

function MenuSubTitle({ children }: { children: string }) {
    return (
        <Row
            style={{
                fontSize: 18,
                textAlign: "center-left",
                fontColor: "#4D4D4D",
                width: 180,
                borderRadius: 10,
                height: 40,
                padding: [10, 10, 10, 40],
                HOVER: {
                    fontColor: "white",
                    backgroundColor: "#0081FF",
                },
            }}
        >
            {children}
        </Row>
    )
}

export function Setting({ onClose }: { onClose: () => void }) {
    return (
        <Root
            style={{
                flexDirection: "column",
                borderRadius: 20,
                backgroundColor: "transparent",
                justifyContent: "center",
                alignItems: "center",
            }}
        >
            <Column
                style={{
                    backgroundColor: "white",
                    borderRadius: 20,
                    border: [1, 1, 1, 1],
                    borderColor: "#9B9B9B",
                    width: "70%",
                    height: "80%",
                }}
            >
                <Row style={{ width: "100%", justifyContent: "flex-end" }}>
                    <Icon icon={"\u{F05AD}"} onClick={onClose}></Icon>
                </Row>
                <Row
                    style={{
                        flexGrow: 1,
                        height: "100%",
                        justifyContent: "flex-start",
                    }}
                >
                    <Column
                        style={{
                            width: 200,
                            flexShrink: 0,
                            flexGrow: 0,
                            height: "100%",

                            padding: [0, 10, 0, 10],
                            justifyContent: "flex-start",
                        }}
                    >
                        <MenuTitle>基础设置</MenuTitle>
                        <MenuSubTitle>打开行为</MenuSubTitle>
                        <MenuSubTitle>新窗口和新标签</MenuSubTitle>
                        <MenuSubTitle>视图</MenuSubTitle>
                        <MenuSubTitle>隐藏文件</MenuSubTitle>
                        <MenuTitle>高级设置</MenuTitle>
                        <MenuSubTitle>索引</MenuSubTitle>
                        <MenuSubTitle>预览</MenuSubTitle>
                        <MenuSubTitle>挂载</MenuSubTitle>
                        <MenuSubTitle>对话框</MenuSubTitle>
                        <MenuSubTitle>其他</MenuSubTitle>
                    </Column>
                    <Column
                        style={{
                            backgroundColor: "#F8F8F8",
                            flexGrow: 1,
                            width: "100%",
                            height: "100%",
                            padding: [10, 10, 10, 10],
                        }}
                    >
                        <Column
                            style={{
                                backgroundColor: "white",
                                flexGrow: 1,
                                width: "100%",
                                height: "100%",
                                padding: [20, 20, 20, 20],
                                borderRadius: 10,
                                alignItems: "flex-start",
                                justifyContent: "flex-start",
                            }}
                        >
                            <Title>基础设置</Title>
                            <SubTitle>打开行为</SubTitle>
                            <Checkbox
                                title="总是在新窗口中打开文件夹"
                                style={{
                                    borderRadius: [10, 10, 0, 0],
                                }}
                            ></Checkbox>
                            <Option
                                title="打开文件："
                                option={["双击", "单机"]}
                                style={{
                                    borderRadius: [0, 0, 10, 10],
                                }}
                            ></Option>
                            <SubTitle>新窗口和新标签</SubTitle>
                            <Option
                                title="从默认窗口打开："
                                option={[
                                    "计算机",
                                    "主目录",
                                    "桌面",
                                    "视频",
                                    "音乐",
                                    "图片",
                                    "文档",
                                    "下载",
                                ]}
                                style={{
                                    borderRadius: [10, 10, 0, 0],
                                }}
                            ></Option>
                            <Option
                                title="从新标签打开："
                                option={[
                                    "计算机",
                                    "主目录",
                                    "桌面",
                                    "视频",
                                    "音乐",
                                    "图片",
                                    "文档",
                                    "下载",
                                ]}
                                style={{
                                    borderRadius: [0, 0, 10, 10],
                                }}
                            ></Option>
                            <SubTitle>视图</SubTitle>
                            <Option
                                title="默认大小："
                                option={["特小", "小", "中", "大", "特大"]}
                                style={{
                                    borderRadius: [10, 10, 0, 0],
                                }}
                            ></Option>
                            <Option
                                title="默认视图："
                                option={["图标视图", "列表视图"]}
                                style={{
                                    borderRadius: [0, 0, 10, 10],
                                }}
                            ></Option>
                        </Column>
                    </Column>
                </Row>
            </Column>
        </Root>
    )
}
