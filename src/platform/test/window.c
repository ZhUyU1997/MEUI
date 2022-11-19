/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdlib.h>

#include "meui.h"

struct window_t
{
    unsigned char *pixels;
    plutovg_surface_t *surface;
};

struct window_t *window_create(const char *title, int width, int height, plutovg_color_format_t format)
{
    if (format != plutovg_color_format_argb32)
        return NULL;

    struct window_t *window = calloc(1, sizeof(struct window_t));
    window->pixels = calloc(1, sizeof(uint32_t) * width * height);
    window->surface = plutovg_surface_create_for_formated_data(window->pixels, width, height, sizeof(uint32_t) * width, format);
    return window;
}

void window_destory(struct window_t *window)
{
    plutovg_surface_destroy(window->surface);
    free(window->pixels);
    free(window);
}

unsigned char *window_get_image_data(struct window_t *window)
{
    return (unsigned char *)window->pixels;
}

plutovg_surface_t *window_get_surface(struct window_t *window)
{
    return window->surface;
}

int window_connect_number(struct window_t *window)
{
    return 0;
}

int window_update_image(struct window_t *window, int x, int y, int w, int h)
{
    return 0;
}

void window_set_name(struct window_t *window, const char *name)
{
    printf("%s\n", name);
}

int window_pending(struct window_t *window)
{
    return 0;
}

void window_next_event(struct window_t *window, struct meui_event_t *meui_event)
{
    meui_event->type = MEUI_EVENT_NULL;
}
