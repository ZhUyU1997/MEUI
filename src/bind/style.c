
#include "cutils.h"
#include "quickjs.h"
#include "quickjs-libc.h"
#include <box.h>
#include <string.h>

#define countof(x) (sizeof(x) / sizeof((x)[0]))

static JSClassID js_box_style_class_id;

#define JS_FLOAT_PROPERTY(Name)                                                            \
    static JSValue js_box_style_##Name(JSContext *ctx, JSValueConst this_val, JSValue val) \
    {                                                                                      \
        box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);          \
        if (!style)                                                                        \
            return JS_EXCEPTION;                                                           \
        double v;                                                                          \
        if (JS_ToFloat64(ctx, &v, val))                                                    \
            return JS_EXCEPTION;                                                           \
        box_style_##Name(style, v);                                                        \
        return JS_UNDEFINED;                                                               \
    }

#define JS_FLOAT_PROPERTY_RW(Name)                                                                 \
    static JSValue js_box_style_##Name##_Internel(JSContext *ctx, box_style_t *style, JSValue val) \
    {                                                                                              \
        double v;                                                                                  \
        if (JS_ToFloat64(ctx, &v, val))                                                            \
            return JS_EXCEPTION;                                                                   \
        box_style_##Name(style, v);                                                                \
        return JS_UNDEFINED;                                                                       \
    }                                                                                              \
    static JSValue js_box_style_##Name(JSContext *ctx, JSValueConst this_val, JSValue val)         \
    {                                                                                              \
        box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);                  \
        if (!style)                                                                                \
            return JS_EXCEPTION;                                                                   \
        return js_box_style_##Name##_Internel(ctx, style, val);                                    \
    }

static JSValue js_box_style_wrap(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return JS_EXCEPTION;

    const static struct
    {
        const char *string_value;
        FlexWrapMode enum_value;
    } map[] = {
        {"nowrap", FlexNoWrap},
        {"wrap", FlexWrap},
        {"wrap-reverse", FlexWrapReverse},
    };

    for (int i = 0; i < countof(map); i++)
    {
        if (!strcmp(map[i].string_value, s))
        {
            box_style_wrap(style, map[i].enum_value);
            return JS_UNDEFINED;
        }
    }
    return JS_EXCEPTION;
}

static JSValue js_box_style_direction(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return JS_EXCEPTION;

    const static struct
    {
        const char *string_value;
        FlexDirection enum_value;
    } map[] = {
        {"row", FlexHorizontal},
        {"column", FlexVertical},
        {"row-reverse", FlexHorizontalReverse},
        {"column-reverse", FlexVerticalReverse},
    };

    for (int i = 0; i < countof(map); i++)
    {
        if (!strcmp(map[i].string_value, s))
        {
            box_style_direction(style, map[i].enum_value);
            return JS_UNDEFINED;
        }
    }
    return JS_EXCEPTION;
}

static JSValue js_box_style_alignItems(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return JS_EXCEPTION;

    const static struct
    {
        const char *string_value;
        FlexAlign enum_value;
    } map[] = {
        {"stretch", FlexStretch},
        {"flex-start", FlexStart},
        {"center", FlexCenter},
        {"flex-end", FlexEnd},
        {"baseline", FlexBaseline},
    };

    for (int i = 0; i < countof(map); i++)
    {
        if (!strcmp(map[i].string_value, s))
        {
            box_style_alignItems(style, map[i].enum_value);
            return JS_UNDEFINED;
        }
    }
    return JS_EXCEPTION;
}

static JSValue js_box_style_alignSelf(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return JS_EXCEPTION;

    const static struct
    {
        const char *string_value;
        FlexAlign enum_value;
    } map[] = {
        {"auto", FlexInherit},
        {"stretch", FlexStretch},
        {"flex-start", FlexStart},
        {"center", FlexCenter},
        {"flex-end", FlexEnd},
        {"baseline", FlexBaseline},
    };

    for (int i = 0; i < countof(map); i++)
    {
        if (!strcmp(map[i].string_value, s))
        {
            box_style_alignSelf(style, map[i].enum_value);
            return JS_UNDEFINED;
        }
    }
    return JS_EXCEPTION;
}

