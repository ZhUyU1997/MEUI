/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <plutovg.h>
#include <stdint.h>
#include <string.h>

#include "meui/box.h"
#include "meui.h"

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

plutovg_path_t *get_textn_oneline_path(const plutovg_font_t *font, const char **utf8, const char *end, double w, double *out_w)
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

plutovg_rect_t measure_textn_oneline_path(const plutovg_font_t *font, const char **utf8, const char *end, double w, double *out_w)
{
    double advance = 0;
    plutovg_rect_t text_rect;
    plutovg_rect_init_empty(&text_rect);

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

        plutovg_rect_t rect;
        plutovg_font_get_char_extents(font, ch, &rect);
        rect.x = advance;

        plutovg_rect_unite(&text_rect, &rect);

        advance += char_advance;
    }

    *out_w = advance;

    return text_rect;
}

FlexSize measure_font_get_textn_path(const plutovg_font_t *font, TEXT_ALIGN align, const char *utf8, int size, double w, double h)
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
        measure_textn_oneline_path(font, &utf8, end, w, &line_width);

        if (line_width > outSize.width)
        {
            outSize.width = line_width + 1; // + 1 to fix precision
        }
    }
    outSize.height = y - descent;

    return outSize;
}

static void draw_char(plutovg_t *pluto, const char *fontFamily, double fontSize, double w, double h, int ch)
{
    plutovg_surface_t *text = meui_text_cache_load(fontFamily, ch, fontSize);

    if (!text)
    {
        printf("Unable to load text: [%c]\n", ch);
        return;
    }

    plutovg_rect(pluto, 0, 0, w, h);
    plutovg_set_source_surface(pluto, text, 0, 0);
    plutovg_fill(pluto);
    plutovg_surface_destroy(text);
}

static void draw_oneline(plutovg_t *pluto, const char *fontFamily, double fontSize, TEXT_ALIGN align, const char **utf8, const char *end, double w)
{
    double advance = 0;
    const plutovg_font_t *font = plutovg_get_font(pluto);
    double scale = plutovg_font_get_scale(font);
    plutovg_font_face_t *face = plutovg_font_get_face(font);
    double ascent = plutovg_font_get_ascent(font);
    double descent = plutovg_font_get_descent(font);
    const char *utf8_origin = *utf8;

    double line_width = 0;
    plutovg_rect_t rect = measure_textn_oneline_path(font, &utf8_origin, end, w, &line_width);

    plutovg_matrix_t origin;
    plutovg_get_matrix(pluto, &origin);

    if (align & TEXT_ALIGN_LEFT)
        plutovg_translate(pluto, 0, 0);
    else if (align & TEXT_ALIGN_RIGHT)
        plutovg_translate(pluto, w - line_width, 0);
    else if (align & TEXT_ALIGN_CENTER_H)
        plutovg_translate(pluto, (w - line_width) / 2.0, 0);

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
        draw_char(pluto, fontFamily, fontSize, char_advance, ascent - descent, ch);
        plutovg_translate(pluto, char_advance, 0);
    }

    plutovg_set_matrix(pluto, &origin);
}

static void draw_textn(plutovg_t *pluto, const char *fontFamily, double fontSize, TEXT_ALIGN align, const char *utf8, int size, double w, double h)
{
    double advance = 0;
    const plutovg_font_t *font = plutovg_get_font(pluto);
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

        draw_oneline(pluto, fontFamily, fontSize, align, &utf8, end, w);
        plutovg_translate(pluto, 0, ascent);
    }
}

void draw_text(Box *box, plutovg_t *pluto, const char *fontFamily, double fontSize, plutovg_color_t *color, TEXT_ALIGN align, const char *utf8, plutovg_rect_t *rect)
{
    plutovg_save(pluto);

    plutovg_font_t *font = meui_get_font(meui_get_instance(), fontFamily, fontSize);
    plutovg_set_font(pluto, font);
    double ascent = plutovg_font_get_ascent(font);
    double text_h = 0.0;

    FlexSize size = measure_font_get_textn_path(font, align, utf8, strlen(utf8), rect->w, rect->h);

    if (align & TEXT_ALIGN_TOP)
        plutovg_translate(pluto, rect->x, rect->y);
    else if (align & TEXT_ALIGN_BOTTOM)
        plutovg_translate(pluto, rect->x, size.height >= rect->h ? rect->y : rect->y + rect->h - size.height);
    else if (align & TEXT_ALIGN_CENTER_V)
        plutovg_translate(pluto, rect->x, size.height >= rect->h ? rect->y : rect->y + (rect->h - size.height) / 2.0);

    plutovg_set_source_color(pluto, color);

    draw_textn(pluto, fontFamily, fontSize, align, utf8, strlen(utf8), rect->w, rect->h);

    plutovg_font_destroy(font);
    plutovg_restore(pluto);
}