#include "cutils.h"
#include "quickjs.h"
#include "quickjs-libc.h"
#include <meui.h>

#define countof(x) (sizeof(x) / sizeof((x)[0]))

static JSClassID js_meui_class_id;
extern JSClassID get_js_box_class_id();

static JSValue js_get_instance(JSContext *ctx, JSValueConst this_val,
                               int argc, JSValueConst *argv)
{
    return JS_UNDEFINED;
}
static JSValue js_register_callback(JSContext *ctx, JSValueConst this_val,
                                    int argc, JSValueConst *argv)
{
    return JS_UNDEFINED;
}

static JSValue js_render(JSContext *ctx, JSValueConst this_val,
                         int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;
    box_t box = JS_GetOpaque(argv[0], get_js_box_class_id());
    meui_render(meui, box);
    return JS_UNDEFINED;
}

static JSValue js_flush(JSContext *ctx, JSValueConst this_val,
                        int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;
    meui_flush(meui);
    return JS_UNDEFINED;
}
static JSValue js_update(JSContext *ctx, JSValueConst this_val,
                         int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;
    meui_update(meui);
    return JS_UNDEFINED;
}
static JSValue js_end(JSContext *ctx, JSValueConst this_val,
                      int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;
    printf("js_end: Not implemented");
    return JS_UNDEFINED;
}
static JSValue js_add_font_face(JSContext *ctx, JSValueConst this_val,
                                int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, argv[0]);
    if (!s)
        return JS_EXCEPTION;
    meui_add_font_face(meui, s);
    return JS_UNDEFINED;
}

static JSValue js_get_connect_number(JSContext *ctx, JSValueConst this_val,
                                     int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, argv[0]);
    if (!s)
        return JS_EXCEPTION;
    int fd = meui_get_connect_number(meui);
    return JS_NewInt32(ctx, fd);
}

static JSValue js_pending(JSContext *ctx, JSValueConst this_val,
                          int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;
    return JS_NewInt32(ctx, meui_pending(meui));
}

static JSValue js_next_event(JSContext *ctx, JSValueConst this_val,
                             int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;

    struct meui_event_t event;
    meui_next_event(meui, &event);
    if (event.type == MEUI_EVENT_NULL)
    {
        return JS_NULL;
    }
    else
    {
        JSValue obj = JS_NewObject(ctx);

        if (event.type == MEUI_EVENT_MOUSE_DOWN)
        {
            JS_SetPropertyStr(ctx, obj, "type", JS_NewString(ctx, "mousedown"));
            JS_SetPropertyStr(ctx, obj, "x", JS_NewInt32(ctx, event.MOUSE_DOWN.x));
            JS_SetPropertyStr(ctx, obj, "y", JS_NewInt32(ctx, event.MOUSE_DOWN.y));
        }
        else if (event.type == MEUI_EVENT_MOUSE_UP)
        {
            JS_SetPropertyStr(ctx, obj, "type", JS_NewString(ctx, "mouseup"));
            JS_SetPropertyStr(ctx, obj, "x", JS_NewInt32(ctx, event.MOUSE_UP.x));
            JS_SetPropertyStr(ctx, obj, "y", JS_NewInt32(ctx, event.MOUSE_UP.y));
        }
        else if (event.type == MEUI_EVENT_MOUSE_MOVE)
        {
            JS_SetPropertyStr(ctx, obj, "type", JS_NewString(ctx, "mousemove"));
            JS_SetPropertyStr(ctx, obj, "x", JS_NewInt32(ctx, event.MOUSE_MOVE.x));
            JS_SetPropertyStr(ctx, obj, "y", JS_NewInt32(ctx, event.MOUSE_MOVE.y));
        }
        return obj;
    }

    return JS_EXCEPTION;
}

struct js_cb_data
{
    JSContext *ctx;
    JSValue *pfunc;
};

JSValue js_createBoxFuncWithOpaque(JSContext *ctx, box_t box);

static void js_search_node_cb(box_t node, bool hit, void *data)
{
    struct js_cb_data *cb_data = data;
    JSContext *ctx = cb_data->ctx;
    JSValue func = JS_DupValue(ctx, *(cb_data->pfunc));

    JSValue argv[2] = {
        JS_NewBool(ctx, hit),
        js_createBoxFuncWithOpaque(ctx, node),
    };

    JS_Call(ctx, func, JS_UNDEFINED, 2, argv);
    JS_FreeValue(ctx, argv[0]);
    JS_FreeValue(ctx, argv[1]);
    JS_FreeValue(ctx, func);
}

