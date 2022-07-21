#include "common/lru.h"
#include "meui/image.h"

#include <plutovg.h>
#include <plutosvg.h>
#include <malloc.h>
#include <string.h>

typedef struct
{
    plutovg_surface_t *image;
} image_cache_t;

typedef struct
{
    int width;
    int height;
    char path[0];
} image_lru_key_t;

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

static void lru_callback(struct lru_t *l, const char *key, int nkey, void *buf, int nbuf)
{
    image_cache_t *cache = buf;
    plutovg_surface_destroy(cache->image);
    free(cache);
}

static image_lru_key_t *key_alloc(const char *path, size_t path_len, int width, int height)
{
    image_lru_key_t *key = malloc(sizeof(image_lru_key_t) + path_len);
    key->width = width;
    key->height = height;
    memcpy(key->path, path, path_len);
    return key;
}

static image_cache_t *lru_get_by_image_key(struct lru_t *lru, const char *path, int width, int height)
{
    size_t path_len = strlen(path);
    image_lru_key_t *key = key_alloc(path, path_len, width, height);
    image_cache_t *cache = NULL;
    lru_get(lru, (const char *)key, sizeof(image_lru_key_t) + path_len, &cache, sizeof(image_cache_t *));
    free(key);
    return cache;
}

static int lru_set_by_image_key(struct lru_t *lru, const char *path, int width, int height, image_cache_t *cache)
{
    size_t path_len = strlen(path);
    image_lru_key_t *key = key_alloc(path, path_len, width, height);
    int ret = lru_set(lru, (const char *)key, sizeof(image_lru_key_t) + path_len, &cache, sizeof(image_cache_t *));
    free(key);
    return ret;
}

plutovg_surface_t *image_lru_get(struct lru_t *lru, const char *path, int width, int height)
{
    image_cache_t *cache = lru_get_by_image_key(lru, path, width, height);
    if (cache)
    {
        plutovg_surface_reference(cache->image);
        return cache->image;
    }
    return NULL;
}

static plutovg_surface_t *__image_lru_set(struct lru_t *lru, const char *path, int width, int height)
{
    printf("__image_lru_set %s %d %d\n", path, width, height);
    plutovg_surface_t *img = NULL;

    if (strcmp(fileext(path), "svg") == 0)
    {
        img = plutosvg_load_from_file(path, NULL, width, height, 96.0);
    }
    else
    {
        img = pluto_load_image_from_file(path, width, height);
    }

    if (img == NULL)
        return NULL;

    image_cache_t *cache = malloc(sizeof(image_cache_t));
    cache->image = img;
    lru_set_by_image_key(lru, path, width, height, cache);
    return img;
}

int image_lru_set(struct lru_t *lru, const char *path, int width, int height)
{
    return __image_lru_set(lru, path, width, height) != NULL;
}

plutovg_surface_t *image_lru_load(struct lru_t *lru, const char *path, int width, int height)
{
    plutovg_surface_t *image = image_lru_get(lru, path, width, height);

    if (image == NULL)
    {
        image = __image_lru_set(lru, path, width, height);
        plutovg_surface_reference(image);
    }
    return image;
}

struct lru_t *image_lru_alloc()
{
    struct lru_t *lru = lru_alloc(sizeof(image_cache_t *) * 32, 0, lru_callback);
    return lru;
}
