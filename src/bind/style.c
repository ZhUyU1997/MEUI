
#include "cutils.h"
#include "quickjs.h"
#include "quickjs-libc.h"
#include <box.h>
#include <string.h>
#include <bind/style.h>

#define JS_FLOAT_PROPERTY(Name)                                                         \
    static JSValue js_box_style_##Name(JSContext *ctx, box_style_t *style, JSValue val) \
    {                                                                                   \
        if (!style)                                                                     \
            return JS_EXCEPTION;                                                        \
        double v;                                                                       \
        if (JS_ToFloat64(ctx, &v, val))                                                 \
            return JS_EXCEPTION;                                                        \
        box_style_##Name(style, v);                                                     \
        return JS_UNDEFINED;                                                            \
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
    static JSValue js_box_style_##Name(JSContext *ctx, box_style_t *style, JSValue val)            \
    {                                                                                              \
        if (!style)                                                                                \
            return JS_EXCEPTION;                                                                   \
        return js_box_style_##Name##_Internel(ctx, style, val);                                    \
    }

static JSValue js_box_style_wrap(JSContext *ctx, box_style_t *style, JSValue val)
{
    JSValue ret = JS_EXCEPTION;
    if (!style)
        return ret;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return ret;

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
            ret = JS_UNDEFINED;
            break;
        }
    }

    JS_FreeCString(ctx, s);
    return ret;
}

static JSValue js_box_style_direction(JSContext *ctx, box_style_t *style, JSValue val)
{
    JSValue ret = JS_EXCEPTION;

    if (!style)
        return ret;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return ret;

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
            ret = JS_UNDEFINED;
            break;
        }
    }
    JS_FreeCString(ctx, s);

    return ret;
}

static JSValue js_box_style_alignItems(JSContext *ctx, box_style_t *style, JSValue val)
{
    JSValue ret = JS_EXCEPTION;

    if (!style)
        return ret;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return ret;

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
            ret = JS_UNDEFINED;
            break;
        }
    }
    JS_FreeCString(ctx, s);

    return ret;
}

static JSValue js_box_style_alignSelf(JSContext *ctx, box_style_t *style, JSValue val)
{
    JSValue ret = JS_EXCEPTION;

    if (!style)
        return ret;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return ret;

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
            ret = JS_UNDEFINED;
            break;
        }
    }
    JS_FreeCString(ctx, s);

    return ret;
}

static JSValue js_box_style_alignContent(JSContext *ctx, box_style_t *style, JSValue val)
{
    JSValue ret = JS_EXCEPTION;

    if (!style)
        return ret;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return ret;

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
            ret = JS_UNDEFINED;
            break;
        }
    }
    JS_FreeCString(ctx, s);

    return ret;
}

