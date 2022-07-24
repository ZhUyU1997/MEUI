/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
struct meui_event_t;
struct window_t;
struct window_t *window_create(const char *title, int width, int height);
void window_destory(struct window_t *window);
unsigned char *window_get_image_data(struct window_t *window);
int window_connect_number(struct window_t *window);
int window_update_image(struct window_t *window);
void window_set_name(struct window_t *window, const char *name);
int window_pending(struct window_t *window);
void window_next_event(struct window_t *window, struct meui_event_t *meui_event);