/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import React, {
    useImperativeHandle,
    useLayoutEffect,
    useRef,
    useState,
} from "react"
import { Div, Stack, Canvas, MeuiElementAttribule } from "@/meui"
import { CanvasElement, CanvasRenderingContext2D, TextMetrics } from "@/meui"

import { MeuiKeyboardEvent, MeuiMouseEvent } from "@/meui"

interface TextRenderInfo {
    content: string
    m: TextMetrics
    result: {
        x: number
        y: number
        width: number
        height: number
    }
}

interface Paragraph {
    children: TextRenderInfo[]
    result: {
        lineHeight: number
        ascent: number
        y: number
    }
}

type Cursor = [number, number]
const duration = 1000

interface CanvasTextEditorOption {
    type: "text" | "password"
    defaultValue: string
    multiline: boolean
    fontSize: number
    fontFamily: string
    fontColor: string
}
class CanvasTextEditor {
    ctx: CanvasRenderingContext2D
    cursor: Cursor = [Infinity, 0]
    startBlinkinTimestamp = 0
    paragraphs: Paragraph[] = []
    cursorStart: Cursor = [Infinity, 0]
    cursorPrev: Cursor = [Infinity, 0]

    mouseDown = false
    clipboard = ""
    updated = false
    updateSelected = false
    focused = false

    rafId = -1
    constructor(
        private canvas: CanvasElement,
        private option: CanvasTextEditorOption
    ) {
        this.ctx = canvas.getContext("2d") as CanvasRenderingContext2D
        this.startBlinkinTimestamp = Date.now()
        this.initData(option.defaultValue)

        this.render = this.render.bind(this)
        this.onKeyDown = this.onKeyDown.bind(this)
        this.onMouseDown = this.onMouseDown.bind(this)
        this.onMouseUp = this.onMouseUp.bind(this)
        this.onMouseMove = this.onMouseMove.bind(this)
        this.onMouseOut = this.onMouseOut.bind(this)

        this.rafId = requestAnimationFrame(this.render)
        this.canvas.addEventListener("mousedown", this.onMouseDown)
        this.canvas.addEventListener("mouseup", this.onMouseUp)
        this.canvas.addEventListener("mousemove", this.onMouseMove)
        this.canvas.addEventListener("mouseout", this.onMouseOut)
    }

    updateOption(option: CanvasTextEditorOption) {
        this.option = option
        this.updated = false
    }

    setFocused(focused: boolean) {
        this.focused = focused
    }

    destory() {
        this.canvas.removeEventListener("keydown", this.onKeyDown)
        this.canvas.removeEventListener("mousedown", this.onMouseDown)
        this.canvas.removeEventListener("mouseup", this.onMouseUp)
        this.canvas.removeEventListener("mousemove", this.onMouseMove)
        this.canvas.removeEventListener("mouseout", this.onMouseOut)

        cancelAnimationFrame(this.rafId)
    }

    get value() {
        return this.paragraphs
            .map((paragraph) =>
                paragraph.children.map((char) => char.content).join("")
            )
            .join("")
            .slice(0, -1)
    }

    adjustCursor() {
        this.cursorStart[1] = Math.max(
            0,
            Math.min(this.cursorStart[1], this.paragraphs.length - 1)
        )
        this.cursorStart[0] = Math.max(
            0,
            Math.min(
                this.cursorStart[0],
                this.paragraphs[this.cursorStart[1]].children.length - 1
            )
        )

        this.cursor[1] = Math.max(
            0,
            Math.min(this.cursor[1], this.paragraphs.length - 1)
        )
        this.cursor[0] = Math.max(
            0,
            Math.min(
                this.cursor[0],
                this.paragraphs[this.cursor[1]].children.length - 1
            )
        )
    }

    initData(content: string) {
        this.paragraphs = [
            {
                children: [
                    {
                        ...this.getTextInfo("\n"),
                        result: {
                            x: 0,
                            y: 0,
                            width: 0,
                            height: 0,
                        },
                    },
                ],
                result: {
                    lineHeight: 0,
                    ascent: 0,
                    y: 0,
                },
            },
        ]

        this.insetText(
            0,
            0,
            this.option.multiline ? content : content.split("\n").join("")
        )
        this.calcLineInfo()
    }

