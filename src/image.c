/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "stb_image.h"
#include "stb_image_resize.h"
#include "plutovg.h"

plutovg_surface_t *pluto_load_image_from_file(const char *filename, int width, int height)
{
    int iw, ih, channels_in_file;
    stbi_uc *image = stbi_load(filename, &iw, &ih, &channels_in_file, STBI_rgb_alpha);

    if (!image)
        return NULL;

    plutovg_surface_t *surface = plutovg_surface_create(width, height);

    if (!surface)
    {
        free(image);
        return NULL;
    }

    stbir_resize_uint8(image, iw, ih, 0, plutovg_surface_get_data(surface), width, height, 0, STBI_rgb_alpha);

    unsigned char *data = plutovg_surface_get_data(surface);
    int stride = plutovg_surface_get_stride(surface);

    for (int y = 0; y < height; y++)
    {
        uint32_t *pixel = (uint32_t *)(data + stride * y);
        for (int x = 0; x < width; x++)
        {
            uint32_t a = pixel[x] >> 24;
            uint32_t b = (pixel[x] >> 16) & 0xff;
            uint32_t g = (pixel[x] >> 8) & 0xff;
            uint32_t r = (pixel[x] >> 0) & 0xff;

            pixel[x] = (a << 24) | (r << 16) | (g << 8) | b;
        }
    }

    free(image);
    return surface;
}