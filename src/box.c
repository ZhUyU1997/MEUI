#include "box.h"
#include "element.h"
#include "image.h"
#include "meui.h"
#include "log.h"
#include "list.h"
#include "pqueue.h"

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

static void box_draw_self(Box *box, plutovg_t *pluto);

class_impl(Box){
    .draw = box_draw_self,
};

constructor(Box)
{
    this->node = Flex_newNode();
    this->style_array[BOX_STATE_DEFAULT] = box_style_new();
    init_list_head(&this->event_list);
    Flex_setContext(this->node, this);
}

destructor(Box)
{
    for (int i = 0; i < BOX_STATE_MAX; i++)
    {
        if (this->style_array[i])
        {
            box_style_free(this->style_array[i]);
            this->style_array[i] = NULL;
        }

        box_style_clear(&this->style);
    }
    Flex_setContext(this->node, NULL);
    Flex_freeNode(this->node);
}

box_t box_new(enum BOX_TYPE type)
{
    Box *box = NULL;
    if (type == BOX_TYPE_DIV)
    {
        box = dynamic_cast(Box)(new (DivEle));
    }
    else if (type == BOX_TYPE_STACK)
    {
        box = dynamic_cast(Box)(new (StackEle));
    }
    else if (type == BOX_TYPE_CANVAS)
    {
        box = dynamic_cast(Box)(new (CanvasEle));
    }

    return box->node;
}

void box_free(box_t node)
{
    Box *box = Flex_getContext(node);
    delete (box);
}

void box_free_recursive(box_t node)
{
    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        box_free_recursive(Flex_getChild(node, i));
    }
    box_free(node);
}

box_style_t *box_get_style(box_t node, enum BOX_STATE state)
{
    assert(node);

    Box *box = Flex_getContext(node);
    return box->style_array[state];
}

void box_set_style(box_t node, box_style_t *style, enum BOX_STATE state)
{
    assert(node && style);

    Box *box = Flex_getContext(node);
    box->style_array[state] = style;
}

enum BOX_STATE box_get_state(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->state;
}

void box_set_state(box_t node, enum BOX_STATE state)
{
    Box *box = Flex_getContext(node);
    box->state = state;
}

int box_get_client_width(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->client_width;
}

int box_get_client_height(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->client_height;
}

int box_get_scroll_width(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->scroll_width;
}

int box_get_scroll_height(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->scroll_height;
}

int box_get_scroll_top(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->scroll_top;
}

void box_set_scroll_top(box_t node, int scroll_top)
{
    Box *box = Flex_getContext(node);

    if (scroll_top < 0)
        scroll_top = 0;
    else if (scroll_top > box->scroll_height - box->client_height)
        scroll_top = box->scroll_height - box->client_height;

    box->scroll_top = scroll_top;
}

int box_get_scroll_left(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->scroll_left;
}

void box_set_scroll_left(box_t node, int scroll_left)
{
    Box *box = Flex_getContext(node);

    if (scroll_left < 0)
        scroll_left = 0;
    else if (scroll_left > box->scroll_width - box->client_width)
        scroll_left = box->scroll_width - box->client_width;

    box->scroll_left = scroll_left;
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

    plutovg_surface_destroy(img);
    plutovg_restore(pluto);
}

static void draw_debug_rect(plutovg_t *pluto, double x, double y, double w, double h, struct plutovg_color fill_color)
{
    plutovg_save(pluto);
    plutovg_rect(pluto, x, y, w, h);
    plutovg_set_source_color(pluto, &fill_color);
    plutovg_fill(pluto);
    plutovg_restore(pluto);
}

