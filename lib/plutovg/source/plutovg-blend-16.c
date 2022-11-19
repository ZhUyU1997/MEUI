#include "plutovg-private.h"

#include <limits.h>
#include <math.h>

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

static inline plutovg_color32_t interpolate_pixel(plutovg_color32_t c, uint32_t a, uint32_t y, uint32_t b)
{
    uint32_t x = c.full;
    uint32_t t = (x & 0xff00ff) * a + (y & 0xff00ff) * b;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;
    x = ((x >> 8) & 0xff00ff) * a + ((y >> 8) & 0xff00ff) * b;
    x = (x + ((x >> 8) & 0xff00ff) + 0x800080);
    x &= 0xff00ff00;
    x |= t;
    return plutovg_bytes_to_color(x);
}

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

static inline void memfill32(plutovg_color32_t *dest, plutovg_color32_t value, int length)
{
    for (int i = 0; i < length; i++)
        dest[i] = value;
}

static void composition_solid_source(plutovg_color32_t *dest, int length, plutovg_color32_t color, uint32_t const_alpha)
{
    if (const_alpha == 255)
    {
        memfill32(dest, color, length);
    }
    else
    {
        uint32_t ialpha = 255 - const_alpha;
        color = BYTE_MUL(color, const_alpha);
        for (int i = 0; i < length; i++)
            dest[i] = plutovg_bytes_to_color(color.full + BYTE_MUL(dest[i], ialpha).full);
    }
}

static void composition_solid_source_over(plutovg_color32_t *dest, int length, plutovg_color32_t color, uint32_t const_alpha)
{
    if (const_alpha != 255)
        color = BYTE_MUL(color, const_alpha);
    uint32_t ialpha = 255 - plutovg_alpha(color);
    for (int i = 0; i < length; i++)
        dest[i] = plutovg_bytes_to_color(color.full + BYTE_MUL(dest[i], ialpha).full);
}

static void composition_solid_destination_in(plutovg_color32_t *dest, int length, plutovg_color32_t color, uint32_t const_alpha)
{
    uint32_t a = plutovg_alpha(color);
    if (const_alpha != 255)
        a = BYTE_MUL(plutovg_bytes_to_color(a), const_alpha).full + 255 - const_alpha;
    for (int i = 0; i < length; i++)
        dest[i] = BYTE_MUL(dest[i], a);
}

static void composition_solid_destination_out(plutovg_color32_t *dest, int length, plutovg_color32_t color, uint32_t const_alpha)
{
    uint32_t a = plutovg_alpha(plutovg_color_invert(color));
    if (const_alpha != 255)
        a = BYTE_MUL(plutovg_bytes_to_color(a), const_alpha).full + 255 - const_alpha;
    for (int i = 0; i < length; i++)
        dest[i] = BYTE_MUL(dest[i], a);
}

static void composition_source(plutovg_color32_t *dest, int length, const plutovg_color32_t *src, uint32_t const_alpha)
{
    if (const_alpha == 255)
    {
        memcpy(dest, src, (size_t)(length) * sizeof(uint32_t));
    }
    else
    {
        uint32_t ialpha = 255 - const_alpha;
        for (int i = 0; i < length; i++)
            dest[i] = interpolate_pixel(src[i], const_alpha, dest[i].full, ialpha);
    }
}

static void composition_source_over(plutovg_color32_t *dest, int length, const plutovg_color32_t *src, uint32_t const_alpha)
{
    plutovg_color32_t s;
    uint32_t sia;
    if (const_alpha == 255)
    {
        for (int i = 0; i < length; i++)
        {
            s = src[i];
            if (s.full >= 0xff000000)
                dest[i] = s;
            else if (s.full != 0)
            {
                sia = plutovg_alpha(plutovg_color_invert(s));
                dest[i] = plutovg_bytes_to_color(s.full + BYTE_MUL(dest[i], sia).full);
            }
        }
    }
    else
    {
        for (int i = 0; i < length; i++)
        {
            s = BYTE_MUL(src[i], const_alpha);
            sia = plutovg_alpha(plutovg_color_invert(s));
            dest[i] = plutovg_bytes_to_color(s.full + BYTE_MUL(dest[i], sia).full);
        }
    }
}

static void composition_destination_in(plutovg_color32_t *dest, int length, const plutovg_color32_t *src, uint32_t const_alpha)
{
    if (const_alpha == 255)
    {
        for (int i = 0; i < length; i++)
            dest[i] = BYTE_MUL(dest[i], plutovg_alpha(src[i]));
    }
    else
    {
        uint32_t cia = 255 - const_alpha;
        uint32_t a;
        for (int i = 0; i < length; i++)
        {
            a = BYTE_MUL(plutovg_mask_alpha(src[i]), const_alpha).full + cia;
            dest[i] = BYTE_MUL(dest[i], a);
        }
    }
}

static void composition_destination_out(plutovg_color32_t *dest, int length, const plutovg_color32_t *src, uint32_t const_alpha)
{
    if (const_alpha == 255)
    {
        for (int i = 0; i < length; i++)
            dest[i] = BYTE_MUL(dest[i], plutovg_alpha(plutovg_color_invert(src[i])));
    }
    else
    {
        uint32_t cia = 255 - const_alpha;
        uint32_t sia;
        for (int i = 0; i < length; i++)
        {
            sia = BYTE_MUL(plutovg_mask_alpha(plutovg_color_invert(src[i])), const_alpha).full + cia;
            dest[i] = BYTE_MUL(dest[i], sia);
        }
    }
}

typedef void (*composition_solid_function_t)(plutovg_color32_t *dest, int length, plutovg_color32_t color, uint32_t const_alpha);
typedef void (*composition_function_t)(plutovg_color32_t *dest, int length, const plutovg_color32_t *src, uint32_t const_alpha);

static const composition_solid_function_t composition_solid_map[] = {
    composition_solid_source,
    composition_solid_source_over,
    composition_solid_destination_in,
    composition_solid_destination_out,
};

static const composition_function_t composition_map[] = {
    composition_source,
    composition_source_over,
    composition_destination_in,
    composition_destination_out,
};