static JSValue js_box_style_alignContent(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return JS_EXCEPTION;

    const static struct
    {
        const char *string_value;
        FlexAlign enum_value;
    } map[] = {
        {"stretch", FlexStretch},
        {"flex-start", FlexStart},
        {"center", FlexCenter},
        {"flex-end", FlexEnd},
        {"space-between", FlexSpaceBetween},
        {"space-around", FlexSpaceAround},
    };

    for (int i = 0; i < countof(map); i++)
    {
        if (!strcmp(map[i].string_value, s))
        {
            box_style_alignContent(style, map[i].enum_value);
            return JS_UNDEFINED;
        }
    }
    return JS_EXCEPTION;
}

static JSValue js_box_style_justifyContent(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return JS_EXCEPTION;

    const static struct
    {
        const char *string_value;
        FlexAlign enum_value;
    } map[] = {
        {"flex-start", FlexStart},
        {"center", FlexCenter},
        {"flex-end", FlexEnd},
        {"space-between", FlexSpaceBetween},
        {"space-around", FlexSpaceAround},
    };

    for (int i = 0; i < countof(map); i++)
    {
        if (!strcmp(map[i].string_value, s))
        {
            box_style_justifyContent(style, map[i].enum_value);
            return JS_UNDEFINED;
        }
    }
    return JS_EXCEPTION;
}

#define JS_FLEX_LENGTH_PROPERTY(Name)                                                              \
    static JSValue js_box_style_##Name##_Internel(JSContext *ctx, box_style_t *style, JSValue val) \
    {                                                                                              \
        if (JS_IsNumber(val))                                                                      \
        {                                                                                          \
            double v;                                                                              \
            if (JS_ToFloat64(ctx, &v, val))                                                        \
                return JS_EXCEPTION;                                                               \
            box_style_##Name(style, v);                                                            \
        }                                                                                          \
        else if (JS_IsString(val))                                                                 \
        {                                                                                          \
            size_t len;                                                                            \
            const char *s = JS_ToCStringLen(ctx, &len, val);                                       \
            if (!s)                                                                                \
                return JS_EXCEPTION;                                                               \
            else if ((len > 0) && (s[len - 1] == '%'))                                             \
            {                                                                                      \
                float percent = strtof(s, NULL);                                                   \
                box_style_##Name##Percent(style, percent);                                         \
            }                                                                                      \
            else                                                                                   \
            {                                                                                      \
                return JS_EXCEPTION;                                                               \
            }                                                                                      \
        }                                                                                          \
        return JS_UNDEFINED;                                                                       \
    }                                                                                              \
    static JSValue js_box_style_##Name(JSContext *ctx, JSValueConst this_val, JSValue val)         \
    {                                                                                              \
        box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);                  \
        if (!style)                                                                                \
            return JS_EXCEPTION;                                                                   \
        return js_box_style_##Name##_Internel(ctx, style, val);                                    \
    }

#define JS_FLEX_LENGTH_PROPERTY_AUTO(Name)                                                         \
    static JSValue js_box_style_##Name##_Internel(JSContext *ctx, box_style_t *style, JSValue val) \
    {                                                                                              \
        if (JS_IsNumber(val))                                                                      \
        {                                                                                          \
            double v;                                                                              \
            if (JS_ToFloat64(ctx, &v, val))                                                        \
                return JS_EXCEPTION;                                                               \
            box_style_##Name(style, v);                                                            \
        }                                                                                          \
        else if (JS_IsString(val))                                                                 \
        {                                                                                          \
            size_t len;                                                                            \
            const char *s = JS_ToCStringLen(ctx, &len, val);                                       \
            if (!s)                                                                                \
                return JS_EXCEPTION;                                                               \
            if (!strcmp(s, "auto"))                                                                \
            {                                                                                      \
                box_style_##Name##Auto(style);                                                     \
            }                                                                                      \
            else if ((len > 0) && (s[len - 1] == '%'))                                             \
            {                                                                                      \
                float percent = strtof(s, NULL);                                                   \
                box_style_##Name##Percent(style, percent);                                         \
            }                                                                                      \
            else                                                                                   \
            {                                                                                      \
                return JS_EXCEPTION;                                                               \
            }                                                                                      \
        }                                                                                          \
        return JS_UNDEFINED;                                                                       \
    }                                                                                              \
    static JSValue js_box_style_##Name(JSContext *ctx, JSValueConst this_val, JSValue val)         \
    {                                                                                              \
        box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);                  \
        if (!style)                                                                                \
            return JS_EXCEPTION;                                                                   \
        return js_box_style_##Name##_Internel(ctx, style, val);                                    \
    }

