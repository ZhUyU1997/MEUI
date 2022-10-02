/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdbool.h>
#include <plutovg.h>

plutovg_rect_t plutovg_surface_get_rect(plutovg_surface_t *surface);
plutovg_rect_t plutovg_surface_get_rect_ext(plutovg_surface_t *surface, int ext_width);
void plutovg_rect_ext(plutovg_rect_t *rect, int ext_width);
bool plutovg_rect_equal(plutovg_rect_t *a, plutovg_rect_t *b);