    onMouseOut(e: MeuiMouseEvent) {
        this.mouseDown = false
    }
    onMouseUp(e: MeuiMouseEvent) {
        this.mouseDown = false
    }
    onMouseMove(e: MeuiMouseEvent) {
        if (this.mouseDown) this.calcCursor(e.clientX, e.clientY)
    }
    onMouseDown(e: MeuiMouseEvent) {
        this.mouseDown = true
        this.calcCursor(e.clientX, e.clientY)

        this.cursorStart = [...this.cursor]

        this.updated = false
    }

    calcMouseX(cursorX: number, cursorY: number) {
        return this.paragraphs[cursorY].children[cursorX].result.x
    }
    calcCursorX(mouseX: number, cursorY: number) {
        const hitLine = this.paragraphs[cursorY]
        let cursorX = hitLine.children.findIndex((info, index, array) => {
            if (index == array.length - 1) return true
            const prevX =
                index == 0
                    ? -Infinity
                    : array[index - 1].result.x +
                      array[index - 1].result.width / 2
            return (
                mouseX > prevX &&
                mouseX <= info.result.x + info.result.width / 2
            )
        })
        if (cursorX === -1) cursorX = hitLine.children.length - 1

        return cursorX
    }

    calcCursor(mouseX: number, mouseY: number) {
        this.cursorPrev = [...this.cursor]
        const hitLineIndex = this.paragraphs.findIndex((lineInfo) => {
            return (
                mouseY > lineInfo.result.y &&
                mouseY <= lineInfo.result.y + lineInfo.result.lineHeight
            )
        })
        if (hitLineIndex !== -1) {
            const hitLine = this.paragraphs[hitLineIndex]
            let hitCharIndex = hitLine.children.findIndex(
                (info, index, array) => {
                    if (index == array.length - 1) return true
                    const prevX =
                        index == 0
                            ? -Infinity
                            : array[index - 1].result.x +
                              array[index - 1].result.width / 2
                    return (
                        mouseX > prevX &&
                        mouseX <= info.result.x + info.result.width / 2
                    )
                }
            )

            if (hitCharIndex === -1) hitCharIndex = hitLine.children.length - 1

            this.cursor[0] = hitCharIndex
            this.cursor[1] = hitLineIndex
        } else {
            this.cursor[1] = this.paragraphs.length - 1
            this.cursor[0] = this.paragraphs[this.cursor[1]].children.length - 1
        }
        this.adjustCursor()

        if (
            this.cursorPrev[0] != this.cursor[0] ||
            this.cursorPrev[1] != this.cursor[1]
        ) {
            this.updated = false
        }
    }

    getTextInfo(c: string) {
        this.ctx.font = `${this.option.fontSize}px ${this.option.fontFamily}`
        if (c === "\n") {
            return {
                content: c,
                m: this.ctx.measureText(" "),
            }
        } else if (c === "\t") {
            return {
                content: c,
                m: this.ctx.measureText("A"),
            }
        }

        return {
            content: c,
            m: this.ctx.measureText(this.option.type === "password" ? "*" : c),
        }
    }

    insertChar(x: number, y: number, c: string): [number, number] {
        const info: TextRenderInfo = {
            ...this.getTextInfo(c),
            result: {
                x: 0,
                y: 0,
                width: 0,
                height: 0,
            },
        }
        this.paragraphs[y].children.splice(x, 0, info)
        return [x + 1, y]
    }

    removeChar(y: number, index: number) {
        this.paragraphs[y].children.splice(index, 1)
    }

    removeLastChar(y: number) {
        const children = this.paragraphs[y].children
        children.splice(children.length - 1, 1)
    }

    insertParagraph(y: number, content: TextRenderInfo[]) {
        this.paragraphs.splice(y, 0, {
            children: content,
            result: {
                lineHeight: 0,
                ascent: 0,
                y: 0,
            },
        })
    }

    removeParagraph(y: number) {
        this.paragraphs.splice(y, 1)
    }

    splitParagraph(x: number, y: number) {
        const line = this.paragraphs[y].children
        this.insertParagraph(y + 1, line.splice(x, line.length - x))
    }

    mergeParagraph(y1: number, y2: number) {
        this.paragraphs[y1].children = this.paragraphs[y1].children.concat(
            this.paragraphs[y2].children
        )
        this.paragraphs.splice(y2, 1)
    }

    getParagraphLength(y: number) {
        return this.paragraphs[y].children.length
    }

