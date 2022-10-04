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
#include "utils/surface.h"

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

static void box_draw_node(box_t node, plutovg_t *pluto);

static void draw_debug_rect(plutovg_t *pluto, double x, double y, double w, double h, plutovg_matrix_t *m)
{
    plutovg_rect_t rect = {x, y, w, h};
    plutovg_matrix_map_rect(m, &rect, &rect);

    plutovg_save(pluto);
    plutovg_set_operator(pluto, plutovg_operator_src_over);
    plutovg_identity_matrix(pluto);
    plutovg_rect(pluto, rect.x, rect.y, rect.w, rect.h);
    plutovg_set_source_rgba(pluto, 1, 0, 0, 0.1);
    plutovg_fill(pluto);
    plutovg_restore(pluto);
}

static plutovg_surface_t *box_get_layer(box_t node)
{
    Box *box = Flex_getContext(node);
    assert(box->result.surface != NULL);
    return box->result.surface;
}

static plutovg_surface_t *box_create_layer(box_t node)
{
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);

    Box *box = Flex_getContext(node);

    if (box->result.surface)
    {
        if (plutovg_surface_get_width(box->result.surface) == (int)width &&
            plutovg_surface_get_height(box->result.surface) == (int)height)
        {
            return box->result.surface;
        }

        plutovg_surface_destroy(box->result.surface);
    }

    box->result.surface = plutovg_surface_create(width, height);
    return box->result.surface;
}

static void box_transform_layer(box_t node)
{
    Box *box = Flex_getContext(node);
    box->result.to_screen_matrix = box->result.layer_to_screen_matrix;
}

static void box_destory_layer(box_t node)
{
    Box *box = Flex_getContext(node);

    if (box->result.surface)
    {
        plutovg_surface_destroy(box->result.surface);
        box->result.surface = NULL;
    }
}

static bool box_is_layer(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->result.is_layer;
}

static void box_mark_layer(box_t node, bool is_layer)
{
    Box *box = Flex_getContext(node);
    box->result.is_layer = is_layer;

    if (is_layer)
    {
        box->result.layer_to_screen_matrix = box->result.to_screen_matrix;
        box_create_layer(node);
    }
    else
        box_destory_layer(node);
}

static plutovg_matrix_t box_transform_by_origin(box_t node)
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

    plutovg_matrix_multiply(&box->result.to_screen_matrix, &m, &box->result.to_screen_matrix);

    return m;
}

static int merge_styles(box_t node)
{
    Box *box = Flex_getContext(node);

    dirty_flags_t dirty = box_get_dirty(node);
    // TODO: make a faster function to check dirty
    if (!(dirty & BOX_DIRTY_STYLE))
    {
        return 0;
    }

    // TODO: Optimze, get previous state, then decide if merge by dirty flags
    box_style_clear(&box->style);
    box_style_merge(&box->style, box_default_style());
    box_style_merge(&box->style, box->style_array[BOX_STATE_DEFAULT]);

    box_style_t *style = box->style_array[box->state];
    if (box->state != BOX_STATE_DEFAULT && style)
        box_style_merge(&box->style, style);

    if (dirty & BOX_DIRTY_LAYOUT)
        box_style_to_flex(&box->style, node);
    return 1;
}

dirty_flags_t box_update_style_recursive(box_t node)
{
    merge_styles(node);

    dirty_flags_t ret = box_get_dirty(node);

    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        dirty_flags_t r = box_update_style_recursive(Flex_getChild(node, i));
        ret = ret | r;
    }

    return ret;
}

static dirty_flags_t box_update_style(box_t node)
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
            box_mark_layer(child, true);
            continue;
        }
        else
        {
            box_mark_layer(child, false);

            plutovg_save(pluto);
            plutovg_set_operator(pluto, plutovg_operator_src_over);

            plutovg_translate(pluto, off_x + Flex_getResultLeft(child), off_y + Flex_getResultTop(child));
            box_draw_node(Flex_getChild(node, i), pluto);
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

    box->result.out_of_screen = plutovg_rect_invalid(&rect);
    if (!box->result.out_of_screen)
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

static void box_draw_node(box_t node, plutovg_t *pluto)
{
    plutovg_matrix_t m = box_transform_by_origin(node);
    box_calc_size_position(node);
    plutovg_transform(pluto, &m);
    box_draw_self(node, pluto);
    box_draw_child(node, pluto);
}

static void box_draw_layer(box_t node, dirty_flags_t dirty)
{
    box_transform_layer(node);
    box_transform_by_origin(node);
    box_calc_size_position(node);

    if (!dirty)
        return;

    plutovg_surface_t *base = box_create_layer(node);

    plutovg_t *pluto = plutovg_create(base);
    plutovg_set_operator(pluto, plutovg_operator_src);
    plutovg_set_source_rgba(pluto, 0, 0, 0, 0);
    plutovg_rect(pluto, 0, 0, plutovg_surface_get_width(base), plutovg_surface_get_height(base));
    plutovg_fill(pluto);

    plutovg_set_operator(pluto, plutovg_operator_src_over);

    box_draw_self(node, pluto);
    box_draw_child(node, pluto);

    plutovg_destroy(pluto);
}

static bool box_check_layout_dirty(box_t node)
{
    Box *box = Flex_getContext(node);
    float left = Flex_getResultLeft(node);
    float top = Flex_getResultTop(node);
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);

    plutovg_rect_t rect = {
        left,
        top,
        width,
        height,
    };

    bool result = plutovg_rect_equal(&rect, &box->result.last_local_rect) == false;

    box->result.last_local_rect = rect;

    return result;
}

