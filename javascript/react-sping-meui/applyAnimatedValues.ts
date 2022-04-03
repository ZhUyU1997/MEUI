import { Lookup } from '@react-spring/types'

type Value = string | number | boolean | null

const attributeCache: Lookup<string> = {}

type Instance = HTMLDivElement & { style?: Lookup }

export function applyAnimatedValues(instance: Instance, props: Lookup) {
}

