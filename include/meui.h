/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "meui/event.h"
#include "meui/box.h"

#include <time.h>
#include <plutovg.h>
#include <FlexLayout.h>
#include <hashmap.h>

struct meui_t;

typedef HASHMAP(char, plutovg_font_face_t) font_map_t;
typedef void (*meui_callback_t)(struct meui_t *);

enum MEUI_CALLBACK
{
    MEUI_CB_ON_CREATE,
    MEUI_CB_ON_EXIT,
    MEUI_CB_MAX,
};

struct meui_render_context_t
{
    font_map_t font_map;
    char *default_font_family;
    plutovg_surface_t *surface;
    box_t root;
};

typedef struct meui_render_context_t meui_render_context_t;

struct meui_t
{
    int width;
    int height;
    plutovg_surface_t *win_surface;
    void *platform_data;
    struct meui_render_context_t render_context;
    meui_callback_t callback[MEUI_CB_MAX];
};

struct meui_t *meui_start(int width, int height);
struct meui_t *meui_get_instance();
void meui_register_callback(struct meui_t *, enum MEUI_CALLBACK, meui_callback_t);
void meui_render(struct meui_t *meui, box_t box);
void meui_flush(struct meui_t *);
void meui_debug(struct meui_t *);
void meui_update(struct meui_t *);
void meui_end(struct meui_t *);

void meui_set_default_font_family(struct meui_t *meui, const char *font_family);
const char *meui_get_default_font_family(struct meui_t *meui);
void meui_add_font_face(struct meui_t *meui, const char *font_family, const char *file);
plutovg_font_t *meui_get_font(struct meui_t *meui, const char *font_family, double size);
plutovg_surface_t *meui_get_surface(struct meui_t *);
box_t meui_get_root_node(struct meui_t *);
int meui_get_connect_number(struct meui_t *meui);
int meui_pending(struct meui_t *meui);
void meui_next_event(struct meui_t *meui, struct meui_event_t *event);

box_t meui_search_node(struct meui_t *meui, box_t node, void *data, plutovg_point_t *point, void (*cb)(box_t node, bool hit, void *data));

void meui_image_cache_init();
void meui_image_cache_exit();
plutovg_surface_t *meui_image_cache_load(const char *path, int width, int height);

void meui_text_cache_init();
void meui_text_cache_exit();
plutovg_surface_t *meui_text_cache_load(const char *font_family, int ch, double font_size, plutovg_color_t font_color);