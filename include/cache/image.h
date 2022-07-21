#pragma once

#include "common/lru.h"
#include <plutovg.h>

struct lru_t *image_lru_alloc();
plutovg_surface_t *image_lru_get(struct lru_t *lru, const char *path, int width, int height);
int image_lru_set(struct lru_t *lru, const char *path, int width, int height);
plutovg_surface_t *image_lru_load(struct lru_t *lru, const char *path, int width, int height);