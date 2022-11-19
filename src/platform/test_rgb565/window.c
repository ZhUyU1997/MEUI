/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdlib.h>

#include "meui.h"

struct window_t
{
    char *pixels;
};

struct window_t *window_create(const char *title, int width, int height)
{
    struct window_t *window = calloc(1, sizeof(struct window_t));
    window->pixels = calloc(1, sizeof(uint16_t) * width * height);
    printf("window_create %p\n", window->pixels);
    return window;
}

void window_destory(struct window_t *window)
{
    free(window->pixels);
    free(window);
}

char *window_get_image_data(struct window_t *window)
{
    return (char *)window->pixels;
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
