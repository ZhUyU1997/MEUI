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

static void box_draw_recursive(box_t node, plutovg_t *pluto);

static void draw_debug_rect(plutovg_t *pluto, double x, double y, double w, double h, plutovg_matrix_t *m)
{
    plutovg_rect_t rect = {x, y, w, h};
    plutovg_matrix_map_rect(m, &rect, &rect);

    plutovg_save(pluto);
    plutovg_identity_matrix(pluto);
    plutovg_rect(pluto, rect.x, rect.y, rect.w, rect.h);
    plutovg_set_source_rgba(pluto, 1, 0, 0, 0.1);
    plutovg_fill(pluto);
    plutovg_restore(pluto);
}

static void box_transform_by_origin(box_t node, plutovg_t *pluto)
{
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);

    Box *box = Flex_getContext(node);

    double x_off = 0.0, y_off = 0.0;
    if (box->style.transformOrigin.type == TRANSFORM_ORIGIN_TYPE_KEYWORD)
    {
        x_off = width * box->style.transformOrigin.x.keyword / 2.0;
        y_off = height * box->style.transformOrigin.y.keyword / 2.0;
    }
    else if (box->style.transformOrigin.type == TRANSFORM_ORIGIN_TYPE_OFFSET)
    {
        x_off = box->style.transformOrigin.x.offset;
        y_off = box->style.transformOrigin.y.offset;
    }

    plutovg_matrix_t m;
    plutovg_matrix_init_identity(&m);
    plutovg_matrix_translate(&m, x_off, y_off);
    plutovg_matrix_multiply(&m, &box->style.transform, &m);
    plutovg_matrix_translate(&m, -x_off, -y_off);

    // if not a layer
    if (box->result.surface == NULL)
        plutovg_transform(pluto, &m);

    plutovg_matrix_multiply(&box->result.to_screen_matrix, &m, &box->result.to_screen_matrix);
}

static int merge_styles(Box *box)
{
    // TODO: make a faster function to check dirty
    if (box_get_dirty(box->node) == 0)
    {
        return 0;
    }

    // TODO: Optimze, get previous state, then decide if merge by dirty flags
    box_style_clear(&box->style);
    box_style_merge(&box->style, box_default_style());
    box_style_merge(&box->style, box->style_array[BOX_STATE_DEFAULT]);

    if (box->state != BOX_STATE_DEFAULT)
    {
        box_style_t *src = box->style_array[box->state];
        if (src)
        {
            box_style_merge(&box->style, src);
            box_style_clear_dirty(src);
        }
    }

    box_style_clear_dirty(box->style_array[BOX_STATE_DEFAULT]);
    return 1;
}

int box_update_style_recursive(box_t node)
{

    Box *box = Flex_getContext(node);
    if (!box)
    {
        LOGE("Node is not box!");
        return 0;
    }

    int ret = merge_styles(box);

    if (ret)
        box_style_to_flex(&box->style, node);

    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        int r = box_update_style_recursive(Flex_getChild(node, i));
        ret = ret || r;
    }

    return ret;
}

static int box_update_style(box_t node)
{
    return box_update_style_recursive(node);
}

static void box_translate(box_t node, plutovg_t *pluto, double x, double y)
{
    Box *box = Flex_getContext(node);
    plutovg_matrix_translate(&box->result.to_screen_matrix, x, y);
}

static void box_matrix_init(box_t node, plutovg_matrix_t *m)
{
    Box *box = Flex_getContext(node);
    box->result.to_screen_matrix = *m;
}

static plutovg_surface_t *box_get_surface(box_t node)
{
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);

    Box *box = Flex_getContext(node);

    if (box->result.surface)
    {
        box->result.to_screen_matrix = box->result.layer_to_screen_matrix;

        if (plutovg_surface_get_width(box->result.surface) == (int)width &&
            plutovg_surface_get_height(box->result.surface) == (int)height)
        {
            return box->result.surface;
        }

        plutovg_surface_destroy(box->result.surface);
    }
    else
    {
        box->result.layer_to_screen_matrix = box->result.to_screen_matrix;
    }
    box->result.surface = plutovg_surface_create(width, height);
    return box->result.surface;
}

static void box_destory_surface(box_t node)
{
    Box *box = Flex_getContext(node);

    if (box->result.surface)
    {
        plutovg_surface_destroy(box->result.surface);
        box->result.surface = NULL;
    }
}

static void box_draw_layer(box_t node)
{
    if (Flex_getParent(node) == NULL)
    {
        Box *box = Flex_getContext(node);
        plutovg_matrix_init_identity(&box->result.to_screen_matrix);
    }

    plutovg_surface_t *base = box_get_surface(node);

    plutovg_t *pluto = plutovg_create(base);
    plutovg_set_operator(pluto, plutovg_operator_src);
    plutovg_set_source_rgba(pluto, 0, 0, 0, 0);
    plutovg_rect(pluto, 0, 0, plutovg_surface_get_width(base), plutovg_surface_get_height(base));
    plutovg_fill(pluto);

    plutovg_set_operator(pluto, plutovg_operator_src_over);
    box_draw_recursive(node, pluto);
    plutovg_destroy(pluto);
}

