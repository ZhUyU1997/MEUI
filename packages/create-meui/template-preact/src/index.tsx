import { createRoot, View } from "@meui-js/preact"

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

createRoot(300, 300).render(<App />)
