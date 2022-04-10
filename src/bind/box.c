#include "cutils.h"
#include "quickjs.h"
#include "quickjs-libc.h"
#include <box.h>
#include <element.h>
#include <string.h>
#include <bind/style.h>

#define countof(x) (sizeof(x) / sizeof((x)[0]))

static JSClassID js_box_class_id;

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

static JSValue js_insert_child(JSContext *ctx, JSValueConst this_val,
                               int argc, JSValueConst *argv)
{
    box_t box = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!box)
        return JS_EXCEPTION;

    box_t child = JS_GetOpaque2(ctx, argv[0], js_box_class_id);
    if (!child)
        return JS_EXCEPTION;

    int32_t index;
    if (JS_ToInt32(ctx, &index, argv[1]))
        return JS_EXCEPTION;
    Flex_insertChild(box, child, index);
    return JS_UNDEFINED;
}

static JSValue js_remove_child(JSContext *ctx, JSValueConst this_val,
                               int argc, JSValueConst *argv)
{
    box_t box = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!box)
        return JS_EXCEPTION;

    box_t child = JS_GetOpaque2(ctx, argv[0], js_box_class_id);
    if (!child)
        return JS_EXCEPTION;
    // TODO: call box_free
    Flex_removeChild(box, child);
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

static JSValue js_get_state(JSContext *ctx, JSValueConst this_val,
                            int argc, JSValueConst *argv)
{
    box_t box = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!box)
        return JS_EXCEPTION;

    return JS_NewInt32(ctx, box_get_state(box));
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

    Box *box = Flex_getContext(node);

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

static const plutovg_surface_t *plutovg_surface_format_convert(const plutovg_surface_t *surface)
{
    unsigned char *data = plutovg_surface_get_data(surface);
    int width = plutovg_surface_get_width(surface);
    int height = plutovg_surface_get_height(surface);
    int stride = plutovg_surface_get_stride(surface);
    unsigned char *image = malloc((size_t)(stride * height));

    for (int y = 0; y < height; y++)
    {
        const uint32_t *src = (uint32_t *)(data + stride * y);
        uint32_t *dst = (uint32_t *)(image + stride * y);
        for (int x = 0; x < width; x++)
        {
            uint32_t b = ((src[x] >> 16) & 0xff);
            uint32_t g = ((src[x] >> 8) & 0xff);
            uint32_t r = ((src[x] >> 0) & 0xff);
            dst[x] = 0xff000000 | (r << 16) | (g << 8) | b;
        }
    }

    return plutovg_surface_create_for_data(image, width, height, width * sizeof(uint32_t));
}

static JSValue js_get_scroll_left(JSContext *ctx, JSValueConst this_val)
{
    box_t node = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!node)
        return JS_EXCEPTION;
    return JS_NewInt32(ctx, box_get_scroll_left(node));
}

static JSValue js_set_scroll_left(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_t node = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!node)
        return JS_EXCEPTION;

    int scroll_left;
    if (JS_ToInt32(ctx, &scroll_left, val))
        return JS_EXCEPTION;
    box_set_scroll_left(node, scroll_left);
    return JS_UNDEFINED;
}

static JSValue js_get_scroll_top(JSContext *ctx, JSValueConst this_val)
{
    box_t node = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!node)
        return JS_EXCEPTION;
    return JS_NewInt32(ctx, box_get_scroll_top(node));
}

static JSValue js_set_scroll_top(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_t node = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!node)
        return JS_EXCEPTION;

    int scroll_top;
    if (JS_ToInt32(ctx, &scroll_top, val))
        return JS_EXCEPTION;
    box_set_scroll_top(node, scroll_top);
    return JS_UNDEFINED;
}

static JSValue js_get_client_width(JSContext *ctx, JSValueConst this_val)
{
    box_t node = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!node)
        return JS_EXCEPTION;
    return JS_NewInt32(ctx, box_get_client_width(node));
}

static JSValue js_get_client_height(JSContext *ctx, JSValueConst this_val)
{
    box_t node = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!node)
        return JS_EXCEPTION;
    return JS_NewInt32(ctx, box_get_client_height(node));
}

static JSValue js_get_scroll_width(JSContext *ctx, JSValueConst this_val)
{
    box_t node = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!node)
        return JS_EXCEPTION;
    return JS_NewInt32(ctx, box_get_scroll_width(node));
}

