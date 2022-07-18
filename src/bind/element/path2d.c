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

static JSClassID js_path2d_class_id;

static JSValue js_path2d_arc(JSContext *ctx, JSValueConst this_val,
                             int argc, JSValueConst *argv)
{
    plutovg_path_t *path = JS_GetOpaque2(ctx, this_val, js_path2d_class_id);
    assert(path);

    double x, y, radius, startAngle, endAngle;
    int counterclockwise = 0;
    if (JS_ToFloat64(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &radius, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &startAngle, argv[3]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &endAngle, argv[4]))
        return JS_EXCEPTION;

    if (argc == 6 && !JS_IsUndefined(argv[5]))
    {
        if (JS_ToInt32(ctx, &counterclockwise, argv[5]))
            return JS_EXCEPTION;
    }

    plutovg_path_add_arc(path, x, y, radius, startAngle, endAngle, counterclockwise);
    return JS_UNDEFINED;
}
static JSValue js_path2d_arcTo(JSContext *ctx, JSValueConst this_val,
                               int argc, JSValueConst *argv)
{
    plutovg_path_t *path = JS_GetOpaque2(ctx, this_val, js_path2d_class_id);
    assert(path);

    double x1, y1, x2, y2, raduis;
    if (JS_ToFloat64(ctx, &x1, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y1, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &x2, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y2, argv[3]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &raduis, argv[4]))
        return JS_EXCEPTION;
    plutovg_path_arc_to(path, x1, y1, x2, y2, raduis);
    return JS_UNDEFINED;
}
static JSValue js_path2d_bezierCurveTo(JSContext *ctx, JSValueConst this_val,
                                       int argc, JSValueConst *argv)
{
    plutovg_path_t *path = JS_GetOpaque2(ctx, this_val, js_path2d_class_id);
    assert(path);
    double cp1x, cp1y, cp2x, cp2y, x, y;
    if (JS_ToFloat64(ctx, &cp1x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &cp1y, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &cp2x, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &cp2y, argv[3]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &x, argv[4]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[5]))
        return JS_EXCEPTION;

    plutovg_path_cubic_to(path, cp1x, cp1y, cp2x, cp2y, x, y);
    return JS_UNDEFINED;
}
static JSValue js_path2d_closePath(JSContext *ctx, JSValueConst this_val,
                                   int argc, JSValueConst *argv)
{
    plutovg_path_t *path = JS_GetOpaque2(ctx, this_val, js_path2d_class_id);
    assert(path);
    plutovg_path_close(path);
    return JS_UNDEFINED;
}
static JSValue js_path2d_ellipse(JSContext *ctx, JSValueConst this_val,
                                 int argc, JSValueConst *argv)
{
    plutovg_path_t *path = JS_GetOpaque2(ctx, this_val, js_path2d_class_id);
    assert(path);

    double x, y, radiusX, radiusY, rotation, startAngle, endAngle;
    int counterclockwise = 0;
    if (JS_ToFloat64(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &radiusX, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &radiusY, argv[3]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &rotation, argv[4]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &startAngle, argv[5]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &endAngle, argv[6]))
        return JS_EXCEPTION;

    if (argc == 8 && !JS_IsUndefined(argv[7]))
    {
        if (JS_ToInt32(ctx, &counterclockwise, argv[7]))
            return JS_EXCEPTION;
    }

    // https://github.com/nilzona/path2d-polyfill/blob/main/src/path2d-polyfill.js
    plutovg_matrix_t matrix;
    plutovg_matrix_init_identity(&matrix);
    plutovg_matrix_translate(&matrix, x, y);
    plutovg_matrix_rotate(&matrix, rotation);
    plutovg_matrix_scale(&matrix, radiusX, radiusY);

    plutovg_path_t *epath = plutovg_path_create();
    plutovg_path_add_arc(epath, 0, 0, 1, startAngle, endAngle, counterclockwise);
    plutovg_path_add_path(path, epath, &matrix);
    plutovg_path_destroy(epath);
    return JS_UNDEFINED;
}
static JSValue js_path2d_lineTo(JSContext *ctx, JSValueConst this_val,
                                int argc, JSValueConst *argv)
{
    plutovg_path_t *path = JS_GetOpaque2(ctx, this_val, js_path2d_class_id);
    assert(path);

    double x, y;
    if (JS_ToFloat64(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[1]))
        return JS_EXCEPTION;

    plutovg_path_line_to(path, x, y);
    return JS_UNDEFINED;
}
static JSValue js_path2d_moveTo(JSContext *ctx, JSValueConst this_val,
                                int argc, JSValueConst *argv)
{
    plutovg_path_t *path = JS_GetOpaque2(ctx, this_val, js_path2d_class_id);
    assert(path);
    double x, y;
    if (JS_ToFloat64(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[1]))
        return JS_EXCEPTION;

    plutovg_path_move_to(path, x, y);
    return JS_UNDEFINED;
}
static JSValue js_path2d_quadraticCurveTo(JSContext *ctx, JSValueConst this_val,
                                          int argc, JSValueConst *argv)
{
    plutovg_path_t *path = JS_GetOpaque2(ctx, this_val, js_path2d_class_id);
    assert(path);

    double cpx, cpy, x, y;
    if (JS_ToFloat64(ctx, &cpx, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &cpy, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &x, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[3]))
        return JS_EXCEPTION;

    plutovg_path_quad_to(path, cpx, cpy, x, y);
    return JS_UNDEFINED;
}
static JSValue js_path2d_rect(JSContext *ctx, JSValueConst this_val,
                              int argc, JSValueConst *argv)
{
    plutovg_path_t *path = JS_GetOpaque2(ctx, this_val, js_path2d_class_id);
    assert(path);

    double x, y, w, h;
    if (JS_ToFloat64(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &w, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &h, argv[3]))
        return JS_EXCEPTION;
    plutovg_path_add_rect(path, x, y, w, h);
    return JS_UNDEFINED;
}

