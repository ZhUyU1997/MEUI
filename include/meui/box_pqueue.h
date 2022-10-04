/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "pqueue.h"
// drawing order
pqueue_t *box_pqueue_init(size_t n);
// search order
pqueue_t *box_reverse_pqueue_init(size_t n);