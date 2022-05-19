/* 
 * MEUI: sdl2 platform driver
 * 
 * Copyright (c) 2022 SimonQian
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <unistd.h>
#include <sys/eventfd.h>

#include <SDL2/SDL.h>
#include <meui.h>

struct window_t {
    struct {
        SDL_Window *window;
        SDL_Surface *surface;
        int event_cnt;
    } sdl;

    struct {
        int fd;
    } event;
};

static int window_event_watcher(void *userdata, SDL_Event * event)
{
    struct window_t *window = userdata;
    uint64_t u = 1;

    write(window->event.fd, &u, sizeof(u));
    return 1;
}

struct window_t *window_create(const char *title, int width, int height)
{
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
        return NULL;
    }

    struct window_t *window = calloc(1, sizeof(struct window_t));
    if (NULL == window) { goto fail; }
    window->sdl.window = SDL_CreateWindow(title, 0, 0, width, height, 0);
    if (NULL == window->sdl.window) { goto free_window_and_fail; }
    window->sdl.surface = SDL_GetWindowSurface(window->sdl.window);
    if (NULL == window->sdl.surface) { goto free_sdl_window_and_fail; }
    window->event.fd = eventfd(0, EFD_NONBLOCK);
    if (window->event.fd < 0) { goto free_sdl_surface_and_fail; }
    SDL_AddEventWatch(window_event_watcher, window);

    return window;
free_sdl_surface_and_fail:
    SDL_FreeSurface(window->sdl.surface);
free_sdl_window_and_fail:
    free(window->sdl.window);
free_window_and_fail:
    free(window);
fail:
    return NULL;
}

void window_destory(struct window_t *window)
{
    SDL_FreeSurface(window->sdl.surface);
    SDL_DestroyWindow(window->sdl.window);
    free(window);
}

char *window_get_image_data(struct window_t *window)
{
    return (char *)window->sdl.surface->pixels;
}

int window_connect_number(struct window_t *window)
{
    return window->event.fd;
}

int window_update_image(struct window_t *window)
{
    SDL_UpdateWindowSurface(window->sdl.window);
    return 0;
}

void window_set_name(struct window_t *window, const char *name)
{
    SDL_SetWindowTitle(window->sdl.window, name);
}

int window_pending(struct window_t *window)
{
    uint64_t u;
    if (read(window->event.fd, &u, sizeof(u)) <= 0) {
        return 0;
    }
    return (int)u;
}

void window_next_event(struct window_t *window, struct meui_event_t *meui_event)
{
    SDL_Event event;

    meui_event->type = MEUI_EVENT_NULL;
read_next:
    if (!SDL_PollEvent(&event)) {
        return;
    }

    switch (event.type) {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        *meui_event         = (struct meui_event_t) {
            .type           = event.type == SDL_KEYDOWN ? MEUI_EVENT_KEY_DOWN : MEUI_EVENT_KEY_UP,
            .KEY_DOWN       = {
                .keyCode    = event.key.keysym.scancode,
                .key        = SDL_GetKeyName(event.key.keysym.sym),
                .shiftKey   = event.key.keysym.mod & KMOD_SHIFT,
                .ctrlKey    = event.key.keysym.mod & KMOD_CTRL,
                .altKey     = event.key.keysym.mod & KMOD_ALT,
            },
        };
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        *meui_event         = (struct meui_event_t) {
            .type           = event.type == SDL_MOUSEBUTTONDOWN ? MEUI_EVENT_MOUSE_DOWN : MEUI_EVENT_MOUSE_UP,
            .MOUSE_DOWN     = {
                .x          = event.button.x,
                .y          = event.button.y,
                .button     = (event.button.button == SDL_BUTTON_LEFT) ? MEUI_MOUSE_PRIMARY_BUTTON
                            : (event.button.button == SDL_BUTTON_MIDDLE) ? MEUI_MOUSE_AUXILIARY_BUTTON
                            : (event.button.button == SDL_BUTTON_RIGHT) ? MEUI_MOUSE_SECONDARY_BUTTON : 0,
            },
        };
        break;
    case SDL_MOUSEMOTION:
        *meui_event         = (struct meui_event_t) {
            .type           = MEUI_EVENT_MOUSE_MOVE,
            .MOUSE_MOVE     = {
                .x          = event.motion.x,
                .y          = event.motion.y,
            },
        };
        break;
    case SDL_MOUSEWHEEL:
        *meui_event         = (struct meui_event_t) {
            .type           = MEUI_EVENT_MOUSE_WHEEL,
            .MOUSE_WHEEL    = {
                .deltaX     = event.wheel.x,
                .deltaY     = event.wheel.y,
            },
        };
        break;
    default:
        goto read_next;
    }
}
