/* 
 * MEUI: vsf platform driver
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

#define __VSF_DISP_CLASS_INHERIT__
#define __VSF_LINUX_FS_CLASS_INHERIT__
#include <unistd.h>
#include <fcntl.h>

#include "usrapp_common.h"

#include <meui.h>

struct window_t {
    struct {
        vk_disp_t *dev;
        char *buff;
        int width;
        int height;
    } disp;

    union {
        struct {
            vsf_linux_fd_t *sfd;
            vk_input_notifier_t notifier;
        } event;
        struct {
            vsf_linux_fd_t *sfd;
            vk_input_notifier_t notifier;
        };
    };
};

typedef struct window_evt_t {
    vk_input_type_t type;
    vk_input_evt_t evt;
} window_evt_t;

static void __window_disp_on_ready(vk_disp_t *disp)
{
    vsf_eda_t *eda = disp->ui_data;
    VSF_ASSERT(eda != NULL);
    vsf_eda_post_evt(eda, VSF_EVT_RETURN);
}

static void __windows_on_event(vk_input_notifier_t *notifier, vk_input_type_t type, vk_input_evt_t *evt)
{
    struct window_t *window = container_of(notifier, struct window_t, notifier);
    vsf_linux_pipe_rx_priv_t *priv_rx = (vsf_linux_pipe_rx_priv_t *)window->event.sfd->priv;
    window_evt_t window_evt = {
        .type   = type,
        .evt    = evt,
    };

    vsf_stream_write(priv_rx->stream_rx, (uint8_t *)&window_evt, sizeof(window_evt));
}

struct window_t *window_create(const char *title, int width, int height)
{
    vk_disp_t *disp = usrapp_ui_common.disp;
    if ((NULL == disp) || (width > disp->param.width) || (height > disp->param.height)) {
        vsf_trace_error("MEUI: display is not large enough to hold MEUI window\n");
        return NULL;
    }

    struct window_t *window = calloc(1, sizeof(struct window_t) + width * height * 4);
    if (window != NULL) {
        window->disp.buff = (char *)&window[1];
        window->disp.dev = usrapp_ui_common.disp;
        window->disp.width = width;
        window->disp.height = height;
        if (window->disp.dev->param.color != VSF_DISP_COLOR_ARGB8888) {
            vsf_trace_warning("MEUI: display required to be ARGB8888 pixel format\n");
            *(vk_disp_color_type_t *)&window->disp.dev->param.color = VSF_DISP_COLOR_ARGB8888;
        }

        window->disp.dev->ui_data = vsf_eda_get_cur();
        window->disp.dev->ui_on_ready = __window_disp_on_ready;
        vk_disp_init(window->disp.dev);
        vsf_thread_wfe(VSF_EVT_RETURN);

        window->event.sfd = vsf_linux_rx_pipe(NULL);
        if (NULL == window->event.sfd) {
            free(window);
            return NULL;
        }
        int fd = window->event.sfd->fd;
        fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL, 0));

        window->event.notifier.mask = (1 << VSF_INPUT_TYPE_TOUCHSCREEN)
                    | (1 << VSF_INPUT_TYPE_KEYBOARD) | (1 << VSF_INPUT_TYPE_MOUSE);
        window->event.notifier.on_evt = __windows_on_event;
        vk_input_notifier_register(&window->event.notifier);
    }
    return window;
}

void window_destory(struct window_t *window)
{
    vk_input_notifier_unregister(&window->event.notifier);
    free(window);
}

char *window_get_image_data(struct window_t *window)
{
    return (char *)window->disp.buff;
}

int window_connect_number(struct window_t *window)
{
    return window->event.sfd->fd;
}

int window_update_image(struct window_t *window)
{
    vk_disp_area_t area = {
        .pos.x  = 0,
        .pos.y  = 0,
        .size.x = window->disp.width,
        .size.y = window->disp.height,
    };
    window->disp.dev->ui_data = vsf_eda_get_cur();
    vk_disp_refresh(window->disp.dev, &area, window->disp.buff);
    vsf_thread_wfe(VSF_EVT_RETURN);
    return 0;
}

void window_set_name(struct window_t *window, const char *name)
{
}

int window_pending(struct window_t *window)
{
    vsf_linux_pipe_rx_priv_t *priv_rx = (vsf_linux_pipe_rx_priv_t *)window->event.sfd->priv;
    vsf_stream_t *stream = priv_rx->stream_rx;
    return vsf_stream_get_data_size(stream) / sizeof(window_evt_t);
}

void window_next_event(struct window_t *window, struct meui_event_t *meui_event)
{
    window_evt_t evt;

    meui_event->type = MEUI_EVENT_NULL;
read_next:
    int result = read(window->event.sfd->fd, &evt, sizeof(evt));
    if (result <= 0) {
        return;
    }

    switch (evt.type) {
    case VSF_INPUT_TYPE_KEYBOARD: {
            uint8_t mod         = vsf_input_keyboard_get_keymod(&evt.evt);
            *meui_event         = (struct meui_event_t) {
                .type           = vsf_input_keyboard_is_down(&evt.evt) ? MEUI_EVENT_KEY_DOWN : MEUI_EVENT_KEY_UP,
                .KEY_DOWN       = {
                    .keyCode    = vsf_input_keyboard_get_keycode(&evt.evt),
                    .key        = NULL,     // TODO
                    .shiftKey   = mod & VSF_KM_SHIFT,
                    .ctrlKey    = mod & VSF_KM_CTRL,
                    .altKey     = mod & VSF_KM_ALT,
                },
            };
        }
        break;
    case VSF_INPUT_TYPE_MOUSE:
        switch (vk_input_mouse_evt_get(&evt.evt)) {
        case VSF_INPUT_MOUSE_EVT_MOVE:
            *meui_event         = (struct meui_event_t) {
                .type           = MEUI_EVENT_MOUSE_MOVE,
                .MOUSE_MOVE     = {
                    .x          = vk_input_mouse_evt_get_x(&evt.evt),
                    .y          = vk_input_mouse_evt_get_y(&evt.evt),
                },
            };
            break;
        case VSF_INPUT_MOUSE_EVT_BUTTON:
            *meui_event         = (struct meui_event_t) {
                .type           = vk_input_mouse_evt_button_is_down(&evt.evt) ? MEUI_EVENT_MOUSE_DOWN : MEUI_EVENT_MOUSE_UP,
                .MOUSE_DOWN     = {
                    .x          = vk_input_mouse_evt_get_x(&evt.evt),
                    .y          = vk_input_mouse_evt_get_y(&evt.evt),
                    .button     = (vk_input_mouse_evt_button_get(&evt.evt) == VSF_INPUT_MOUSE_BUTTON_LEFT) ? MEUI_MOUSE_PRIMARY_BUTTON
                                : (vk_input_mouse_evt_button_get(&evt.evt) == VSF_INPUT_MOUSE_BUTTON_MIDDLE) ? MEUI_MOUSE_AUXILIARY_BUTTON
                                : (vk_input_mouse_evt_button_get(&evt.evt) == VSF_INPUT_MOUSE_BUTTON_RIGHT) ? MEUI_MOUSE_SECONDARY_BUTTON : 0,
                },
            };
            break;
        }
        break;
    default:
        goto read_next;
    }
}
