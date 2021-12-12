#include "box.h"
#include "image.h"
#include "meui.h"

#include <FlexLayout.h>
#include <plutovg.h>
#include <plutosvg.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>

static const char *fileext(const char *filename)
{
    const char *ret = NULL;
    const char *p;

    if (filename != NULL)
    {
        ret = p = strchr(filename, '.');
        while (p != NULL)
        {
            p = strchr(p + 1, '.');
            if (p != NULL)
                ret = p;
        }
        if (ret != NULL)
            ret++;
    }
    return ret;
}

#define KAPPA90 (0.5522847493f)

FlexNodeRef Flex_newBox()
{
    FlexNodeRef node = Flex_newNode();
    struct Box *box = calloc(1, sizeof(struct Box));
    box->font_color.a = 1.0;
    box->text = strdup("");
    box->background_image = strdup("");
    box->content_image = strdup("");
    box->font_size = 16;
    box->align = TEXT_ALIGN_LEFT;
    box->transform_origin.type = TRANSFORM_ORIGIN_TYPE_KEYWORD;
    box->transform_origin.x.keyword = TRANSFORM_ORIGIN_CENTER;
    box->transform_origin.y.keyword = TRANSFORM_ORIGIN_CENTER;

    plutovg_matrix_init_identity(box->transform);

    Flex_setContext(node, box);
    return node;
}

void Flex_freeBox(FlexNodeRef node)
{
    struct Box *box = Flex_getContext(node);

    if (box)
    {
        free(box);
        Flex_setContext(node, NULL);
    }
    Flex_freeNode(node);
}

void Flex_freeRecursive(FlexNodeRef node)
{
    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        Flex_freeRecursive(Flex_getChild(node, i));
    }
    Flex_freeBox(node);
}

static plutovg_path_t *round4_rect(float r[4][2], int x, int y, int w, int h)
{
    const char a[4][3][2] = {
        {{0, -KAPPA90}, {1 - KAPPA90, -1}, {1, -1}},
        {{KAPPA90, 0}, {1, 1 - KAPPA90}, {1, 1}},
        {{0, KAPPA90}, {-(1 - KAPPA90), 1}, {-1, 1}},
        {{-KAPPA90, 0}, {-1, -(1 - KAPPA90)}, {-1, -1}},
    };

    plutovg_path_t *path = plutovg_path_create();

    int index = 0;
    plutovg_path_move_to(path, x, y + r[index][1]);
    plutovg_path_rel_cubic_to(path,
                              r[index][0] * a[index][0][0], r[index][1] * a[index][0][1],
                              r[index][0] * a[index][1][0], r[index][1] * a[index][1][1],
                              r[index][0] * a[index][2][0], r[index][1] * a[index][2][1]);

    plutovg_path_rel_line_to(path, w - (r[index][0] + r[index + 1][0]), 0);
    index = 1;
    plutovg_path_rel_cubic_to(path,
                              r[index][0] * a[index][0][0], r[index][1] * a[index][0][1],
                              r[index][0] * a[index][1][0], r[index][1] * a[index][1][1],
                              r[index][0] * a[index][2][0], r[index][1] * a[index][2][1]);

    plutovg_path_rel_line_to(path, 0, h - (r[index][1] + r[index + 1][1]));
    index = 2;
    plutovg_path_rel_cubic_to(path,
                              r[index][0] * a[index][0][0], r[index][1] * a[index][0][1],
                              r[index][0] * a[index][1][0], r[index][1] * a[index][1][1],
                              r[index][0] * a[index][2][0], r[index][1] * a[index][2][1]);
    plutovg_path_rel_line_to(path, -w + (r[index][0] + r[index + 1][0]), 0);
    index = 3;
    plutovg_path_rel_cubic_to(path,
                              r[index][0] * a[index][0][0], r[index][1] * a[index][0][1],
                              r[index][0] * a[index][1][0], r[index][1] * a[index][1][1],
                              r[index][0] * a[index][2][0], r[index][1] * a[index][2][1]);

    plutovg_path_close(path);
    return path;
}

static void DrawImage(plutovg_t *pluto, const char *path, plutovg_rect_t *r)
{
    plutovg_save(pluto);

    plutovg_surface_t *img = NULL;

    if (strcmp(fileext(path), "svg") == 0)
    {
        img = plutosvg_load_from_file(path, NULL, r->w, r->h, 96.0);
    }
    else
    {
        img = pluto_load_image_from_file(path, r->w, r->h);
    }

    if (img)
    {
        plutovg_set_source_surface(pluto, img, r->x, r->y);
        plutovg_fill_preserve(pluto);
    }

    plutovg_restore(pluto);
}

