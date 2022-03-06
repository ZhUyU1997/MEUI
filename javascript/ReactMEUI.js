import ReactReconciler from 'react-reconciler';
import { Box } from "./meui"

const rootHostContext = {};
const childHostContext = {};

// const log = console.log
const log = ()=>{}
const hostConfig = {
    now: Date.now,
    getRootHostContext: () => {
        log("getRootHostContext")
        return rootHostContext;
    },
    prepareForCommit: () => {
        log("prepareForCommit")
    },
    resetAfterCommit: () => {
        log("resetAfterCommit")
    },
    getChildHostContext: () => {
        log("getChildHostContext")
        return childHostContext;
    },
    shouldSetTextContent: (type, props) => {
        log("# shouldSetTextContent", type, props.children)
        if (Array.isArray(props.children)) {
            return props.children.every((item) => typeof item === 'string' || typeof item === 'number')
        }
        return typeof props.children === 'string' || typeof props.children === 'number';
    },
    createInstance: (type, { style, children, ...newProps }, rootContainerInstance, _currentHostContext, workInProgress) => {
        log("createInstance " + type)

        const domElement = new Box();

        if (typeof children === 'string' || typeof children === 'number') {
            domElement.setStyle({ text: children })
        }
        else if (children.every((item) => typeof item === 'string' || typeof item === 'number')) {

            domElement.setStyle({ text: children.join("") })
        }

        if (style) {
            console.log(JSON.stringify(style))
            domElement.setStyle(style)
        }

        Object.keys(newProps).forEach(propName => {
            const propValue = newProps[propName];
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

                let type, useCapture;
                if (index !== -1) {
                    type = propName.toLowerCase().substring(2, index)
                    useCapture = true
                }
                else {
                    type = propName.toLowerCase().substring(2)
                    useCapture = false
                }
                domElement.addEventListener(type, propValue, useCapture);
            }
        });
        return domElement;
    },
    createTextInstance: text => {
        return text;
    },
    appendInitialChild: (parent, child) => {
        log("appendInitialChild", child)
        parent.addChild(child);
    },
    appendChild(parent, child) {
        log("appendChild", child)
        parent.addChild(child);
    },
    finalizeInitialChildren: (domElement, type, props) => {
        log("finalizeInitialChildren")
    },
    supportsMutation: true,
    appendChildToContainer: (parent, child) => {
        log("appendChildToContainer", child)
        parent.addChild(child);
    },
    prepareUpdate(domElement, oldProps, newProps) {
        log("prepareUpdate")

        return true;
    },
    commitUpdate(domElement, updatePayload, type, oldProps, newProps) {
        log("commitUpdate")

        Object.keys(newProps).forEach(propName => {
            const propValue = newProps[propName];
            if (propName === 'children') {
                if (typeof propValue === 'string' || typeof propValue === 'number') {
                    domElement.setStyle({ text: propValue })
                }
                else if (propValue.every((item) => typeof item === 'string' || typeof item === 'number')) {
                    domElement.setStyle({ text: propValue.join("") })
                }
            } else if (propName.startsWith("on")) {
                const index = propName.indexOf("Capture")
                let type, useCapture;
                if (index !== -1) {
                    type = propName.toLowerCase().substring(2, index)
                    useCapture = true
                }
                else {
                    type = propName.toLowerCase().substring(2)
                    useCapture = false
                }
                domElement.removeEventListener(type, oldProps[propName], useCapture);
                domElement.addEventListener(type, propValue, useCapture);
            }
        });
    },
    commitTextUpdate(textInstance, oldText, newText) {
        log("commitTextUpdate")
    },
    removeChild(parentInstance, child) {
        log("removeChild")
        parentInstance.removeChild(child);
    },
    clearContainer(container) {
        log("clearContainer")
        // container.remove()
    }
};

const ReactReconcilerInst = ReactReconciler(hostConfig);

export default {
    render: (reactElement, meui, callback) => {
        const container = ReactReconcilerInst.createContainer(meui.getRoot(), false);
        // update the root Container
        return ReactReconcilerInst.updateContainer(reactElement, container, null, callback);
    }
};