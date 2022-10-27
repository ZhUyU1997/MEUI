import Loading from "./Loading"
import MonacoEditor from "@monaco-editor/react"

export default function Editor({ style, value, onChange, options }) {
    return (
        <div
            style={{
                position: "relative",
                ...style,
            }}
        >
            <div
                style={{
                    position: "absolute",
                    height: "100%",
                    width: "100%",
                }}
            >
                <MonacoEditor
                    language="javascript"
                    value={value}
                    loading={
                        <Loading
                            style={{
                                backgroundColor: "#c0c0c0",
                            }}
                        ></Loading>
                    }
                    onChange={onChange}
                    {...{
                        height: "100%",
                        width: "100%",
                        options: {
                            scrollBeyondLastLine: false,
                            minimap: { enabled: false },
                            contextmenu: false,
                            automaticLayout: true,
                            ...options,
                        },
                    }}
                ></MonacoEditor>
            </div>
        </div>
    )
}
