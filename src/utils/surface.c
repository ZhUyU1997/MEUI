/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <plutovg.h>
#include <stdint.h>
#include <stdbool.h>

static inline void memfill32(uint32_t *dest, uint32_t value, int length)
{
    for (int i = 0; i < length; i++)
        dest[i] = value;
}

static inline uint32_t combine_opacity(const plutovg_color_t *color, double opacity)
{
    uint32_t a = (uint8_t)(color->a * opacity * 255);
    uint32_t r = (uint8_t)(color->r * 255);
    uint32_t g = (uint8_t)(color->g * 255);
    uint32_t b = (uint8_t)(color->b * 255);

    return (a << 24) | (r << 16) | (g << 8) | (b);
}

void plutovg_surface_fill(const plutovg_surface_t *s, const plutovg_color_t color)
{
    unsigned char *data = plutovg_surface_get_data(s);
    int width = plutovg_surface_get_width(s);
    int height = plutovg_surface_get_height(s);
    int stride = plutovg_surface_get_stride(s);

    uint32_t ucolor = combine_opacity(&color, 1);
    for (int y = 0; y < height; y++)
    {
        uint32_t *dest = (uint32_t *)(data + stride * y);
        for (int x = 0; x < width; x++)
        {
            memfill32(dest, ucolor, width);
        }
    }
}

plutovg_rect_t plutovg_surface_get_rect(plutovg_surface_t *surface)
{
    return (plutovg_rect_t){0, 0, plutovg_surface_get_width(surface), plutovg_surface_get_height(surface)};
}

plutovg_rect_t plutovg_surface_get_rect_ext(plutovg_surface_t *surface, int ext_width)
{
    return (plutovg_rect_t){0 - ext_width, 0 - ext_width, plutovg_surface_get_width(surface) + 2 * ext_width, plutovg_surface_get_height(surface) + 2 * ext_width};
}

void plutovg_rect_ext(plutovg_rect_t *rect, int ext_width)
{
    rect->x -= ext_width;
    rect->y -= ext_width;
    rect->w += 2 * ext_width;
    rect->h += 2 * ext_width;
}

bool plutovg_rect_equal(plutovg_rect_t *a, plutovg_rect_t *b)
{
    return a->x == b->x &&
           a->y == b->y &&
           a->w == b->w &&
           a->h == b->h;
}