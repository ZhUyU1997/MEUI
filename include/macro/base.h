/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#define PRIMITIVE_CAT(x, y) x##y
#define CAT(x, y) PRIMITIVE_CAT(x, y)

#define __IS_EMPTY(a, b, size, ...) size
#define _IS_EMPTY(...) __IS_EMPTY(__VA_OPT__(,), 0, 1)
#define IS_EMPTY(...) _IS_EMPTY(__VA_ARGS__)

#define VARIADIC_SIZE_I(x, e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, size, ...) size
#define VARIADIC_SIZE(...) VARIADIC_SIZE_I(x, ##__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
