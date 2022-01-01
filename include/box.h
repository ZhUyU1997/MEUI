#pragma once

#include <FlexLayout.h>
#include <plutovg.h>
#include <meui/event.h>

#include <list.h>

typedef FlexNodeRef box_t;

typedef enum TEXT_ALIGN
{
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_RIGHT,
    TEXT_ALIGN_CENTER,
} TEXT_ALIGN;

typedef enum TRANSFORM_ORIGIN_TYPE
{
    TRANSFORM_ORIGIN_TYPE_KEYWORD,
    TRANSFORM_ORIGIN_TYPE_OFFSET,

} TRANSFORM_ORIGIN_TYPE;

typedef enum TRANSFORM_ORIGIN
{
    TRANSFORM_ORIGIN_LEFT = 0,
    TRANSFORM_ORIGIN_TOP = 0,
    TRANSFORM_ORIGIN_CENTER = 1,
    TRANSFORM_ORIGIN_RIGHT = 2,
    TRANSFORM_ORIGIN_BOTTOM = 2,
} TRANSFORM_ORIGIN;

struct transform_origin_t
{
    TRANSFORM_ORIGIN_TYPE type;

    union
    {
        TRANSFORM_ORIGIN keyword;
        double offset;
    } x, y;
};

typedef struct box_event_t
{
    meui_event_t e;
    enum MEUI_EVENT_TYPE type;
    box_t target;
    box_t currentTarget;
} box_event_t;

typedef void (*box_event_cb_t)(box_event_t *e);

typedef struct box_event_dsc_t
{
    struct list_head node;
    enum MEUI_EVENT_TYPE type;
    box_event_cb_t cb;
} box_event_dsc_t;

#define NULLABLE(type, name, arr...) \
    struct                           \
    {                                \
        unsigned char name##_flags;  \
        type name arr;               \
    }

#define NULLABLE_ASSIGN(name, value, arr...) \
    ({                                       \
        name##_flags = 1;                    \
        name arr = value;                    \
    })

#define NULLABLE_FLAG(name) \
    ({                      \
        name##_flags = 1;   \
    })

#define ISNULL(name) (name##_flags == 1)

typedef struct box_style_t
{
    NULLABLE(float, border_radius, [4]);
    NULLABLE(plutovg_color_t, border_color);
    NULLABLE(plutovg_color_t, fill_color);
    NULLABLE(plutovg_color_t, font_color);
    NULLABLE(TEXT_ALIGN, align);
    NULLABLE(double, font_size);
    NULLABLE(char *, background_image);
    NULLABLE(char *, content_image);
    NULLABLE(plutovg_matrix_t, transform, [1]);
    NULLABLE(struct transform_origin_t, transform_origin);
    NULLABLE(char *, text);
} box_style_t;

enum BOX_STATE
{
    BOX_STATE_DEFAULT,
    BOX_STATE_FOCUS,

    BOX_STATE_SELECT,
    BOX_STATE_DISABLE,
    BOX_STATE_CHECK,
    BOX_STATE_MAX,
};

struct Box
{
    enum BOX_STATE state;
    box_style_t *style_array[BOX_STATE_MAX];
    box_style_t style;

    struct list_head event_list;

    struct
    {
        plutovg_matrix_t to_screen_matrix; // box to screen
    } result;
};

box_t box_new();
void box_free(box_t node);
void box_freeRecursive(box_t node);

void box_default_style_border_radius(box_t node, float tl, float tr, float br, float bl);
void box_default_style_border_color(box_t node, plutovg_color_t c);
void box_default_style_fill_color(box_t node, plutovg_color_t c);
void box_default_style_font_color(box_t node, plutovg_color_t c);
void box_default_style_text(box_t node, const char *text);
void box_default_style_font_size(box_t node, double font_size);
void box_default_style_text_align(box_t node, TEXT_ALIGN align);
void box_default_style_background_image(box_t node, const char *background_image);
void box_default_style_content_image(box_t node, const char *content_image);
void box_default_style_transform_matrix(box_t node, double m00, double m10, double m01, double m11, double m02, double m12);
void box_default_style_transform_translate(box_t node, double x, double y);
void box_default_style_transform_rotate(box_t node, double radians);
void box_default_style_transform_scale(box_t node, double x, double y);
void box_default_style_transform_skew(box_t node, double x, double y);
void box_default_style_transform_origin_keyword(box_t node, TRANSFORM_ORIGIN x, TRANSFORM_ORIGIN y);
void box_default_style_transform_origin_offset(box_t node, double x, double y);

void box_merge_styles(box_style_t *dst, box_style_t *src);

void box_style_init(box_style_t *style);
void box_style_border_radius(box_style_t *style, float tl, float tr, float br, float bl);
void box_style_border_color(box_style_t *style, plutovg_color_t c);
void box_style_fill_color(box_style_t *style, plutovg_color_t c);
void box_style_font_color(box_style_t *style, plutovg_color_t c);
void box_style_text(box_style_t *style, const char *text);
void box_style_font_size(box_style_t *style, double font_size);
void box_style_text_align(box_style_t *style, TEXT_ALIGN align);
void box_style_background_image(box_style_t *style, const char *background_image);
void box_style_content_image(box_style_t *style, const char *content_image);
void box_style_transform_matrix(box_style_t *style, double m00, double m10, double m01, double m11, double m02, double m12);
void box_style_transform_translate(box_style_t *style, double x, double y);
void box_style_transform_rotate(box_style_t *style, double radians);
void box_style_transform_scale(box_style_t *style, double x, double y);
void box_style_transform_skew(box_style_t *style, double x, double y);
void box_style_transform_origin_keyword(box_style_t *style, TRANSFORM_ORIGIN x, TRANSFORM_ORIGIN y);
void box_style_transform_origin_offset(box_style_t *style, double x, double y);

void box_add_event_listener(box_t node, enum MEUI_EVENT_TYPE type, box_event_cb_t cb);
void box_dispatch_event(box_t node, enum MEUI_EVENT_TYPE type, meui_event_t *e);

void box_drawRecursive(plutovg_t *pluto, box_t node);
void box_draw(box_t root);