static JSValue js_get_scroll_height(JSContext *ctx, JSValueConst this_val)
{
    box_t node = JS_GetOpaque2(ctx, this_val, js_box_class_id);

    if (!node)
        return JS_EXCEPTION;
    return JS_NewInt32(ctx, box_get_scroll_height(node));
}

static const JSCFunctionListEntry js_box_proto_funcs[] = {
    JS_CFUNC_DEF("getStyle", 1, js_get_style),
    JS_CFUNC_DEF("setStyle", 2, js_set_style),
    JS_CFUNC_DEF("addChild", 1, js_add_child),
    JS_CFUNC_DEF("insertChild", 2, js_insert_child),
    JS_CFUNC_DEF("removeChild", 1, js_remove_child),
    JS_CFUNC_DEF("setState", 1, js_set_state),
    JS_CFUNC_DEF("getState", 0, js_get_state),
    JS_CFUNC_DEF("hit", 1, js_hit),

    JS_CGETSET_DEF("scrollLeft", js_get_scroll_left, js_set_scroll_left),
    JS_CGETSET_DEF("scrollTop", js_get_scroll_top, js_set_scroll_top),
    JS_CGETSET_DEF("scrollWidth", js_get_scroll_width, NULL),
    JS_CGETSET_DEF("scrollHeight", js_get_scroll_height, NULL),
    JS_CGETSET_DEF("clientWidth", js_get_client_width, NULL),
    JS_CGETSET_DEF("clientHeight", js_get_client_height, NULL),
};

static void js_box_finalizer(JSRuntime *rt, JSValue val)
{
    box_t node = JS_GetOpaque(val, js_box_class_id);
    JS_SetOpaque(val, NULL);
    box_free(node);
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

static JSValue js_box_ctor(JSContext *ctx,
                           JSValueConst new_target,
                           int argc, JSValueConst *argv)
{
    enum BOX_TYPE type = BOX_TYPE_DIV;
    if (argc != 1)
        return JS_EXCEPTION;

    /* using new_target to get the prototype is necessary when the
       class is extended. */
    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto))
        goto fail;
    JSValue obj = JS_NewObjectProtoClass(ctx, proto, js_box_class_id);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj))
        goto fail;

    const char *s = JS_ToCString(ctx, argv[0]);
    if (!s)
        goto fail;

    const static struct
    {
        const char *string_value;
        enum BOX_TYPE enum_value;
    } map[] = {
        {"div", BOX_TYPE_DIV},
        {"stack", BOX_TYPE_STACK},
        {"canvas", BOX_TYPE_CANVAS},
    };

    for (int i = 0; i < countof(map); i++)
    {
        if (!strcmp(map[i].string_value, s))
        {
            type = map[i].enum_value;
            break;
        }
    }

    JS_FreeCString(ctx, s);

    box_t box = box_new(type);

    JS_SetOpaque(obj, box);
    return obj;
fail:
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
}


static JSValue js_canvas_ctor(JSContext *ctx,
                           JSValueConst new_target,
                           int argc, JSValueConst *argv)
{
    enum BOX_TYPE type = BOX_TYPE_DIV;
    if (argc != 1)
        return JS_EXCEPTION;

    /* using new_target to get the prototype is necessary when the
       class is extended. */
    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto))
        goto fail;
    JSValue obj = JS_NewObjectProtoClass(ctx, proto, js_box_class_id);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj))
        goto fail;

    return obj;
fail:
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
}

int js_canvas_define(JSContext *ctx, JSModuleDef *m);

int js_box_class_define(JSContext *ctx, JSModuleDef *m)
{
    JS_NewClassID(&js_box_class_id);
    JS_NewClass(JS_GetRuntime(ctx), js_box_class_id, &js_box_class);

    JSValue box_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, box_proto, js_box_proto_funcs, countof(js_box_proto_funcs));
    JSValue box_class = JS_NewCFunction2(ctx, js_box_ctor, "Box", 1, JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, box_class, box_proto);
    JS_SetClassProto(ctx, js_box_class_id, box_proto);

    JSValue js_box_state = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, js_box_state, box_state_entries, countof(box_state_entries));
    JS_SetModuleExport(ctx, m, "BOX_STATE", js_box_state);
    JS_SetModuleExport(ctx, m, "Box", box_class);

    js_canvas_define(ctx, m);
    return 0;
}