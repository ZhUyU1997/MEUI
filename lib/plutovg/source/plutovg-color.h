/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>

typedef union
{
    struct
    {
        uint16_t blue : 5;
        uint16_t green : 6;
        uint16_t red : 5;
    } ch;
    uint16_t full;
} plutovg_color16_t;

typedef union
{
    struct
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    } ch;
    uint32_t full;
} plutovg_color32_t;

#define plutovg_alpha(c) ((c).ch.alpha)
#define plutovg_red(c) ((c).ch.red)
#define plutovg_green(c) ((c).ch.green)
#define plutovg_blue(c) ((c).ch.blue)

#define plutovg_bytes_to_color(bytes) ((plutovg_color32_t){.full = (bytes)})
#define plutovg_color_to_bytes(c) ((c).full)
#define plutovg_color_invert(c) ((plutovg_color32_t){.full = (~(c).full)})
#define plutovg_mask_alpha(c) (plutovg_bytes_to_color((c).ch.alpha))

#define plutovg_bytes_to_color16(bytes) ((plutovg_color16_t){.full = (bytes)})

static inline plutovg_color16_t plutovg_color32_to_color16(plutovg_color32_t color)
{
    const int r = plutovg_red(color) << 8;
    const int g = plutovg_green(color) << 3;
    const int b = plutovg_blue(color) >> 3;
    return plutovg_bytes_to_color16((r & 0xf800) | (g & 0x07e0) | (b & 0x001f));
}

static inline plutovg_color32_t plutovg_color16_to_color32(plutovg_color16_t color)
{
    const uint32_t c = color.full;
    const int r = (c & 0xf800);
    const int g = (c & 0x07e0);
    const int b = (c & 0x001f);
    const int tr = (r >> 8) | (r >> 13);
    const int tg = (g >> 3) | (g >> 9);
    const int tb = (b << 3) | (b >> 2);
    return (plutovg_color32_t){
        .ch = {
            .red = tr,
            .green = tg,
            .blue = tb,
        },
    };
}

// https://github.com/qt/qtbase/blob/dev/src/gui/painting/qdrawhelper_p.h#L611-L620
static inline plutovg_color32_t interpolate_pixel(plutovg_color32_t c1, uint32_t a, plutovg_color32_t c2, uint32_t b)
{
    uint32_t x = c1.full;
    uint32_t y = c2.full;

    uint32_t t = (x & 0xff00ff) * a + (y & 0xff00ff) * b;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;
    x = ((x >> 8) & 0xff00ff) * a + ((y >> 8) & 0xff00ff) * b;
    x = (x + ((x >> 8) & 0xff00ff) + 0x800080);
    x &= 0xff00ff00;
    x |= t;
    return plutovg_bytes_to_color(x);
}

static inline plutovg_color16_t interpolate_pixel_16(plutovg_color32_t c1, uint32_t a, plutovg_color16_t c2, uint32_t b)
{
    return plutovg_color32_to_color16(interpolate_pixel(c1, a, plutovg_color16_to_color32(c2), b));
}

// https://github.com/qt/qtbase/blob/dev/src/gui/painting/qdrawhelper_p.h#L622-L632
static inline plutovg_color32_t BYTE_MUL(plutovg_color32_t c, uint32_t a)
{
    uint32_t x = c.full;
    uint32_t t = (x & 0xff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;
    x = ((x >> 8) & 0xff00ff) * a;
    x = (x + ((x >> 8) & 0xff00ff) + 0x800080);
    x &= 0xff00ff00;
    x |= t;
    return plutovg_bytes_to_color(x);
}

static inline plutovg_color16_t BYTE_MUL_16(plutovg_color16_t c, uint32_t a)
{
    uint32_t x = c.full;
    a += 1;
    uint32_t t = (((x & 0x07e0) * a) >> 8) & 0x07e0;
    t |= (((x & 0xf81f) * (a >> 2)) >> 6) & 0xf81f;
    return plutovg_bytes_to_color16(t);
}

// https://github.com/qt/qtbase/blob/dev/src/gui/painting/qdrawhelper_p.h#L884
static inline DIV_255(int x) { return (x + (x >> 8) + 0x80) >> 8; }

static inline uint32_t BYTE_ALPHA_MUL(uint32_t x, uint32_t a)
{
    return DIV_255(x * a);
}

static inline void memfill32(plutovg_color32_t *dest, plutovg_color32_t value, int length)
{
    for (int i = 0; i < length; i++)
        dest[i] = value;
}

static inline void memfill16(plutovg_color16_t *dest, plutovg_color16_t value, int length)
{
    for (int i = 0; i < length; i++)
        dest[i] = value;
}

typedef void (*composition_solid_function_t)(void *dest, int length, plutovg_color32_t color, uint32_t const_alpha);
typedef void (*composition_function_t)(void *dest, int length, const plutovg_color32_t *src, uint32_t const_alpha);
