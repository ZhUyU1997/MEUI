import ReactReconciler from "react-reconciler"
import { createBox } from "."
import { CanvasElement } from "./canvas"

function shallowDiff(oldObj, newObj) {
    // Return a diff between the new and the old object
    const uniqueProps = new Set([
        ...Object.keys(oldObj),
        ...Object.keys(newObj),
    ])
    const changedProps = Array.from(uniqueProps).filter(
        (propName) => oldObj[propName] !== newObj[propName]
    )

    return changedProps
}

const rootHostContext = {}
const childHostContext = {}

// const log = console.log
const log = (...args) => {}
const hostConfig = {
    now: Date.now,
    supportsMutation: true,
    getRootHostContext: () => {
        log("getRootHostContext")
        return rootHostContext
    },
    prepareForCommit: () => {
        log("prepareForCommit")
        return null
    },
    resetAfterCommit: (containerInfo) => {
        log("resetAfterCommit")
    },
    getChildHostContext: () => {
        log("getChildHostContext")
        return childHostContext
    },
    shouldSetTextContent: (type, props) => {
        log("# shouldSetTextContent", type, props.children)
        return (
            typeof props.children === "string" ||
            typeof props.children === "number"
        )
    },
    createInstance: (
        type,
        { style, children, ...newProps },
        rootContainerInstance,
        _currentHostContext,
        workInProgress
    ) => {
        log("createInstance " + type)
        const domElement = createBox(type)

        if (typeof children === "string" || typeof children === "number") {
            domElement.setStyle({ text: children })
        }

        if (style) {
            domElement.setStyle(style)
        }

        Object.keys(newProps).forEach((propName) => {
            const propValue = newProps[propName]
            // if (propName === 'children') {
            //     if (typeof propValue === 'string' || typeof propValue === 'number') {
            //         domElement.textContent = propValue;
            //     }
            // }

            // else if (propName === 'onClick') {
            //     domElement.addEventListener('click', propValue);
            // } else if (propName === 'className') {
            //     domElement.setAttribute('class', propValue);
            // } else {
            //     const propValue = newProps[propName];
            //     domElement.setAttribute(propName, propValue);
            // }
            if (propName.startsWith("on")) {
                const index = propName.indexOf("Capture")

                let type, useCapture
                if (index !== -1) {
                    type = propName.toLowerCase().substring(2, index)
                    useCapture = true
                } else {
                    type = propName.toLowerCase().substring(2)
                    useCapture = false
                }
                domElement.addEventListener(type, propValue, useCapture)
            }
        })

        if (domElement instanceof CanvasElement) {
            if ("width" in newProps) {
                domElement.width = newProps.width
            }
            if ("height" in newProps) {
                domElement.height = newProps.height
            }

            if ("fit" in newProps) {
                domElement.fit = newProps.fit
            }
        }
        return domElement
    },
    createTextInstance: (text) => {
        log("createTextInstance", text)
        return text
    },
    appendInitialChild: (parent, child) => {
        log("appendInitialChild", child)
        parent.addChild(child)
    },
    appendChild(parent, child) {
        log("appendChild", child)
        parent.addChild(child)
    },
    finalizeInitialChildren: (domElement, type, props) => {
        log("finalizeInitialChildren")
    },
    appendChildToContainer: (parent, child) => {
        log("appendChildToContainer", child)
        parent.addChild(child)
    },
    prepareUpdate(domElement, oldProps, newProps, rootContainer, hostContext) {
        // Return a diff between the new and the old props
        log("prepareUpdate")
        return shallowDiff(oldProps, newProps)
    },
    commitUpdate(
        domElement,
        updatePayload,
        type,
        _oldProps,
        { style, ...newProps }
    ) {
        log("commitUpdate")

        const { style: oldStyle, oldProps } = _oldProps

        if (style) {
            const styleDiffs = shallowDiff(oldStyle, style)
            const finalStyles = styleDiffs.reduce((acc, styleName) => {
                // Style marked to be unset
                if (!style[styleName]) acc[styleName] = null
                else acc[styleName] = style[styleName]

                return acc
            }, {})
            domElement.setStyle(finalStyles)
        }

        const propsDiffs = shallowDiff(oldProps, newProps)
        const finalProps = propsDiffs.reduce((acc, name) => {
            // Style marked to be unset
            if (!newProps[name]) acc[name] = null
            else acc[name] = newProps[name]

            return acc
        }, {})

        Object.keys(finalProps).forEach((propName) => {
            const propValue = finalProps[propName]
            if (propName === "children") {
                if (
                    typeof propValue === "string" ||
                    typeof propValue === "number"
                ) {
                    domElement.setStyle({ text: propValue })
                }
            } else if (propName.startsWith("on")) {
                const index = propName.indexOf("Capture")
                let type, useCapture
                if (index !== -1) {
                    type = propName.toLowerCase().substring(2, index)
                    useCapture = true
                } else {
                    type = propName.toLowerCase().substring(2)
                    useCapture = false
                }
                domElement.removeEventListener(
                    type,
                    oldProps[propName],
                    useCapture
                )
                domElement.addEventListener(type, propValue, useCapture)
            }
        })

        if (domElement instanceof CanvasElement) {
            if ("width" in finalProps) {
                domElement.width = finalProps.width
            }
            if ("height" in finalProps) {
                domElement.height = finalProps.height
            }
            if ("fit" in finalProps) {
                domElement.fit = finalProps.fit
            }
        }
    },
    commitTextUpdate(textInstance, oldText, newText) {
        log("commitTextUpdate")
    },
    removeChild(parentInstance, child) {
        log("removeChild")
        parentInstance.removeChild(child)
    },
    removeChildFromContainer(container, child) {
        container.removeChild(child)
    },
    clearContainer(container) {
        log("clearContainer")
        // container.remove()
    },
    insertBefore(parentInstance, child, beforeChild) {
        log("insertBefore")
        parentInstance.insertBefore(child, beforeChild)
    },
    getPublicInstance(instance) {
        log("getPublicInstance")
        return instance
    },
}

const ReactReconcilerInst = ReactReconciler(hostConfig)

export default {
    render: (reactElement, meui, callback) => {
        const container = ReactReconcilerInst.createContainer(
            meui.getRoot(),
            false
        )
        // update the root Container
        return ReactReconcilerInst.updateContainer(
            reactElement,
            container,
            null,
            callback
        )
    },
}

export { batchedUpdates } from "react-reconciler"