static JSValue js_path2d_addPath(JSContext *ctx, JSValueConst this_val,
                                 int argc, JSValueConst *argv)
{
    plutovg_path_t *path = JS_GetOpaque2(ctx, this_val, js_path2d_class_id);
    assert(path);

    plutovg_path_t *source = JS_GetOpaque2(ctx, argv[0], js_path2d_class_id);

    plutovg_matrix_t matrix;

    plutovg_matrix_init_identity(&matrix);

    if (argc == 2 && !JS_IsUndefined(argv[1]))
    {
        if (JS_IsArray(ctx, argv[1]) <= 0)
            return JS_EXCEPTION;

        double m[6];
        for (int i = 0; i < 6; i++)
        {
            JSValue v = JS_GetPropertyUint32(ctx, argv[1], i);
            if (JS_ToFloat64(ctx, &m[i], v))
                return JS_EXCEPTION;
            JS_FreeValue(ctx, v);
        }
        plutovg_matrix_init(&matrix, m[0], m[1], m[2], m[3], m[4], m[5]);
    }

    plutovg_path_add_path(path, source, &matrix);
    return JS_UNDEFINED;
}

static const JSCFunctionListEntry js_path2d_proto_funcs[] = {
    JS_CFUNC_DEF("arc", 6, js_path2d_arc),
    JS_CFUNC_DEF("arcTo", 5, js_path2d_arcTo),
    JS_CFUNC_DEF("bezierCurveTo", 6, js_path2d_bezierCurveTo),
    JS_CFUNC_DEF("closePath", 0, js_path2d_closePath),
    JS_CFUNC_DEF("ellipse", 8, js_path2d_ellipse),
    JS_CFUNC_DEF("lineTo", 2, js_path2d_lineTo),
    JS_CFUNC_DEF("moveTo", 2, js_path2d_moveTo),
    JS_CFUNC_DEF("quadraticCurveTo", 4, js_path2d_quadraticCurveTo),
    JS_CFUNC_DEF("rect", 4, js_path2d_rect),
    JS_CFUNC_DEF("addPath", 2, js_path2d_addPath),
};

static void js_path2d_finalizer(JSRuntime *rt, JSValue val)
{
    plutovg_path_t *path = JS_GetOpaque(val, js_path2d_class_id);
    JS_SetOpaque(val, NULL);
    plutovg_path_destroy(path);
}

static JSClassDef js_path2d_class = {
    "Path2D",
    .finalizer = js_path2d_finalizer,
};

JSClassID get_js_path2d_class_id()
{
    return js_path2d_class_id;
}

static JSValue js_path2d_ctor(JSContext *ctx,
                              JSValueConst new_target,
                              int argc, JSValueConst *argv)
{
    /* using new_target to get the prototype is necessary when the
       class is extended. */
    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto))
        goto fail;
    JSValue obj = JS_NewObjectProtoClass(ctx, proto, js_path2d_class_id);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj))
        goto fail;

    plutovg_path_t *path = NULL;
    if (argc == 1)
    {
        if (JS_IsString(argv[0]))
        {
            size_t len = 0;
            const char *s = JS_ToCStringLen(ctx, &len, argv[0]);
            if (s == NULL)
                goto fail;

            path = plutosvg_parse_path(s, len);
            JS_FreeCString(ctx, s);

            if (path == NULL)
            {
                printf("Failed to parse svg path");
            }
        }
        else
        {
            plutovg_path_t *source = JS_GetOpaque2(ctx, argv[0], js_path2d_class_id);

            if (source == NULL)
                goto fail;
            path = plutovg_path_clone(source);
        }
    }

    if (path == NULL)
    {
        path = plutovg_path_create();
    }

    JS_SetOpaque(obj, path);
    return obj;
fail:
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
}

int js_path2d_class_define(JSContext *ctx, JSModuleDef *m)
{
    JS_NewClassID(&js_path2d_class_id);
    JS_NewClass(JS_GetRuntime(ctx), js_path2d_class_id, &js_path2d_class);

    JSValue path2d_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, path2d_proto, js_path2d_proto_funcs, countof(js_path2d_proto_funcs));
    JSValue path2d_class = JS_NewCFunction2(ctx, js_path2d_ctor, "Path2D", 1, JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, path2d_class, path2d_proto);
    JS_SetClassProto(ctx, js_path2d_class_id, path2d_proto);

    JS_SetModuleExport(ctx, m, "Path2D", path2d_class);
    return 0;
}