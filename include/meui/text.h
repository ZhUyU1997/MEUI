/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <plutovg.h>

#include "meui/box.h"

plutovg_path_t *get_textn_oneline_path(const plutovg_font_t *font, const char **utf8, const char *end, double w, double *out_w);
plutovg_rect_t measure_textn_oneline_path(const plutovg_font_t *font, const char **utf8, const char *end, double w, double *out_w);
FlexSize measure_font_get_textn_path(const plutovg_font_t *font, TEXT_ALIGN align, const char *utf8, int size, double w, double h);
void draw_text(Box *box, plutovg_t *pluto, const char *fontFamily, double fontSize, plutovg_color_t *color, TEXT_ALIGN align, const char *utf8, plutovg_rect_t *rect);
