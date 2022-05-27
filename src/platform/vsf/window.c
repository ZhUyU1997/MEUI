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

// covert HID scancode to w3c key value
// https://developer.mozilla.org/en-US/docs/Web/API/UI_Events/Keyboard_event_key_values
static const char * vsf_input_keyboard_get_key(struct window_t *window, vk_input_evt_t *evt)
{
    static const char * keymap[] = {
        NULL, NULL, NULL, NULL,     // 0 - 3

        "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p",
        "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6",
        "7", "8", "9", "0",         // 4 - 39
        "Enter",                    // 40
        "Escape",                   // 41
        "Backspace",                // 42
        "Tab",                      // 43
        " ", "-", "=", "[", "]", "\\", "#", ";", "'", ",", ".", "/",
                                    // 44 - 56
        "CapsLock",                 // 57
        "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
                                    // 58 - 69
        "PrintScreen",              // 70
        "ScrollLock",               // 71
        "Pause",                    // 72
        "Insert",                   // 73
        "Home",                     // 74
        "PageUp",                   // 75
        "Delete",                   // 76
        "End",                      // 77
        "PageDown",                 // 78
        "ArrowRight",               // 79
        "ArrowLeft",                // 80
        "ArrowDown",                // 81
        "ArrowUp",                  // 82
        "NumLock",                  // 83
        "Divide",                   // 84
        "Multiply",                 // 85
        "Subtract",                 // 86
        "Add",                      // 87
        "Enter",                    // 88
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
                                    // 89 - 98
        "Decimal",                  // 99
        NULL,                       // 100
        "Compose",                  // 101
        "Power",                    // 102
        "=",                        // 103
        "F13", "F14", "F15",        // 104 - 106
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                    // 107 - 115: F16 - F24
        "Execute",                  // 116
        "Help",                     // 117
        "Props",                    // 118
        "Select",                   // 119
        "Stop",                     // 120 TODO:
        "Again",                    // 121
        "Undo",                     // 122
        "Cut",                      // 123
        "Copy",                     // 124
        "Paste",                    // 125
        "Find",                     // 126
        "AudioVolumeMute",          // 127
        "AudioVolumeUp",            // 128
        "AudioVolumeDown",          // 129
        NULL, NULL, NULL,           // 130 - 132: Locking CapsLock, Locking NumLock, Locking ScrollLock
        "Separator",                // 133
        NULL,                       // 134
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                    // 135 - 143: International1 - International9
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                    // 144 - 152: LANG1 - LANG9
        NULL,                       // 153: Alternate Erase
        "Attn",                     // 154
        "Cancel",                   // 155
        "Clear",                    // 156
        NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        "CrSel",                    // 163
        "ExSel",                    // 164
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL,                 // 176 - 177: KP_00 - KP_000
        NULL, NULL,                 // 178 - 179: Thounds Separator - Decimal Separator
        NULL, NULL,                 // 180 - 181: Currency Unit - Currency Sub-unit

        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, 
//        "(", ")", "{", "}", "Tab", "Backspace", "A", "B", "C", "D", "E", "F",
//        "XOR", "^", "%", "<", ">", "&", "&&", "|", "||", ":", "#", " ", "@", "!",
                                    // 182 - 207

        NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                    // 208 - 214: Memory Store/Recall/Clear/Add/Subtract/Multiply/Divide
        NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                    // 215 - 221: Keypad +-/Clear/Clear Entry/Binary/Octal/Decimal/Hexadecimal
        NULL, NULL,
        "Control", "Shift", "Alt", "Meta",
        "Control", "Shift", "Alt", "Meta",
                                    // 224 - 231
        NULL,                       // 232: MEDIA PLAYPAUSE
        NULL,                       // 233: MEDIA STOPCD
        "MediaTrackPrevious",       // 234
        "MediaTrackNext",           // 235
        "Eject",                    // 236
        "AudioVolumeUp",            // 237
        "AudioVolumeDown",          // 238
        "AudioVolumeMute",          // 239
        "LaunchWebBrowser",         // 240
        NULL, NULL,                 // 241 - 242: MEDIA BACK/FORWARD
        "MediaStop",                // 243
        NULL, NULL, NULL,           // 244: MEDIA FIND/SCROLLUP/SCROLLDOWN
        NULL,                       // 245: MEDIA EDIT
        NULL,                       // 246: MEDIA SLEEP
        NULL,                       // 247: MEDIA COFFEE
        NULL,                       // 248: MEDIA REFRESH
        NULL,                       // 249: MEDIA CALC
    };
    static const char * KEYMAP[] = {
        NULL, NULL, NULL, NULL,     // 0 - 3

        "A", "B", "C", "D", "E", "F", "F", "H", "I", "J", "K", "L", "M", "N", "O", "P",
        "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "!", "@", "#", "$", "%", "^",
        "&", "*", "(", ")",         // 4 - 39
        NULL, NULL, NULL, NULL,     // 40 - 43
        " ", "_", "+", "{", "}", "|", "#", ":", "\"", "<", ">", "?",
                                    // 44 - 56
    };

    uint16_t mod = vsf_input_keyboard_get_keymod(evt);
    uint16_t keycode = vsf_input_keyboard_get_keycode(evt);
    bool is_shift_down = mod & VSF_KM_SHIFT, is_upper = false;

    if (    (mod & VSF_KM_CAPSLOCK)
        &&  ((keycode >= VSF_KB_a) && (keycode <= VSF_KB_z))) {
        is_upper = !is_upper;
     }
    if (    is_shift_down
        &&  (   ((keycode >= VSF_KB_1) && (keycode <= VSF_KB_0))
            ||  ((keycode >= VSF_KB_MINUS) && (keycode <= VSF_KB_SLASH))
            )
       ) {
        is_upper = !is_upper;
    }
    const char *str = is_upper ? KEYMAP[keycode] : keymap[keycode];
    return (keycode > dimof(keymap)) || (NULL == str) ? "Unidentified" : str;
}

void window_next_event(struct window_t *window, struct meui_event_t *meui_event)
{
    window_evt_t evt;
    int result;

    meui_event->type = MEUI_EVENT_NULL;
read_next:
    result = read(window->event.sfd->fd, &evt, sizeof(evt));
    if (result <= 0) {
        return;
    }

    switch (evt.type) {
    case VSF_INPUT_TYPE_KEYBOARD: {
            uint16_t mod        = vsf_input_keyboard_get_keymod(&evt.evt);
            *meui_event         = (struct meui_event_t) {
                .type           = vsf_input_keyboard_is_down(&evt.evt) ? MEUI_EVENT_KEY_DOWN : MEUI_EVENT_KEY_UP,
                .KEY_DOWN       = {
                    .keyCode    = vsf_input_keyboard_get_keycode(&evt.evt),
                    .key        = vsf_input_keyboard_get_key(window, &evt.evt),
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
