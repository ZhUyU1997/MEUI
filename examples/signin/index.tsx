import React, { useState } from "react"
import { Div, MEUI } from "@/meui"
import ReactMEUI from "@/react-meui"

import { Center, Column, MaterialDesignIcon, Root, Row } from "@/components"
import TextArea from "@/components/text-area"
import { MeuiStyle } from "@/meui/style"

function Input({
    placeholder,
    type = "text",
    style,
}: {
    placeholder: string
    type?: "text" | "password"
    style?: MeuiStyle
}) {
    return (
        <Center
            style={{
                height: 40,
                marginBottom: 10,
                borderRadius: 12.5,
                border: 3,
                borderColor: "black",
                padding: 10,
                ...style,
            }}
        >
            <TextArea
                type={type}
                placeholder={placeholder}
                style={{ width: "100%", height: 20 }}
            ></TextArea>
        </Center>
    )
}

function SignIn({ onClickBtn }: { onClickBtn: (btn: string) => void }) {
    return (
        <Column
            style={{
                justifyContent: "flex-start",
                alignItems: "flex-start",
                borderRadius: 25,
                backgroundColor: "white",
                width: 500,
                height: 505,
                padding: [25, 25, 25, 25],
            }}
        >
            <Div style={{ fontSize: 15 }}>Welcome Back</Div>
            <Div style={{ fontSize: 25, marginBottom: 10 }}>Sign In</Div>
            <Input
                placeholder="email"
                style={{
                    width: 450,
                }}
            ></Input>
            <Input
                type="password"
                placeholder="password"
                style={{
                    width: 450,
                    marginTop: 10,
                }}
            ></Input>
            <Div
                style={{
                    width: "100%",
                    height: 25,
                    fontSize: 15,
                    textAlign: "center-right",
                    margin: [10, 0, 10, 0],
                }}
            >
                {"forgot password?"}
            </Div>
            <Div
                style={{
                    borderRadius: 12.5,
                    fontColor: "white",
                    backgroundColor: "#007AFF",
                    fontSize: 15,
                    width: "100%",
                    height: 40,
                    textAlign: "center",
                }}
                onClick={() => {
                    onClickBtn("Continue")
                }}
            >
                Continue
            </Div>

            <Div
                style={{
                    width: "100%",
                    height: 50,
                    fontSize: 15,
                    textAlign: "center",
                    margin: [10, 0, 10, 0],
                }}
            >
                or continue with
            </Div>
            <Row style={{ width: "100%" }}>
                <Center
                    style={{
                        borderRadius: 12.5,
                        backgroundColor: "black",
                        width: 220,
                        height: 40,
                    }}
                >
                    <MaterialDesignIcon
                        icon="google"
                        color="white"
                        width={20}
                        height={20}
                    ></MaterialDesignIcon>
                </Center>
                <Center
                    style={{
                        borderRadius: 12.5,
                        backgroundColor: "black",
                        width: 220,
                        height: 40,
                        marginLeft: 10,
                    }}
                >
                    <MaterialDesignIcon
                        icon="facebook"
                        color="white"
                        width={20}
                        height={20}
                    ></MaterialDesignIcon>
                </Center>
            </Row>
            <Div
                style={{
                    width: "100%",
                    height: 50,
                    fontSize: 15,
                    textAlign: "center",
                    margin: [10, 0, 10, 0],
                }}
            >
                {"don't have an account?"}
            </Div>
            <Div
                style={{
                    borderRadius: 12.5,
                    fontColor: "white",
                    backgroundColor: "#007AFF",
                    fontSize: 15,
                    width: "100%",
                    height: 40,
                    textAlign: "center",
                }}
                onClick={() => {
                    onClickBtn("SignUp")
                }}
            >
                Sign Up
            </Div>
        </Column>
    )
}

function SignUp({ onClickBtn }: { onClickBtn: (btn: string) => void }) {
    return (
        <Column
            style={{
                justifyContent: "flex-start",
                alignItems: "flex-start",
                borderRadius: 25,
                backgroundColor: "white",
                width: 500,
                height: 420,
                padding: [25, 25, 25, 25],
            }}
        >
            <Div style={{ fontSize: 15 }}>Hello There</Div>
            <Div style={{ fontSize: 25, marginBottom: 10 }}>Sign Up</Div>

            <Row style={{ width: "100%" }}>
                <Input
                    placeholder="first name"
                    style={{
                        width: 220,
                    }}
                ></Input>
                <Input
                    placeholder="last name"
                    style={{
                        width: 220,
                        marginLeft: 10,
                    }}
                ></Input>
            </Row>

            <Input
                placeholder="email"
                style={{
                    width: 450,
                    marginTop: 10,
                }}
            ></Input>

            <Input
                type="password"
                placeholder="password"
                style={{
                    width: 450,
                    marginTop: 10,
                }}
            ></Input>
            <Div
                style={{
                    borderRadius: 12.5,
                    fontColor: "white",
                    backgroundColor: "#007AFF",
                    fontSize: 15,
                    width: "100%",
                    height: 40,
                    textAlign: "center",
                }}
                onClick={() => {
                    onClickBtn("Continue")
                }}
            >
                Continue
            </Div>

            <Div
                style={{
                    width: "100%",
                    height: 50,
                    fontSize: 15,
                    textAlign: "center",
                    margin: [10, 0, 10, 0],
                }}
            >
                have an account?
            </Div>
            <Div
                style={{
                    borderRadius: 12.5,
                    fontColor: "white",
                    backgroundColor: "#007AFF",
                    fontSize: 15,
                    width: "100%",
                    height: 40,
                    textAlign: "center",
                }}
                onClick={() => {
                    onClickBtn("SignIn")
                }}
            >
                Sign In
            </Div>
        </Column>
    )
}

function App() {
    const [showSignIn, setShowSignIn] = useState(true)
    const onClickBtn = (btn: string) => {
        if (btn === "SignIn") setShowSignIn(true)
        if (btn === "SignUp") setShowSignIn(false)
    }
    return (
        <Root
            style={{
                flexDirection: "column",
                justifyContent: "center",
                alignItems: "center",
                backgroundColor: "#f0f0f0",
            }}
        >
            {showSignIn ? (
                <SignIn onClickBtn={onClickBtn} />
            ) : (
                <SignUp onClickBtn={onClickBtn} />
            )}
        </Root>
    )
}

const meui = new MEUI(600, 600)
ReactMEUI.render(<App />, meui)
