
#include "meui.h"
#include "meui/box.h"
#include "meui/render.h"
#include "common/log.h"
#include "cache/image.h"
#include "cache/text.h"

static struct lru_t *image_lru = NULL;

void meui_image_cache_init()
{
    if (image_lru == NULL)
        image_lru = image_lru_alloc();
}

void meui_image_cache_exit()
{
    if (image_lru)
    {
        lru_free(image_lru);
        image_lru = NULL;
    }
}

plutovg_surface_t *meui_image_cache_load(const char *path, int width, int height)
{
    return image_lru_load(image_lru, path, width, height);
}

static struct lru_t *text_lru = NULL;

void meui_text_cache_init()
{
    if (text_lru == NULL)
        text_lru = text_lru_alloc();
}

void meui_text_cache_exit()
{
    if (text_lru)
    {
        lru_free(text_lru);
        text_lru = NULL;
    }
}

plutovg_surface_t *meui_text_cache_load(const char *font_family, int ch, double font_size, plutovg_color_t font_color)
{
    return text_lru_load(text_lru, font_family, ch, font_size, font_color);
}