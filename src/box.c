/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "meui/box.h"
#include "meui/element.h"
#include "meui/image.h"
#include "meui/text.h"
#include "meui.h"
#include "common/log.h"
#include "common/list.h"
#include "pqueue.h"

#include <FlexLayout.h>
#include <plutovg.h>
#include <plutosvg.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define KAPPA90 (0.5522847493f)

static const char *fileext(const char *filename)
{
    const char *ret = NULL;
    const char *p;

    if (filename != NULL)
    {
        ret = p = strchr(filename, '.');
        while (p != NULL)
        {
            p = strchr(p + 1, '.');
            if (p != NULL)
                ret = p;
        }
        if (ret != NULL)
            ret++;
    }
    return ret;
}

static void box_draw_self(Box *box, plutovg_t *pluto);

class_impl(Box){
    .draw = box_draw_self,
};

constructor(Box)
{
    this->node = Flex_newNode();
    this->style_array[BOX_STATE_DEFAULT] = box_style_new();
    this->state_changed = 1;
    Flex_setContext(this->node, this);
}

destructor(Box)
{
    for (int i = 0; i < BOX_STATE_MAX; i++)
    {
        if (this->style_array[i])
        {
            box_style_free(this->style_array[i]);
            this->style_array[i] = NULL;
        }
    }

    box_style_clear(&this->style);

    if (this->text)
        free(this->text);

    Flex_setContext(this->node, NULL);
    Flex_freeNode(this->node);
}

box_t box_new(enum BOX_TYPE type)
{
    Box *box = NULL;
    if (type == BOX_TYPE_DIV)
    {
        box = dynamic_cast(Box)(new (DivEle));
    }
    else if (type == BOX_TYPE_STACK)
    {
        box = dynamic_cast(Box)(new (StackEle));
    }
    else if (type == BOX_TYPE_CANVAS)
    {
        box = dynamic_cast(Box)(new (CanvasEle));
    }

    return box->node;
}

void box_free(box_t node)
{
    Box *box = Flex_getContext(node);
    delete (box);
}

void box_free_recursive(box_t node)
{
    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        box_free_recursive(Flex_getChild(node, i));
    }
    box_free(node);
}

box_style_t *box_get_style(box_t node, enum BOX_STATE state)
{
    assert(node);

    Box *box = Flex_getContext(node);
    return box->style_array[state];
}

void box_set_style(box_t node, box_style_t *style, enum BOX_STATE state)
{
    assert(node && style);

    Box *box = Flex_getContext(node);
    box->style_array[state] = style;
}

enum BOX_STATE box_get_state(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->state;
}

void box_set_state(box_t node, enum BOX_STATE state)
{
    Box *box = Flex_getContext(node);
    box->state = state;
    box->state_changed = 1;
}

const char *box_get_text_content(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->text;
}

void box_set_text_content(box_t node, const char *text)
{
    Box *box = Flex_getContext(node);
    if (box->text)
        free(box->text);
    box->text = strdup(text);
}

int box_get_client_width(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->client_width;
}

int box_get_client_height(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->client_height;
}

int box_get_scroll_width(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->scroll_width;
}

int box_get_scroll_height(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->scroll_height;
}

int box_get_scroll_top(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->scroll_top;
}

void box_set_scroll_top(box_t node, int scroll_top)
{
    Box *box = Flex_getContext(node);

    if (scroll_top < 0)
        scroll_top = 0;
    else if (scroll_top > box->scroll_height - box->client_height)
        scroll_top = box->scroll_height - box->client_height;

    box->scroll_top = scroll_top;
}

int box_get_scroll_left(box_t node)
{
    Box *box = Flex_getContext(node);
    return box->scroll_left;
}

void box_set_scroll_left(box_t node, int scroll_left)
{
    Box *box = Flex_getContext(node);

    if (scroll_left < 0)
        scroll_left = 0;
    else if (scroll_left > box->scroll_width - box->client_width)
        scroll_left = box->scroll_width - box->client_width;

    box->scroll_left = scroll_left;
}

int box_hit(box_t node, int x, int y)
{
    Box *box = Flex_getContext(node);

    if (box->out_of_screen)
        return 0;

    float left = Flex_getResultLeft(node);
    float top = Flex_getResultTop(node);
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);

    box_t target = node;

    plutovg_matrix_t to_local = box->result.to_screen_matrix;
    plutovg_matrix_invert(&to_local);

    plutovg_point_t dst;
    plutovg_matrix_map_point(&to_local, &(plutovg_point_t){x, y}, &dst);

    if (dst.x >= 0 && dst.x < width && dst.y >= 0 && dst.y < height)
    {
        for (box_t node = Flex_getParent(target); node != NULL; node = Flex_getParent(node))
        {
            Box *box = Flex_getContext(node);
            if (box->style.overflow != CSS_OVERFLOW_VISIBLE)
            {
                plutovg_matrix_t to_local = box->result.to_screen_matrix;
                plutovg_matrix_invert(&to_local);
                plutovg_point_t dst;
                plutovg_matrix_map_point(&to_local, &(plutovg_point_t){x, y}, &dst);
                float width = Flex_getResultWidth(node);
                float height = Flex_getResultHeight(node);

                int x = dst.x - Flex_getResultBorderLeft(node);
                int y = dst.y - Flex_getResultBorderTop(node);

                if (x >= 0 && x < box->client_width && y >= 0 && y < box->client_height)
                    continue;
                else
                    return 0;
            }
        }
        return 1;
    }

    return 0;
}

