#pragma once

#include <FlexLayout.h>
#include <plutovg.h>
#include <meui/event.h>

#include <list.h>

typedef FlexNodeRef box_t;

#include "style.h"

typedef struct box_event_t
{
    meui_event_t e;
    enum MEUI_EVENT_TYPE type;
    box_t target;
    box_t currentTarget;
} box_event_t;

typedef void (*box_event_cb_t)(box_event_t *e);

typedef struct box_event_dsc_t
{
    struct list_head node;
    enum MEUI_EVENT_TYPE type;
    box_event_cb_t cb;
} box_event_dsc_t;

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

struct Box
{
    enum BOX_STATE state;
    box_style_t *style_array[BOX_STATE_MAX];
    box_style_t style;

    struct list_head event_list;

    struct
    {
        plutovg_matrix_t to_screen_matrix; // box to screen
    } result;

    void *opaque;
};

box_t box_new();
void box_free(box_t node);
void box_free_recursive(box_t node);
void box_set_opaque(box_t node, void *opaque);
void *box_get_opaque(box_t node);
void box_clear_opaque(box_t node);

void box_set_style(box_t node, box_style_t *style, enum BOX_STATE state);
void box_set_state(box_t node, enum BOX_STATE state);
box_style_t *box_get_style(box_t node, enum BOX_STATE state);

void box_add_event_listener(box_t node, enum MEUI_EVENT_TYPE type, box_event_cb_t cb);
void box_dispatch_event(box_t node, enum MEUI_EVENT_TYPE type, meui_event_t *e);

void box_updateStyleRecursive(box_t node);
void box_drawRecursive(plutovg_t *pluto, box_t node);
void box_draw(box_t root);