/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <FlexLayout.h>
#include <plutovg.h>
#include "meui/event.h"

#include "common/list.h"
#include "common/class.h"

typedef FlexNodeRef box_t;

#include "style.h"
#include "pqueue.h"

enum BOX_TYPE
{
    BOX_TYPE_DIV,
    BOX_TYPE_STACK,
    BOX_TYPE_CANVAS,
};

enum BOX_STATE
{
    BOX_STATE_DEFAULT,
    BOX_STATE_FOCUS,
    BOX_STATE_ACTIVE,
    BOX_STATE_HOVER,
    BOX_STATE_SELECT,
    BOX_STATE_DISABLE,
    BOX_STATE_CHECK,
    BOX_STATE_MAX,
};

enum BOX_DIRTY_TYPE
{
    BOX_DIRTY_CHILD = 1 << 0,
    BOX_DIRTY_LAYOUT = 1 << 1,
    BOX_DIRTY_STYLE = 1 << 2,
    BOX_DIRTY_TEXT = 1 << 3,
    BOX_DIRTY_SCROLL = 1 << 4,
    BOX_DIRTY_IMAGE = 1 << 5,
    BOX_DIRTY_OTHER = 1 << 6,
};

typedef uint32_t dirty_flags_t;

class(Box)
{
    box_t node;
    box_style_t style;
    box_style_t *style_array[BOX_STATE_MAX];
    char *text;

    size_t index; // for search hitbox
    size_t queue_pos;

    void (*draw)(Box * this, plutovg_t * pluto);
    enum BOX_STATE state;

    float scroll_top, scroll_left;
    float scroll_width, scroll_height;
    float client_width, client_height;
    float offset_width, offset_height;

    struct
    {
        plutovg_matrix_t to_screen_matrix; // box to screen
        plutovg_matrix_t layer_to_screen_matrix;
        plutovg_rect_t last_local_rect;
        plutovg_rect_t last_screen_rect;
        plutovg_surface_t *surface;

        bool is_layer;
        bool out_of_screen;
    } result;

    dirty_flags_t dirty;
};

box_t box_new(enum BOX_TYPE type);
void box_free(box_t node);
void box_free_recursive(box_t node);

void box_mark_dirty(box_t node, enum BOX_DIRTY_TYPE type, int need);
dirty_flags_t box_get_dirty(box_t node);
void box_clear_dirty(box_t node);

void box_set_style(box_t node, box_style_t *style, enum BOX_STATE state);
enum BOX_STATE box_get_state(box_t node);
void box_set_state(box_t node, enum BOX_STATE state);
box_style_t *box_get_style(box_t node, enum BOX_STATE state);

const char *box_get_text_content(box_t node);
void box_set_text_content(box_t node, const char *text);

int box_get_client_width(box_t node);
int box_get_client_height(box_t node);
int box_get_scroll_width(box_t node);
int box_get_scroll_height(box_t node);
int box_get_scroll_top(box_t node);
void box_set_scroll_top(box_t node, int scroll_top);
int box_get_scroll_left(box_t node);
void box_set_scroll_left(box_t node, int scroll_left);
int box_hit(box_t node, int x, int y);

void box_add_child(box_t node, box_t child);
void box_insert_child(box_t node, box_t child, int32_t index);
void box_remove_child(box_t node, box_t child);

FlexSize box_measure_text(void *context, FlexSize constrainedSize);

box_t box_search_queue(box_t node, int x, int y);