static plutovg_path_t *round4_rect(float r[4][2], int x, int y, int w, int h)
{
    static const float a[4][3][2] = {
        {{0, -KAPPA90}, {1 - KAPPA90, -1}, {1, -1}},
        {{KAPPA90, 0}, {1, 1 - KAPPA90}, {1, 1}},
        {{0, KAPPA90}, {-(1 - KAPPA90), 1}, {-1, 1}},
        {{-KAPPA90, 0}, {-1, -(1 - KAPPA90)}, {-1, -1}},
    };

    plutovg_path_t *path = plutovg_path_create();

    int index = 0;
    plutovg_path_move_to(path, x, y + r[index][1]);
    plutovg_path_rel_cubic_to(path,
                              r[index][0] * a[index][0][0], r[index][1] * a[index][0][1],
                              r[index][0] * a[index][1][0], r[index][1] * a[index][1][1],
                              r[index][0] * a[index][2][0], r[index][1] * a[index][2][1]);

    plutovg_path_rel_line_to(path, w - (r[index][0] + r[index + 1][0]), 0);
    index = 1;
    plutovg_path_rel_cubic_to(path,
                              r[index][0] * a[index][0][0], r[index][1] * a[index][0][1],
                              r[index][0] * a[index][1][0], r[index][1] * a[index][1][1],
                              r[index][0] * a[index][2][0], r[index][1] * a[index][2][1]);

    plutovg_path_rel_line_to(path, 0, h - (r[index][1] + r[index + 1][1]));
    index = 2;
    plutovg_path_rel_cubic_to(path,
                              r[index][0] * a[index][0][0], r[index][1] * a[index][0][1],
                              r[index][0] * a[index][1][0], r[index][1] * a[index][1][1],
                              r[index][0] * a[index][2][0], r[index][1] * a[index][2][1]);
    plutovg_path_rel_line_to(path, -w + (r[index][0] + r[index + 1][0]), 0);
    index = 3;
    plutovg_path_rel_cubic_to(path,
                              r[index][0] * a[index][0][0], r[index][1] * a[index][0][1],
                              r[index][0] * a[index][1][0], r[index][1] * a[index][1][1],
                              r[index][0] * a[index][2][0], r[index][1] * a[index][2][1]);

    plutovg_path_close(path);
    return path;
}

static void draw_image(plutovg_t *pluto, const char *path, plutovg_rect_t *r)
{
    if (!pluto || !path || !r)
    {
        LOGE("Invalid parameter");
        return;
    }

    plutovg_surface_t *img = meui_image_cache_load(path, r->w, r->h);

    if (!img)
    {
        LOGE("Unable to load image: %s\n", path);
        return;
    }

    plutovg_save(pluto);
    plutovg_set_source_surface(pluto, img, r->x, r->y);
    plutovg_fill_preserve(pluto);
    plutovg_surface_destroy(img);
    plutovg_restore(pluto);
}

static void draw_debug_rect(plutovg_t *pluto, double x, double y, double w, double h, struct plutovg_color fill_color)
{
    plutovg_save(pluto);
    plutovg_rect(pluto, x, y, w, h);
    plutovg_set_source_color(pluto, &fill_color);
    plutovg_fill(pluto);
    plutovg_restore(pluto);
}