static void draw_box_background(Box *box, plutovg_t *pluto, plutovg_rect_t *rect, plutovg_rect_t *content_rect, float radius[4], float border[4], struct plutovg_color border_color, struct plutovg_color fill_color)
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
    plutovg_set_source_color(pluto, &fill_color);

    plutovg_fill_preserve(pluto);

    if (box->style.backgroundImage && box->style.backgroundImage[0] != '\0')
    {
        draw_image(pluto, box->style.backgroundImage, rect);
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

        if (box->style.contentImage && box->style.contentImage[0] != '\0')
        {
            draw_image(pluto, box->style.contentImage, content_rect);
        }

        plutovg_add_path(pluto, outer_path);
        plutovg_set_fill_rule(pluto, plutovg_fill_rule_even_odd);

        plutovg_set_source_color(pluto, &border_color);

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

static void draw_text(Box *box, plutovg_t *pluto, const char *fontFamily, double size, struct plutovg_color color, TEXT_ALIGN align, const char *utf8, plutovg_rect_t *rect)
{
    plutovg_save(pluto);

    plutovg_font_t *font = meui_get_font(meui_get_instance(), fontFamily, size);

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

    plutovg_set_source_color(pluto, &color);
    plutovg_fill(pluto);

    plutovg_font_destroy(font);
    plutovg_restore(pluto);
}

static void measure_font_get_textn_oneline_path(const plutovg_font_t *font, const char **utf8, const char *end, double w, double *out_w)
{
    double advance = 0;
    while (*utf8 < end)
    {
        int ch = 0;
        const char *start = *utf8;
        if (!decode_utf8(utf8, end, &ch))
            break;

        if (ch == '\n')
            break;

        double char_advance = plutovg_font_get_char_advance(font, ch);

        if (advance + char_advance > w)
        {
            *utf8 = start;
            break;
        }

        advance += char_advance;
    }

    *out_w = advance;
}

static FlexSize measure_font_get_textn_path(const plutovg_font_t *font, TEXT_ALIGN align, const char *utf8, int size, double w, double h)
{
    FlexSize outSize = {.width = 0, .height = 0};

    double advance = 0;
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
        draw_font_get_textn_oneline_path(font, &utf8, end, w, &line_width);

        if (line_width > outSize.width)
        {
            outSize.width = line_width + 1; // + 1 to fix precision
        }
    }
    outSize.height = y - descent;

    return outSize;
}

FlexSize box_measure_text(void *context, FlexSize constrainedSize)
{
    Box *box = context;

    const char *fontFamily = box->style.fontFamily;
    double size = box->style.fontSize;
    TEXT_ALIGN align = box->style.textAlign;
    const char *utf8 = box->style.text;
    if (utf8 && utf8[0] != '\0')
    {
        plutovg_font_t *font = meui_get_font(meui_get_instance(), fontFamily, size);
        FlexSize outSize = measure_font_get_textn_path(font, align, utf8, strlen(utf8), constrainedSize.width, constrainedSize.height);
        return outSize;
    }
    return (FlexSize){.width = constrainedSize.width, .height = 0};
}

FlexSize box_stack_layout(FlexNodeRef node, float constrainedWidth, float constrainedHeight, float scale)
{
    Box *box = Flex_getContext(node);

    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        FlexNodeRef item = Flex_getChild(node, i);
        Flex_layout(item, FlexUndefined, FlexUndefined, scale);
        Box *childBox = Flex_getContext(item);
        float left = flex_resolve(childBox->style.left, NULL, constrainedWidth);
        float right = flex_resolve(childBox->style.right, NULL, constrainedWidth);
        float top = flex_resolve(childBox->style.top, NULL, constrainedHeight);
        float bottom = flex_resolve(childBox->style.bottom, NULL, constrainedHeight);

        if (FlexIsResolved(right))
            Flex_setResultLeft(item, constrainedWidth - Flex_getResultWidth(item) - right);

        if (FlexIsResolved(bottom))
            Flex_setResultTop(item, constrainedHeight - Flex_getResultHeight(item) - bottom);

        // below code with high priority
        if (FlexIsResolved(left))
            Flex_setResultLeft(item, left);

        if (FlexIsResolved(top))
            Flex_setResultTop(item, top);
    }
    return (FlexSize){.width = constrainedWidth, .height = constrainedHeight};
}

