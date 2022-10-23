/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { spawn } from "child_process"
import kill from "tree-kill"

let childProcess = null
export default function execute(command) {
    return {
        name: "execute",
        writeBundle: async function () {
            if (childProcess) {
                kill(childProcess.pid, "SIGKILL")
            }

            childProcess = spawn(command, {
                shell: true,
                stdio: "inherit",
                env: process.env,
            })

            console.log("spawn", command, childProcess.pid)
        },
    }
}
