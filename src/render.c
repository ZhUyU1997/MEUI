/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "meui/box.h"
#include "meui/box_pqueue.h"
#include "meui/element.h"
#include "meui/image.h"
#include "meui/text.h"
#include "meui.h"

#include "utils/file.h"

#include "common/log.h"
#include "common/list.h"
#include "pqueue.h"

#include <FlexLayout.h>
#include <plutovg.h>
#include <plutosvg.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

static void box_draw_recursive(plutovg_t *pluto, box_t node, pqueue_t *pq);

static void draw_debug_rect(plutovg_t *pluto, double x, double y, double w, double h, struct plutovg_color fill_color)
{
    plutovg_save(pluto);
    plutovg_rect(pluto, x, y, w, h);
    plutovg_set_source_color(pluto, &fill_color);
    plutovg_fill(pluto);
    plutovg_restore(pluto);
}

static void box_transform_by_origin(Box *box, plutovg_t *pluto, plutovg_rect_t *rect)
{
    double x_off = 0.0, y_off = 0.0;
    if (box->style.transformOrigin.type == TRANSFORM_ORIGIN_TYPE_KEYWORD)
    {
        x_off = rect->w * box->style.transformOrigin.x.keyword / 2.0;
        y_off = rect->h * box->style.transformOrigin.y.keyword / 2.0;
    }
    else if (box->style.transformOrigin.type == TRANSFORM_ORIGIN_TYPE_OFFSET)
    {
        x_off = box->style.transformOrigin.x.offset;
        y_off = box->style.transformOrigin.y.offset;
    }

    plutovg_matrix_t m;
    plutovg_matrix_init_identity(&m);
    plutovg_matrix_translate(&m, rect->x + x_off, rect->y + y_off);
    plutovg_matrix_multiply(&m, &box->style.transform, &m);
    plutovg_matrix_translate(&m, -x_off, -y_off);

    plutovg_transform(pluto, &m);

    box_t parent = Flex_getParent(box->node);
    if (parent)
    {
        // For supporting scroll, must transfrom by parent's martrix, because pluto has been changed
        Box *parentBox = Flex_getContext(parent);
        plutovg_matrix_multiply(&box->result.to_screen_matrix, &m, &parentBox->result.to_screen_matrix);
    }
    else
    {
        plutovg_get_matrix(pluto, &box->result.to_screen_matrix);
    }
}

static void merge_styles(Box *box)
{
    if (!box->state_changed && box->state == BOX_STATE_DEFAULT && box_style_is_dirty(box->style_array[BOX_STATE_DEFAULT]) == 0)
    {
        return;
    }

    // TODO: Optimze, get previous state, then decide if merge by dirty flags
    box_style_clear(&box->style);
    box_style_merge(&box->style, box_default_style());
    box_style_merge(&box->style, box->style_array[BOX_STATE_DEFAULT]);

    if (box->state != BOX_STATE_DEFAULT)
    {
        box_style_t *src = box->style_array[box->state];
        if (src)
            box_style_merge(&box->style, src);
    }

    box_style_clear_dirty(box->style_array[BOX_STATE_DEFAULT]);
    box->state_changed = 0;
}

void box_update_style_recursive(box_t node)
{

    Box *box = Flex_getContext(node);
    if (!box)
    {
        LOGE("Node is not box!");
        return;
    }

    merge_styles(box);
    box_style_to_flex(&box->style, node);

    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        box_update_style_recursive(Flex_getChild(node, i));
    }
}

static void box_update_style(box_t node)
{
    box_update_style_recursive(node);
}

static void box_draw_child(box_t node, plutovg_t *pluto, pqueue_t *pq)
{
    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        box_t child = Flex_getChild(node, i);
        Box *box = Flex_getContext(child);

        if (!box_style_is_unset(&box->style, BOX_STYLE_zIndex))
        {
            pqueue_insert(pq, child);
            continue;
        }
        plutovg_save(pluto);
        box_draw_recursive(pluto, Flex_getChild(node, i), pq);
        plutovg_restore(pluto);
    }
}

static void box_draw_recursive(plutovg_t *pluto, box_t node, pqueue_t *pq)
{
    float left = Flex_getResultLeft(node);
    float top = Flex_getResultTop(node);
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);

    Box *box = Flex_getContext(node);

    if (!box)
    {
        LOGE("Node is not box!");
        return;
    }

    box_transform_by_origin(box, pluto, &(plutovg_rect_t){left, top, width, height});

    box->client_width = width - Flex_getResultBorderLeft(node) - Flex_getResultBorderRight(node);
    box->client_height = height - Flex_getResultBorderTop(node) - Flex_getResultBorderBottom(node);
    box->offset_width = width;
    box->offset_height = height;
    box->scroll_width = fmax(Flex_getResultScrollWidth(node), box->client_width);
    box->scroll_height = fmax(Flex_getResultScrollHeight(node), box->client_height);

    struct meui_t *meui = meui_get_instance();
    plutovg_matrix_t *m = &box->result.to_screen_matrix;
    plutovg_rect_t rect = {0, 0, width, height};
    plutovg_matrix_map_rect(m, &rect, &rect);
    plutovg_rect_intersect(&rect, &(plutovg_rect_t){0, 0, meui->width, meui->height});

    box->out_of_screen = plutovg_rect_invalid(&rect);
    if (!box->out_of_screen)
    {
        if (box->draw)
            box->draw(box, pluto);
    }

    if (box->style.overflow == CSS_OVERFLOW_VISIBLE)
    {
        box_draw_child(node, pluto, pq);
    }
    else
    {
        plutovg_save(pluto);
        // printf("%f %f\n", box->client_width, box->client_height);
        {
            plutovg_translate(pluto, -box->scroll_left, -box->scroll_top);

            // Fix scroll offset
            plutovg_matrix_translate(&box->result.to_screen_matrix, -box->scroll_left, -box->scroll_top);
            box_draw_child(node, pluto, pq);
            plutovg_matrix_translate(&box->result.to_screen_matrix, box->scroll_left, box->scroll_top);
        }
        plutovg_fill(pluto);

        plutovg_restore(pluto);
    }

    // if (Flex_getResultScrollWidth(node) != meui_get_instance()->width && Flex_getResultScrollHeight(node) != meui_get_instance()->height)
    //     draw_debug_rect(pluto, Flex_getResultBorderLeft(node) - box->scroll_left, Flex_getResultBorderTop(node) - box->scroll_top, box->client_width, box->scroll_height, (plutovg_color_t){1, 0, 0, 0.5});
}

static void box_draw_recursive_queue(plutovg_t *pluto, box_t root)
{
    pqueue_t *pq = box_pqueue_init(10);
    box_draw_recursive(pluto, root, pq);

    if (pqueue_peek(pq) != NULL)
    {
        box_t box = NULL;
        while ((box = pqueue_pop(pq)))
        {
            box_t parent = Flex_getParent(box);
            Box *parentBox = Flex_getContext(parent);

            plutovg_save(pluto);

            plutovg_set_matrix(pluto, &parentBox->result.to_screen_matrix);
            box_draw_recursive_queue(pluto, box);
            plutovg_restore(pluto);
        }
    }
    pqueue_free(pq);
}

void box_render(box_t root, plutovg_surface_t *surface)
{
    box_update_style(root);
    Flex_layout(root, FlexUndefined, FlexUndefined, 1);

    plutovg_t *pluto = plutovg_create(surface);
    Box *box = Flex_getContext(root);
    plutovg_matrix_init_identity(&box->result.to_screen_matrix);

    box_draw_recursive_queue(pluto, root);
    plutovg_destroy(pluto);
}
