/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <plutovg.h>
#include <stdint.h>

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