/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "common/lru.h"
#include <plutovg.h>

struct lru_t *text_lru_alloc();
plutovg_surface_t *text_lru_get(struct lru_t *lru, const char *font_family, int ch, double font_size);
int text_lru_set(struct lru_t *lru, const char *font_family, int ch, double font_size);
plutovg_surface_t *text_lru_load(struct lru_t *lru, const char *font_family, int ch, double font_size);