static void draw_box_background(Box *box, plutovg_t *pluto, plutovg_rect_t *rect, plutovg_rect_t *content_rect, float radius[4], float border[4], struct plutovg_color border_color, struct plutovg_color fill_color)
{
    float r[4][2] = {0};

    float w = rect->w;
    float h = rect->h;

    if (radius != NULL)
    {
        int max_index = 0;

        for (int i = 1; i < 4; i++)
            if (radius[i] > radius[max_index])
                max_index = i;

        float radius_max = radius[max_index];

        if (radius_max > 0.0f)
        {
            float radius_next = radius[(max_index + 1) % 4];
            float radius_other = radius[(max_index + 2) % 4];
            float radius_prev = radius[(max_index + 3) % 4];

            float r1 = radius_next + radius_max > w ? w * radius_max / (radius_next + radius_max) : radius_max;
            float r2 = radius_prev + radius_max > w ? w * radius_max / (radius_prev + radius_max) : radius_max;

            r[max_index][0] = r[max_index][1] = fmaxf(r1, r2);
            r[(max_index + 1) % 4][0] = r[(max_index + 1) % 4][1] = r[max_index][0] * radius_next / radius_max;
            r[(max_index + 2) % 4][0] = r[(max_index + 2) % 4][1] = r[max_index][0] * radius_other / radius_max;
            r[(max_index + 3) % 4][0] = r[(max_index + 3) % 4][1] = r[max_index][0] * radius_prev / radius_max;
        }
    }

    plutovg_save(pluto);

    plutovg_path_t *outer_path = round4_rect(r, 0, 0, w, h);

    if (fill_color.a != 0)
    {
        plutovg_add_path(pluto, outer_path);
        plutovg_set_source_color(pluto, &fill_color);
        plutovg_fill_preserve(pluto);
    }

    if (box->style.backgroundImage && box->style.backgroundImage[0] != '\0')
    {
        draw_image(pluto, box->style.backgroundImage, rect);
    }

    plutovg_path_t *clip_path = NULL;

    if (border != NULL)
    {
        plutovg_new_path(pluto);

        float border_top = border[0] < 0.0 ? 0.0 : border[0];
        float border_right = border[1] < 0.0 ? 0.0 : border[1];
        float border_bottom = border[2] < 0.0 ? 0.0 : border[2];
        float border_left = border[3] < 0.0 ? 0.0 : border[3];
        float border_h = border_right + border_left;
        float border_v = border_top + border_bottom;

        float r_inner[4][2] = {
            {
                (r[0][0] - border_left) > 0.0f ? ((w - r[0][0] > border_right) ? (r[0][0] - border_left) : h - border_h) : 0,
                (r[0][1] - border_top) > 0.0f ? ((h - r[0][1] > border_bottom) ? (r[0][1] - border_top) : h - border_v) : 0,
            },
            {
                (r[1][0] - border_right) > 0.0f ? ((w - r[1][0] > border_left) ? (r[1][0] - border_right) : h - border_h) : 0,
                (r[1][1] - border_top) > 0.0f ? ((h - r[1][1] > border_bottom) ? (r[1][1] - border_top) : h - border_v) : 0,
            },
            {
                (r[2][0] - border_right) > 0.0f ? ((w - r[2][0] > border_left) ? (r[2][0] - border_right) : h - border_h) : 0,
                (r[2][1] - border_bottom) > 0.0f ? ((h - r[2][1] > border_top) ? (r[2][1] - border_bottom) : h - border_v) : 0,
            },
            {
                (r[3][0] - border_left) > 0.0f ? ((w - r[3][0] > border_right) ? (r[3][0] - border_left) : h - border_h) : 0,
                (r[3][1] - border_bottom) > 0.0f ? ((h - r[3][1] > border_top) ? (r[3][1] - border_bottom) : h - border_v) : 0,
            },
        };

        plutovg_path_t *inner_path = round4_rect(r_inner, border_left, border_top,
                                                 w - border_right - border_left,
                                                 h - border_top - border_bottom);
        plutovg_add_path(pluto, inner_path);

        if (box->style.contentImage && box->style.contentImage[0] != '\0')
        {
            draw_image(pluto, box->style.contentImage, content_rect);
        }

        if (border_color.a != 0)
        {
            plutovg_add_path(pluto, outer_path);
            plutovg_set_fill_rule(pluto, plutovg_fill_rule_even_odd);

            plutovg_set_source_color(pluto, &border_color);
            plutovg_fill(pluto);
        }
        else
        {
            plutovg_new_path(pluto);
        }

        if (box->style.overflow != CSS_OVERFLOW_VISIBLE)
        {
            clip_path = plutovg_path_reference(inner_path);
        }
        plutovg_path_destroy(inner_path);
    }
    plutovg_path_destroy(outer_path);

    plutovg_restore(pluto);

    if (clip_path)
    {
        plutovg_add_path(pluto, clip_path);
        plutovg_path_destroy(clip_path);
        plutovg_clip(pluto);
    }
}

FlexSize box_measure_text(void *context, FlexSize constrainedSize)
{
    Box *box = context;

    const char *fontFamily = box->style.fontFamily;
    double size = box->style.fontSize;
    TEXT_ALIGN align = box->style.textAlign;
    const char *utf8 = box->text;
    if (utf8 && utf8[0] != '\0')
    {
        plutovg_font_t *font = meui_get_font(meui_get_instance(), fontFamily, size);
        FlexSize outSize = measure_font_get_textn_path(font, align, utf8, strlen(utf8), constrainedSize.width, constrainedSize.height);
        return outSize;
    }
    return (FlexSize){.width = constrainedSize.width, .height = 0};
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

void box_updateStyleRecursive(box_t node)
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
        box_updateStyleRecursive(Flex_getChild(node, i));
    }
}

