import { useEffect } from "react"
import { getTime, getVersion } from "./util"
import { useWindowSize } from "@react-hookz/web"
import useESBuild from "./build"

import Editor from "./Editor"
import MEUI from "./MEUI"
import Alert from "./Alert"
import { useUpdatePrompt } from "./UpdatePopup"

const codeRaw = `import { createRoot, View } from "@meui-js/preact"

function App() {
    return (
        <View
            style={{
                width: 120,
                height: 120,
                backgroundColor: "#46e891",
                borderRadius: 16,
                textAlign: "center",
            }}
        >
            Hello MEUI!
        </View>
    )
}

createRoot(300, 300).render(<App />)`

export default function App() {
    useUpdatePrompt()
    const [state, execute] = useESBuild()
    const { width, height } = useWindowSize()
    const isPortrait = width < height
    console.log(width, height)

    useEffect(() => {
        execute(codeRaw)
        // eslint-disable-next-line react-hooks/exhaustive-deps
    }, [])
    return (
        <div
            style={{
                display: "flex",
                width: "100%",
                height: "100%",
                flexDirection: "column",
                justifyContent: "center",
                alignItems: "center",
                backgroundColor: "#F3F6F9",
                paddingLeft: isPortrait ? 20 : "10%",
                paddingRight: isPortrait ? 20 : "10%",
                paddingTop: 20,
                boxSizing: "border-box",
            }}
        >
            <div
                style={{
                    width: "min(100%, 1500px)",
                    display: "flex",
                    flexDirection: "column",
                    justifyContent: "center",
                    alignItems: "center",
                    flexGrow: 1,
                }}
            >
                <Alert
                    status={state.status}
                    style={{
                        alignSelf: "flex-start",
                    }}
                ></Alert>
                <div
                    style={{
                        width: "100%",
                        flexGrow: 1,
                        borderRadius: 4,
                        overflow: "hidden",
                        backgroundColor: "white",
                        boxSizing: "border-box",
                        display: "flex",
                        flexDirection: isPortrait ? "column" : "row",
                        padding: 10,
                        boxShadow:
                            "0px 2px 1px -1px rgb(0 0 0 / 20%), 0px 1px 1px 0px rgb(0 0 0 / 14%), 0px 1px 3px 0px rgb(0 0 0 / 12%)",
                    }}
                >
                    <Editor
                        value={codeRaw}
                        style={{
                            height: isPortrait ? "50%" : "100%",
                            width: isPortrait ? "100%" : "50%",
                            maxHeight: isPortrait ? "50%" : "100%",
                            maxWidth: isPortrait ? "100%" : "50%",
                            display: "flex",
                            justifyContent: "center",
                            alignItems: "center",
                        }}
                        onChange={(value) => {
                            execute(value)
                        }}
                        options={{
                            lineNumbers: isPortrait ? "off" : "on",
                            fontSize: isPortrait ? "10pt" : "20pt",
                        }}
                    ></Editor>

                    <MEUI
                        input={state.result}
                        style={{
                            height: isPortrait ? "50%" : "100%",
                            width: isPortrait ? "100%" : "50%",
                            maxHeight: isPortrait ? "50%" : "100%",
                            maxWidth: isPortrait ? "100%" : "50%",
                            display: "flex",
                            justifyContent: "center",
                            alignItems: "center",
                            backgroundColor: "#606060",
                        }}
                    ></MEUI>
                </div>
            </div>

            <div
                style={{
                    marginTop: 10,
                    marginBottom: 10,
                    fontSize: "14pt",
                    height: "30pt",
                    color: "#3E5060",
                }}
            >
                MEUI {getVersion()} {getTime()}
            </div>
        </div>
    )
}
