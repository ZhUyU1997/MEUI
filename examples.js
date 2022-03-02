import { NativeMEUI, BOX_STATE, createBox } from "MEUI"
import * as os from "os"

class Box {
    constructor() {
        this.nativeBox = createBox()
        this.children = []
    }

    getStyle(state) {
        return this.nativeBox.getStyle(state)
    }
    setStyle(style, state) {
        this.nativeBox.setStyle(style, state)
    }
    addChild(child) {
        this.nativeBox.addChild(child.nativeBox);
        this.children.push(child)
    }
    setState(state) {
        this.nativeBox.setState(state)
    }
    hit(x, y) {
        return this.nativeBox.hit(x, y)
    }
    getNativeObject() {
        return this.nativeBox
    }
}


class MEUI {
    constructor(width, height) {
        this.nativeMEUI = new NativeMEUI(width, height)
    }

    registerCallback() {

    }
    render(root) {
        this.root = root
        this.nativeMEUI.render(root.getNativeObject())
    }
    flush() {
        this.nativeMEUI.flush()
    }
    update() {
        this.nativeMEUI.update()

    }
    addFontFace(fileName) {
        this.nativeMEUI.addFontFace(fileName)
    }
    getConnectNumber() {
        return this.nativeMEUI.getConnectNumber()
    }
    pending() {
        return this.nativeMEUI.pending()

    }
    nextEvent() {
        return this.nativeMEUI.nextEvent()

    }
    _searchNode(node, x, y, callback) {
        let target = null
        if (node.hit(x, y)) {
            target = node
        }

        if (callback)
            callback(target === node, node);

        for (const item of node.children) {
            this._searchNode(item, x, y, callback)
        }
        return target
    }

    searchNode(x, y, callback) {
        return this._searchNode(this.root, x, y, callback)
    }
}

function HEX(color) {
    const c = [(color >> 24) & 0xff, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff]

    return [c[0] / 255.0, c[1] / 255.0, c[2] / 255.0, c[3] / 255.0]
}

function matrix_multiply(_a, _b) {
    let a, b;
    {
        const [m00, m10, m01, m11, m02, m12] = _a
        a = { m00, m10, m01, m11, m02, m12 }
    }

    {
        const [m00, m10, m01, m11, m02, m12] = _b
        b = { m00, m10, m01, m11, m02, m12 }
    }

    const m00 = a.m00 * b.m00 + a.m10 * b.m01;
    const m10 = a.m00 * b.m10 + a.m10 * b.m11;
    const m01 = a.m01 * b.m00 + a.m11 * b.m01;
    const m11 = a.m01 * b.m10 + a.m11 * b.m11;
    const m02 = a.m02 * b.m00 + a.m12 * b.m01 + b.m02;
    const m12 = a.m02 * b.m10 + a.m12 * b.m11 + b.m12;

    return [m00, m10, m01, m11, m02, m12]
}

const Transform = {
    identity() {
        return [1.0, 0.0, 0.0, 1.0, 0.0, 0.0]
    },
    translate(m, x, y) {
        return matrix_multiply(m, [1.0, 0.0, 0.0, 1.0, x, y])
    },
    rotate(m, radians) {
        const c = Math.cos(radians);
        const s = Math.sin(radians);
        return matrix_multiply(m, [c, s, -s, c, 0.0, 0.0])
    },
    scale(m, x, y) {
        return matrix_multiply(m, [0.0, 0.0, y, 0.0, 0.0])
    },
    skew(m, x, y) {
        return matrix_multiply(m, [1.0, Math.tan(y), Math.tan(x), 1.0, 0.0, 0.0])
    }
}


function Fragment({ children, ...props }) {
    return children
}

function createElement(tag, attrs, ...children) {
    attrs = attrs || {};
    const stack = [...children];

    if (typeof tag === "function") {
        attrs.children = stack;
        const elm = tag(attrs)

        if (typeof elm === 'undefined')
            throw new Error("Function component needs return value")
        return elm;
    }
    else if (tag !== "div") {
        throw new Error(`Not support other tag (${tag})`)
    }

    const elm = new Box();
    for (let [name, val] of Object.entries(attrs)) {
        // if (name.startsWith("on") && name.toLowerCase() in window) {
        //     elm.addEventListener(name.toLowerCase().substr(2), val);
        // } else 
        if (name === "ref") {
            val(elm);
        } else if (name === "style") {
            let style = {}
            for (let [k, v] of Object.entries(val)) {
                if (k in BOX_STATE) {
                    let style = {};
                    for (let [key, val] of Object.entries(v)) {
                        style[key] = val
                    }
                    elm.setStyle(style, BOX_STATE[k])
                }
                else {
                    style[k] = v
                }
            }
            elm.setStyle(style, BOX_STATE.DEFAULT)
        }
    }

    // Append children
    while (stack.length) {
        const child = stack.shift();

        // Is child a leaf?
        if (!Array.isArray(child)) {
            if (typeof child === "string") {
                let style = elm.setStyle({ text: child }, BOX_STATE.DEFAULT);
                // style.text = style.text + child;
            }
            else {
                elm.addChild(child)
            }

        } else {
            stack.push(...child);
        }
    }

    return elm;
}