static JSValue js_search_node(JSContext *ctx, JSValueConst this_val,
                              int argc, JSValueConst *argv)
{
    struct meui_t *meui = JS_GetOpaque(this_val, js_meui_class_id);
    if (!meui)
        return JS_EXCEPTION;
    if (argc != 3)
        return JS_EXCEPTION;

    int x, y;
    if (JS_ToInt32(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &y, argv[1]))
        return JS_EXCEPTION;
    if (!JS_IsFunction(ctx, argv[2]))
        return JS_EXCEPTION;

    plutovg_point_t point = {x, y};
    struct js_cb_data data = {
        .ctx = ctx,
        .pfunc = &argv[2],
    };

    box_t node = meui_search_node(meui, meui_get_root_node(meui), &data, &point, js_search_node_cb);

    if (node == NULL)
        return JS_NULL;
    else
        return js_createBoxFuncWithOpaque(ctx, node);
}

static void js_meui_finalizer(JSRuntime *rt, JSValue val)
{
    printf("js_meui_finalizer is called!\n");

    struct meui_t *meui = JS_GetOpaque(val, js_meui_class_id);
    /* Note: 's' can be NULL in case JS_SetOpaque() was not called */
    meui_end(meui);
}

static JSClassDef js_meui_class = {
    "NativeMEUI",
    .finalizer = js_meui_finalizer,
};

static JSValue js_meui_ctor(JSContext *ctx,
                            JSValueConst new_target,
                            int argc, JSValueConst *argv)
{
    int32_t width, height;

    JSValue obj = JS_UNDEFINED;
    JSValue proto;

    if (JS_ToInt32(ctx, &width, argv[0]))
        goto fail;
    if (JS_ToInt32(ctx, &height, argv[1]))
        goto fail;
    /* using new_target to get the prototype is necessary when the
       class is extended. */
    proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto))
        goto fail;
    obj = JS_NewObjectProtoClass(ctx, proto, js_meui_class_id);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj))
        goto fail;

    struct meui_t *meui = meui_start(width, height);

    JS_SetOpaque(obj, meui);
    return obj;
fail:
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
}

static const JSCFunctionListEntry js_meui_proto_funcs[] = {
    JS_CFUNC_DEF("registerCallback", 2, js_register_callback),
    JS_CFUNC_DEF("render", 1, js_render),
    JS_CFUNC_DEF("flush", 0, js_flush),
    JS_CFUNC_DEF("update", 0, js_update),
    JS_CFUNC_DEF("addFontFace", 1, js_add_font_face),
    JS_CFUNC_DEF("getConnectNumber", 0, js_get_connect_number),
    JS_CFUNC_DEF("pending", 0, js_pending),
    JS_CFUNC_DEF("nextEvent", 0, js_next_event),
    JS_CFUNC_DEF("searchNode", 3, js_search_node),
};

static int js_meui_class_define(JSContext *ctx, JSModuleDef *m)
{
    JSValue meui_proto, meui_class;
    JS_NewClassID(&js_meui_class_id);
    JS_NewClass(JS_GetRuntime(ctx), js_meui_class_id, &js_meui_class);

    meui_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, meui_proto, js_meui_proto_funcs, countof(js_meui_proto_funcs));
    meui_class = JS_NewCFunction2(ctx, js_meui_ctor, "NativeMEUI", 2, JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, meui_class, meui_proto);
    JS_SetClassProto(ctx, js_meui_class_id, meui_proto);
    JS_SetModuleExport(ctx, m, "NativeMEUI", meui_class);

    return 0;
}

extern int js_box_class_define(JSContext *ctx, JSModuleDef *m);
extern int js_box_style_class_define(JSContext *ctx, JSModuleDef *m);
static int js_meui_module_init(JSContext *ctx, JSModuleDef *m)
{
    js_meui_class_define(ctx, m);
    js_box_class_define(ctx, m);
    js_box_style_class_define(ctx, m);
    return 0;
}

JSModuleDef *js_init_module_meui(JSContext *ctx, const char *module_name)
{
    JSModuleDef *m;
    m = JS_NewCModule(ctx, module_name, js_meui_module_init);
    if (!m)
        return NULL;
    JS_AddModuleExport(ctx, m, "NativeMEUI");
    JS_AddModuleExport(ctx, m, "Box");
    JS_AddModuleExport(ctx, m, "createBoxStyle");
    JS_AddModuleExport(ctx, m, "createBox");
    JS_AddModuleExport(ctx, m, "BOX_STATE");
    return m;
}