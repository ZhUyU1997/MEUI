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

#include <stdlib.h>
#include <SDL.h>
#include "meui.h"

#ifndef dimof
#define dimof(__arr) (sizeof(__arr) / sizeof((__arr)[0]))
#endif

struct window_t
{
    struct
    {
        SDL_Window *window;
        SDL_Surface *surface;
    } sdl;
    plutovg_surface_t *surface;
};

struct window_t *window_create(const char *title, int width, int height, plutovg_color_format_t format)
{
    if (format != plutovg_color_format_argb32)
        return NULL;

    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS))
    {
        return NULL;
    }

    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");

    struct window_t *window = calloc(1, sizeof(struct window_t));
    if (NULL == window)
    {
        goto fail;
    }
    window->sdl.window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (NULL == window->sdl.window)
    {
        goto free_window_and_fail;
    }
    window->sdl.surface = SDL_GetWindowSurface(window->sdl.window);
    if (NULL == window->sdl.surface)
    {
        goto free_sdl_window_and_fail;
    }
    window->surface = plutovg_surface_create_for_formated_data((unsigned char *)window->sdl.surface->pixels, width, height, sizeof(uint32_t) * width, format);
    if (NULL == window->surface)
    {
        goto free_sdl_surface_and_fail;
    }
    return window;
free_sdl_surface_and_fail:
    plutovg_surface_destroy(window->surface);
free_sdl_window_and_fail:
    SDL_DestroyWindow(window->sdl.window);
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

unsigned char *window_get_image_data(struct window_t *window)
{
    return (unsigned char *)window->sdl.surface->pixels;
}

plutovg_surface_t *window_get_surface(struct window_t *window)
{
    return window->surface;
}

int window_connect_number(struct window_t *window)
{
    return -1;
}

int window_update_image(struct window_t *window, int x, int y, int w, int h)
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
    return SDL_HasEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
}

// covert HID scancode to w3c key value
// https://developer.mozilla.org/en-US/docs/Web/API/UI_Events/Keyboard_event_key_values
static const char *vsf_input_keyboard_get_key(struct window_t *window, SDL_Event *evt)
{
    static const char *keymap[] = {
        NULL, NULL, NULL, NULL, // 0 - 3

        "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p",
        "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6",
        "7", "8", "9", "0", // 4 - 39
        "Enter",            // 40
        "Escape",           // 41
        "Backspace",        // 42
        "Tab",              // 43
        " ", "-", "=", "[", "]", "\\", "#", ";", "'", "`", ",", ".", "/",
        // 44 - 56
        "CapsLock", // 57
        "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
        // 58 - 69
        "PrintScreen", // 70
        "ScrollLock",  // 71
        "Pause",       // 72
        "Insert",      // 73
        "Home",        // 74
        "PageUp",      // 75
        "Delete",      // 76
        "End",         // 77
        "PageDown",    // 78
        "ArrowRight",  // 79
        "ArrowLeft",   // 80
        "ArrowDown",   // 81
        "ArrowUp",     // 82
        "NumLock",     // 83
        "Divide",      // 84
        "Multiply",    // 85
        "Subtract",    // 86
        "Add",         // 87
        "Enter",       // 88
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
        // 89 - 98
        "Decimal",           // 99
        NULL,                // 100
        "Compose",           // 101
        "Power",             // 102
        "=",                 // 103
        "F13", "F14", "F15", // 104 - 106
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        // 107 - 115: F16 - F24
        "Execute",         // 116
        "Help",            // 117
        "Props",           // 118
        "Select",          // 119
        "Stop",            // 120 TODO:
        "Again",           // 121
        "Undo",            // 122
        "Cut",             // 123
        "Copy",            // 124
        "Paste",           // 125
        "Find",            // 126
        "AudioVolumeMute", // 127
        "AudioVolumeUp",   // 128
        "AudioVolumeDown", // 129
        NULL, NULL, NULL,  // 130 - 132: Locking CapsLock, Locking NumLock, Locking ScrollLock
        "Separator",       // 133
        NULL,              // 134
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        // 135 - 143: International1 - International9
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        // 144 - 152: LANG1 - LANG9
        NULL,     // 153: Alternate Erase
        "Attn",   // 154
        "Cancel", // 155
        "Clear",  // 156
        NULL, NULL, NULL, NULL, NULL, NULL,
        "CrSel", // 163
        "ExSel", // 164
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, // 176 - 177: KP_00 - KP_000
        NULL, NULL, // 178 - 179: Thounds Separator - Decimal Separator
        NULL, NULL, // 180 - 181: Currency Unit - Currency Sub-unit

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
        NULL,                 // 232: MEDIA PLAYPAUSE
        NULL,                 // 233: MEDIA STOPCD
        "MediaTrackPrevious", // 234
        "MediaTrackNext",     // 235
        "Eject",              // 236
        "AudioVolumeUp",      // 237
        "AudioVolumeDown",    // 238
        "AudioVolumeMute",    // 239
        "LaunchWebBrowser",   // 240
        NULL, NULL,           // 241 - 242: MEDIA BACK/FORWARD
        "MediaStop",          // 243
        NULL, NULL, NULL,     // 244: MEDIA FIND/SCROLLUP/SCROLLDOWN
        NULL,                 // 245: MEDIA EDIT
        NULL,                 // 246: MEDIA SLEEP
        NULL,                 // 247: MEDIA COFFEE
        NULL,                 // 248: MEDIA REFRESH
        NULL,                 // 249: MEDIA CALC
    };
    static const char *KEYMAP[] = {
        NULL, NULL, NULL, NULL, // 0 - 3

        "A", "B", "C", "D", "E", "F", "F", "H", "I", "J", "K", "L", "M", "N", "O", "P",
        "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "!", "@", "#", "$", "%", "^",
        "&", "*", "(", ")",     // 4 - 39
        NULL, NULL, NULL, NULL, // 40 - 43
        " ", "_", "+", "{", "}", "|", "#", ":", "\"", "~", "<", ">", "?",
        // 44 - 56
    };

    Uint16 mod = evt->key.keysym.mod;
    SDL_Scancode keycode = evt->key.keysym.scancode;
    bool is_upper = false;

    if ((mod & KMOD_CAPS) && ((keycode >= SDL_SCANCODE_A) && (keycode <= SDL_SCANCODE_Z)))
    {
        is_upper = !is_upper;
    }
    if ((mod & KMOD_SHIFT) && (((keycode >= SDL_SCANCODE_1) && (keycode <= SDL_SCANCODE_0)) || ((keycode >= SDL_SCANCODE_MINUS) && (keycode <= SDL_SCANCODE_SLASH)) || ((keycode >= SDL_SCANCODE_A) && (keycode <= SDL_SCANCODE_Z))))
    {
        is_upper = !is_upper;
    }
    const char *str = is_upper ? KEYMAP[keycode] : keymap[keycode];
    return (keycode > dimof(keymap)) || (NULL == str) ? "Unidentified" : str;
}