void box_add_event_listener(box_t node, enum MEUI_EVENT_TYPE type, box_event_cb_t cb)
{
    Box *box = Flex_getContext(node);
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
    Box *box = Flex_getContext(node);
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

static void box_transform_by_origin(Box *box, plutovg_t *pluto, plutovg_rect_t *rect)
{
    double x_off = 0.0, y_off = 0.0;
    if (box->style.transformOrigin.type == TRANSFORM_ORIGIN_TYPE_KEYWORD)
    {
        x_off = rect->w * box->style.transformOrigin.x.keyword / 2.0;
        y_off = rect->h * box->style.transformOrigin.y.keyword / 2.0;
    }
    else if (box->style.transformOrigin.type == TRANSFORM_ORIGIN_TYPE_OFFSET)
    {
        x_off = box->style.transformOrigin.x.offset;
        y_off = box->style.transformOrigin.y.offset;
    }

    plutovg_translate(pluto, rect->x + x_off, rect->y + y_off);
    plutovg_transform(pluto, &box->style.transform);
    plutovg_translate(pluto, -x_off, -y_off);
}

static void merge_styles(Box *box)
{
    box_style_clear(&box->style);
    box_style_merge(&box->style, box_default_style());
    box_style_merge(&box->style, box->style_array[BOX_STATE_DEFAULT]);

    if (box->state != BOX_STATE_DEFAULT)
    {
        box_style_t *src = box->style_array[box->state];
        if (src)
            box_style_merge(&box->style, src);
    }
}

void box_updateStyleRecursive(box_t node)
{

    Box *box = Flex_getContext(node);
    if (!box)
    {
        LOGE("Node is not box!");
        return;
    }

    merge_styles(box);
    box_style_to_flex(&box->style, node);

    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        box_updateStyleRecursive(Flex_getChild(node, i));
    }
}

static void box_draw_self(Box *box, plutovg_t *pluto)
{
    box_t node = box->node;

    float left = Flex_getResultLeft(node);
    float top = Flex_getResultTop(node);
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);

    double content_width = width - Flex_getResultPaddingLeft(node) - Flex_getResultPaddingRight(node);
    double content_height = height - Flex_getResultPaddingTop(node) - Flex_getResultPaddingBottom(node);
    double content_left = Flex_getResultPaddingLeft(node);
    double content_top = Flex_getResultPaddingTop(node);

    plutovg_rect_t content_rect = {content_left, content_top, content_width, content_height};

    draw_box_background(box, pluto, &(plutovg_rect_t){0, 0, width, height},
                        &content_rect,
                        (float[]){
                            box->style.borderTopLeftRadius,
                            box->style.borderTopRightRadius,
                            box->style.borderBottomRightRadius,
                            box->style.borderBottomLeftRadius,
                        },
                        (float[]){
                            Flex_getResultBorderTop(node),
                            Flex_getResultBorderRight(node),
                            Flex_getResultBorderBottom(node),
                            Flex_getResultBorderLeft(node),
                        },
                        box->style.borderColor, box->style.backgroundColor);

    if (box->style.text && box->style.text[0] != '\0')
        draw_text(box, pluto, box->style.fontFamily, box->style.fontSize, box->style.fontColor, box->style.textAlign, box->style.text, &content_rect);
}

static void box_draw_child(box_t node, plutovg_t *pluto, pqueue_t *pq)
{
    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        box_t child = Flex_getChild(node, i);
        Box *box = Flex_getContext(child);

        if (!box_style_is_unset(&box->style, BOX_STYLE_zIndex))
        {
            pqueue_insert(pq, child);
            continue;
        }
        plutovg_save(pluto);
        box_drawRecursive(pluto, Flex_getChild(node, i), pq);
        plutovg_restore(pluto);
    }
}