function render(root) {
    const meui = new MEUI(1920, 1080)
    meui.addFontFace("res/font/Droid-Sans-Fallback.ttf")
    meui.render(root)

    os.setReadHandler(meui.getConnectNumber(), () => {
        while (meui.pending() > 0) {
            const event = meui.nextEvent();
            if (!event) continue
            const box = meui.searchNode(event.x, event.y, (hit, box) => {
                box.setState(hit ? BOX_STATE.HOVER : BOX_STATE.DEFAULT)
                if (event.type === "mousedown" && hit) {
                    box.setState(BOX_STATE.ACTIVE)
                }
            })

            if (box && event.type === "mousedown") {
            }
        }
        meui.update()
    })
}

function Flex_Test() {
    function TestItem() {
        return <div style={{
            backgroundColor: [Math.random(), Math.random(), Math.random(), 1.0],
            border: [Math.random() * 20, Math.random() * 20, Math.random() * 20, Math.random() * 20],
            padding: [Math.random() * 20, Math.random() * 20, Math.random() * 20, Math.random() * 20],
            width: 340,
            height: 240,
            borderRadius: [20, 20, 20, 20],
            borderColor: [Math.random(), Math.random(), Math.random(), 1.0],
            backgroundColor: [Math.random(), Math.random(), Math.random(), 1.0],
            fontColor: [Math.random(), Math.random(), Math.random(), 1.0],
            text: "MEUI\nModern embedded UI\n现代化嵌入式用户界面",
            textAlign: "center",
            contentImage: "res/img/picture.png",
            backgroundImage: "res/img/Gradient2.svg",
            transform: Transform.rotate(Transform.identity(), 0.25 * 3.1415926),
            transformOrigin: ["center", "center"],
        }}></div>
    }
    return <div style={{
        width: "100%",
        height: "100%",
    }}>
        <div style={{
            flexWrap: "wrap",
            justifyContent: "center",
            alignItems: "center",
            alignContent: "center",
            width: "100%",
            height: "100%",
        }}>
            <TestItem /><TestItem /><TestItem /><TestItem /><TestItem /><TestItem /><TestItem /><TestItem />
            <div style={{
                width: 340,
                height: 240,
                backgroundColor: [Math.random(), Math.random(), Math.random(), 1.0],
                transform: Transform.rotate(Transform.identity(), 0.25 * 3.1415926),
            }}>
                <div style={{

                    backgroundColor: [Math.random(), Math.random(), Math.random(), 1.0],
                    width: 100,
                    height: 100,
                    transform: Transform.rotate(Transform.identity(), 0.25 * 3.1415926),
                }}></div>
                <div style={{
                    backgroundColor: [Math.random(), Math.random(), Math.random(), 1.0],
                    width: 100,
                    height: 100,
                    transform: Transform.rotate(Transform.identity(), 0.25 * 3.1415926),
                }}></div>

            </div>
        </div>
    </div >
}


