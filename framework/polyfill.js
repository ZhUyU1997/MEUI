/* eslint-disable no-inner-declarations */
/* global globalThis */
import * as os from "os"
export const process = {
    env: { NODE_ENV: "development" },
    platform: os.platform,
    cwd: () => {
        const [path, err] = os.getcwd()
        if (err !== 0) throw new Error(" os.cwd() errno")
        return path
    },
}

globalThis.process = process

export const window = {}
globalThis.window = window

export function setTimeout(func, delay) {
    if (func) {
        return os.setTimeout(func, delay)
    }
}

export function clearTimeout(timer) {
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

globalThis.console.warn = console.log
globalThis.console.error = console.log

export function setInterval(callback, time) {
    let timer = null
    function run() {
        if (timer) clearTimeout(timer)
        timer = setTimeout(() => {
            callback()
            run() // Timer priority is lower than callback
        }, time)
    }
    run()
    return {
        clear() {
            if (timer) clearTimeout(timer)
        },
    }
}

export function clearInterval(timer) {
    timer.clear()
}

window.setInterval = setInterval
window.clearInterval = clearInterval
globalThis.setInterval = setInterval
globalThis.clearInterval = clearInterval

let frameArray = []
export function requestAnimationFrame(fn) {
    return frameArray.push(fn) - 1
}
export function cancelAnimationFrame(index) {
    if (index >= 0 && index < frameArray.length) frameArray[index] = null
}
export function triggerAnimationFrame() {
    const temp = frameArray
    frameArray = []
    temp.forEach((fn) => fn?.())
}

window.requestAnimationFrame = requestAnimationFrame
window.cancelAnimationFrame = cancelAnimationFrame
window.triggerAnimationFrame = triggerAnimationFrame

globalThis.requestAnimationFrame = requestAnimationFrame
globalThis.cancelAnimationFrame = cancelAnimationFrame
globalThis.triggerAnimationFrame = triggerAnimationFrame