void box_drawRecursive(plutovg_t *pluto, box_t node, pqueue_t *pq)
{
    float left = Flex_getResultLeft(node);
    float top = Flex_getResultTop(node);
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);

    Box *box = Flex_getContext(node);

    if (!box)
    {
        LOGE("Node is not box!");
        return;
    }

    box_transform_by_origin(box, pluto, &(plutovg_rect_t){left, top, width, height});

    plutovg_get_matrix(pluto, &box->result.to_screen_matrix);

    if (box->draw)
        box->draw(box, pluto);

    box->client_width = width - Flex_getResultBorderLeft(node) - Flex_getResultBorderRight(node);
    box->client_height = height - Flex_getResultBorderTop(node) - Flex_getResultBorderBottom(node);
    box->offset_width = width;
    box->offset_height = height;
    box->scroll_width = Flex_getResultScrollWidth(node);
    box->scroll_height = Flex_getResultScrollHeight(node);

    if (box->style.overflow == CSS_OVERFLOW_VISIBLE || (box->client_width >= box->scroll_width && box->client_height >= box->scroll_height))
    {
        box_draw_child(node, pluto, pq);
    }
    else
    {
        plutovg_save(pluto);
        // printf("%f %f\n", box->client_width, box->client_height);
        plutovg_surface_t *surface = plutovg_surface_create(box->client_width, box->client_height);
        plutovg_translate(pluto, Flex_getResultBorderLeft(node), Flex_getResultBorderTop(node));

        {
            plutovg_t *pluto = plutovg_create(surface);
            plutovg_translate(pluto, -box->scroll_left - Flex_getResultBorderLeft(node), -box->scroll_top - Flex_getResultBorderTop(node));
            box_draw_child(node, pluto, pq);
            plutovg_destroy(pluto);
        }

        plutovg_rect(pluto, 0, 0, box->client_width, box->client_height);
        plutovg_set_source_surface(pluto, surface, 0, 0);
        plutovg_fill(pluto);

        plutovg_surface_destroy(surface);

        plutovg_restore(pluto);
    }

    // if (Flex_getResultScrollWidth(node) != meui_get_instance()->width && Flex_getResultScrollHeight(node) != meui_get_instance()->height)
    //     draw_debug_rect(pluto, Flex_getResultBorderLeft(node) - box->scroll_left, Flex_getResultBorderTop(node) - box->scroll_top, box->client_width, box->scroll_height, (plutovg_color_t){1, 0, 0, 0.5});
}

static int cmp_pri(pqueue_pri_t next, pqueue_pri_t curr)
{
    return (next >= curr);
}

static pqueue_pri_t get_pri(void *a)
{
    Box *box = Flex_getContext((box_t)a);
    return box->style.zIndex;
}

static void set_pri(void *a, pqueue_pri_t pri)
{
    Box *box = Flex_getContext((box_t)a);
    box->style.zIndex = pri;
}

static size_t get_pos(void *a)
{
    Box *box = Flex_getContext((box_t)a);

    return box->queue_pos;
}

static void set_pos(void *a, size_t pos)
{
    Box *box = Flex_getContext((box_t)a);
    box->queue_pos = pos;
}

void box_drawRecursiveQueue(plutovg_t *pluto, box_t root)
{
    pqueue_t *pq = pqueue_init(10, cmp_pri, get_pri, set_pri, get_pos, set_pos);
    box_drawRecursive(pluto, root, pq);

    if (pqueue_peek(pq) != NULL)
    {
        box_t box = NULL;
        while ((box = pqueue_pop(pq)))
        {
            box_t parent = Flex_getParent(box);
            Box *parentBox = Flex_getContext(parent);

            plutovg_save(pluto);

            plutovg_set_matrix(pluto, &parentBox->result.to_screen_matrix);
            box_drawRecursiveQueue(pluto, box);
            plutovg_restore(pluto);
        }
    }
    pqueue_free(pq);
}

void box_draw(box_t root)
{
    plutovg_t *pluto = plutovg_create(meui_get_surface(meui_get_instance()));
    box_drawRecursiveQueue(pluto, root);
    plutovg_destroy(pluto);
}