/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "common/lru.h"
#include "meui.h"

#include <plutovg.h>
#include <plutosvg.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    plutovg_surface_t *text;
} text_cache_t;

typedef struct
{
    double font_size;
    plutovg_color_t font_color;
    int ch;
    char font_family[0];
} text_lru_key_t;

static void lru_callback(struct lru_t *l, const char *key, int nkey, void *buf, int nbuf)
{
    text_cache_t *cache = buf;
    plutovg_surface_destroy(cache->text);
}

static text_lru_key_t *key_alloc(const char *font_family, size_t path_len, int ch, double font_size, plutovg_color_t font_color)
{
    text_lru_key_t *key = calloc(1, sizeof(text_lru_key_t) + path_len);
    key->ch = ch;
    key->font_size = font_size;
    key->font_color = font_color;
    memcpy(key->font_family, font_family, path_len);
    return key;
}

static int lru_get_by_text_key(struct lru_t *lru, text_cache_t *cache, const char *font_family, int ch, double font_size, plutovg_color_t font_color)
{
    size_t len = strlen(font_family);
    text_lru_key_t *key = key_alloc(font_family, len, ch, font_size, font_color);
    int ret = lru_get(lru, (const char *)key, sizeof(text_lru_key_t) + len, cache, sizeof(text_cache_t));
    free(key);
    return ret;
}

static int lru_set_by_text_key(struct lru_t *lru, const char *font_family, int ch, double font_size, plutovg_color_t font_color, text_cache_t *cache)
{
    size_t len = strlen(font_family);
    text_lru_key_t *key = key_alloc(font_family, len, ch, font_size, font_color);
    int ret = lru_set(lru, (const char *)key, sizeof(text_lru_key_t) + len, cache, sizeof(text_cache_t));
    free(key);
    return ret;
}

plutovg_surface_t *text_lru_get(struct lru_t *lru, const char *font_family, int ch, double font_size, plutovg_color_t font_color)
{
    text_cache_t cache;
    int ret = lru_get_by_text_key(lru, &cache, font_family, ch, font_size, font_color);
    if (ret)
    {
        plutovg_surface_reference(cache.text);
        return cache.text;
    }
    return NULL;
}

static plutovg_surface_t *__text_lru_set(struct lru_t *lru, const char *font_family, int ch, double font_size, plutovg_color_t font_color)
{
    plutovg_surface_t *text = NULL;
    plutovg_font_t *font = meui_get_font(meui_get_instance(), font_family, font_size);
    plutovg_font_face_t *face = plutovg_font_get_face(font);

    double scale = plutovg_font_get_scale(font);
    double ascent = plutovg_font_get_ascent(font);
    double descent = plutovg_font_get_descent(font);
    double line_gap = plutovg_font_get_line_gap(font);
    double leading = plutovg_font_get_leading(font);
    double char_advance = plutovg_font_get_char_advance(font, ch);

    plutovg_matrix_t m;
    plutovg_matrix_init_translate(&m, 0, ascent);
    plutovg_matrix_scale(&m, scale, -scale);

    text = plutovg_surface_create(char_advance, ascent - descent);
    plutovg_t *pluto = plutovg_create(text);

    plutovg_path_t *clone = plutovg_path_create();

    plutovg_path_t *path = plutovg_font_face_get_char_path(face, ch);
    plutovg_path_add_path(clone, path, &m);
    plutovg_path_destroy(path);

    plutovg_add_path(pluto, clone);
    plutovg_set_source_color(pluto, &font_color);
    plutovg_fill(pluto);

    plutovg_path_destroy(clone);

    plutovg_destroy(pluto);
    plutovg_font_destroy(font);

    if (text == NULL)
        return NULL;

    text_cache_t cache;
    cache.text = text;
    lru_set_by_text_key(lru, font_family, ch, font_size, font_color, &cache);
    return text;
}

int text_lru_set(struct lru_t *lru, const char *font_family, int ch, double font_size, plutovg_color_t font_color)
{
    return __text_lru_set(lru, font_family, ch, font_size, font_color) != NULL;
}

plutovg_surface_t *text_lru_load(struct lru_t *lru, const char *font_family, int ch, double font_size, plutovg_color_t font_color)
{
    plutovg_surface_t *text = text_lru_get(lru, font_family, ch, font_size, font_color);

    if (text == NULL)
    {
        text = __text_lru_set(lru, font_family, ch, font_size, font_color);
        plutovg_surface_reference(text);
    }
    return text;
}

struct lru_t *text_lru_alloc()
{
    struct lru_t *lru = lru_alloc(sizeof(text_cache_t) * 1024, 0, lru_callback);
    return lru;
}