#define JS_FLEX_PROPERTYES()                      \
    JS_FLOAT_PROPERTY(flexBasis)                  \
    JS_FLOAT_PROPERTY_RW(flexGrow)                \
    JS_FLOAT_PROPERTY_RW(flexShrink)              \
    JS_FLEX_LENGTH_PROPERTY_AUTO(width)           \
    JS_FLEX_LENGTH_PROPERTY_AUTO(height)          \
    JS_FLEX_LENGTH_PROPERTY(minWidth)             \
    JS_FLEX_LENGTH_PROPERTY(minHeight)            \
    JS_FLEX_LENGTH_PROPERTY(maxWidth)             \
    JS_FLEX_LENGTH_PROPERTY(maxHeight)            \
    JS_FLEX_LENGTH_PROPERTY_AUTO(marginLeft)      \
    JS_FLEX_LENGTH_PROPERTY_AUTO(marginTop)       \
    JS_FLEX_LENGTH_PROPERTY_AUTO(marginBottom)    \
    JS_FLEX_LENGTH_PROPERTY_AUTO(marginRight)     \
    JS_FLEX_LENGTH_PROPERTY_AUTO(marginStart)     \
    JS_FLEX_LENGTH_PROPERTY_AUTO(marginEnd)       \
    JS_FLEX_LENGTH_PROPERTY(paddingLeft)          \
    JS_FLEX_LENGTH_PROPERTY(paddingTop)           \
    JS_FLEX_LENGTH_PROPERTY(paddingBottom)        \
    JS_FLEX_LENGTH_PROPERTY(paddingRight)         \
    JS_FLEX_LENGTH_PROPERTY(paddingStart)         \
    JS_FLEX_LENGTH_PROPERTY(paddingEnd)           \
    JS_FLOAT_PROPERTY_RW(borderLeft)              \
    JS_FLOAT_PROPERTY_RW(borderTop)               \
    JS_FLOAT_PROPERTY_RW(borderBottom)            \
    JS_FLOAT_PROPERTY_RW(borderRight)             \
    JS_FLOAT_PROPERTY_RW(borderStart)             \
    JS_FLOAT_PROPERTY_RW(borderEnd)               \
    JS_FLOAT_PROPERTY_RW(borderTopLeftRadius)     \
    JS_FLOAT_PROPERTY_RW(borderTopRightRadius)    \
    JS_FLOAT_PROPERTY_RW(borderBottomRightRadius) \
    JS_FLOAT_PROPERTY_RW(borderBottomLeftRadius)

JS_FLEX_PROPERTYES()

#undef JS_FLOAT_PROPERTY
#undef JS_FLOAT_PROPERTY_RW
#undef JS_FLEX_LENGTH_PROPERTY
#undef JS_FLEX_LENGTH_PROPERTY_AUTO

#define JS_FLOAT_PROPERTY(Name) JS_CGETSET_DEF(#Name, NULL, js_box_style_##Name),
#define JS_FLOAT_PROPERTY_RW(Name) JS_CGETSET_DEF(#Name, NULL, js_box_style_##Name),
#define JS_FLEX_LENGTH_PROPERTY(Name) JS_CGETSET_DEF(#Name, NULL, js_box_style_##Name),
#define JS_FLEX_LENGTH_PROPERTY_AUTO(Name) JS_CGETSET_DEF(#Name, NULL, js_box_style_##Name),

