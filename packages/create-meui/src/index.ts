import fs from "fs"
import path from "path"
import { fileURLToPath } from "url"
import minimist from "minimist"

const argv = minimist<{
    t?: string
    template?: string
}>(process.argv.slice(2), { string: ["_"] })

const cwd = process.cwd()

function copy(src: string, dest: string) {
    const stat = fs.statSync(src)
    if (stat.isDirectory()) {
        copyDir(src, dest)
    } else {
        fs.copyFileSync(src, dest)
    }
}

function copyDir(srcDir: string, destDir: string) {
    fs.mkdirSync(destDir, { recursive: true })
    for (const file of fs.readdirSync(srcDir)) {
        const srcFile = path.resolve(srcDir, file)
        const destFile = path.resolve(destDir, file)
        copy(srcFile, destFile)
    }
}

function formatTargetDir(targetDir: string | undefined) {
    return targetDir?.trim().replace(/\/+$/g, "")
}

async function init() {
    const argTargetDir = formatTargetDir(argv._[0])
    const argTemplate = argv.template || argv.t || "preact"
    const defaultTargetDir = "meui-project"
    const targetDir = argTargetDir || defaultTargetDir
    const templateDir = path.resolve(
        fileURLToPath(import.meta.url),
        "../..",
        `template-${argTemplate}`
    )

    const root = path.join(cwd, targetDir)

    if (!fs.existsSync(root)) {
        fs.mkdirSync(root, { recursive: true })
    }

    copy(templateDir, targetDir)
}
init().catch((e) => {
    console.error(e)
})