static void box_draw_root_layer(box_t node)
{
    Box *box = Flex_getContext(node);
    plutovg_matrix_init_identity(&box->result.to_screen_matrix);
    box_draw_layer(node);
}

static void box_draw_child(box_t node, plutovg_t *pluto)
{
    Box *parent = Flex_getContext(node);

    int is_visible = parent->style.overflow == CSS_OVERFLOW_VISIBLE;
    int off_x = 0, off_y = 0;
    if (!is_visible)
    {
        off_x = -parent->scroll_left;
        off_y = -parent->scroll_top;
    }

    plutovg_save(pluto);

    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        box_t child = Flex_getChild(node, i);
        Box *box = Flex_getContext(child);

        box_matrix_init(child, &parent->result.to_screen_matrix);
        box_translate(child, pluto, off_x + Flex_getResultLeft(child), off_y + Flex_getResultTop(child));

        if (!box_style_is_unset(&box->style, BOX_STYLE_zIndex))
        {
            continue;
        }
        else
        {
            box_destory_surface(child);

            plutovg_save(pluto);
            plutovg_set_operator(pluto, plutovg_operator_src_over);

            plutovg_translate(pluto, off_x + Flex_getResultLeft(child), off_y + Flex_getResultTop(child));
            box_draw_recursive(Flex_getChild(node, i), pluto);
            plutovg_restore(pluto);
        }
    }
    plutovg_restore(pluto);
}

static void box_draw_self(box_t node, plutovg_t *pluto)
{
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);
    Box *box = Flex_getContext(node);

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
}

static void box_calc_size_position(box_t node)
{
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);

    Box *box = Flex_getContext(node);
    box->client_width = width - Flex_getResultBorderLeft(node) - Flex_getResultBorderRight(node);
    box->client_height = height - Flex_getResultBorderTop(node) - Flex_getResultBorderBottom(node);
    box->offset_width = width;
    box->offset_height = height;
    box->scroll_width = fmax(Flex_getResultScrollWidth(node), box->client_width);
    box->scroll_height = fmax(Flex_getResultScrollHeight(node), box->client_height);
}

static void box_draw_recursive(box_t node, plutovg_t *pluto)
{
    box_transform_by_origin(node, pluto);
    box_calc_size_position(node);
    box_draw_self(node, pluto);
    box_draw_child(node, pluto);

    // if (Flex_getResultScrollWidth(node) != meui_get_instance()->width && Flex_getResultScrollHeight(node) != meui_get_instance()->height)
    // {
    //     // draw_debug_rect(pluto, Flex_getResultBorderLeft(node) - box->scroll_left, Flex_getResultBorderTop(node) - box->scroll_top, box->client_width, box->scroll_height, m);
    //     draw_debug_rect(pluto, rect.x, rect.y, rect.w, rect.h, m);
    // }
}

static void box_get_zindex_queue(box_t node, pqueue_t *pq)
{
    int len = Flex_getChildrenCount(node);

    for (int i = 0; i < len; i++)
    {
        box_t child = Flex_getChild(node, i);
        Box *box = Flex_getContext(child);
        box->index_in_parent = i;

        if (!box_style_is_unset(&box->style, BOX_STYLE_zIndex))
        {
            pqueue_insert(pq, child);
            continue;
        }

        box_get_zindex_queue(child, pq);
    }
}

static int box_mark_dirty_layer_queue(box_t node, pqueue_t *pq)
{
    int len = Flex_getChildrenCount(node);
    int result = box_get_dirty(node);
    for (int i = 0; i < len; i++)
    {
        box_t child = Flex_getChild(node, i);
        Box *box = Flex_getContext(child);
        box->index_in_parent = i;

        if (!box_style_is_unset(&box->style, BOX_STYLE_zIndex))
        {
            pqueue_insert(pq, child);
            continue;
        }

        int ret = box_mark_dirty_layer_queue(child, pq);
        result = result || ret;
    }

    box_clear_dirty(node);

    return result;
}

static void box_mark_dirty_layer(box_t root)
{
    pqueue_t *pq = box_pqueue_init(10);

    int result = box_mark_dirty_layer_queue(root, pq);
    Box *box = Flex_getContext(root);
    box->result.dirty_layer = result;

    if (pqueue_peek(pq) != NULL)
    {
        box_t node = NULL;
        while ((node = pqueue_pop(pq)))
            box_mark_dirty_layer(node);
    }

    pqueue_free(pq);
}