static void DrawBoxBackground(struct Box *box, plutovg_t *pluto, plutovg_rect_t *rect, plutovg_rect_t *content_rect, float radius[4], float border[4], struct plutovg_color border_color, struct plutovg_color fill_color)
{
    float r[4][2] = {0};

    float x = rect->x;
    float y = rect->y;
    float w = rect->w;
    float h = rect->h;

    if (radius != NULL)
    {
        int max_index = 0;

        for (int i = 1; i < 4; i++)
            if (radius[i] > radius[max_index])
                max_index = i;

        float radius_max = radius[max_index];

        if (radius_max > 0.0f)
        {
            float radius_next = radius[(max_index + 1) % 4];
            float radius_other = radius[(max_index + 2) % 4];
            float radius_prev = radius[(max_index + 3) % 4];

            float r1 = radius_next + radius_max > w ? w * radius_max / (radius_next + radius_max) : radius_max;
            float r2 = radius_prev + radius_max > w ? w * radius_max / (radius_prev + radius_max) : radius_max;

            r[max_index][0] = r[max_index][1] = fmaxf(r1, r2);
            r[(max_index + 1) % 4][0] = r[(max_index + 1) % 4][1] = r[max_index][0] * radius_next / radius_max;
            r[(max_index + 2) % 4][0] = r[(max_index + 2) % 4][1] = r[max_index][0] * radius_other / radius_max;
            r[(max_index + 3) % 4][0] = r[(max_index + 3) % 4][1] = r[max_index][0] * radius_prev / radius_max;
        }
    }

    plutovg_save(pluto);

    plutovg_path_t *outer_path = round4_rect(r, 0, 0, w, h);
    plutovg_add_path(pluto, outer_path);
    plutovg_set_source_rgba(pluto, fill_color.r, fill_color.g, fill_color.b, fill_color.a);

    plutovg_fill_preserve(pluto);

    if (box->background_image[0] != '\0')
    {
        DrawImage(pluto, box->background_image, rect);
    }

    if (border != NULL)
    {
        plutovg_new_path(pluto);

        float border_top = border[0] < 0.0 ? 0.0 : border[0];
        float border_right = border[1] < 0.0 ? 0.0 : border[1];
        float border_bottom = border[2] < 0.0 ? 0.0 : border[2];
        float border_left = border[3] < 0.0 ? 0.0 : border[3];
        float border_h = border_right + border_left;
        float border_v = border_top + border_bottom;

        float r_inner[4][2] = {
            {
                (r[0][0] - border_left) > 0.0f ? ((w - r[0][0] > border_right) ? (r[0][0] - border_left) : h - border_h) : 0,
                (r[0][1] - border_top) > 0.0f ? ((h - r[0][1] > border_bottom) ? (r[0][1] - border_top) : h - border_v) : 0,
            },
            {
                (r[1][0] - border_right) > 0.0f ? ((w - r[1][0] > border_left) ? (r[1][0] - border_right) : h - border_h) : 0,
                (r[1][1] - border_top) > 0.0f ? ((h - r[1][1] > border_bottom) ? (r[1][1] - border_top) : h - border_v) : 0,
            },
            {
                (r[2][0] - border_right) > 0.0f ? ((w - r[2][0] > border_left) ? (r[2][0] - border_right) : h - border_h) : 0,
                (r[2][1] - border_bottom) > 0.0f ? ((h - r[2][1] > border_top) ? (r[2][1] - border_bottom) : h - border_v) : 0,
            },
            {
                (r[3][0] - border_left) > 0.0f ? ((w - r[3][0] > border_right) ? (r[3][0] - border_left) : h - border_h) : 0,
                (r[3][1] - border_bottom) > 0.0f ? ((h - r[3][1] > border_top) ? (r[3][1] - border_bottom) : h - border_v) : 0,
            },
        };

        plutovg_path_t *inner_path = round4_rect(r_inner, border_left, border_top,
                                                 w - border_right - border_left,
                                                 h - border_top - border_bottom);
        plutovg_add_path(pluto, inner_path);

        if (box->content_image[0] != '\0')
        {
            DrawImage(pluto, box->content_image, content_rect);
        }

        plutovg_add_path(pluto, outer_path);
        plutovg_set_fill_rule(pluto, plutovg_fill_rule_even_odd);

        plutovg_set_source_rgba(pluto, border_color.r, border_color.g, border_color.b, border_color.a);

        plutovg_fill(pluto);
        plutovg_path_destroy(inner_path);
    }
    plutovg_path_destroy(outer_path);

    plutovg_restore(pluto);
}

