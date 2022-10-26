import { existsSync, readdirSync, writeFileSync } from "node:fs"
import path from "node:path"
import colors from "picocolors"
import type { Options as ExecaOptions, ExecaReturnValue } from "execa"
import { execa } from "execa"
import type { ReleaseType } from "semver"
import semver from "semver"
import fs from "fs-extra"
import minimist from "minimist"
import npa from "npm-package-arg"

export const args = minimist(process.argv.slice(2))
export const isDryRun = !!args.dry

if (isDryRun) {
    console.log(colors.inverse(colors.yellow(" DRY RUN ")))
    console.log()
}

export const packagesPath = {
    "@meui-js/core": "packages/core",
    "@meui-js/preact": "packages/preact",
    "@meui-js/spring": "packages/spring",
    "@meui-js/components": "packages/components",
    "@meui-js/native": "packages/@types/meui-native",
    "@meui-js/os": "packages/@types/os",
    "@meui-js/std": "packages/@types/std",
} as const

export function parseTag(tag: string) {
    try {
        const parsed = npa(tag)

        if (parsed.name == null) {
            throw new Error()
        }

        if (parsed.fetchSpec == null) {
            throw new Error()
        }
        return {
            pkgName: parsed.name,
            version: parsed.fetchSpec,
        }
    } catch (error) {
        throw new Error(`Failed to parse git tag "${tag}"`)
    }
}

interface Pkg {
    name: string
    version: string
    private?: boolean
}

export function getPackageInfo(pkgName: string): {
    pkg: Pkg
    pkgName: string
    pkgDir: string
    pkgPath: string
    currentVersion: string
} {
    if (!(pkgName in packagesPath)) {
        throw new Error(`Package ${pkgName} not found in packagesPath)`)
    }

    const pkgDir = path.resolve(__dirname, "..", packagesPath[pkgName])

    if (!existsSync(pkgDir)) {
        throw new Error(`Package ${pkgName} not found (${pkgDir})`)
    }

    const pkgPath = path.resolve(pkgDir, "package.json")
    const pkg: Pkg = fs.readJsonSync(pkgPath)
    const currentVersion = pkg.version

    if (pkg.private) {
        throw new Error(`Package ${pkgName} is private`)
    }

    return {
        pkg,
        pkgName,
        pkgDir,
        pkgPath,
        currentVersion,
    }
}

export async function run(
    bin: string,
    args: string[],
    opts: ExecaOptions<string> = {}
): Promise<ExecaReturnValue<string>> {
    return execa(bin, args, { stdio: "inherit", ...opts })
}

export async function dryRun(
    bin: string,
    args: string[],
    opts?: ExecaOptions<string>
): Promise<void> {
    return console.log(
        colors.blue(`[dryrun] ${bin} ${args.join(" ")}`),
        opts || ""
    )
}

export const runIfNotDry = isDryRun ? dryRun : run

export function step(msg: string): void {
    return console.log(colors.cyan(msg))
}

export async function publishPackage(
    pkdDir: string,
    tag?: string
): Promise<void> {
    const publicArgs = ["publish", "--access", "public", "--no-git-checks"]
    if (tag) {
        publicArgs.push(`--tag`, tag)
    }
    await runIfNotDry("pnpm", publicArgs, {
        cwd: pkdDir,
    })
}

export async function getActiveVersion(pkgName: string): Promise<string> {
    return (await run("npm", ["info", pkgName, "version"], { stdio: "pipe" }))
        .stdout
}
