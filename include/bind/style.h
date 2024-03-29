/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "quickjs.h"
#include "gen/style.h"
#include "meui/box.h"

typedef struct box_style_t box_style_t;

typedef struct
{
    const char *name;
    JSValue (*get)(JSContext *ctx, box_t node, box_style_t *style);
    JSValue (*set)(JSContext *ctx, box_t node, box_style_t *style, JSValue val);
    box_style_flag_t enumValue;
} JSStyleGetSet;

extern const JSStyleGetSet jsStyleGetSet[];
extern const int jsStyleGetSetLength;