static inline int decode_utf8(const char **begin, const char *end, int *codepoint)
{
    static const int trailing[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5};

    static const uint32_t offsets[6] = {
        0x00000000, 0x00003080, 0x000E2080, 0x03C82080, 0xFA082080, 0x82082080};

    const char *ptr = *begin;

    int trailing_bytes = trailing[(uint8_t)(*ptr)];
    if (ptr + trailing_bytes >= end)
        return 0;

    uint32_t output = 0;
    switch (trailing_bytes)
    {
    case 5:
        output += (uint8_t)(*ptr++);
        output <<= 6;
    case 4:
        output += (uint8_t)(*ptr++);
        output <<= 6;
    case 3:
        output += (uint8_t)(*ptr++);
        output <<= 6;
    case 2:
        output += (uint8_t)(*ptr++);
        output <<= 6;
    case 1:
        output += (uint8_t)(*ptr++);
        output <<= 6;
    case 0:
        output += (uint8_t)(*ptr++);
    }

    output -= offsets[trailing_bytes];
    *begin = ptr;
    *codepoint = output;
    return 1;
}

static plutovg_path_t *draw_font_get_textn_oneline_path(const plutovg_font_t *font, const char **utf8, const char *end, double w, double *out_w)
{
    plutovg_path_t *result = plutovg_path_create();
    double advance = 0;
    double scale = plutovg_font_get_scale(font);
    plutovg_font_face_t *face = plutovg_font_get_face(font);

    while (*utf8 < end)
    {
        int ch = 0;
        const char *start = *utf8;
        if (!decode_utf8(utf8, end, &ch))
            break;

        if (ch == '\n')
            break;

        plutovg_matrix_t matrix;
        plutovg_matrix_init_translate(&matrix, advance, 0);
        plutovg_matrix_scale(&matrix, scale, -scale);

        double char_advance = plutovg_font_get_char_advance(font, ch);

        if (advance + char_advance > w)
        {
            *utf8 = start;
            break;
        }

        advance += char_advance;
        plutovg_path_t *path = plutovg_font_face_get_char_path(face, ch);
        plutovg_path_add_path(result, path, &matrix);
        plutovg_path_destroy(path);
    }

    *out_w = advance;

    return result;
}

static plutovg_path_t *draw_font_get_textn_path(const plutovg_font_t *font, TEXT_ALIGN align, const char *utf8, int size, double w, double h)
{
    plutovg_path_t *result = plutovg_path_create();
    double advance = 0;
    double scale = plutovg_font_get_scale(font);
    const char *end = utf8 + size;
    plutovg_font_face_t *face = plutovg_font_get_face(font);
    double ascent = plutovg_font_get_ascent(font);
    double descent = plutovg_font_get_descent(font);
    double line_gap = plutovg_font_get_line_gap(font);
    double leading = plutovg_font_get_leading(font);

    double y = 0.0;

    while (utf8 < end)
    {
        if (y + ascent > h)
            break;

        double line_width = 0;
        plutovg_path_t *line_path = draw_font_get_textn_oneline_path(font, &utf8, end, w, &line_width);

        plutovg_matrix_t matrix;

        if (align == TEXT_ALIGN_LEFT)
            plutovg_matrix_init_translate(&matrix, 0, y);
        else if (align == TEXT_ALIGN_RIGHT)
            plutovg_matrix_init_translate(&matrix, w - line_width, y);
        else if (align == TEXT_ALIGN_CENTER)
            plutovg_matrix_init_translate(&matrix, (w - line_width) / 2.0, y);

        plutovg_path_add_path(result, line_path, &matrix);
        plutovg_path_destroy(line_path);

        y += ascent;
    }

    return result;
}

static void DrawText(struct Box *box, plutovg_t *pluto, double size, struct plutovg_color color, TEXT_ALIGN align, const char *utf8, plutovg_rect_t *rect)
{
    plutovg_save(pluto);

    plutovg_font_t *font = meui_get_font(meui_get_instance(), size);

    plutovg_set_font(pluto, font);
    double ascent = plutovg_font_get_ascent(font);

    plutovg_matrix_t matrix[1];
    plutovg_matrix_init_translate(matrix, rect->x, rect->y + ascent);

    plutovg_path_t *path = draw_font_get_textn_path(plutovg_get_font(pluto), align, utf8, strlen(utf8), rect->w, rect->h);

    plutovg_path_transform(path, matrix);
    plutovg_add_path(pluto, path);
    plutovg_path_destroy(path);

    plutovg_set_source_rgba(pluto, color.r, color.g, color.b, color.a);
    plutovg_fill(pluto);
    plutovg_restore(pluto);
}

void Flex_setBorderRadius(FlexNodeRef node, float tl, float tr, float br, float bl)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->border_radius[0] = tl;
        box->border_radius[1] = tr;
        box->border_radius[2] = br;
        box->border_radius[3] = bl;
    }
}

void Flex_setBorderColor(FlexNodeRef node, struct plutovg_color c)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->border_color = c;
    }
}

void Flex_setFillColor(FlexNodeRef node, struct plutovg_color c)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->fill_color = c;
    }
}