static JSValue js_box_style_margin(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;

    if (JS_IsArray(ctx, val) < 0)
        return JS_EXCEPTION;

    js_box_style_marginTop_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 0));
    js_box_style_marginRight_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 1));
    js_box_style_marginBottom_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 2));
    js_box_style_marginLeft_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 3));

    return JS_UNDEFINED;
}

static JSValue js_box_style_border(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;

    if (JS_IsArray(ctx, val) < 0)
        return JS_EXCEPTION;

    js_box_style_borderTop_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 0));
    js_box_style_borderRight_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 1));
    js_box_style_borderBottom_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 2));
    js_box_style_borderLeft_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 3));

    return JS_UNDEFINED;
}

static JSValue js_box_style_padding(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;

    if (JS_IsArray(ctx, val) < 0)
        return JS_EXCEPTION;

    js_box_style_paddingTop_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 0));
    js_box_style_paddingRight_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 1));
    js_box_style_paddingBottom_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 2));
    js_box_style_paddingLeft_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 3));

    return JS_UNDEFINED;
}

static JSValue js_box_style_borderRadius(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;

    if (JS_IsArray(ctx, val) < 0)
        return JS_EXCEPTION;

    js_box_style_borderTopLeftRadius_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 0));
    js_box_style_borderTopRightRadius_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 1));
    js_box_style_borderBottomRightRadius_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 2));
    js_box_style_borderBottomLeftRadius_Internel(ctx, style, JS_GetPropertyUint32(ctx, val, 3));

    return JS_UNDEFINED;
}

static JSValue js_set_style_border_radius(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;

    if (JS_IsArray(ctx, val) < 0)
        return JS_EXCEPTION;

    double d[4];

    for (int i = 0; i < countof(d); i++)
    {
        if (JS_ToFloat64(ctx, &d[i], JS_GetPropertyUint32(ctx, val, i)))
            return JS_EXCEPTION;
    }
    box_style_borderTopLeftRadius(style, d[0]);
    box_style_borderTopRightRadius(style, d[1]);
    box_style_borderBottomRightRadius(style, d[2]);
    box_style_borderBottomLeftRadius(style, d[3]);
    return JS_UNDEFINED;
}

static JSValue js_set_style_border_color(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;

    if (JS_IsArray(ctx, val) < 0)
        return JS_EXCEPTION;

    double d[4];

    for (int i = 0; i < countof(d); i++)
    {
        if (JS_ToFloat64(ctx, &d[i], JS_GetPropertyUint32(ctx, val, i)))
            return JS_EXCEPTION;
    }

    box_style_borderColor(style, (plutovg_color_t){d[0], d[1], d[2], d[3]});
    return JS_UNDEFINED;
}

static JSValue js_set_style_fill_color(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;

    if (JS_IsArray(ctx, val) < 0)
        return JS_EXCEPTION;

    double d[4];

    for (int i = 0; i < countof(d); i++)
    {
        if (JS_ToFloat64(ctx, &d[i], JS_GetPropertyUint32(ctx, val, i)))
            return JS_EXCEPTION;
    }

    box_style_backgroundColor(style, (plutovg_color_t){d[0], d[1], d[2], d[3]});
    return JS_UNDEFINED;
}

static JSValue js_set_style_font_color(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;

    if (JS_IsArray(ctx, val) < 0)
        return JS_EXCEPTION;

    double d[4];

    for (int i = 0; i < countof(d); i++)
    {
        if (JS_ToFloat64(ctx, &d[i], JS_GetPropertyUint32(ctx, val, i)))
            return JS_EXCEPTION;
    }

    box_style_fontColor(style, (plutovg_color_t){d[0], d[1], d[2], d[3]});
    return JS_UNDEFINED;
}

static JSValue js_get_style_text(JSContext *ctx, JSValueConst this_val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;

    return JS_NewString(ctx, style->text);
}

static JSValue js_set_style_text(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return JS_EXCEPTION;

    box_style_text(style, s);

    return JS_UNDEFINED;
}

