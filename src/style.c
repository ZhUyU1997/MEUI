#include "box.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void assign_text(char **text, const char *new_text)
{
    if (*text)
        free(*text);
    *text = strdup(new_text);
}

box_style_t *box_style_new()
{
    box_style_t *style = calloc(1, sizeof(box_style_t));
    style->font_color.a = 1.0;
    style->text = strdup("");
    style->background_image = strdup("");
    style->content_image = strdup("");
    style->font_size = 16;
    style->align = TEXT_ALIGN_CENTER_LEFT;
    style->transform_origin.type = TRANSFORM_ORIGIN_TYPE_KEYWORD;
    style->transform_origin.x.keyword = TRANSFORM_ORIGIN_CENTER;
    style->transform_origin.y.keyword = TRANSFORM_ORIGIN_CENTER;
    plutovg_matrix_init_identity(style->transform);
    return style;
}

void box_style_free(box_style_t *style)
{
    assert(style);

    if (style->text)
        free(style->text);
    if (style->background_image)
        free(style->text);
    if (style->content_image)
        free(style->text);
    free(style);
}

void box_merge_styles(box_style_t *dst, box_style_t *src)
{
    if (!ISNULL(src->border_radius))
    {
        dst->border_radius[0] = src->border_radius[0];
        dst->border_radius[1] = src->border_radius[1];
        dst->border_radius[2] = src->border_radius[2];
        dst->border_radius[3] = src->border_radius[3];
    }

    if (!ISNULL(src->border_color))
        dst->border_color = src->border_color;
    if (!ISNULL(src->fill_color))
        dst->fill_color = src->fill_color;
    if (!ISNULL(src->font_color))
        dst->font_color = src->font_color;
    if (!ISNULL(src->align))
        dst->align = src->align;
    if (!ISNULL(src->font_size))
        dst->font_size = src->font_size;
    if (!ISNULL(src->background_image))
        dst->background_image = src->background_image;
    if (!ISNULL(src->content_image))
        dst->content_image = src->content_image;
    if (!ISNULL(src->transform))
        dst->transform[0] = src->transform[0];
    if (!ISNULL(src->transform_origin))
        dst->transform_origin = src->transform_origin;
    if (!ISNULL(src->text))
        dst->text = src->text;
}

void box_style_border_radius(box_style_t *style, float tl, float tr, float br, float bl)
{
    assert(style);
    NULLABLE_ASSIGN(style->border_radius, tl, [0]);
    NULLABLE_ASSIGN(style->border_radius, tr, [1]);
    NULLABLE_ASSIGN(style->border_radius, br, [2]);
    NULLABLE_ASSIGN(style->border_radius, bl, [3]);
}

void box_style_border_color(box_style_t *style, struct plutovg_color c)
{
    assert(style);
    NULLABLE_ASSIGN(style->border_color, c);
}

void box_style_fill_color(box_style_t *style, struct plutovg_color c)
{
    assert(style);
    NULLABLE_ASSIGN(style->fill_color, c);
}

void box_style_font_color(box_style_t *style, struct plutovg_color c)
{
    assert(style);
    NULLABLE_ASSIGN(style->font_color, c);
}

void box_style_text(box_style_t *style, const char *text)
{
    assert(style);
    NULLABLE_FLAG(style->text);
    assign_text(&style->text, text);
}

void box_style_font_size(box_style_t *style, double font_size)
{
    assert(style);
    NULLABLE_ASSIGN(style->font_size, font_size);
}

void box_style_text_align(box_style_t *style, TEXT_ALIGN align)
{
    assert(style);
    NULLABLE_ASSIGN(style->align, align);
}

void box_style_background_image(box_style_t *style, const char *background_image)
{
    assert(style);
    NULLABLE_FLAG(style->background_image);
    assign_text(&style->background_image, background_image);
}

void box_style_content_image(box_style_t *style, const char *content_image)
{
    assert(style);
    NULLABLE_FLAG(style->content_image);
    assign_text(&style->content_image, content_image);
}

void box_style_transform_matrix(box_style_t *style, double m00, double m10, double m01, double m11, double m02, double m12)
{
    assert(style);
    NULLABLE_FLAG(style->transform);

    plutovg_matrix_t m;
    plutovg_matrix_init(&m, m00, m10, m01, m11, m02, m12);
    plutovg_matrix_multiply(style->transform, &m, style->transform);
}

void box_style_transform_translate(box_style_t *style, double x, double y)
{
    assert(style);
    NULLABLE_FLAG(style->transform);

    plutovg_matrix_t m;
    plutovg_matrix_init_translate(&m, x, y);
    plutovg_matrix_multiply(style->transform, &m, style->transform);
}

void box_style_transform_rotate(box_style_t *style, double radians)
{
    assert(style);
    NULLABLE_FLAG(style->transform);

    plutovg_matrix_t m;
    plutovg_matrix_init_rotate(&m, radians);
    plutovg_matrix_multiply(style->transform, &m, style->transform);
}

void box_style_transform_scale(box_style_t *style, double x, double y)
{
    assert(style);
    NULLABLE_FLAG(style->transform);

    plutovg_matrix_t m;
    plutovg_matrix_init_scale(&m, x, y);
    plutovg_matrix_multiply(style->transform, &m, style->transform);
}

void box_style_transform_skew(box_style_t *style, double x, double y)
{
    assert(style);
    NULLABLE_FLAG(style->transform);

    plutovg_matrix_t m;
    plutovg_matrix_init_shear(&m, x, y);
    plutovg_matrix_multiply(style->transform, &m, style->transform);
}

void box_style_transform_origin_keyword(box_style_t *style, TRANSFORM_ORIGIN x, TRANSFORM_ORIGIN y)
{
    assert(style);
    NULLABLE_FLAG(style->transform_origin);

    style->transform_origin.type = TRANSFORM_ORIGIN_TYPE_KEYWORD;
    style->transform_origin.x.keyword = x;
    style->transform_origin.y.keyword = y;
}

void box_style_transform_origin_offset(box_style_t *style, double x, double y)
{
    assert(style);
    NULLABLE_FLAG(style->transform_origin);

    style->transform_origin.type = TRANSFORM_ORIGIN_TYPE_OFFSET;
    style->transform_origin.x.offset = x;
    style->transform_origin.y.offset = y;
}