function Flex_Test_State() {
    const style_name = ["primary", "secondary", "success", "info", "warning", "danger", "light", "dark"];
    const style_color = [
        [
            [0x536de6ff, 0xffffffff, 0x00000000],
            [0x3a57e2ff, 0xffffffff, 0x00000000],
            [0x2647e0ff, 0xffffffff, 0x536de660]
        ],
        [
            [0x6c757dff, 0xffffffff, 0x00000000],
            [0x5a6268ff, 0xffffffff, 0x00000000],
            [0x545b62ff, 0xffffffff, 0x6c757d60]
        ],
        [
            [0x10c469ff, 0xffffffff, 0x00000000],
            [0x0da156ff, 0xffffffff, 0x00000000],
            [0x0c9550ff, 0xffffffff, 0x10c46960]
        ],
        [
            [0x35b8e0ff, 0xffffffff, 0x00000000],
            [0x20a6cfff, 0xffffffff, 0x00000000],
            [0x1e9dc4ff, 0xffffffff, 0x35b8e060]
        ],
        [
            [0xf9c851ff, 0xffffffff, 0x00000000],
            [0xf8bc2cff, 0xffffffff, 0x00000000],
            [0xf7b820ff, 0xffffffff, 0xf9c85160]
        ],
        [
            [0xff5b5bff, 0xffffffff, 0x00000000],
            [0xff3535ff, 0xffffffff, 0x00000000],
            [0xff2828ff, 0xffffffff, 0xff5b5b60]
        ],
        [
            [0xeef2f7ff, 0x323a46ff, 0x00000000],
            [0xd4deebff, 0x323a46ff, 0x00000000],
            [0xcbd7e7ff, 0x323a46ff, 0xeef2f760]
        ],
        [
            [0x323a46ff, 0xffffffff, 0x00000000],
            [0x222830ff, 0xffffffff, 0x00000000],
            [0x1d2128ff, 0xffffffff, 0x323a4660]
        ]
    ];

    return <div
        style={{
            width: "100%",
            height: "100%",
            backgroundColor: [1, 1, 1, 1],
        }}
    >
        <div style={{
            flexWrap: "wrap",
            flexDirection: "column",
            justifyContent: "center",
            alignItems: "center",
            alignContent: "center",
            width: "100%",
            height: "100%",
        }}>
            {
                style_color.map((list, i) => {
                    return <div style={{
                        width: 340,
                        height: 80,
                        marginBottom: 10,
                        border: [1, 1, 1, 1],
                        borderRadius: [20, 20, 20, 20],
                        backgroundColor: HEX(list[0][0]),
                        fontColor: HEX(list[0][1]),
                        borderColor: HEX(list[0][2]),
                        text: style_name[i],
                        textAlign: "center",
                        HOVER: {
                            backgroundColor: HEX(list[1][0]),
                            fontColor: HEX(list[1][1]),
                            borderColor: HEX(list[1][2]),
                            text: "Box State: Hover",
                        },
                        ACTIVE: {
                            backgroundColor: HEX(list[2][0]),
                            fontColor: HEX(list[2][1]),
                            borderColor: HEX(list[2][2]),
                            text: "Box State: Active",
                        }
                    }}></div>
                })
            }
        </div>
    </div>
}

function Flex_Test_Align() {
    return <div style={{
        flexWrap: "wrap",
        flexDirection: "column",
        justifyContent: "center",
        alignItems: "center",
        alignContent: "center",
        width: "100%",
        height: "100%",
        backgroundColor: [1, 1, 1, 1],
    }}>
        {
            [
                "center-left",
                "center-right",
                "center",
                "top-left",
                "top-right",
                "top-center",
                "bottom-left",
                "bottom-right",
                "bottom-center",
            ].map((align) => {
                return <div style={{
                    width: 340,
                    height: 80,
                    border: [1, 1, 1, 1],
                    borderColor: [0, 0, 0, 1.0],
                    backgroundColor: [0x18 / 255.0, 0x80 / 255.0, 1, 1.0],
                    fontColor: [0, 0, 0, 1],
                    text: "MEUI\nModern embedded UI\n现代化嵌入式用户界面",
                    textAlign: align,
                }} />
            })
        }
    </div>
}

function Circle({ color }) {
    return <div style={{
        width: 20,
        height: 20,
        marginRight: 20,
        backgroundColor: color,
        borderRadius: [10, 10, 10, 10],
    }}></div>
}

function Head() {
    return <div style={{
        width: "100%",
        height: 60,
        alignItems: "center",
        alignContent: "center",
    }}>
        <Circle color={HEX(0xDF491AFF)} />
        <Circle color={HEX(0xFFCC0EFF)} />
        <Circle color={HEX(0x19DF74FF)} />
    </div>
}

function Bar() {
    return <div style={{
        width: "100%",
        alignItems: "center",
        height: 60,
    }}>
        <div style={{
            width: 50,
            height: 50,
            padding: [12.5, 12.5, 12.5, 12.5],
            contentImage: "examples/left.svg",
            backgroundColor: HEX(0x555A64FF),
            borderRadius: [7, 7, 7, 7],
        }}></div>
        <div style={{
            width: 50,
            height: 50,
            padding: [12.5, 12.5, 12.5, 12.5],
            contentImage: "examples/right.svg",
        }}></div>
        <div style={{
            width: 50,
            height: 50,
            marginLeft: 110,
            padding: [12.5, 12.5, 12.5, 12.5],
            backgroundColor: HEX(0x555A64FF),
            borderRadius: [7, 0, 0, 7],
            contentImage: "examples/small-icon-normal.svg",
        }}></div>
        <div style={{
            width: 50,
            height: 50,
            padding: [12.5, 12.5, 12.5, 12.5],
            backgroundColor: HEX(0xFFFFFFFF),
            contentImage: "examples/list-select.svg",
        }}></div>
        <div style={{
            width: 50,
            height: 50,
            padding: [12.5, 12.5, 12.5, 12.5],
            backgroundColor: HEX(0x555A64FF),
            borderRadius: [0, 7, 7, 0],
            contentImage: "examples/card-normal.svg",
        }}></div>
        <div style={{
            width: 225,
            height: 50,
            marginLeft: 50,
            backgroundColor: HEX(0x555A64FF),
            borderRadius: [25, 25, 25, 25],
        }}>
            <div style={{
                flexDirection: "row",
                width: 50,
                height: 50,
                paddingRight: 12.5,
                paddingTop: 12.5,
                paddingLeft: 12.5,
                paddingBottom: 12.5,
                contentImage: "examples/search.svg",
            }}></div>
        </div>


    </div >
}

