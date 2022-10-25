import semver from "semver"

import {
    args,
    getActiveVersion,
    getPackageInfo,
    publishPackage,
    parseTag,
    step,
} from "./releaseUtils"

async function main() {
    const tag = args._[0]

    if (!tag) {
        throw new Error("No tag specified")
    }

    const { pkgName, version } = parseTag(tag)

    const { currentVersion, pkgDir } = getPackageInfo(pkgName)
    if (currentVersion !== version)
        throw new Error(
            `Package version from tag "${version}" mismatches with current version "${currentVersion}"`
        )

    const activeVersion =
        currentVersion === "0.0.1-alpha.0"
            ? currentVersion
            : await getActiveVersion(pkgName)

    step("Publishing package...")
    const releaseTag = version.includes("beta")
        ? "beta"
        : version.includes("alpha")
        ? "alpha"
        : semver.lt(currentVersion, activeVersion)
        ? "previous"
        : undefined
    await publishPackage(pkgDir, releaseTag)
}

main().catch((err) => {
    console.error(err)
    process.exit(1)
})
