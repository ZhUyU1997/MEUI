#include "box.h"
#include "image.h"
#include "meui.h"
#include "log.h"
#include "list.h"

#include <FlexLayout.h>
#include <plutovg.h>
#include <plutosvg.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

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

box_t box_new()
{
    box_t node = Flex_newNode();
    struct Box *box = calloc(1, sizeof(struct Box));
    box->style_array[BOX_STATE_DEFAULT] = box_style_new();

    init_list_head(&box->event_list);

    Flex_setContext(node, box);
    return node;
}

void box_free(box_t node)
{
    struct Box *box = Flex_getContext(node);

    if (box)
    {
        free(box);
        Flex_setContext(node, NULL);
    }
    Flex_freeNode(node);
}

void box_free_recursive(box_t node)
{
    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        box_free_recursive(Flex_getChild(node, i));
    }
    box_free(node);
}

void box_set_style(box_t node, box_style_t *style, enum BOX_STATE state)
{
    assert(node && style);

    struct Box *box = Flex_getContext(node);
    box->style_array[state] = style;
}

void box_set_state(box_t node, enum BOX_STATE state)
{
    struct Box *box = Flex_getContext(node);
    box->state = state;
}

void box_default_style_border_radius(box_t node, float tl, float tr, float br, float bl)
{
    struct Box *box = Flex_getContext(node);
    box_style_border_radius(box->style_array[BOX_STATE_DEFAULT], tl, tr, br, bl);
}
void box_default_style_border_color(box_t node, plutovg_color_t c)
{
    struct Box *box = Flex_getContext(node);
    box_style_border_color(box->style_array[BOX_STATE_DEFAULT], c);
}
void box_default_style_fill_color(box_t node, plutovg_color_t c)
{
    struct Box *box = Flex_getContext(node);
    box_style_fill_color(box->style_array[BOX_STATE_DEFAULT], c);
}
void box_default_style_font_color(box_t node, plutovg_color_t c)
{
    struct Box *box = Flex_getContext(node);
    box_style_font_color(box->style_array[BOX_STATE_DEFAULT], c);
}
void box_default_style_text(box_t node, const char *text)
{
    struct Box *box = Flex_getContext(node);
    box_style_text(box->style_array[BOX_STATE_DEFAULT], text);
}
void box_default_style_font_size(box_t node, double font_size)
{
    struct Box *box = Flex_getContext(node);
    box_style_font_size(box->style_array[BOX_STATE_DEFAULT], font_size);
}
void box_default_style_text_align(box_t node, TEXT_ALIGN align)
{
    struct Box *box = Flex_getContext(node);
    box_style_text_align(box->style_array[BOX_STATE_DEFAULT], align);
}
void box_default_style_background_image(box_t node, const char *background_image)
{
    struct Box *box = Flex_getContext(node);
    box_style_background_image(box->style_array[BOX_STATE_DEFAULT], background_image);
}
void box_default_style_content_image(box_t node, const char *content_image)
{
    struct Box *box = Flex_getContext(node);
    box_style_content_image(box->style_array[BOX_STATE_DEFAULT], content_image);
}
void box_default_style_transform_matrix(box_t node, double m00, double m10, double m01, double m11, double m02, double m12)
{
    struct Box *box = Flex_getContext(node);
    box_style_transform_matrix(box->style_array[BOX_STATE_DEFAULT], m00, m10, m01, m11, m02, m12);
}
void box_default_style_transform_translate(box_t node, double x, double y)
{
    struct Box *box = Flex_getContext(node);
    box_style_transform_translate(box->style_array[BOX_STATE_DEFAULT], x, y);
}
void box_default_style_transform_rotate(box_t node, double radians)
{
    struct Box *box = Flex_getContext(node);
    box_style_transform_rotate(box->style_array[BOX_STATE_DEFAULT], radians);
}
void box_default_style_transform_scale(box_t node, double x, double y)
{
    struct Box *box = Flex_getContext(node);
    box_style_transform_scale(box->style_array[BOX_STATE_DEFAULT], x, y);
}
void box_default_style_transform_skew(box_t node, double x, double y)
{
    struct Box *box = Flex_getContext(node);
    box_style_transform_skew(box->style_array[BOX_STATE_DEFAULT], x, y);
}
void box_default_style_transform_origin_keyword(box_t node, TRANSFORM_ORIGIN x, TRANSFORM_ORIGIN y)
{
    struct Box *box = Flex_getContext(node);
    box_style_transform_origin_keyword(box->style_array[BOX_STATE_DEFAULT], x, y);
}
void box_default_style_transform_origin_offset(box_t node, double x, double y)
{
    struct Box *box = Flex_getContext(node);
    box_style_transform_origin_offset(box->style_array[BOX_STATE_DEFAULT], x, y);
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

static void draw_image(plutovg_t *pluto, const char *path, plutovg_rect_t *r)
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

static void draw_box_background(struct Box *box, plutovg_t *pluto, plutovg_rect_t *rect, plutovg_rect_t *content_rect, float radius[4], float border[4], struct plutovg_color border_color, struct plutovg_color fill_color)
{
    float r[4][2] = {0};

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

    if (box->style.background_image[0] != '\0')
    {
        draw_image(pluto, box->style.background_image, rect);
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

        if (box->style.content_image[0] != '\0')
        {
            draw_image(pluto, box->style.content_image, content_rect);
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

static plutovg_path_t *draw_font_get_textn_path(const plutovg_font_t *font, TEXT_ALIGN align, const char *utf8, int size, double w, double h, double *text_h)
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

    double y = 0;

    while (utf8 < end)
    {
        if (y + ascent > h)
            break;

        y += ascent;

        double line_width = 0;
        plutovg_path_t *line_path = draw_font_get_textn_oneline_path(font, &utf8, end, w, &line_width);

        plutovg_matrix_t matrix;

        if (align & TEXT_ALIGN_LEFT)
            plutovg_matrix_init_translate(&matrix, 0, y);
        else if (align & TEXT_ALIGN_RIGHT)
            plutovg_matrix_init_translate(&matrix, w - line_width, y);
        else if (align & TEXT_ALIGN_CENTER_H)
            plutovg_matrix_init_translate(&matrix, (w - line_width) / 2.0, y);

        plutovg_path_add_path(result, line_path, &matrix);
        plutovg_path_destroy(line_path);

    }

    *text_h = y - descent;

    return result;
}

static void draw_text(struct Box *box, plutovg_t *pluto, double size, struct plutovg_color color, TEXT_ALIGN align, const char *utf8, plutovg_rect_t *rect)
{
    plutovg_save(pluto);

    plutovg_font_t *font = meui_get_font(meui_get_instance(), size);

    plutovg_set_font(pluto, font);
    double ascent = plutovg_font_get_ascent(font);
    double text_h = 0.0;

    plutovg_path_t *path = draw_font_get_textn_path(plutovg_get_font(pluto), align, utf8, strlen(utf8), rect->w, rect->h, &text_h);

    plutovg_matrix_t matrix[1];
    // LOGI($(rect->w) " "$(text_h));
    if (align & TEXT_ALIGN_TOP)
        plutovg_matrix_init_translate(matrix, rect->x, rect->y);
    else if (align & TEXT_ALIGN_BOTTOM)
        plutovg_matrix_init_translate(matrix, rect->x, text_h >= rect->h ? rect->y : rect->y + rect->h - text_h);
    else if (align & TEXT_ALIGN_CENTER_V)
        plutovg_matrix_init_translate(matrix, rect->x, text_h >= rect->h ? rect->y : rect->y + (rect->h - text_h) / 2.0);

    plutovg_path_transform(path, matrix);
    plutovg_add_path(pluto, path);
    plutovg_path_destroy(path);

    plutovg_set_source_rgba(pluto, color.r, color.g, color.b, color.a);
    plutovg_fill(pluto);
    plutovg_restore(pluto);
}

void box_add_event_listener(box_t node, enum MEUI_EVENT_TYPE type, box_event_cb_t cb)
{
    struct Box *box = Flex_getContext(node);
    if (!box)
        return;

    box_event_dsc_t *dsc = malloc(sizeof(box_event_dsc_t));

    if (!dsc)
        return;

    init_list_head(&dsc->node);
    dsc->type = type;
    dsc->cb = cb;

    list_add_tail(&dsc->node, &box->event_list);
}

void box_dispatch_event(box_t node, enum MEUI_EVENT_TYPE type, meui_event_t *e)
{
    struct Box *box = Flex_getContext(node);
    if (!box)
        return;

    box_event_dsc_t *i;
    list_for_each_entry(i, &box->event_list, node)
    {
        if (i->type == type && i->cb)
        {
            box_event_t event = {
                .e = *e,
                .type = type,
                .target = node,
                .currentTarget = node,
            };

            i->cb(&event);
        }
    }
}

static void box_transform_by_origin(struct Box *box, plutovg_t *pluto, plutovg_rect_t *rect)
{
    double x_off = 0.0, y_off = 0.0;
    if (box->style.transform_origin.type == TRANSFORM_ORIGIN_TYPE_KEYWORD)
    {
        x_off = rect->w * box->style.transform_origin.x.keyword / 2.0;
        y_off = rect->h * box->style.transform_origin.y.keyword / 2.0;
    }
    else if (box->style.transform_origin.type == TRANSFORM_ORIGIN_TYPE_OFFSET)
    {
        x_off = box->style.transform_origin.x.offset;
        y_off = box->style.transform_origin.y.offset;
    }

    plutovg_translate(pluto, rect->x + x_off, rect->y + y_off);
    plutovg_transform(pluto, box->style.transform);
    plutovg_translate(pluto, -x_off, -y_off);
}

static void merge_styles(struct Box *box)
{
    box->style = *(box->style_array[BOX_STATE_DEFAULT]);

    if (box->state != BOX_STATE_DEFAULT)
    {
        box_style_t *src = box->style_array[box->state];
        box_style_t *dst = &box->style;
        if (src)
            box_merge_styles(dst, src);
    }
}
void box_drawRecursive(plutovg_t *pluto, box_t node)
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

    if (!box)
    {
        LOGE("Node is not box!");
        return;
    }

    merge_styles(box);

    box_transform_by_origin(box, pluto, &(plutovg_rect_t){left, top, width, height});

    plutovg_get_matrix(pluto, &box->result.to_screen_matrix);

    plutovg_rect_t content_rect = {content_left, content_top, content_width, content_height};
    draw_box_background(box, pluto, &(plutovg_rect_t){0, 0, width, height},
                        &content_rect,
                        box->style.border_radius,
                        (float[]){
                            Flex_getBorderTop(node),
                            Flex_getBorderRight(node),
                            Flex_getBorderBottom(node),
                            Flex_getBorderLeft(node),
                        },
                        box->style.border_color, box->style.fill_color);

    if (box->style.text[0] != '\0')

        draw_text(box, pluto, box->style.font_size, box->style.font_color, box->style.align, box->style.text, &content_rect);

    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        plutovg_save(pluto);
        box_drawRecursive(pluto, Flex_getChild(node, i));
        plutovg_restore(pluto);
    }
}

void box_draw(box_t root)
{
    plutovg_t *pluto = plutovg_create(meui_get_surface(meui_get_instance()));
    box_drawRecursive(pluto, root);
    plutovg_destroy(pluto);
}