    insertNewLine(x: number, y: number): [number, number] {
        if (this.option.multiline) {
            this.insertChar(x, y, "\n")
            this.splitParagraph(x + 1, y)
            return [0, y + 1]
        }
        return [x, y]
    }

    insertChars(x: number, y: number, s: string): [number, number] {
        this.paragraphs[y].children.splice(
            x,
            0,
            ...s.split("").map((c) => ({
                ...this.getTextInfo(c),
                result: {
                    x: 0,
                    y: 0,
                    width: 0,
                    height: 0,
                },
            }))
        )
        return [x + s.length, y]
    }

    insetText(x: number, y: number, s: string) {
        let newPos: [number, number] = [x, y]
        s.split("\n").forEach((s, index, array) => {
            newPos = this.insertChars(newPos[0], newPos[1], s)
            if (index !== array.length - 1) {
                newPos = this.insertNewLine(newPos[0], newPos[1])
            }
        })

        return newPos
    }

    removeSelected() {
        const cursor = this.handleSelectedChar((p, start, end) => {
            p.children.splice(start, end - start)
        })

        if (cursor[0][1] !== cursor[1][1]) {
            this.mergeParagraph(cursor[0][1], cursor[1][1])
        }

        this.paragraphs = this.paragraphs.filter(
            (paragraph) => paragraph.children.length > 0
        )

        this.cursor = [...cursor[0]]
        this.cursorStart = [...this.cursor]
        return cursor
    }

    onKeyDown(e: MeuiKeyboardEvent) {
        this.cursorPrev = [...this.cursor]

        if (e.ctrlKey) {
            if (e.key.toUpperCase() === "C") {
                let text = ""
                this.handleSelectedChar((p, start, end) => {
                    text += p.children
                        .slice(start, end)
                        .map((item) => item.content)
                        .join("")
                })
                this.clipboard = text
            } else if (e.key.toUpperCase() === "X") {
                let text = ""
                this.handleSelectedChar((p, start, end) => {
                    text += p.children
                        .slice(start, end)
                        .map((item) => item.content)
                        .join("")
                })
                this.clipboard = text
                this.removeSelected()
            } else if (e.key.toUpperCase() === "V") {
                this.removeSelected()
                const cursor = this.insetText(
                    this.cursor[0],
                    this.cursor[1],
                    this.clipboard
                )
                this.cursor[0] = cursor[0]
                this.cursor[1] = cursor[1]
            }
        } else if (/^[ -~]$/.test(e.key)) {
            // https://stackoverflow.com/questions/12052825/regular-expression-for-all-printable-characters-in-javascript
            this.removeSelected()
            this.insertChar(this.cursor[0], this.cursor[1], e.key)
            this.cursor[0]++
        } else if (e.key == "\n") {
            this.cursor = this.insertNewLine(this.cursor[0], this.cursor[1])
        } else if (e.key === "Backspace") {
            if (this.hasSelected()) {
                this.removeSelected()
            } else if (this.cursor[0] === 0) {
                if (this.cursor[1] > 0) {
                    this.cursor[0] =
                        this.getParagraphLength(this.cursor[1] - 1) - 1
                    this.removeLastChar(this.cursor[1] - 1)
                    this.mergeParagraph(this.cursor[1] - 1, this.cursor[1])
                    this.cursor[1]--
                }
            } else {
                this.paragraphs[this.cursor[1]].children.splice(
                    this.cursor[0] - 1,
                    1
                )
                this.cursor[0]--
            }
        } else if (e.key === "ArrowLeft") {
            if (this.cursor[0] === 0) {
                if (this.cursor[1] > 0) {
                    this.cursor[1]--
                    this.cursor[0] = Infinity
                }
            } else {
                this.cursor[0]--
            }
        } else if (e.key === "ArrowRight") {
            if (this.cursor[0] < this.getParagraphLength(this.cursor[1]) - 1) {
                this.cursor[0]++
            } else {
                if (this.cursor[1] < this.paragraphs.length - 1) {
                    this.cursor[0] = 0
                    this.cursor[1]++
                }
            }
        } else if (e.key === "ArrowUp") {
            if (this.cursor[1] > 0) {
                this.cursor[0] = this.calcCursorX(
                    this.calcMouseX(this.cursor[0], this.cursor[1]),
                    this.cursor[1] - 1
                )

                this.cursor[1]--
            }
        } else if (e.key === "ArrowDown") {
            if (this.cursor[1] < this.paragraphs.length - 1) {
                this.cursor[0] = this.calcCursorX(
                    this.calcMouseX(this.cursor[0], this.cursor[1]),
                    this.cursor[1] + 1
                )

                this.cursor[1]++
            }
        } else if (e.key === "Home") {
            this.cursor[0] = 0
        } else if (e.key === "End") {
            this.cursor[0] = this.getParagraphLength(this.cursor[1]) - 1
        }

        const cursorLine = this.paragraphs[this.cursor[1]].children
        if (
            cursorLine.length === 0 ||
            cursorLine[cursorLine.length - 1].content !== "\n"
        ) {
            throw new Error("Unexcepted status")
        }
        this.adjustCursor()

        if (
            this.cursorPrev[0] !== this.cursor[0] ||
            this.cursorPrev[1] !== this.cursor[1]
        ) {
            this.cursorStart = [...this.cursor]
        }

        this.calcLineInfo()
    }
    hasSelected() {
        return (
            this.cursorStart[0] !== this.cursor[0] ||
            this.cursorStart[1] !== this.cursor[1]
        )
    }

