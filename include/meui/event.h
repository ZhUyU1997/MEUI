#pragma once

enum MEUI_EVENT_TYPE
{
    MEUI_EVENT_NULL,
    MEUI_EVENT_LOAD,
    MEUI_EVENT_UNLOAD,
    MEUI_EVENT_KEY_DOWN,
    MEUI_EVENT_KEY_PRESS,
    MEUI_EVENT_KEY_UP,
    MEUI_EVENT_MOUSE_DOWN,
    MEUI_EVENT_MOUSE_ENTER,
    MEUI_EVENT_MOUSE_LEAVE,
    MEUI_EVENT_MOUSE_MOVE,
    MEUI_EVENT_MOUSE_OVER,
    MEUI_EVENT_MOUSE_OUT,
    MEUI_EVENT_MOUSE_UP,
    MEUI_EVENT_MOUSE_WHEEL,
};

typedef struct meui_event_t
{
    enum MEUI_EVENT_TYPE type;

    union
    {
        struct {} LOAD;
        struct {} UNLOAD;
        struct {} KEY_DOWN;
        struct {} KEY_PRESS;
        struct {} KEY_UP;
        struct { int x;int y; } MOUSE_DOWN;
        struct {} MOUSE_ENTER;
        struct {} MOUSE_LEAVE;
        struct {int x;int y; } MOUSE_MOVE;
        struct {} MOUSE_OVER;
        struct {} MOUSE_OUT;
        struct {int x;int y; } MOUSE_UP;
        struct {} MOUSE_WHEEL;
    };
} meui_event_t;
