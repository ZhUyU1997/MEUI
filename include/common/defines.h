/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#define container_of(ptr, type, member)		({const typeof(((type *)0)->member) *__mptr = (ptr); (type *)((char *)__mptr - offsetof(type,member));})