static plutovg_rect_t box_rect(box_t node)
{
    Box *box = Flex_getContext(node);

    plutovg_surface_t *surface = box->result.surface;
    plutovg_rect_t rect = {0, 0, plutovg_surface_get_width(surface), plutovg_surface_get_height(surface)};
    plutovg_matrix_t *m = &box->result.to_screen_matrix;

    plutovg_matrix_map_rect(m, &rect, &rect);

    plutovg_rect_t temp = rect;
    plutovg_rect_unite(&rect, &box->result.rect);
    box->result.rect = temp;
    return rect;
}

static plutovg_rect_t box_draw(box_t root)
{
    pqueue_t *pq = box_pqueue_init(10);
    Box *box = Flex_getContext(root);

    plutovg_rect_t result;

    plutovg_rect_init_invalid(&result);

    if (box->result.dirty_layer)
    {
        box_draw_layer(root);
        Box *box = Flex_getContext(root);

        result = box_rect(root);

        box->result.dirty_layer = 0;
    }

    box_get_zindex_queue(root, pq);

    if (pqueue_peek(pq) != NULL)
    {
        box_t node = NULL;
        while ((node = pqueue_pop(pq)))
        {
            plutovg_rect_t rect = box_draw(node);

            plutovg_rect_unite(&result, &rect);
        }
    }

    pqueue_free(pq);

    return result;
}

static void box_composite_layer(plutovg_t *pluto, box_t lower, box_t upper, const plutovg_rect_t *rect)
{
    Box *upper_box = Flex_getContext(upper);

    plutovg_matrix_t m;
    plutovg_rect_t update_rect = *rect;

    if (lower)
    {
        Box *lower_box = Flex_getContext(lower);

        plutovg_matrix_t *lower_m = &lower_box->result.to_screen_matrix;
        plutovg_matrix_t *upper_m = &upper_box->result.to_screen_matrix;

        m = *lower_m;
        plutovg_matrix_invert(&m);
        plutovg_matrix_multiply(&m, upper_m, &m);
    }
    else
    {
        plutovg_matrix_t *upper_m = &upper_box->result.to_screen_matrix;
        m = *upper_m;
    }

    {
        plutovg_matrix_t m = upper_box->result.to_screen_matrix;
        plutovg_matrix_invert(&m);
        plutovg_matrix_map_rect(&m, &update_rect, &update_rect);
    }

    plutovg_surface_t *surface = upper_box->result.surface;
    plutovg_set_matrix(pluto, &m);
    plutovg_set_source_surface(pluto, surface, 0, 0);

    if (lower)
        plutovg_set_operator(pluto, plutovg_operator_src_over);
    else
        plutovg_set_operator(pluto, plutovg_operator_src);

    plutovg_rect_t surface_rect = {0, 0, plutovg_surface_get_width(surface), plutovg_surface_get_height(surface)};
    plutovg_rect_intersect(&update_rect, &surface_rect);
    // update_rect = surface_rect;

    // draw_debug_rect(pluto, update_rect.x + 1, update_rect.y + 1, update_rect.w - 2, update_rect.h - 2, &m);
    // printf("%f %f %f %f\n", update_rect.x, update_rect.y, update_rect.w, update_rect.h);
    plutovg_rect(pluto, update_rect.x, update_rect.y, update_rect.w, update_rect.h);
    plutovg_fill(pluto);
}

static void box_composite(plutovg_t *pluto, box_t lower, box_t upper, const plutovg_rect_t *rect)
{
    pqueue_t *pq = box_pqueue_init(10);

    box_composite_layer(pluto, lower, upper, rect);

    box_get_zindex_queue(upper, pq);

    if (pqueue_peek(pq) != NULL)
    {
        box_t node = NULL;
        while ((node = pqueue_pop(pq)))
            box_composite(pluto, upper, node, rect);
    }

    pqueue_free(pq);
}

plutovg_rect_t box_render(box_t root, plutovg_surface_t *surface)
{
    if (!box_update_style(root))
    {
        plutovg_rect_t rect;
        plutovg_rect_init_invalid(&rect);
        return rect;
    }

    Flex_layout(root, FlexUndefined, FlexUndefined, 1);

    box_mark_dirty_layer(root);

    plutovg_t *pluto = plutovg_create(surface);

    plutovg_rect_t rect = box_draw(root);
    plutovg_rect_intersect(&rect,
                           &(plutovg_rect_t){
                               0,
                               0,
                               plutovg_surface_get_width(surface),
                               plutovg_surface_get_height(surface),
                           });

    // printf("screen %f %f %f %f\n", rect.x, rect.y, rect.w, rect.h);

    box_composite(pluto, NULL, root, &rect);

    plutovg_destroy(pluto);

    // printf("render end\n");
    return rect;
}
