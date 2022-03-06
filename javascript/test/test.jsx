import React from "react"

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
            ["#536de6ff", "#ffffffff", "#00000000"],
            ["#3a57e2ff", "#ffffffff", "#00000000"],
            ["#2647e0ff", "#ffffffff", "#536de660"]
        ],
        [
            ["#6c757dff", "#ffffffff", "#00000000"],
            ["#5a6268ff", "#ffffffff", "#00000000"],
            ["#545b62ff", "#ffffffff", "#6c757d60"]
        ],
        [
            ["#10c469ff", "#ffffffff", "#00000000"],
            ["#0da156ff", "#ffffffff", "#00000000"],
            ["#0c9550ff", "#ffffffff", "#10c46960"]
        ],
        [
            ["#35b8e0ff", "#ffffffff", "#00000000"],
            ["#20a6cfff", "#ffffffff", "#00000000"],
            ["#1e9dc4ff", "#ffffffff", "#35b8e060"]
        ],
        [
            ["#f9c851ff", "#ffffffff", "#00000000"],
            ["#f8bc2cff", "#ffffffff", "#00000000"],
            ["#f7b820ff", "#ffffffff", "#f9c85160"]
        ],
        [
            ["#ff5b5bff", "#ffffffff", "#00000000"],
            ["#ff3535ff", "#ffffffff", "#00000000"],
            ["#ff2828ff", "#ffffffff", "#ff5b5b60"]
        ],
        [
            ["#eef2f7ff", "#323a46ff", "#00000000"],
            ["#d4deebff", "#323a46ff", "#00000000"],
            ["#cbd7e7ff", "#323a46ff", "#eef2f760"]
        ],
        [
            ["#323a46ff", "#ffffffff", "#00000000"],
            ["#222830ff", "#ffffffff", "#00000000"],
            ["#1d2128ff", "#ffffffff", "#323a4660"]
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
                        backgroundColor: (list[0][0]),
                        fontColor: (list[0][1]),
                        borderColor: (list[0][2]),
                        text: style_name[i],
                        textAlign: "center",
                        HOVER: {
                            backgroundColor: (list[1][0]),
                            fontColor: (list[1][1]),
                            borderColor: (list[1][2]),
                            text: "Box State: Hover",
                        },
                        ACTIVE: {
                            backgroundColor: (list[2][0]),
                            fontColor: (list[2][1]),
                            borderColor: (list[2][2]),
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
        <Circle color="#DF491AFF" />
        <Circle color="#FFCC0EFF" />
        <Circle color="#19DF74FF" />
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
            backgroundColor: "#555A64FF",
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
            backgroundColor: "#555A64FF",
            borderRadius: [7, 0, 0, 7],
            contentImage: "examples/small-icon-normal.svg",
        }}></div>
        <div style={{
            width: 50,
            height: 50,
            padding: [12.5, 12.5, 12.5, 12.5],
            backgroundColor: "#FFFFFFFF",
            contentImage: "examples/list-select.svg",
        }}></div>
        <div style={{
            width: 50,
            height: 50,
            padding: [12.5, 12.5, 12.5, 12.5],
            backgroundColor: "#555A64FF",
            borderRadius: [0, 7, 7, 0],
            contentImage: "examples/card-normal.svg",
        }}></div>
        <div style={{
            width: 225,
            height: 50,
            marginLeft: 50,
            backgroundColor: "#555A64FF",
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
            {["Desktop", "Dribbble", "Images", "Downloads"].map((text, index) => {
                return <div
                    style={{
                        width: 200,
                        height: 55,
                        alignItems: "center",
                        HOVER: {
                            backgroundColor: "#41464EFF"
                        },
                    }}
                    onMouseDown={(e) => {
                        console.log("onMouseDown:" + index)
                    }}
                >
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
                        fontColor: "#ABAFB7FF",
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
                    backgroundColor: "#41464EFF",
                    borderRadius: [7, 7, 7, 7],
                },
                ACTIVE: {
                    backgroundColor: "#32C1FFFF",
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
                    fontColor: "#DDDDDFFF",
                    fontSize: 20,
                    ACTIVE: {
                        fontColor: "#FFFFFFFF",
                    }
                }}></div>
                <div style={{
                    width: 50,
                    height: 38,
                    text: text[1],
                    fontColor: "#DDDDDFFF",
                    fontSize: 20,
                    textAlign: "center-right",
                    ACTIVE: {
                        fontColor: "#FFFFFFFF",
                    }
                }}></div>
                <div style={{
                    width: 110,
                    height: 38,
                    text: text[2],
                    fontColor: "#DDDDDFFF",
                    fontSize: 20,
                    textAlign: "center-right",
                    ACTIVE: {
                        fontColor: "#FFFFFFFF",
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
        backgroundColor: "#31353DFF",
        borderRadius: [15, 15, 15, 15],
    }}>{children}</div>
}

function Root({ children, ...props }) {
    return <div style={{
        backgroundColor: "#0D0F12FF",
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

export {
    Flex_Test, Flex_Test_State, Flex_Test_Align, TestApp
}