static JSValue js_set_style_font_size(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;

    double d;

    if (JS_ToFloat64(ctx, &d, val))
        return JS_EXCEPTION;

    box_style_fontSize(style, d);
    return JS_UNDEFINED;
}

static JSValue js_set_style_text_align(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;

    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return JS_EXCEPTION;

    const static struct
    {
        const char *string_value;
        TEXT_ALIGN enum_value;
    } map[] = {
        {"center-left", TEXT_ALIGN_CENTER_LEFT},
        {"center-right", TEXT_ALIGN_CENTER_RIGHT},
        {"center", TEXT_ALIGN_CENTER_CENTER},
        {"top-left", TEXT_ALIGN_TOP_LEFT},
        {"top-right", TEXT_ALIGN_TOP_RIGHT},
        {"top-center", TEXT_ALIGN_TOP_CENTER},
        {"bottom-left", TEXT_ALIGN_BOTTOM_LEFT},
        {"bottom-right", TEXT_ALIGN_BOTTOM_RIGHT},
        {"bottom-center", TEXT_ALIGN_BOTTOM_CENTER},
    };

    for (int i = 0; i < countof(map); i++)
    {
        if (!strcmp(map[i].string_value, s))
        {
            box_style_textAlign(style, map[i].enum_value);
            return JS_UNDEFINED;
        }
    }
    return JS_EXCEPTION;
}

static JSValue js_set_style_background_image(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return JS_EXCEPTION;

    box_style_backgroundImage(style, s);
    return JS_UNDEFINED;
}

static JSValue js_set_style_content_image(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return JS_EXCEPTION;

    box_style_contentImage(style, s);
    return JS_UNDEFINED;
}

static JSValue js_set_style_transform_matrix(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;

    if (JS_IsArray(ctx, val) < 0)
        return JS_EXCEPTION;

    double d[6];

    for (int i = 0; i < countof(d); i++)
    {
        if (JS_ToFloat64(ctx, &d[i], JS_GetPropertyUint32(ctx, val, i)))
            return JS_EXCEPTION;
    }

    box_style_transform(style, d[0], d[1], d[2], d[3], d[4], d[5]);
    return JS_UNDEFINED;
}

static JSValue js_set_style_transform_origin(JSContext *ctx, JSValueConst this_val, JSValue val)
{
    box_style_t *style = JS_GetOpaque2(ctx, this_val, js_box_style_class_id);
    if (!style)
        return JS_EXCEPTION;

    if (JS_IsArray(ctx, val) < 0)
        return JS_EXCEPTION;

    const static struct
    {
        const char *string_value;
        TRANSFORM_ORIGIN enum_value;
    } x_map[] = {
        {"left", TRANSFORM_ORIGIN_LEFT},
        {"center", TRANSFORM_ORIGIN_CENTER},
        {"right", TRANSFORM_ORIGIN_RIGHT},
    };

    const static struct
    {
        const char *string_value;
        TRANSFORM_ORIGIN enum_value;
    } y_map[] = {
        {"top", TRANSFORM_ORIGIN_TOP},
        {"center", TRANSFORM_ORIGIN_CENTER},
        {"bottom", TRANSFORM_ORIGIN_BOTTOM},
    };

    double d[2] = {};
    const char *s[2] = {};

    JSValue v0 = JS_GetPropertyUint32(ctx, val, 0);
    JSValue v1 = JS_GetPropertyUint32(ctx, val, 1);

    if (JS_IsNumber(v0))
    {
        if (JS_ToFloat64(ctx, &d[0], v0))
            return JS_EXCEPTION;
        if (JS_ToFloat64(ctx, &d[1], v1))
            return JS_EXCEPTION;
        box_style_transformOriginOffset(style, d[0], d[1]);
    }
    else if (JS_IsString(v0))
    {
        TRANSFORM_ORIGIN o[2];

        s[0] = JS_ToCString(ctx, v0);
        if (!s[0])
            return JS_EXCEPTION;

        for (int i = 0; i < countof(x_map); i++)
        {
            if (!strcmp(x_map[i].string_value, s[0]))
            {
                o[0] = x_map[i].enum_value;
            }
        }

        s[1] = JS_ToCString(ctx, v0);
        if (!s[1])
            return JS_EXCEPTION;

        for (int i = 0; i < countof(y_map); i++)
        {
            if (!strcmp(y_map[i].string_value, s[1]))
            {
                o[1] = y_map[i].enum_value;
            }
        }

        // TODO: check not found?
        // TODO:  x is offset y is keyword
        box_style_transformOriginKeyword(style, o[0], o[1]);
    }
    else
    {
        return JS_EXCEPTION;
    }

    return JS_UNDEFINED;
}

