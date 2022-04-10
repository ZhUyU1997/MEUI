#include "cutils.h"
#include "quickjs.h"
#include "quickjs-libc.h"
#include <box.h>
#include <element.h>
#include <string.h>
#include <bind/style.h>

#define countof(x) (sizeof(x) / sizeof((x)[0]))

JSClassID get_js_box_class_id();

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

static JSValue js_canvas_put_image(JSContext *ctx, JSValueConst this_val,
                                   int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    int width, height;
    if (JS_ToInt32(ctx, &width, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &height, argv[2]))
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    size_t size, ret;
    uint8_t *buf = JS_GetArrayBuffer(ctx, &size, argv[0]);
    if (!buf)
        return JS_EXCEPTION;
    if (width * height * sizeof(uint32_t) != size)
        return JS_ThrowRangeError(ctx, "width * height * sizeof(uint32_t) != size");

    plutovg_surface_t *img = plutovg_surface_create_for_data(buf, width, height, width * sizeof(uint32_t));
    plutovg_surface_t *convert = plutovg_surface_format_convert(img);
    plutovg_t *pluto = plutovg_create(e->surface);
    plutovg_rect(pluto, 0, 0, width, height);
    plutovg_set_source_surface(pluto, convert, 0, 0);
    plutovg_fill(pluto);
    plutovg_destroy(pluto);
    plutovg_surface_destroy(img);
    plutovg_surface_destroy(convert);
    return JS_UNDEFINED;
}

static JSValue js_canvas_get_width(JSContext *ctx, JSValueConst this_val,
                                   int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));
    return JS_NewInt32(ctx, canvas_get_width(e));
}

static JSValue js_canvas_set_width(JSContext *ctx, JSValueConst this_val,
                                   int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    int width;
    if (JS_ToInt32(ctx, &width, argv[0]))
        return JS_EXCEPTION;
    canvas_set_width(e, width);
    return JS_UNDEFINED;
}

static JSValue js_canvas_get_height(JSContext *ctx, JSValueConst this_val,
                                    int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));
    return JS_NewInt32(ctx, canvas_get_height(e));
}

static JSValue js_canvas_set_height(JSContext *ctx, JSValueConst this_val,
                                    int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    int height;
    if (JS_ToInt32(ctx, &height, argv[0]))
        return JS_EXCEPTION;
    canvas_set_height(e, height);
    return JS_UNDEFINED;
}

static const JSCFunctionListEntry js_canvas_funcs[] = {
    JS_CFUNC_DEF("putImage", 3, js_canvas_put_image),
    JS_CFUNC_DEF("getWidth", 0, js_canvas_get_width),
    JS_CFUNC_DEF("getHeight", 0, js_canvas_get_height),
    JS_CFUNC_DEF("setWidth", 1, js_canvas_set_width),
    JS_CFUNC_DEF("setHeight", 1, js_canvas_set_height),
};

int js_canvas_define(JSContext *ctx, JSModuleDef *m)
{
    JSValue canvas = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, canvas, js_canvas_funcs, countof(js_canvas_funcs));
    JS_SetModuleExport(ctx, m, "Canvas", canvas);
    return 0;
}