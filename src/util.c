#include <plutovg.h>
#include <stdint.h>

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
        const uint32_t *dest = (uint32_t *)(data + stride * y);
        for (int x = 0; x < width; x++)
        {
            memfill32(dest, ucolor, width);
        }
    }
}
