// export type Primitives = 'div' | 'stack'

export type Primitives = keyof JSX.IntrinsicElements

export const primitives: Primitives[] = [
  'div',
  // 'stack',
]
