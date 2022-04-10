/* eslint-disable no-inner-declarations */
/* global globalThis */
import * as os from "os"
globalThis.process = {
    env: { NODE_ENV: "development" },
    platform: os.platform,
    cwd: () => {
        const [path, err] = os.getcwd()
        if (err !== 0) throw new Error(" os.cwd() errno")
        return path
    },
}

if (!globalThis.window) {
    globalThis.window = {}
}

if (!globalThis.setTimeout) {
    function setTimeout(func, delay) {
        if (func) {
            return os.setTimeout(func, delay)
        }
    }

    function clearTimeout(timer) {
        // After checking React/invokeGuardedCallbackProd, it is found that if the parameter is not defined, you will get an exception
        // So for compatibility we have to handle it.
        if (timer) {
            os.clearTimeout(timer)
        }
    }
    window.setTimeout = setTimeout
    window.clearTimeout = clearTimeout
    globalThis.setTimeout = setTimeout
    globalThis.clearTimeout = clearTimeout
}

globalThis.console.warn = console.log
globalThis.console.error = console.log

if (!globalThis.setInterval) {
    function setInterval(callback, time) {
        let timer = null
        function run() {
            if (timer) clearTimeout(timer)
            timer = setTimeout(() => {
                run()
                callback()
            }, time)
        }
        run()
        return {
            clear() {
                if (timer) clearTimeout(timer)
            },
        }
    }

    function clearInterval(timer) {
        timer.clear()
    }

    window.setInterval = setInterval
    window.clearInterval = clearInterval
    globalThis.setInterval = setInterval
    globalThis.clearInterval = clearInterval
}

if (!globalThis.requestAnimationFrame) {
    let frameArray = []
    function requestAnimationFrame(fn) {
        frameArray.push(fn)
    }
    function cancelAnimationFrame(fn) {
        const index = frameArray.indexOf(fn)
        if (index !== -1) {
            frameArray.splice(index, 1)
        }
    }
    function triggerAnimationFrame() {
        const temp = frameArray
        frameArray = []
        temp.forEach((fn) => fn())
    }

    window.requestAnimationFrame = requestAnimationFrame
    window.cancelAnimationFrame = cancelAnimationFrame
    window.triggerAnimationFrame = triggerAnimationFrame

    globalThis.requestAnimationFrame = requestAnimationFrame
    globalThis.cancelAnimationFrame = cancelAnimationFrame
    globalThis.triggerAnimationFrame = triggerAnimationFrame
}