    handleSelectedChar(
        cb: (p: Paragraph, start: number, end: number) => void
    ): [Cursor, Cursor] {
        const wrap =
            this.cursorStart[1] != this.cursor[1]
                ? this.cursorStart[1] > this.cursor[1]
                : this.cursorStart[0] != this.cursor[0]
                ? this.cursorStart[0] > this.cursor[0]
                : false
        const cursor: [Cursor, Cursor] = wrap
            ? [
                  [this.cursor[0], this.cursor[1]],
                  [this.cursorStart[0], this.cursorStart[1]],
              ]
            : [
                  [this.cursorStart[0], this.cursorStart[1]],
                  [this.cursor[0], this.cursor[1]],
              ]
        this.paragraphs
            .filter(
                (lineInfo, index) =>
                    index >= cursor[0][1] && index <= cursor[1][1]
            )
            .forEach((lineInfo, index, array) => {
                const line = lineInfo.children
                cb(
                    lineInfo,
                    index === 0 ? cursor[0][0] : 0,
                    index === array.length - 1 ? cursor[1][0] : line.length
                )
            })
        return cursor
    }
    calcLineInfo() {
        let yOffset = 0
        this.updated = false
        this.paragraphs.forEach((paragraph, i) => {
            let advance = 0
            let ascent = 0,
                lineHeight = 0
            paragraph.children.forEach((info, i) => {
                let width = info.m.width
                if (info.content === "\t") {
                    const tabWidth = info.m.width * 4
                    width =
                        Math.floor(advance / tabWidth + 1) * tabWidth - advance
                }
                info.result = {
                    x: advance,
                    y: info.m.fontBoundingBoxAscent,
                    width,
                    height:
                        info.m.fontBoundingBoxAscent +
                        info.m.fontBoundingBoxDescent,
                }
                advance += width

                ascent = Math.max(ascent, info.result.y)
                lineHeight = Math.max(lineHeight, info.result.height)
            })

            paragraph.result = {
                ascent,
                lineHeight,
                y: yOffset,
            }

            yOffset += lineHeight
        }, [])
    }
    renderChar(paragraph: Paragraph, char: TextRenderInfo, selected = false) {
        let fillRectStyle = "#00000000"
        let fillTextStyle = this.option.fontColor

        if (selected) {
            fillRectStyle = "#3390ff"
            fillTextStyle = "white"
        }

        this.ctx.fillStyle = fillRectStyle
        this.ctx.font = `${this.option.fontSize}px ${this.option.fontFamily}`
        this.ctx.fillRect(
            char.result.x,
            paragraph.result.y,
            char.result.width + 0.5,
            paragraph.result.lineHeight
        )

        if (char.content !== "\t" && char.content !== "\n") {
            this.ctx.fillStyle = fillTextStyle
            this.ctx.fillText(
                this.option.type === "password" ? "*" : char.content,
                char.result.x,
                paragraph.result.y + paragraph.result.ascent
            )
        }
    }

    renderParagraphs() {
        this.ctx.save()
        this.ctx.strokeStyle = "red"
        this.ctx.fillStyle = "black"

        this.paragraphs.forEach((paragraph) => {
            paragraph.children.forEach((char) => {
                const x = char.result.x
                const y = paragraph.result.y
                if (x > this.canvas.width || y > this.canvas.height) return

                // this.ctx.strokeRect(
                //     char.result.x,
                //     paragraph.result.y +
                //         paragraph.result.ascent -
                //         char.result.y,
                //     char.result.width,
                //     char.result.height
                // )
                this.renderChar(paragraph, char)
            })
        }, 0)
        this.ctx.restore()
    }

