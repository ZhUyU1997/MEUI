/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "plutovg-private.h"
#include "plutovg-color.h"

#include <limits.h>
#include <math.h>

static void composition_solid_source(void *_dest, int length, plutovg_color32_t color, uint32_t const_alpha)
{
    plutovg_color16_t *dest = (plutovg_color16_t *)_dest;
    plutovg_color16_t color16 = plutovg_color32_to_color16(color);
    if (const_alpha == 255)
    {
        memfill16(dest, color16, length);
    }
    else
    {
        uint32_t ialpha = 255 - const_alpha;
        color16 = BYTE_MUL_16(color16, const_alpha);
        for (int i = 0; i < length; i++)
            dest[i] = plutovg_bytes_to_color16(color16.full + BYTE_MUL_16(dest[i], ialpha).full);
    }
}

static void composition_solid_source_over(void *_dest, int length, plutovg_color32_t color, uint32_t const_alpha)
{
    plutovg_color16_t *dest = (plutovg_color16_t *)_dest;
    plutovg_color16_t color16 = plutovg_color32_to_color16(color);

    if (const_alpha != 255)
        color16 = BYTE_MUL_16(color16, const_alpha);
    uint32_t ialpha = 255 - plutovg_alpha(color);
    for (int i = 0; i < length; i++)
        dest[i] = plutovg_bytes_to_color16(color16.full + BYTE_MUL_16(dest[i], ialpha).full);
}

static void composition_solid_destination_in(void *_dest, int length, plutovg_color32_t color, uint32_t const_alpha)
{
    plutovg_color16_t *dest = (plutovg_color16_t *)_dest;
    uint32_t a = plutovg_alpha(color);
    if (const_alpha != 255)
        a = BYTE_ALPHA_MUL(a, const_alpha) + 255 - const_alpha;
    for (int i = 0; i < length; i++)
        dest[i] = BYTE_MUL_16(dest[i], a);
}

static void composition_solid_destination_out(void *_dest, int length, plutovg_color32_t color, uint32_t const_alpha)
{
    plutovg_color16_t *dest = (plutovg_color16_t *)_dest;
    uint32_t a = plutovg_alpha(plutovg_color_invert(color));
    if (const_alpha != 255)
        a = BYTE_ALPHA_MUL(a, const_alpha) + 255 - const_alpha;
    for (int i = 0; i < length; i++)
        dest[i] = BYTE_MUL_16(dest[i], a);
}

static void composition_source(void *_dest, int length, const plutovg_color32_t *src, uint32_t const_alpha)
{
    plutovg_color16_t *dest = (plutovg_color16_t *)_dest;

    if (const_alpha == 255)
    {
        for (int i = 0; i < length; i++)
            dest[i] = plutovg_color32_to_color16(src[i]);
    }
    else
    {
        uint32_t ialpha = 255 - const_alpha;
        for (int i = 0; i < length; i++)
            dest[i] = interpolate_pixel_16(src[i], const_alpha, dest[i], ialpha);
    }
}

static void composition_source_over(void *_dest, int length, const plutovg_color32_t *src, uint32_t const_alpha)
{
    plutovg_color16_t *dest = (plutovg_color16_t *)_dest;

    plutovg_color32_t s;
    uint32_t sia;
    if (const_alpha == 255)
    {
        for (int i = 0; i < length; i++)
        {
            s = src[i];
            if (s.full >= 0xff000000)
                dest[i] = plutovg_color32_to_color16(s);
            else if (s.full != 0)
            {
                sia = plutovg_alpha(plutovg_color_invert(s));
                dest[i] = plutovg_bytes_to_color16(plutovg_color32_to_color16(s).full + BYTE_MUL_16(dest[i], sia).full);
            }
        }
    }
    else
    {
        for (int i = 0; i < length; i++)
        {
            s = BYTE_MUL(src[i], const_alpha);
            sia = 255 - BYTE_ALPHA_MUL(plutovg_alpha(src[i]), const_alpha);
            dest[i] = plutovg_bytes_to_color16(plutovg_color32_to_color16(s).full + BYTE_MUL_16(dest[i], sia).full);
        }
    }
}

static void composition_destination_in(void *_dest, int length, const plutovg_color32_t *src, uint32_t const_alpha)
{
    plutovg_color16_t *dest = (plutovg_color16_t *)_dest;

    if (const_alpha == 255)
    {
        for (int i = 0; i < length; i++)
            dest[i] = BYTE_MUL_16(dest[i], plutovg_alpha(src[i]));
    }
    else
    {
        uint32_t cia = 255 - const_alpha;
        uint32_t a;
        for (int i = 0; i < length; i++)
        {
            a = BYTE_MUL_16(plutovg_color32_to_color16(plutovg_mask_alpha(src[i])), const_alpha).full + cia;
            dest[i] = BYTE_MUL_16(dest[i], a);
        }
    }
}

static void composition_destination_out(void *_dest, int length, const plutovg_color32_t *src, uint32_t const_alpha)
{
    plutovg_color16_t *dest = (plutovg_color16_t *)_dest;

    if (const_alpha == 255)
    {
        for (int i = 0; i < length; i++)
            dest[i] = BYTE_MUL_16(dest[i], plutovg_alpha(plutovg_color_invert(src[i])));
    }
    else
    {
        uint32_t cia = 255 - const_alpha;
        uint32_t sia;
        for (int i = 0; i < length; i++)
        {
            sia = BYTE_MUL_16(plutovg_color32_to_color16(plutovg_mask_alpha(plutovg_color_invert(src[i]))), const_alpha).full + cia;
            dest[i] = BYTE_MUL_16(dest[i], sia);
        }
    }
}

const composition_solid_function_t composition_solid_map_16[] = {
    composition_solid_source,
    composition_solid_source_over,
    composition_solid_destination_in,
    composition_solid_destination_out,
};

const composition_function_t composition_map_16[] = {
    composition_source,
    composition_source_over,
    composition_destination_in,
    composition_destination_out,
};
