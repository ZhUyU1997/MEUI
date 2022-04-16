#include "cutils.h"
#include "quickjs.h"
#include "quickjs-libc.h"
#include <box.h>
#include <element.h>
#include <string.h>
#include <bind/style.h>

#define countof(x) (sizeof(x) / sizeof((x)[0]))

JSClassID get_js_box_class_id();
JSClassID get_js_path2d_class_id();

enum COLOR_FORMAT
{
    COLOR_RGBA,
    COLOR_RGB,
    COLOR_BGRA,
    COLOR_BGR,
};

static const plutovg_surface_t *plutovg_surface_format_convert(const plutovg_surface_t *surface, enum COLOR_FORMAT format)
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

static JSValue js_canvas_getImage(JSContext *ctx, JSValueConst this_val,
                                  int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    int sx, sy, sw, sh;
    if (JS_ToInt32(ctx, &sx, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &sy, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &sw, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &sh, argv[3]))
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    plutovg_surface_t *img = plutovg_surface_create(sw, sh);
    plutovg_t *pluto = plutovg_create(img);
    plutovg_rect(pluto, 0, 0, sw, sh);
    plutovg_set_source_surface(pluto, e->surface, -sx, -sy);
    plutovg_fill(pluto);
    plutovg_destroy(pluto);

    JSValue buffer = JS_NewArrayBufferCopy(ctx, plutovg_surface_get_data(img),
                                           plutovg_surface_get_stride(img) * plutovg_surface_get_height(img));
    plutovg_surface_destroy(img);

    return buffer;
}

static JSValue js_canvas_putImage(JSContext *ctx, JSValueConst this_val,
                                  int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    int dx, dy;
    if (JS_ToInt32(ctx, &dx, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &dy, argv[2]))
        return JS_EXCEPTION;

    int dirtyX, dirtyY;
    if (JS_ToInt32(ctx, &dirtyX, argv[3]))
        return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &dirtyY, argv[4]))
        return JS_EXCEPTION;

    int dirtyWidth, dirtyHeight;
    if (JS_ToInt32(ctx, &dirtyWidth, argv[5]))
        return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &dirtyHeight, argv[6]))
        return JS_EXCEPTION;

    int width, height;
    if (JS_ToInt32(ctx, &width, argv[7]))
        return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &height, argv[8]))
        return JS_EXCEPTION;

    int format = COLOR_RGBA;

    if (argc == 10)
    {
        if (JS_ToInt32(ctx, &format, argv[9]))
            return JS_EXCEPTION;
    }

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    size_t size, ret;
    uint8_t *buf = JS_GetArrayBuffer(ctx, &size, argv[0]);
    if (!buf)
        return JS_EXCEPTION;
    if (width * height * sizeof(uint32_t) != size)
        return JS_ThrowRangeError(ctx, "width * height * sizeof(uint32_t) != size");

    plutovg_surface_t *img = plutovg_surface_create_for_data(buf, width, height, width * sizeof(uint32_t));

    if (format != COLOR_RGBA)
    {
        plutovg_surface_t *convert = plutovg_surface_format_convert(img, format);
        plutovg_surface_destroy(img);
        img = convert;
    }

    plutovg_t *pluto = plutovg_create(e->surface);
    plutovg_rect(pluto, dx, dy, dirtyWidth, dirtyHeight);
    plutovg_set_operator(pluto, plutovg_operator_src);
    plutovg_set_source_surface(pluto, img, dx - dirtyX, dy - dirtyY);
    plutovg_fill(pluto);
    plutovg_destroy(pluto);
    plutovg_surface_destroy(img);
    return JS_UNDEFINED;
}

static JSValue js_canvas_getWidth(JSContext *ctx, JSValueConst this_val,
                                  int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));
    return JS_NewInt32(ctx, canvas_get_width(e));
}

static JSValue js_canvas_setWidth(JSContext *ctx, JSValueConst this_val,
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

static JSValue js_canvas_getHeight(JSContext *ctx, JSValueConst this_val,
                                   int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));
    return JS_NewInt32(ctx, canvas_get_height(e));
}

