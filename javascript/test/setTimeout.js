import * as std from "std"
import * as os from "os"

for (let i = 0; i < 10000; i++) {
    const timer = os.setTimeout(() => {}, 100)
}


for (let i = 0; i < 10000; i++) {
    const timer = os.setTimeout(() => {}, 100)
}

function test() {
    let timer = null
    function run() {
        timer = os.setTimeout(() => {
            console.log(run, typeof timer)
        }, 1000)
    }
    run()
}

test();

std.gc();