static JSValue js_box_style_justifyContent(JSContext *ctx, box_style_t *style, JSValue val)
{
    JSValue ret = JS_EXCEPTION;

    if (!style)
        return ret;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return ret;

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
            ret = JS_UNDEFINED;
            break;
        }
    }
    JS_FreeCString(ctx, s);

    return ret;
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
    static JSValue js_box_style_##Name(JSContext *ctx, box_style_t *style, JSValue val)            \
    {                                                                                              \
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
    static JSValue js_box_style_##Name(JSContext *ctx, box_style_t *style, JSValue val)            \
    {                                                                                              \
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

static JSValue js_box_style_margin(JSContext *ctx, box_style_t *style, JSValue val)
{
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

static JSValue js_box_style_border(JSContext *ctx, box_style_t *style, JSValue val)
{
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

static JSValue js_box_style_padding(JSContext *ctx, box_style_t *style, JSValue val)
{
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

static JSValue js_box_style_borderRadius(JSContext *ctx, box_style_t *style, JSValue val)
{
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

static JSValue js_set_style_border_radius(JSContext *ctx, box_style_t *style, JSValue val)
{
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

static JSValue js_set_style_border_color(JSContext *ctx, box_style_t *style, JSValue val)
{
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

static JSValue js_set_style_fill_color(JSContext *ctx, box_style_t *style, JSValue val)
{
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

static JSValue js_set_style_font_color(JSContext *ctx, box_style_t *style, JSValue val)
{
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

static JSValue js_get_style_text(JSContext *ctx, box_style_t *style)
{
    if (!style)
        return JS_EXCEPTION;

    return JS_NewString(ctx, style->text);
}

static JSValue js_set_style_text(JSContext *ctx, box_style_t *style, JSValue val)
{
    if (!style)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return JS_EXCEPTION;

    box_style_text(style, s);
    JS_FreeCString(ctx, s);
    return JS_UNDEFINED;
}

static JSValue js_set_style_font_size(JSContext *ctx, box_style_t *style, JSValue val)
{
    if (!style)
        return JS_EXCEPTION;

    double d;

    if (JS_ToFloat64(ctx, &d, val))
        return JS_EXCEPTION;

    box_style_fontSize(style, d);
    return JS_UNDEFINED;
}

static JSValue js_set_style_font_family(JSContext *ctx, box_style_t *style, JSValue val)
{
    if (!style)
        return JS_EXCEPTION;

    const char *font_family = JS_ToCString(ctx, val);
    if (!font_family)
        return JS_EXCEPTION;

    box_style_fontFamily(style, font_family);
    JS_FreeCString(ctx, font_family);
    return JS_UNDEFINED;
}

static JSValue js_set_style_text_align(JSContext *ctx, box_style_t *style, JSValue val)
{
    JSValue ret = JS_EXCEPTION;

    if (!style)
        return ret;

    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return ret;

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
            ret = JS_UNDEFINED;
            break;
        }
    }

    JS_FreeCString(ctx, s);

    return ret;
}

static JSValue js_box_style_overflow(JSContext *ctx, box_style_t *style, JSValue val)
{
    JSValue ret = JS_EXCEPTION;
    if (!style)
        return ret;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return ret;

    const static struct
    {
        const char *string_value;
        CSS_OVERFLOW enum_value;
    } map[] = {
        {"visible", CSS_OVERFLOW_VISIBLE},
        {"hidden", CSS_OVERFLOW_HIDDEN},
        {"scroll", CSS_OVERFLOW_SCROLL},
        {"auto", CSS_OVERFLOW_AUTO},
    };

    for (int i = 0; i < countof(map); i++)
    {
        if (!strcmp(map[i].string_value, s))
        {
            box_style_overflow(style, map[i].enum_value);
            ret = JS_UNDEFINED;
            break;
        }
    }

    JS_FreeCString(ctx, s);
    return ret;
}

static JSValue js_set_style_background_image(JSContext *ctx, box_style_t *style, JSValue val)
{
    if (!style)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return JS_EXCEPTION;

    box_style_backgroundImage(style, s);
    JS_FreeCString(ctx, s);

    return JS_UNDEFINED;
}

static JSValue js_set_style_content_image(JSContext *ctx, box_style_t *style, JSValue val)
{
    if (!style)
        return JS_EXCEPTION;
    const char *s = JS_ToCString(ctx, val);
    if (!s)
        return JS_EXCEPTION;

    box_style_contentImage(style, s);
    JS_FreeCString(ctx, s);

    return JS_UNDEFINED;
}

static JSValue js_set_style_transform_matrix(JSContext *ctx, box_style_t *style, JSValue val)
{
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

static JSValue js_set_style_transform_origin(JSContext *ctx, box_style_t *style, JSValue val)
{
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
        {
            JS_FreeCString(ctx, s[0]);
            return JS_EXCEPTION;
        }

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
        JS_FreeCString(ctx, s[0]);
        JS_FreeCString(ctx, s[1]);
    }
    else
    {
        return JS_EXCEPTION;
    }

    return JS_UNDEFINED;
}

#undef JS_FLOAT_PROPERTY
#undef JS_FLOAT_PROPERTY_RW
#undef JS_FLEX_LENGTH_PROPERTY
#undef JS_FLEX_LENGTH_PROPERTY_AUTO

#define JS_FLOAT_PROPERTY(Name) {#Name, NULL, js_box_style_##Name},
#define JS_FLOAT_PROPERTY_RW(Name) {#Name, NULL, js_box_style_##Name},
#define JS_FLEX_LENGTH_PROPERTY(Name) {#Name, NULL, js_box_style_##Name},
#define JS_FLEX_LENGTH_PROPERTY_AUTO(Name) {#Name, NULL, js_box_style_##Name},

const JSStyleGetSet jsStyleGetSet[] = {
    {"borderRadius", NULL, js_set_style_border_radius},
    {"borderColor", NULL, js_set_style_border_color},
    {"backgroundColor", NULL, js_set_style_fill_color},
    {"fontColor", NULL, js_set_style_font_color},
    {"text", js_get_style_text, js_set_style_text},
    {"fontSize", NULL, js_set_style_font_size},
    {"fontFamily", NULL, js_set_style_font_family},
    {"textAlign", NULL, js_set_style_text_align},
    {"backgroundImage", NULL, js_set_style_background_image},
    {"contentImage", NULL, js_set_style_content_image},
    {"transform", NULL, js_set_style_transform_matrix},
    {"transformOrigin", NULL, js_set_style_transform_origin},
    {"flexWrap", NULL, js_box_style_wrap},
    {"flexDirection", NULL, js_box_style_direction},
    {"alignItems", NULL, js_box_style_alignItems},
    {"alignSelf", NULL, js_box_style_alignSelf},
    {"alignContent", NULL, js_box_style_alignContent},
    {"justifyContent", NULL, js_box_style_justifyContent},
    {"margin", NULL, js_box_style_margin},
    {"border", NULL, js_box_style_border},
    {"padding", NULL, js_box_style_padding},
    {"overflow", NULL, js_box_style_overflow},
    JS_FLEX_PROPERTYES()};

const int jsStyleGetSetLength = sizeof(jsStyleGetSet) / sizeof(JSStyleGetSet);