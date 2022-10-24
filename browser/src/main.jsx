import React from "react"
import ReactDOM from "react-dom/client"
import App from "./App"

/* code to prevent emscripten compiled code from eating key input */
const old = window.addEventListener

window.addEventListener = (...args) => {
    if (args[0].startsWith("key")) {
        if (args[1].name === "jsEventHandler") {
            console.log("filter out jsEventHandler")
            return
        }
    }

    return old.apply(window, args)
}

ReactDOM.createRoot(document.getElementById("root")).render(
    <React.StrictMode>
        <App />
    </React.StrictMode>
)