function Body() {
    return <div style={{
        width: "100%",
        height: 360,
        paddingTop: 30,
        paddingBottom: 30,
        flexDirection: "row",
    }}>
        <div style={{
            width: 212,
            height: "100%",
            flexDirection: "column",
        }}>
            {["Desktop", "Dribbble", "Images", "Downloads"].map((text) => {
                return <div style={{
                    width: 200,
                    height: 55,
                    alignItems: "center",
                    HOVER: {
                        backgroundColor: HEX(0x41464EFF)
                    },
                }}>
                    <div style={{
                        width: 38,
                        height: 38,
                        contentImage: "examples/file.svg",
                    }}>
                    </div>
                    <div style={{
                        width: 145,
                        height: 38,
                        marginLeft: 18,
                        text: text,
                        fontColor: HEX(0xABAFB7FF),
                        fontSize: 24,
                    }}>
                    </div>
                </div>
            })}
        </div>
        <div style={{
            width: 422,
            height: "100%",
            flexDirection: "column",
        }}>{[
            ["ab.png", "3h", "1MB"],
            ["cd.png", "4h", "4MB"],
            ["ef.png", "9h", "1MB"],
            ["gh.png", "1d", "3MB"],
            ["ij.png", "1d", "5MB"],
            ["kl.png", "2d", "2MB"],
        ].map((text) => {
            return <div style={{
                width: "100%",
                height: 55,
                padding: [7, 7, 7, 7],

                HOVER: {
                    backgroundColor: HEX(0x41464EFF),
                    borderRadius: [7, 7, 7, 7],
                },
                ACTIVE: {
                    backgroundColor: HEX(0x32C1FFFF),
                    borderRadius: [7, 7, 7, 7],

                }
            }}>
                <div style={{
                    width: 38,
                    height: 38,
                    backgroundColor: [Math.random(), Math.random(), Math.random(), 1.0],
                }}></div>
                <div style={{
                    width: 180,
                    height: 38,
                    marginLeft: 16,
                    text: text[0],
                    fontColor: HEX(0xDDDDDFFF),
                    fontSize: 20,
                    ACTIVE: {
                        fontColor: HEX(0xFFFFFFFF),
                    }
                }}></div>
                <div style={{
                    width: 50,
                    height: 38,
                    text: text[1],
                    fontColor: HEX(0xDDDDDFFF),
                    fontSize: 20,
                    textAlign: "center-right",
                    ACTIVE: {
                        fontColor: HEX(0xFFFFFFFF),
                    }
                }}></div>
                <div style={{
                    width: 110,
                    height: 38,
                    text: text[2],
                    fontColor: HEX(0xDDDDDFFF),
                    fontSize: 20,
                    textAlign: "center-right",
                    ACTIVE: {
                        fontColor: HEX(0xFFFFFFFF),
                    }
                }}></div>
            </div>
        })}</div>
    </div>
}

function Window({ children, ...props }) {
    return <div style={{
        flexWrap: "wrap",
        flexDirection: "column",
        justifyContent: "center",
        alignItems: "center",
        alignContent: "center",
        paddingLeft: 25,
        paddingRight: 25,
        width: 680,
        height: 480,
        backgroundColor: HEX(0x31353DFF),
        borderRadius: [15, 15, 15, 15],
    }}>{children}</div>
}

function Root({ children, ...props }) {
    return <div style={{
        backgroundColor: HEX(0x0D0F12FF),
        justifyContent: "center",
        alignItems: "center",
        alignContent: "center",
        width: "100%",
        height: "100%",
    }}>{children}</div>
}

function TestApp() {
    return <Root>
        <Window>
            <Head />
            <Bar />
            <Body />
        </Window>
    </Root >
}

render(<TestApp />)
