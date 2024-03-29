import { useEffect } from "react"
import NiceModal, { useModal } from "@ebay/nice-modal-react"
import { useRegisterSW } from "virtual:pwa-register/react"

export const UpdatePopup = NiceModal.create(() => {
    // Use a hook to manage the modal state
    const modal = useModal()
    return (
        <div
            style={{
                width: "100%",
                height: "100%",
                position: "fixed",
                left: 0,
                top: 0,
                display: "flex",
                justifyContent: "center",
                alignItems: "center",
                backgroundColor: "rgb(0,0,0,0.3)",
            }}
        >
            <div
                style={{
                    display: "flex",
                    justifyContent: "center",
                    alignItems: "center",
                    flexDirection: "column",
                    lineHeight: 1.5,
                    backgroundColor: "white",
                    color: "rgba(0,0,0,0.87)",
                    fontSize: "1.25rem",
                    margin: 20,
                    padding: 16,
                    paddingLeft: 23,
                    paddingRight: 23,
                    borderRadius: 4,
                    boxShadow:
                        "rgb(0 0 0 / 20%) 0px 11px 15px -7px, rgb(0 0 0 / 14%) 0px 24px 38px 3px, rgb(0 0 0 / 12%) 0px 9px 46px 8px",
                }}
                onClick={() => {
                    modal.resolve()
                    modal.remove()
                }}
            >
                A new version is available!{" "}
                <div
                    style={{
                        color: "rgb(25, 118, 210)",
                        fontWeight: 700,
                        paddingTop: 20,
                        alignSelf: "flex-end",
                    }}
                >
                    RELOAD
                </div>
            </div>
        </div>
    )
})

export function useUpdatePrompt() {
    /// https://vite-plugin-pwa.netlify.app/frameworks/react.html#prompt-for-update
    const {
        offlineReady: [offlineReady, setOfflineReady],
        needRefresh: [needRefresh, setNeedRefresh],
        updateServiceWorker,
    } = useRegisterSW({
        onRegistered(r) {
            // eslint-disable-next-line prefer-template
            console.log("SW Registered: " + r)
        },
        onRegisterError(error) {
            console.log("SW registration error", error)
        },
    })

    useEffect(() => {
        if (needRefresh) {
            NiceModal.show(UpdatePopup).then(() => {
                updateServiceWorker(true)
            })
        }
    }, [needRefresh, updateServiceWorker])
}
