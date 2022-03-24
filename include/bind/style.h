#pragma once
#include "quickjs.h"

typedef struct box_style_t box_style_t;

typedef struct
{
    const char *name;
    JSValue (*get)(JSContext *ctx, box_style_t *style);
    JSValue (*set)(JSContext *ctx, box_style_t *style, JSValue val);
} JSStyleGetSet;

extern const JSStyleGetSet jsStyleGetSet[];
extern const int jsStyleGetSetLength;