void Flex_setFontColor(FlexNodeRef node, struct plutovg_color c)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->font_color = c;
    }
}

void Flex_setText(FlexNodeRef node, const char *text)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        if (box->text)
            free(box->text);
        box->text = strdup(text);
    }
}

void Flex_setFontSize(FlexNodeRef node, double font_size)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->font_size = font_size;
    }
}

void Flex_setTextAlign(FlexNodeRef node, TEXT_ALIGN align)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->align = align;
    }
}

void Flex_setBackgroundImage(FlexNodeRef node, const char *background_image)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        if (box->background_image)
            free(box->background_image);
        box->background_image = strdup(background_image);
    }
}

void Flex_setContentImage(FlexNodeRef node, const char *content_image)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        if (box->content_image)
            free(box->content_image);
        box->content_image = strdup(content_image);
    }
}

void Flex_transform_matrix(FlexNodeRef node, double m00, double m10, double m01, double m11, double m02, double m12)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        plutovg_matrix_t m;
        plutovg_matrix_init(&m, m00, m10, m01, m11, m02, m12);
        plutovg_matrix_multiply(box->transform, &m, box->transform);
    }
}

void Flex_transform_translate(FlexNodeRef node, double x, double y)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        plutovg_matrix_t m;
        plutovg_matrix_init_translate(&m, x, y);
        plutovg_matrix_multiply(box->transform, &m, box->transform);
    }
}

void Flex_transform_rotate(FlexNodeRef node, double radians)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        plutovg_matrix_t m;
        plutovg_matrix_init_rotate(&m, radians);
        plutovg_matrix_multiply(box->transform, &m, box->transform);
    }
}

void Flex_transform_skew(FlexNodeRef node, double x, double y)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        plutovg_matrix_t m;
        plutovg_matrix_init_shear(&m, x, y);
        plutovg_matrix_multiply(box->transform, &m, box->transform);
    }
}

void Flex_transform_origin_keyword(FlexNodeRef node, TRANSFORM_ORIGIN x, TRANSFORM_ORIGIN y)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->transform_origin.type = TRANSFORM_ORIGIN_TYPE_KEYWORD;
        box->transform_origin.x.keyword = x;
        box->transform_origin.y.keyword = y;
    }
}

void Flex_transform_origin_offset(FlexNodeRef node, double x, double y)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->transform_origin.type = TRANSFORM_ORIGIN_TYPE_OFFSET;
        box->transform_origin.x.offset = x;
        box->transform_origin.y.offset = y;
    }
}

static void box_transform_by_origin(struct Box *box, plutovg_t *pluto, plutovg_rect_t *rect)
{
    double x_off, y_off;
    if (box->transform_origin.type == TRANSFORM_ORIGIN_TYPE_KEYWORD)
    {
        x_off = rect->w * box->transform_origin.x.keyword / 2.0;
        y_off = rect->h * box->transform_origin.y.keyword / 2.0;
    }
    else if (box->transform_origin.type == TRANSFORM_ORIGIN_TYPE_OFFSET)
    {
        x_off = box->transform_origin.x.offset;
        y_off = box->transform_origin.y.offset;
    }

    plutovg_translate(pluto, rect->x + x_off, rect->y + y_off);
    plutovg_transform(pluto, box->transform);
    plutovg_translate(pluto, -x_off, -y_off);
}

void Flex_drawNode(FlexNodeRef node, float x, float y)
{
    float left = Flex_getResultLeft(node);
    float top = Flex_getResultTop(node);
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);

    struct Box *box = Flex_getContext(node);

    double content_width = width - Flex_getResultPaddingLeft(node) - Flex_getResultPaddingRight(node);
    double content_height = height - Flex_getResultPaddingTop(node) - Flex_getResultPaddingBottom(node);
    double content_left = Flex_getResultPaddingLeft(node);
    double content_top = Flex_getResultPaddingTop(node);

    if (box)
    {
        plutovg_t *pluto = plutovg_create(meui_get_surface(meui_get_instance()));

        box_transform_by_origin(box, pluto, &(plutovg_rect_t){x + left, y + top, width, height});

        plutovg_rect_t content_rect = {content_left, content_top, content_width, content_height};
        DrawBoxBackground(box, pluto, &(plutovg_rect_t){x, y, width, height},
                          &content_rect,
                          box->border_radius,
                          (float[]){
                              Flex_getBorderTop(node),
                              Flex_getBorderRight(node),
                              Flex_getBorderBottom(node),
                              Flex_getBorderLeft(node),
                          },
                          box->fill_color, box->border_color);

        if (box->text[0] != '\0')
            DrawText(box, pluto, box->font_size, box->font_color, box->align, box->text, &content_rect);

        plutovg_destroy(pluto);
    }

    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        Flex_drawNode(Flex_getChild(node, i), x + left, y + top);
    }
}