static JSValue js_canvas_setHeight(JSContext *ctx, JSValueConst this_val,
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

static JSValue js_canvas_beignPath(JSContext *ctx, JSValueConst this_val,
                                   int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    plutovg_path_clear(e->path);
    return JS_UNDEFINED;
}

static JSValue js_canvas_closePath(JSContext *ctx, JSValueConst this_val,
                                   int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    plutovg_path_close(e->path);
    return JS_UNDEFINED;
}

static JSValue js_canvas_rotate(JSContext *ctx, JSValueConst this_val,
                                int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double radians;
    if (JS_ToFloat64(ctx, &radians, argv[0]))
        return JS_EXCEPTION;

    plutovg_rotate(e->pluto, radians);
    return JS_UNDEFINED;
}

static JSValue js_canvas_scale(JSContext *ctx, JSValueConst this_val,
                               int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double x, y;
    if (JS_ToFloat64(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[1]))
        return JS_EXCEPTION;
    plutovg_scale(e->pluto, x, y);
    return JS_UNDEFINED;
}

static JSValue js_canvas_translate(JSContext *ctx, JSValueConst this_val,
                                   int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double x, y;
    if (JS_ToFloat64(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[1]))
        return JS_EXCEPTION;

    plutovg_translate(e->pluto, x, y);
    return JS_UNDEFINED;
}

static JSValue js_canvas_setTransform(JSContext *ctx, JSValueConst this_val,
                                      int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double m[6];

    if (JS_IsArray(ctx, argv[0]))
    {
        for (int i = 0; i < 6; i++)
        {
            JSValue v = JS_GetPropertyUint32(ctx, argv[0], i);
            if (JS_ToFloat64(ctx, &m[i], v))
                return JS_EXCEPTION;
            JS_FreeValue(ctx, v);
        }
    }
    else
    {
        for (int i = 0; i < 6; i++)
        {
            if (JS_ToFloat64(ctx, &m[i], argv[i]))
                return JS_EXCEPTION;
        }
    }

    plutovg_matrix_t matrix;
    plutovg_matrix_init(&matrix, m[0], m[1], m[2], m[3], m[4], m[5]);
    plutovg_set_matrix(e->pluto, &matrix);
    return JS_UNDEFINED;
}

static JSValue js_canvas_transform(JSContext *ctx, JSValueConst this_val,
                                   int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double m[6];
    for (int i = 0; i < 6; i++)
    {
        if (JS_ToFloat64(ctx, &m[i], argv[i]))
            return JS_EXCEPTION;
    }

    plutovg_matrix_t matrix;
    plutovg_matrix_init(&matrix, m[0], m[1], m[2], m[3], m[4], m[5]);
    plutovg_transform(e->pluto, &matrix);
    return JS_UNDEFINED;
}

static JSValue js_canvas_resetTransform(JSContext *ctx, JSValueConst this_val,
                                        int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    plutovg_matrix_t matrix;
    plutovg_matrix_init_identity(&matrix);
    plutovg_transform(e->pluto, &matrix);
    return JS_UNDEFINED;
}

static JSValue js_canvas_getTransform(JSContext *ctx, JSValueConst this_val,
                                      int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    plutovg_matrix_t matrix;
    plutovg_get_matrix(e->pluto, &matrix);

    JSValue arr = JS_NewArray(ctx);
    JS_SetPropertyUint32(ctx, arr, 0, JS_NewFloat64(ctx, matrix.m00));
    JS_SetPropertyUint32(ctx, arr, 1, JS_NewFloat64(ctx, matrix.m10));
    JS_SetPropertyUint32(ctx, arr, 2, JS_NewFloat64(ctx, matrix.m01));
    JS_SetPropertyUint32(ctx, arr, 3, JS_NewFloat64(ctx, matrix.m11));
    JS_SetPropertyUint32(ctx, arr, 4, JS_NewFloat64(ctx, matrix.m02));
    JS_SetPropertyUint32(ctx, arr, 5, JS_NewFloat64(ctx, matrix.m12));

    return arr;
}

static JSValue js_canvas_moveTo(JSContext *ctx, JSValueConst this_val,
                                int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double x, y;
    if (JS_ToFloat64(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[1]))
        return JS_EXCEPTION;

    plutovg_path_move_to(e->path, x, y);
    return JS_UNDEFINED;
}

static JSValue js_canvas_ellipse(JSContext *ctx, JSValueConst this_val,
                                 int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double x, y, radiusX, radiusY, rotation, startAngle, endAngle;
    int counterclockwise = 1;
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
    if (argc == 8 && JS_ToInt32(ctx, &counterclockwise, argv[7]))
        return JS_EXCEPTION;

    printf("%f %f %f %f %f %f %f\n", x, y, radiusX, radiusY, rotation, startAngle, endAngle);
    // https://github.com/nilzona/path2d-polyfill/blob/main/src/path2d-polyfill.js
    plutovg_matrix_t matrix;
    plutovg_matrix_init_identity(&matrix);
    plutovg_matrix_translate(&matrix, x, y);
    plutovg_matrix_rotate(&matrix, rotation);
    plutovg_matrix_scale(&matrix, radiusX, radiusY);

    plutovg_path_t *path = plutovg_path_create();
    plutovg_path_add_arc(path, 0, 0, 1, startAngle, endAngle, counterclockwise);
    plutovg_path_add_path(e->path, path, &matrix);
    plutovg_path_destroy(path);
    return JS_UNDEFINED;
}

static JSValue js_canvas_arc(JSContext *ctx, JSValueConst this_val,
                             int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double x, y, radius, startAngle, endAngle;
    int counterclockwise = 1;
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
    if (argc == 6 && JS_ToInt32(ctx, &counterclockwise, argv[5]))
        return JS_EXCEPTION;
    plutovg_path_add_arc(e->path, x, y, radius, startAngle, endAngle, counterclockwise);
    return JS_UNDEFINED;
}

static JSValue js_canvas_arcTo(JSContext *ctx, JSValueConst this_val,
                               int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

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
    plutovg_path_arc_to(e->path, x1, y1, x2, y2, raduis);
    return JS_UNDEFINED;
}

static JSValue js_canvas_lineTo(JSContext *ctx, JSValueConst this_val,
                                int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double x, y;
    if (JS_ToFloat64(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[1]))
        return JS_EXCEPTION;

    plutovg_path_line_to(e->path, x, y);
    return JS_UNDEFINED;
}

static JSValue js_canvas_bezierCurveTo(JSContext *ctx, JSValueConst this_val,
                                       int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

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

    plutovg_path_cubic_to(e->path, cp1x, cp1y, cp2x, cp2y, x, y);
    return JS_UNDEFINED;
}

static JSValue js_canvas_quadraticCurveTo(JSContext *ctx, JSValueConst this_val,
                                          int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double cpx, cpy, x, y;
    if (JS_ToFloat64(ctx, &cpx, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &cpy, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &x, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[3]))
        return JS_EXCEPTION;

    plutovg_path_quad_to(e->path, cpx, cpy, x, y);
    return JS_UNDEFINED;
}

static JSValue js_canvas_rect(JSContext *ctx, JSValueConst this_val,
                              int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double x, y, w, h;
    if (JS_ToFloat64(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &w, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &h, argv[3]))
        return JS_EXCEPTION;
    plutovg_path_add_rect(e->path, x, y, w, h);
    return JS_UNDEFINED;
}

static JSValue js_canvas_stroke(JSContext *ctx, JSValueConst this_val,
                                int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    if (argc == 1)
    {
        plutovg_path_t *path = JS_GetOpaque2(ctx, argv[0], get_js_path2d_class_id());

        if (!path)
            return JS_EXCEPTION;

        plutovg_add_path(e->pluto, path);
        plutovg_stroke(e->pluto);
        plutovg_path_destroy(path);
    }
    else
    {
        plutovg_add_path(e->pluto, e->path);
        plutovg_stroke(e->pluto);
    }

    return JS_UNDEFINED;
}

static plutovg_fill_rule_t get_fill_rule(const char *s)
{
    plutovg_fill_rule_t rule = plutovg_fill_rule_non_zero;

    const static struct
    {
        const char *string_value;
        plutovg_fill_rule_t enum_value;
    } map[] = {
        {"nonzero", plutovg_fill_rule_non_zero},
        {"evenodd", plutovg_fill_rule_even_odd},
    };

    for (int i = 0; i < countof(map); i++)
    {
        if (!strcmp(map[i].string_value, s))
        {
            return map[i].enum_value;
        }
    }

    return rule;
}

static JSValue js_canvas_fill(JSContext *ctx, JSValueConst this_val,
                              int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    if (argc == 0)
    {
        plutovg_add_path(e->pluto, e->path);
        plutovg_fill(e->pluto);
    }
    else if (argc == 1)
    {
        if (JS_IsString(argv[0]))
        {
            const char *s = JS_ToCString(ctx, argv[0]);
            if (s == NULL)
                return JS_EXCEPTION;

            plutovg_fill_rule_t rule = get_fill_rule(s);
            JS_FreeCString(ctx, s);

            plutovg_save(e->pluto);

            plutovg_set_fill_rule(e->pluto, rule);
            plutovg_add_path(e->pluto, e->path);
            plutovg_fill(e->pluto);

            plutovg_restore(e->pluto);
        }
        else
        {
            plutovg_path_t *path = JS_GetOpaque2(ctx, argv[0], get_js_path2d_class_id());

            if (!path)
                return JS_EXCEPTION;

            plutovg_add_path(e->pluto, path);
            plutovg_fill(e->pluto);
            plutovg_path_destroy(path);
        }
    }
    else if (argc == 2)
    {
        plutovg_path_t *path = JS_GetOpaque2(ctx, argv[0], get_js_path2d_class_id());

        if (!path)
            return JS_EXCEPTION;

        const char *s = JS_ToCString(ctx, argv[0]);
        if (s == NULL)
            return JS_EXCEPTION;

        plutovg_fill_rule_t rule = get_fill_rule(s);
        JS_FreeCString(ctx, s);

        plutovg_save(e->pluto);

        plutovg_set_fill_rule(e->pluto, rule);
        plutovg_add_path(e->pluto, path);
        plutovg_fill(e->pluto);

        plutovg_restore(e->pluto);

        plutovg_path_destroy(path);
    }
    else
    {
        return JS_EXCEPTION;
    }

    return JS_UNDEFINED;
}

static JSValue js_canvas_clip(JSContext *ctx, JSValueConst this_val,
                              int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    if (argc == 0)
    {
        plutovg_add_path(e->pluto, e->path);
        plutovg_clip(e->pluto);
    }
    else if (argc == 1)
    {
        if (JS_IsString(argv[0]))
        {
            const char *s = JS_ToCString(ctx, argv[0]);
            if (s == NULL)
                return JS_EXCEPTION;

            plutovg_fill_rule_t rule = get_fill_rule(s);
            JS_FreeCString(ctx, s);

            plutovg_save(e->pluto);

            plutovg_set_fill_rule(e->pluto, rule);
            plutovg_add_path(e->pluto, e->path);
            plutovg_clip(e->pluto);

            plutovg_restore(e->pluto);
        }
        else
        {
            plutovg_path_t *path = JS_GetOpaque2(ctx, argv[0], get_js_path2d_class_id());

            if (!path)
                return JS_EXCEPTION;

            plutovg_add_path(e->pluto, path);
            plutovg_clip(e->pluto);
            plutovg_path_destroy(path);
        }
    }
    else if (argc == 2)
    {
        plutovg_path_t *path = JS_GetOpaque2(ctx, argv[0], get_js_path2d_class_id());

        if (!path)
            return JS_EXCEPTION;

        const char *s = JS_ToCString(ctx, argv[0]);
        if (s == NULL)
            return JS_EXCEPTION;

        plutovg_fill_rule_t rule = get_fill_rule(s);
        JS_FreeCString(ctx, s);

        plutovg_save(e->pluto);

        plutovg_set_fill_rule(e->pluto, rule);
        plutovg_add_path(e->pluto, path);
        plutovg_clip(e->pluto);

        plutovg_restore(e->pluto);

        plutovg_path_destroy(path);
    }
    else
    {
        return JS_EXCEPTION;
    }

    return JS_UNDEFINED;
}

static JSValue js_canvas_strokeRect(JSContext *ctx, JSValueConst this_val,
                                    int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double x, y, w, h;
    if (JS_ToFloat64(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &w, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &h, argv[3]))
        return JS_EXCEPTION;

    plutovg_save(e->pluto);

    plutovg_path_t *path = plutovg_path_create();
    plutovg_path_add_rect(path, x, y, w, h);
    plutovg_add_path(e->pluto, path);

    plutovg_stroke(e->pluto);

    plutovg_path_destroy(path);

    plutovg_restore(e->pluto);
    return JS_UNDEFINED;
}

static JSValue js_canvas_fillRect(JSContext *ctx, JSValueConst this_val,
                                  int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double x, y, w, h;
    if (JS_ToFloat64(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &w, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &h, argv[3]))
        return JS_EXCEPTION;

    plutovg_save(e->pluto);

    plutovg_path_t *path = plutovg_path_create();
    plutovg_path_add_rect(path, x, y, w, h);
    plutovg_add_path(e->pluto, path);

    plutovg_fill(e->pluto);

    plutovg_path_destroy(path);

    plutovg_restore(e->pluto);
    return JS_UNDEFINED;
}

static JSValue js_canvas_clearRect(JSContext *ctx, JSValueConst this_val,
                                   int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double x, y, w, h;
    if (JS_ToFloat64(ctx, &x, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &y, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &w, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &h, argv[3]))
        return JS_EXCEPTION;

    plutovg_save(e->pluto);

    plutovg_path_t *path = plutovg_path_create();
    plutovg_path_add_rect(path, x, y, w, h);
    plutovg_add_path(e->pluto, path);

    plutovg_set_source_rgba(e->pluto, 0, 0, 0, 0);
    plutovg_set_operator(e->pluto, plutovg_operator_dst_in);
    plutovg_fill(e->pluto);

    plutovg_path_destroy(path);

    plutovg_restore(e->pluto);
    return JS_UNDEFINED;
}

static JSValue js_canvas_setStrokeStyle(JSContext *ctx, JSValueConst this_val,
                                        int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double r, g, b, a;
    if (JS_ToFloat64(ctx, &r, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &g, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &b, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &a, argv[3]))
        return JS_EXCEPTION;

    plutovg_set_source_rgba(e->pluto, r, g, b, a);
    return JS_UNDEFINED;
}

static JSValue js_canvas_setFillStyle(JSContext *ctx, JSValueConst this_val,
                                      int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double r, g, b, a;
    if (JS_ToFloat64(ctx, &r, argv[0]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &g, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &b, argv[2]))
        return JS_EXCEPTION;
    if (JS_ToFloat64(ctx, &a, argv[3]))
        return JS_EXCEPTION;

    plutovg_set_source_rgba(e->pluto, r, g, b, a);
    return JS_UNDEFINED;
}

static JSValue js_canvas_setLineCap(JSContext *ctx, JSValueConst this_val,
                                    int argc, JSValueConst *argv)
{
    JSValue ret = JS_EXCEPTION;

    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    const char *s = JS_ToCString(ctx, argv[0]);
    if (s == NULL)
        return JS_EXCEPTION;

    const static struct
    {
        const char *string_value;
        plutovg_line_cap_t enum_value;
    } map[] = {
        {"butt", plutovg_line_cap_butt},
        {"round", plutovg_line_cap_round},
        {"square", plutovg_line_cap_square},
    };

    for (int i = 0; i < countof(map); i++)
    {
        if (!strcmp(map[i].string_value, s))
        {
            plutovg_set_line_cap(e->pluto, map[i].enum_value);
            ret = JS_UNDEFINED;
            break;
        }
    }

    JS_FreeCString(ctx, s);
    return ret;
}

static JSValue js_canvas_setLineJoin(JSContext *ctx, JSValueConst this_val,
                                     int argc, JSValueConst *argv)
{
    JSValue ret = JS_EXCEPTION;

    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    const char *s = JS_ToCString(ctx, argv[0]);
    if (s == NULL)
        return JS_EXCEPTION;

    const static struct
    {
        const char *string_value;
        plutovg_line_join_t enum_value;
    } map[] = {
        {"miter", plutovg_line_join_miter},
        {"round", plutovg_line_join_round},
        {"bevel", plutovg_line_join_bevel},
    };

    for (int i = 0; i < countof(map); i++)
    {
        if (!strcmp(map[i].string_value, s))
        {
            plutovg_set_line_join(e->pluto, map[i].enum_value);
            ret = JS_UNDEFINED;
            break;
        }
    }

    JS_FreeCString(ctx, s);
    return ret;
}

static JSValue js_canvas_setMiterLimit(JSContext *ctx, JSValueConst this_val,
                                       int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double limter;
    if (JS_ToFloat64(ctx, &limter, argv[0]))
        return JS_EXCEPTION;

    plutovg_set_miter_limit(e->pluto, limter);
    return JS_UNDEFINED;
}

static JSValue js_canvas_setLineWidth(JSContext *ctx, JSValueConst this_val,
                                      int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double line_width;
    if (JS_ToFloat64(ctx, &line_width, argv[0]))
        return JS_EXCEPTION;

    plutovg_set_line_width(e->pluto, line_width);
    return JS_UNDEFINED;
}

static JSValue js_canvas_setLineDash(JSContext *ctx, JSValueConst this_val,
                                     int argc, JSValueConst *argv)
{
    box_t node = JS_GetOpaque2(ctx, this_val, get_js_box_class_id());

    if (!node)
        return JS_EXCEPTION;

    CanvasEle *e = dynamic_cast(CanvasEle)(Flex_getContext(node));

    double offset;
    if (JS_ToFloat64(ctx, &offset, argv[0]))
        return JS_EXCEPTION;

    if (JS_IsArray(ctx, argv[0]))
        return JS_EXCEPTION;

    uint32_t length;
    JSValue val = JS_GetPropertyStr(ctx, argv[1], "length");

    JS_ToUint32(ctx, &length, val);
    JS_FreeValue(ctx, val);

    double *segments = length > 0 ? malloc(length * sizeof(double)) : NULL;
    for (int i = 0; i < length; i++)
    {
        JSValue val = JS_GetPropertyUint32(ctx, argv[1], i);
        if (JS_IsException(val))
            return JS_EXCEPTION;
        JS_ToFloat64(ctx, &segments[i], val);
    }

    plutovg_set_dash(e->pluto, offset, segments, length);

    free(segments);
    return JS_UNDEFINED;
}

static const JSCFunctionListEntry js_canvas_funcs[] = {
    JS_CFUNC_DEF("getImage", 4, js_canvas_getImage),
    JS_CFUNC_DEF("putImage", 9, js_canvas_putImage),
    JS_CFUNC_DEF("getWidth", 0, js_canvas_getWidth),
    JS_CFUNC_DEF("getHeight", 0, js_canvas_getHeight),
    JS_CFUNC_DEF("setWidth", 1, js_canvas_setWidth),
    JS_CFUNC_DEF("setHeight", 1, js_canvas_setHeight),
    JS_CFUNC_DEF("beginPath", 0, js_canvas_beignPath),
    JS_CFUNC_DEF("closePath", 0, js_canvas_closePath),

    JS_CFUNC_DEF("rotate", 1, js_canvas_rotate),
    JS_CFUNC_DEF("scale", 2, js_canvas_scale),
    JS_CFUNC_DEF("translate", 2, js_canvas_translate),

    JS_CFUNC_DEF("setTransform", 6, js_canvas_setTransform),
    JS_CFUNC_DEF("transform", 6, js_canvas_transform),
    JS_CFUNC_DEF("resetTransform", 0, js_canvas_resetTransform),
    JS_CFUNC_DEF("getTransform", 0, js_canvas_getTransform),

    JS_CFUNC_DEF("ellipse", 8, js_canvas_ellipse),
    JS_CFUNC_DEF("arc", 6, js_canvas_arc),
    JS_CFUNC_DEF("arcTo", 5, js_canvas_arcTo),

    JS_CFUNC_DEF("moveTo", 2, js_canvas_moveTo),
    JS_CFUNC_DEF("lineTo", 2, js_canvas_lineTo),
    JS_CFUNC_DEF("bezierCurveTo", 6, js_canvas_bezierCurveTo),
    JS_CFUNC_DEF("quadraticCurveTo", 4, js_canvas_quadraticCurveTo),

    JS_CFUNC_DEF("rect", 4, js_canvas_rect),

    JS_CFUNC_DEF("stroke", 1, js_canvas_stroke),
    JS_CFUNC_DEF("fill", 2, js_canvas_fill),
    JS_CFUNC_DEF("clip", 2, js_canvas_clip),

    JS_CFUNC_DEF("strokeRect", 4, js_canvas_strokeRect),
    JS_CFUNC_DEF("fillRect", 4, js_canvas_fillRect),
    JS_CFUNC_DEF("clearRect", 4, js_canvas_clearRect),

    JS_CFUNC_DEF("setStrokeStyle", 4, js_canvas_setStrokeStyle),
    JS_CFUNC_DEF("setFillStyle", 4, js_canvas_setFillStyle),

    JS_CFUNC_DEF("setLineCap", 1, js_canvas_setLineCap),
    JS_CFUNC_DEF("setLineJoin", 1, js_canvas_setLineJoin),
    JS_CFUNC_DEF("setMiterLimit", 1, js_canvas_setMiterLimit),
    JS_CFUNC_DEF("setLineWidth", 1, js_canvas_setLineWidth),
    JS_CFUNC_DEF("setLineDash", 2, js_canvas_setLineDash),
};

int js_canvas_define(JSContext *ctx, JSModuleDef *m)
{
    JSValue canvas = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, canvas, js_canvas_funcs, countof(js_canvas_funcs));
    JS_SetModuleExport(ctx, m, "Canvas", canvas);
    return 0;
}