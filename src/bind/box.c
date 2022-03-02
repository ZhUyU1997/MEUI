
#include "cutils.h"
#include "quickjs.h"
#include "quickjs-libc.h"
#include <box.h>
#include <string.h>
#include <bind/style.h>

#define countof(x) (sizeof(x) / sizeof((x)[0]))

static JSClassID js_box_class_id;

JSValue js_createBoxStyleFuncWithOpaque(JSContext *ctx, box_style_t *style);

static JSValue js_get_style(JSContext *ctx, JSValueConst this_val,
                            int argc, JSValueConst *argv)
{
    box_t box = JS_GetOpaque2(ctx, this_val, js_box_class_id);
    if (!box)
        return JS_EXCEPTION;
    uint32_t v;
    if (JS_ToUint32(ctx, &v, argv[0]))
        return JS_EXCEPTION;

    box_style_t *style = box_get_style(box, v);
    // TODO:
    // if (style)
    //     return js_createBoxStyleFuncWithOpaque(ctx, box_get_style(box, v));
    // else
    return JS_NULL;
}

static void setBoxStyleFromJSValue(JSContext *ctx, box_style_t *style, JSValueConst obj)
{
    for (int i = 0; i < jsStyleGetSetLength; i++)
    {
        JSValue value = JS_GetPropertyStr(ctx, obj, jsStyleGetSet[i].name);
        if (JS_IsUndefined(value))
            continue;

        jsStyleGetSet[i].set(ctx, style, value);
        JS_FreeValue(ctx, value);
    }
}

static JSValue js_set_style(JSContext *ctx, JSValueConst this_val,
                            int argc, JSValueConst *argv)
{
    box_t box = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!box)
        return JS_EXCEPTION;

    uint32_t v;
    if (JS_ToUint32(ctx, &v, argv[1]))
        return JS_EXCEPTION;

    box_style_t *style = box_get_style(box, v);

    if (!style)
    {
        style = box_style_new();
        box_set_style(box, style, v);
    }

    setBoxStyleFromJSValue(ctx, style, argv[0]);
    return JS_UNDEFINED;
}

static JSValue js_add_child(JSContext *ctx, JSValueConst this_val,
                            int argc, JSValueConst *argv)
{
    box_t box = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!box)
        return JS_EXCEPTION;

    box_t child = JS_GetOpaque2(ctx, argv[0], js_box_class_id);
    if (!child)
        return JS_EXCEPTION;

    Flex_addChild(box, child);
    return JS_UNDEFINED;
}

static JSValue js_set_state(JSContext *ctx, JSValueConst this_val,
                            int argc, JSValueConst *argv)
{
    box_t box = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!box)
        return JS_EXCEPTION;

    int state;

    if (JS_ToInt32(ctx, &state, argv[0]))
        return JS_EXCEPTION;

    box_set_state(box, state);
    return JS_UNDEFINED;
}

static JSValue js_hit(JSContext *ctx, JSValueConst this_val,
                      int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!node)
        return JS_EXCEPTION;

    int x, y;
    if (JS_ToInt32(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &y, argv[1]))
        return JS_EXCEPTION;

    float left = Flex_getResultLeft(node);
    float top = Flex_getResultTop(node);
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);

    struct Box *box = Flex_getContext(node);

    double content_width = width - Flex_getResultPaddingLeft(node) - Flex_getResultPaddingRight(node);
    double content_height = height - Flex_getResultPaddingTop(node) - Flex_getResultPaddingBottom(node);
    double content_left = Flex_getResultPaddingLeft(node);
    double content_top = Flex_getResultPaddingTop(node);

    box_t target = NULL;

    plutovg_matrix_t m = box->result.to_screen_matrix;
    plutovg_matrix_invert(&m);

    plutovg_point_t dst;
    plutovg_matrix_map_point(&m, &(plutovg_point_t){x, y}, &dst);

    if (dst.x >= 0 && dst.x < width && dst.y >= 0 && dst.y < height)
    {
        return JS_NewBool(ctx, 1);
    }

    return JS_NewBool(ctx, 0);
}

static const JSCFunctionListEntry js_box_proto_funcs[] = {
    JS_CFUNC_DEF("getStyle", 1, js_get_style),
    JS_CFUNC_DEF("setStyle", 2, js_set_style),
    JS_CFUNC_DEF("addChild", 1, js_add_child),
    JS_CFUNC_DEF("setState", 1, js_set_state),
    JS_CFUNC_DEF("hit", 1, js_hit),
};

static void js_box_finalizer(JSRuntime *rt, JSValue val)
{
    struct box_t *box = JS_GetOpaque(val, js_box_class_id);
    printf("js_box_finalizer\n");
}

static JSClassDef js_box_class = {
    "Box",
    .finalizer = js_box_finalizer,
};

JSClassID get_js_box_class_id()
{
    return js_box_class_id;
}

static const JSCFunctionListEntry box_state_entries[] = {
    JS_PROP_INT32_DEF("DEFAULT", BOX_STATE_DEFAULT, JS_PROP_CONFIGURABLE),
    JS_PROP_INT32_DEF("FOCUS", BOX_STATE_FOCUS, JS_PROP_CONFIGURABLE),
    JS_PROP_INT32_DEF("ACTIVE", BOX_STATE_ACTIVE, JS_PROP_CONFIGURABLE),
    JS_PROP_INT32_DEF("HOVER", BOX_STATE_HOVER, JS_PROP_CONFIGURABLE),
    JS_PROP_INT32_DEF("SELECT", BOX_STATE_SELECT, JS_PROP_CONFIGURABLE),
    JS_PROP_INT32_DEF("DISABLE", BOX_STATE_DISABLE, JS_PROP_CONFIGURABLE),
    JS_PROP_INT32_DEF("CHECK", BOX_STATE_CHECK, JS_PROP_CONFIGURABLE),
};

JSValue js_createBoxFuncWithOpaque(JSContext *ctx, box_t box)
{
    JSValue obj = JS_NewObjectClass(ctx, js_box_class_id);
    if (JS_IsException(obj))
        return obj;

    JS_SetOpaque(obj, box);
    return obj;
}

static JSValue js_createBoxFunc(JSContext *ctx,
                                JSValueConst new_target,
                                int argc, JSValueConst *argv)
{
    box_t box = box_new();
    return js_createBoxFuncWithOpaque(ctx, box);
}

static const JSCFunctionListEntry entries[] = {
    JS_CFUNC_DEF("createBox", 0, js_createBoxFunc)};

int js_box_class_define(JSContext *ctx, JSModuleDef *m)
{
    JS_NewClassID(&js_box_class_id);
    JS_NewClass(JS_GetRuntime(ctx), js_box_class_id, &js_box_class);

    JSValue box_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, box_proto, js_box_proto_funcs, countof(js_box_proto_funcs));
    JS_SetClassProto(ctx, js_box_class_id, box_proto);
    JS_SetModuleExportList(ctx, m, entries, countof(entries));

    JSValue js_box_state = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, js_box_state, box_state_entries, countof(box_state_entries));
    JS_SetModuleExport(ctx, m, "BOX_STATE", js_box_state);
    return 0;
}