static dirty_flags_t box_check_dirty(box_t node)
{
    dirty_flags_t result = box_get_dirty(node);

    // FlexLayout != layout change
    // Mark layout dirty != layout change
    // so we have to check the layout change flag regardless of flags
    if (box_check_layout_dirty(node))
    {
        result |= BOX_DIRTY_LAYOUT;
    }
    return result;
}

static dirty_flags_t box_get_zindex_queue(box_t node, pqueue_t *pq)
{
    int len = Flex_getChildrenCount(node);
    dirty_flags_t result = 0;

    for (int i = 0; i < len; i++)
    {
        box_t child = Flex_getChild(node, i);
        Box *box = Flex_getContext(child);

        if (!box_style_is_unset(&box->style, BOX_STYLE_zIndex))
        {
            pqueue_insert(pq, child);
            continue;
        }

        result |= box_check_dirty(child);
        result |= box_get_zindex_queue(child, pq);
    }

    box_clear_dirty(node);
    return result;
}

static plutovg_rect_t box_rect(box_t node)
{
    Box *box = Flex_getContext(node);

    plutovg_surface_t *surface = box->result.surface;
    plutovg_rect_t rect = plutovg_surface_get_rect(surface);
    plutovg_matrix_t *m = &box->result.to_screen_matrix;

    plutovg_matrix_map_rect(m, &rect, &rect);

    plutovg_rect_t temp = rect;
    plutovg_rect_unite(&rect, &box->result.last_screen_rect);
    box->result.last_screen_rect = temp;
    return rect;
}

static plutovg_rect_t box_draw(box_t root)
{
    pqueue_t *pq = box_pqueue_init(10);

    plutovg_rect_t result;
    plutovg_rect_init_invalid(&result);

    dirty_flags_t dirty = box_check_dirty(root);
    dirty |= box_get_zindex_queue(root, pq);

    box_draw_layer(root, dirty);

    if (dirty)
        result = box_rect(root);

    box_t node = NULL;
    while ((node = pqueue_pop(pq)))
    {
        plutovg_rect_t rect = box_draw(node);
        plutovg_rect_unite(&result, &rect);
    }

    pqueue_free(pq);

    return result;
}

static bool box_map_rect_to_local(box_t node, plutovg_rect_t *rect)
{
    Box *box = Flex_getContext(node);

    plutovg_matrix_t m = box->result.to_screen_matrix;
    if (plutovg_matrix_invert(&m) == 0)
        return false;
    plutovg_matrix_map_rect(&m, rect, rect);

    return true;
}

static void box_composite_layer(plutovg_t *pluto, box_t lower, box_t upper, plutovg_rect_t *rect)
{
    plutovg_rect_t update_rect = *rect;

    Box *upper_box = Flex_getContext(upper);
    plutovg_matrix_t m = upper_box->result.to_screen_matrix;

    if (box_map_rect_to_local(upper, &update_rect) == false)
        return;

    plutovg_surface_t *surface = box_get_layer(upper);
    plutovg_set_matrix(pluto, &m);
    plutovg_set_source_surface(pluto, surface, 0, 0);

    if (lower)
        plutovg_set_operator(pluto, plutovg_operator_src_over);
    else
        plutovg_set_operator(pluto, plutovg_operator_src);

    plutovg_rect_t surface_rect = plutovg_surface_get_rect(surface);
    plutovg_rect_intersect(&update_rect, &surface_rect);
    plutovg_rect_ext(&update_rect, 1);
    plutovg_rect(pluto, update_rect.x, update_rect.y, update_rect.w, update_rect.h);
    plutovg_fill(pluto);

    // draw_debug_rect(pluto, update_rect.x + 1, update_rect.y + 1, update_rect.w - 2, update_rect.h - 2, &m);

    plutovg_matrix_map_rect(&m, &update_rect, &update_rect);
    plutovg_rect_unite(rect, &update_rect);
    plutovg_rect_ext(rect, 1);
}

static void box_composite(plutovg_t *pluto, box_t lower, box_t upper, plutovg_rect_t *rect)
{
    pqueue_t *pq = box_pqueue_init(10);

    box_composite_layer(pluto, lower, upper, rect);

    box_get_zindex_queue(upper, pq);

    box_t node = NULL;
    while ((node = pqueue_pop(pq)))
        box_composite(pluto, upper, node, rect);
    pqueue_free(pq);
}

plutovg_rect_t box_render(box_t root, plutovg_surface_t *surface)
{
    // printf("render\n");
    dirty_flags_t dirty = box_update_style(root);

    if (!dirty)
    {
        plutovg_rect_t rect;
        plutovg_rect_init_invalid(&rect);
        return rect;
    }

    dirty_flags_t dirty_layout = dirty & (BOX_DIRTY_CHILD | BOX_DIRTY_LAYOUT | BOX_DIRTY_TEXT);

    if (dirty_layout)
    {
        // printf("Flex_layout\n");
        Flex_layout(root, FlexUndefined, FlexUndefined, 1);
    }

    box_mark_layer(root, true);

    plutovg_rect_t rect = box_draw(root);

    if (plutovg_rect_invalid(&rect))
        return rect;

    plutovg_t *pluto = plutovg_create(surface);
    plutovg_rect_t surface_rect = plutovg_surface_get_rect(surface);
    plutovg_rect_intersect(&rect, &surface_rect);
    plutovg_rect_ext(&rect, 1);

    // printf("screen %f %f %f %f\n", rect.x, rect.y, rect.w, rect.h);

    box_composite(pluto, NULL, root, &rect);

    plutovg_destroy(pluto);

    // printf("render end\n");
    return rect;
}