static void box_draw_self(Box *box, plutovg_t *pluto)
{
    box_t node = box->node;

    float left = Flex_getResultLeft(node);
    float top = Flex_getResultTop(node);
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);

    double content_width = width - Flex_getResultPaddingLeft(node) - Flex_getResultPaddingRight(node);
    double content_height = height - Flex_getResultPaddingTop(node) - Flex_getResultPaddingBottom(node);
    double content_left = Flex_getResultPaddingLeft(node);
    double content_top = Flex_getResultPaddingTop(node);

    plutovg_rect_t content_rect = {content_left, content_top, content_width, content_height};

    draw_box_background(box, pluto, &(plutovg_rect_t){0, 0, width, height},
                        &content_rect,
                        (float[]){
                            box->style.borderTopLeftRadius,
                            box->style.borderTopRightRadius,
                            box->style.borderBottomRightRadius,
                            box->style.borderBottomLeftRadius,
                        },
                        (float[]){
                            Flex_getResultBorderTop(node),
                            Flex_getResultBorderRight(node),
                            Flex_getResultBorderBottom(node),
                            Flex_getResultBorderLeft(node),
                        },
                        box->style.borderColor, box->style.backgroundColor);

    if (box->text && box->text[0] != '\0')
        draw_text(box, pluto, box->style.fontFamily, box->style.fontSize, &box->style.fontColor, box->style.textAlign, box->text, &content_rect);
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
        box_drawRecursive(pluto, Flex_getChild(node, i), pq);
        plutovg_restore(pluto);
    }
}

void box_drawRecursive(plutovg_t *pluto, box_t node, pqueue_t *pq)
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

static int cmp_pri(pqueue_pri_t next, pqueue_pri_t curr)
{
    return (next >= curr);
}

static pqueue_pri_t get_pri(void *a)
{
    Box *box = Flex_getContext((box_t)a);
    return box->style.zIndex;
}

static void set_pri(void *a, pqueue_pri_t pri)
{
    Box *box = Flex_getContext((box_t)a);
    box->style.zIndex = pri;
}

static size_t get_pos(void *a)
{
    Box *box = Flex_getContext((box_t)a);

    return box->queue_pos;
}

static void set_pos(void *a, size_t pos)
{
    Box *box = Flex_getContext((box_t)a);
    box->queue_pos = pos;
}

void box_drawRecursiveQueue(plutovg_t *pluto, box_t root)
{
    pqueue_t *pq = pqueue_init(10, cmp_pri, get_pri, set_pri, get_pos, set_pos);
    box_drawRecursive(pluto, root, pq);

    if (pqueue_peek(pq) != NULL)
    {
        box_t box = NULL;
        while ((box = pqueue_pop(pq)))
        {
            box_t parent = Flex_getParent(box);
            Box *parentBox = Flex_getContext(parent);

            plutovg_save(pluto);

            plutovg_set_matrix(pluto, &parentBox->result.to_screen_matrix);
            box_drawRecursiveQueue(pluto, box);
            plutovg_restore(pluto);
        }
    }
    pqueue_free(pq);
}

void box_draw(box_t root)
{
    plutovg_t *pluto = plutovg_create(meui_get_surface(meui_get_instance()));
    Box *box = Flex_getContext(root);
    plutovg_matrix_init_identity(&box->result.to_screen_matrix);

    box_drawRecursiveQueue(pluto, root);
    plutovg_destroy(pluto);
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

static box_t box_search(box_t node, int x, int y)
{
    int len = Flex_getChildrenCount(node);

    for (int i = len - 1; i >= 0; i--)
    {
        box_t child = Flex_getChild(node, i);
        Box *box = Flex_getContext(child);
        box->index_in_parent = i;

        if (!box_style_is_unset(&box->style, BOX_STYLE_zIndex))
            continue;

        box_t ret = box_search(child, x, y);

        if (ret)
            return ret;
    }

    int hit = box_hit(node, x, y);

    if (hit)
        return node;
    else
        return NULL;
}

box_t box_search_queue(box_t node, int x, int y)
{
    pqueue_t *pq = pqueue_init(10, cmp_pri, get_pri, set_pri, get_pos, set_pos);
    box_get_zindex_queue(node, pq);

    if (pqueue_peek(pq) != NULL)
    {
        box_t box = NULL;
        while ((box = pqueue_pop(pq)))
        {
            Box *pBox = Flex_getContext(box);
            box_t ret = box_search_queue(box, x, y);

            if (ret)
            {
                pqueue_free(pq);
                return ret;
            }
        }
    }

    pqueue_free(pq);

    return box_search(node, x, y);
}