    renderCursor() {
        const paragraph = this.paragraphs[this.cursor[1]]
        const char = paragraph.children[this.cursor[0]]

        this.ctx.clearRect(
            char.result.x,
            paragraph.result.y,
            2,
            paragraph.result.lineHeight
        )

        if (this.focused === false) return

        this.ctx.save()

        const phase = (Date.now() - this.startBlinkinTimestamp) % duration
        const showCursor = phase / duration < 0.5

        this.renderChar(
            paragraph,
            char,
            this.cursorStart[0] > this.cursor[0] ||
                this.cursorStart[1] > this.cursor[1]
        )

        if (showCursor) {
            this.ctx.fillStyle = "black"

            this.ctx.fillRect(
                char.result.x,
                paragraph.result.y,
                2,
                paragraph.result.lineHeight
            )
        }

        this.ctx.restore()
    }

    renderSelected() {
        this.handleSelectedChar((paragraph, start, end) => {
            this.ctx.save()
            for (let i = start; i < end; i++) {
                const char = paragraph.children[i]
                this.renderChar(paragraph, char, true)
            }
            this.ctx.restore()
        })
    }

    render() {
        this.rafId = requestAnimationFrame(this.render)
        this.ctx.save()

        this.adjustCursor()

        if (this.updated === false) {
            // this.ctx.fillStyle = "red"
            // this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height)

            this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height)
            this.renderParagraphs()
            this.renderSelected()
        }

        this.renderCursor()

        this.updated = true
        this.ctx.restore()
    }
}
export interface TextAreaHandle {
    value: string
}

export interface Props extends Omit<MeuiElementAttribule, "children"> {
    type?: "text" | "password"
    multiline?: boolean
    placeholder?: string
    children?: string
    onChange?: (text: string) => any
}

export default React.forwardRef<TextAreaHandle, Props>(function TextArea(
    {
        style,
        children = "",
        placeholder = "",
        multiline = true,
        type = "text",
        onChange,
        ...props
    }: Props,
    ref
) {
    const canvasRef = useRef<CanvasElement>(null)
    const editorRef = useRef<CanvasTextEditor>()
    const fontSize = style?.fontSize ?? 16
    const fontFamily = style?.fontFamily ?? "sans-serif"
    const fontColor = style?.fontColor ?? "black"
    const [placeholderText, setPlaceholderText] = useState(
        children === "" ? placeholder : ""
    )
    useLayoutEffect(() => {
        const canvas = canvasRef.current
        if (!canvas) return

        const editor = new CanvasTextEditor(canvas, {
            defaultValue: children,
            type,
            multiline,
            fontSize,
            fontFamily,
            fontColor,
        })
        editorRef.current = editor
        return () => {
            editor.destory()
            editorRef.current = undefined
        }
        // eslint-disable-next-line react-hooks/exhaustive-deps
    }, [])

    useLayoutEffect(() => {
        editorRef.current?.updateOption({
            defaultValue: children,
            type,
            multiline,
            fontSize,
            fontFamily,
            fontColor,
        })
    }, [children, fontColor, fontFamily, fontSize, multiline, type])

    useImperativeHandle(ref, () => ({
        get value() {
            return editorRef.current?.value ?? ""
        },
    }))
    return (
        <Stack
            focusable={true}
            style={{
                width: 100,
                height: 100,
                ...style,
            }}
            {...props}
            onKeyDown={(e) => {
                editorRef.current?.onKeyDown(e)
                const value = editorRef.current?.value
                const isEmpty = (value ?? "") === ""
                setPlaceholderText(isEmpty ? placeholder : "")
                onChange?.(value ?? "")
            }}
            onFocusIn={() => {
                editorRef.current?.setFocused(true)
            }}
            onFocusOut={() => {
                editorRef.current?.setFocused(false)
            }}
        >
            <Div
                style={{
                    fontColor: "grey",
                    fontFamily,
                    fontSize,
                }}
            >
                {placeholderText}
            </Div>
            <Canvas
                fit={true}
                ref={canvasRef}
                style={{
                    width: "100%",
                    height: "100%",
                }}
            ></Canvas>
        </Stack>
    )
})
