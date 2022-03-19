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

// https://www.w3.org/TR/uievents/#dom-mouseevent-button
enum MEUI_MOUSE_BUTTON_TYPE
{
    MEUI_MOUSE_PRIMARY_BUTTON = 0,
    MEUI_MOUSE_AUXILIARY_BUTTON = 1,
    MEUI_MOUSE_SECONDARY_BUTTON = 2,
    MEUI_MOUSE_BACK_BUTTON = 3,
    MEUI_MOUSE_FORWARD_BUTTON = 4,
};

typedef struct meui_event_t
{
    enum MEUI_EVENT_TYPE type;

    union
    {
        struct
        {
        } LOAD;
        struct
        {
        } UNLOAD;
        struct
        {
        } KEY_DOWN;
        struct
        {
        } KEY_PRESS;
        struct
        {
        } KEY_UP;
        struct
        {
            int x;
            int y;
            enum MEUI_MOUSE_BUTTON_TYPE button;
        } MOUSE_DOWN;
        struct
        {
        } MOUSE_ENTER;
        struct
        {
        } MOUSE_LEAVE;
        struct
        {
            int x;
            int y;
        } MOUSE_MOVE;
        struct
        {
        } MOUSE_OVER;
        struct
        {
        } MOUSE_OUT;
        struct
        {
            int x;
            int y;
            enum MEUI_MOUSE_BUTTON_TYPE button;
        } MOUSE_UP;
        struct
        {
            double deltaX;
            double deltaY;
            double deltaZ;
        } MOUSE_WHEEL;
    };
} meui_event_t;