static const JSCFunctionListEntry js_box_style_proto_funcs[] = {
    JS_CGETSET_DEF("borderRadius", NULL, js_set_style_border_radius),
    JS_CGETSET_DEF("borderColor", NULL, js_set_style_border_color),
    JS_CGETSET_DEF("backgroundColor", NULL, js_set_style_fill_color), //  fillColor => backgroundColor
    JS_CGETSET_DEF("fontColor", NULL, js_set_style_font_color),
    JS_CGETSET_DEF("text", js_get_style_text, js_set_style_text),
    JS_CGETSET_DEF("fontSize", NULL, js_set_style_font_size),
    JS_CGETSET_DEF("textAlign", NULL, js_set_style_text_align),
    JS_CGETSET_DEF("backgroundImage", NULL, js_set_style_background_image),
    JS_CGETSET_DEF("contentImage", NULL, js_set_style_content_image),
    JS_CGETSET_DEF("transform", NULL, js_set_style_transform_matrix),
    JS_CGETSET_DEF("transformOrigin", NULL, js_set_style_transform_origin),
    JS_CGETSET_DEF("flexWrap", NULL, js_box_style_wrap),
    JS_CGETSET_DEF("flexDirection", NULL, js_box_style_direction),
    JS_CGETSET_DEF("alignItems", NULL, js_box_style_alignItems),
    JS_CGETSET_DEF("alignSelf", NULL, js_box_style_alignSelf),
    JS_CGETSET_DEF("alignContent", NULL, js_box_style_alignContent),
    JS_CGETSET_DEF("justifyContent", NULL, js_box_style_justifyContent),
    JS_CGETSET_DEF("margin", NULL, js_box_style_margin),
    JS_CGETSET_DEF("border", NULL, js_box_style_border),
    JS_CGETSET_DEF("padding", NULL, js_box_style_padding),
    JS_FLEX_PROPERTYES()};

JSValue js_createBoxStyleFuncWithOpaque(JSContext *ctx, box_style_t *style)
{
    JSValue obj = JS_NewObjectClass(ctx, js_box_style_class_id);
    if (JS_IsException(obj))
        return obj;

    JS_SetOpaque(obj, style);
    return obj;
}

static JSValue js_createBoxStyleFunc(JSContext *ctx,
                                     JSValueConst new_target,
                                     int argc, JSValueConst *argv)
{
    box_style_t *style = box_style_new();
    return js_createBoxStyleFuncWithOpaque(ctx, style);
}

static void js_box_style_finalizer(JSRuntime *rt, JSValue val)
{
    struct box_style_t *style = JS_GetOpaque(val, js_box_style_class_id);
}

static JSClassDef js_box_style_class = {
    "BoxStyle",
    .finalizer = js_box_style_finalizer,
};

JSClassID get_js_box_style_class_id()
{
    return js_box_style_class_id;
}

static const JSCFunctionListEntry entry = JS_CFUNC_DEF("createBoxStyle", 0, js_createBoxStyleFunc);

int js_box_style_class_define(JSContext *ctx, JSModuleDef *m)
{
    JS_NewClassID(&js_box_style_class_id);
    JS_NewClass(JS_GetRuntime(ctx), js_box_style_class_id, &js_box_style_class);

    JSValue box_style_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, box_style_proto, js_box_style_proto_funcs, countof(js_box_style_proto_funcs));
    JS_SetClassProto(ctx, js_box_style_class_id, box_style_proto);
    JS_SetModuleExportList(ctx, m, &entry, 1);
    return 0;
}