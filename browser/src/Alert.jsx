export default function Alert({ status, style }) {
    const colorMap = {
        loading: {
            color: "rgb(30, 70, 32)",
            borderColor: "rgb(76, 175, 80)",
        },
        success: {
            color: "rgb(30, 70, 32)",
            borderColor: "rgb(76, 175, 80)",
        },
        error: {
            color: "rgb(95, 33, 32)",
            borderColor: "rgb(239, 83, 80)",
        },
    }
    return (
        <div
            style={{
                borderRadius: 4,
                marginBottom: 10,
                paddingLeft: 16,
                paddingTop: 6,
                paddingRight: 16,
                paddingBottom: 6,
                borderStyle: "solid",
                borderWidth: 1,
                fontSize: "15pt",
                ...colorMap[status],
                ...style,
            }}
        >
            {status}
        </div>
    )
}