void window_next_event(struct window_t *window, struct meui_event_t *meui_event)
{
    SDL_Event event;

    meui_event->type = MEUI_EVENT_NULL;
read_next:
    if (!SDL_PollEvent(&event))
    {
        return;
    }

    switch (event.type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        *meui_event = (struct meui_event_t){
            .type = event.type == SDL_KEYDOWN ? MEUI_EVENT_KEY_DOWN : MEUI_EVENT_KEY_UP,
            .KEY_DOWN = {
                .keyCode = event.key.keysym.scancode,
                .key = vsf_input_keyboard_get_key(window, &event),
                .shiftKey = event.key.keysym.mod & KMOD_SHIFT,
                .ctrlKey = event.key.keysym.mod & KMOD_CTRL,
                .altKey = event.key.keysym.mod & KMOD_ALT,
            },
        };
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        *meui_event = (struct meui_event_t){
            .type = event.type == SDL_MOUSEBUTTONDOWN ? MEUI_EVENT_MOUSE_DOWN : MEUI_EVENT_MOUSE_UP,
            .MOUSE_DOWN = {
                .x = event.button.x,
                .y = event.button.y,
                .button = (event.button.button == SDL_BUTTON_LEFT)     ? MEUI_MOUSE_PRIMARY_BUTTON
                          : (event.button.button == SDL_BUTTON_MIDDLE) ? MEUI_MOUSE_AUXILIARY_BUTTON
                          : (event.button.button == SDL_BUTTON_RIGHT)  ? MEUI_MOUSE_SECONDARY_BUTTON
                                                                       : 0,
            },
        };
        break;
    case SDL_MOUSEMOTION:
        *meui_event = (struct meui_event_t){
            .type = MEUI_EVENT_MOUSE_MOVE,
            .MOUSE_MOVE = {
                .x = event.motion.x,
                .y = event.motion.y,
            },
        };
        break;
    case SDL_MOUSEWHEEL:
        *meui_event = (struct meui_event_t){
            .type = MEUI_EVENT_MOUSE_WHEEL,
            .MOUSE_WHEEL = {
                .deltaX = event.wheel.x,
                .deltaY = event.wheel.y,
            },
        };
        break;
    case SDL_QUIT:
        *meui_event = (struct meui_event_t){
            .type = MEUI_EVENT_UNLOAD,
        };
        break;
    default:
        goto read_next;
    }
}
