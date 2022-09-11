/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "pqueue.h"
#include "meui/box.h"

static int cmp_pri(pqueue_pri_t next, pqueue_pri_t curr)
{
    return (next > curr);
}

static pqueue_pri_t get_pri(void *a)
{
    Box *box = Flex_getContext((box_t)a);
    return box->style.zIndex;
}

static void set_pri(void *a, pqueue_pri_t pri)
{
    Box *box = Flex_getContext((box_t)a);
    box->style.zIndex = pri;
}

static size_t get_pos(void *a)
{
    Box *box = Flex_getContext((box_t)a);

    return box->queue_pos;
}

static void set_pos(void *a, size_t pos)
{
    Box *box = Flex_getContext((box_t)a);
    box->queue_pos = pos;
}

pqueue_t *box_pqueue_init(size_t n)
{
    return pqueue_init(10, cmp_pri, get_pri, set_pri, get_pos, set_pos);
}
