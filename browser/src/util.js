import dayjs from "dayjs"


export function getVersion() {
    return __APP_VERSION__
}

export function getTime() {
    return dayjs(__APP_BUILD_TIME__).format("YYYY-MM-DD HH:mm:ss")
}
