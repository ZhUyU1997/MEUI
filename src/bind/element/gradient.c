/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "cutils.h"
#include "quickjs.h"
#include "quickjs-libc.h"
#include "meui/box.h"
#include "meui/element.h"

#include <plutosvg.h>
#include <string.h>
#include <bind/style.h>

#include <assert.h>

#define countof(x) (sizeof(x) / sizeof((x)[0]))

static JSClassID js_gradient_class_id;

static JSValue js_gradient_addColorStop(JSContext *ctx, JSValueConst this_val,
                                        int argc, JSValueConst *argv)
{
    plutovg_gradient_t *gradient = JS_GetOpaque2(ctx, this_val, js_gradient_class_id);
    assert(gradient);

    double offset, r, g, b, a;
    if (JS_ToFloat64(ctx, &offset, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &r, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &g, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &b, argv[3]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &a, argv[4]))
        return JS_EXCEPTION;

    plutovg_gradient_add_stop_rgba(gradient, offset, r, g, b, a);
    return JS_UNDEFINED;
}

static const JSCFunctionListEntry js_gradient_proto_funcs[] = {
    JS_CFUNC_DEF("addColorStop", 4, js_gradient_addColorStop),
};

static void js_gradient_finalizer(JSRuntime *rt, JSValue val)
{
    plutovg_gradient_t *gradient = JS_GetOpaque(val, js_gradient_class_id);
    JS_SetOpaque(val, NULL);
    plutovg_gradient_destroy(gradient);
}

static JSClassDef js_gradient_class = {
    "gradient",
    .finalizer = js_gradient_finalizer,
};

JSClassID get_js_gradient_class_id()
{
    return js_gradient_class_id;
}

static JSValue js_gradient_ctor(JSContext *ctx,
                                JSValueConst new_target,
                                int argc, JSValueConst *argv)
{
    /* using new_target to get the prototype is necessary when the
       class is extended. */
    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto))
        goto fail;
    JSValue obj = JS_NewObjectProtoClass(ctx, proto, js_gradient_class_id);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj))
        goto fail;

    const char *s = JS_ToCString(ctx, argv[0]);
    if (s == NULL)
        goto fail;

    plutovg_gradient_t *gradient = NULL;
    if (strcmp(s, "linear") == 0)
    {

        double x1, y1, x2, y2;
        if (JS_ToFloat64(ctx, &x1, argv[1]))
            return JS_EXCEPTION;
        if (JS_ToFloat64(ctx, &y1, argv[2]))
            return JS_EXCEPTION;
        if (JS_ToFloat64(ctx, &x2, argv[3]))
            return JS_EXCEPTION;
        if (JS_ToFloat64(ctx, &y2, argv[4]))
            return JS_EXCEPTION;
        gradient = plutovg_gradient_create_linear(x1, y1, x2, y2);
    }
    else if (strcmp(s, "radial") == 0)
    {

        double cx, cy, cr, fx, fy, fr;
        if (JS_ToFloat64(ctx, &fx, argv[1]))
            return JS_EXCEPTION;
        if (JS_ToFloat64(ctx, &fy, argv[2]))
            return JS_EXCEPTION;
        if (JS_ToFloat64(ctx, &fr, argv[3]))
            return JS_EXCEPTION;
        if (JS_ToFloat64(ctx, &cx, argv[4]))
            return JS_EXCEPTION;
        if (JS_ToFloat64(ctx, &cy, argv[5]))
            return JS_EXCEPTION;
        if (JS_ToFloat64(ctx, &cr, argv[6]))
            return JS_EXCEPTION;
        gradient = plutovg_gradient_create_radial(cx, cy, cr, fx, fy, fr);
    }
    else
    {
        JS_FreeCString(ctx, s);
        goto fail;
    }

    JS_FreeCString(ctx, s);

    JS_SetOpaque(obj, gradient);
    return obj;
fail:
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
}

int js_gradient_class_define(JSContext *ctx, JSModuleDef *m)
{
    JS_NewClassID(&js_gradient_class_id);
    JS_NewClass(JS_GetRuntime(ctx), js_gradient_class_id, &js_gradient_class);

    JSValue gradient_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, gradient_proto, js_gradient_proto_funcs, countof(js_gradient_proto_funcs));
    JSValue gradient_class = JS_NewCFunction2(ctx, js_gradient_ctor, "CanvasGradient", 1, JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, gradient_class, gradient_proto);
    JS_SetClassProto(ctx, js_gradient_class_id, gradient_proto);

    JS_SetModuleExport(ctx, m, "CanvasGradient", gradient_class);
    return 0;
}