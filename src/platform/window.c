#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <meui.h>

struct window_t
{
    int width;
    int height;
    Display *dis;
    int screen;
    Window win;
    GC gc;
    XImage *ximage;
    XVisualInfo vinfo;

    int x11_fd;
    Atom wmDeleteMessage;
};

struct window_t *window_create(const char *title, int width, int height)
{
    struct window_t *window = calloc(1, sizeof(struct window_t));

    /* use the information from the environment variable DISPLAY
       to create the X connection:
    */
    Display *dis = window->dis = XOpenDisplay((char *)0);
    int screen = window->screen = DefaultScreen(dis);

    /* get the colors black and white (see section for details) */

    unsigned long black = BlackPixel(dis, screen); /* get color black */
    unsigned long white = WhitePixel(dis, screen); /* get color white */

    if (!XMatchVisualInfo(dis, DefaultScreen(dis), 32, TrueColor, &window->vinfo))
    {
        fprintf(stderr, "no such visual\n");
        return NULL;
    }

    /* once the display is initialized, create the window.
       This window will be have be 200 pixels across and 300 down.
       It will have the foreground white and background black
    */
    XSetWindowAttributes attr;
    attr.colormap = XCreateColormap(dis, DefaultRootWindow(dis),
                                    window->vinfo.visual, AllocNone);
    attr.border_pixel = 50;
    attr.background_pixel = white;

    int x = (XDisplayWidth(dis, screen) - width) / 2;
    int y = (XDisplayHeight(dis, screen) - height) / 2;

    Window win = window->win = XCreateWindow(dis,
                                             DefaultRootWindow(dis), x, y,
                                             width, height, 0,
                                             window->vinfo.depth,
                                             InputOutput,
                                             window->vinfo.visual,
                                             CWColormap | CWBorderPixel | CWBackPixel, &attr);

    window->width = width;
    window->height = height;

    /* here is where some properties of the window can be set.
       The third and fourth items indicate the name which appears
       at the top of the window and the name of the minimized window
       respectively.
    */
    XSetStandardProperties(dis, win, title, NULL, None, NULL, 0, NULL);

    /* this routine determines which types of input are allowed in
       the input.  see the appropriate section for details...
    */
    XSelectInput(dis, win,
                 ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask |
                     ButtonPressMask | ButtonReleaseMask | StructureNotifyMask);

    /* create the Graphics Context */
    GC gc = window->gc = XCreateGC(dis, win, 0, 0);

    /* here is another routine to set the foreground and background
       colors _currently_ in use in the window.
    */
    XSetBackground(dis, gc, white);
    XSetForeground(dis, gc, black);

    /* clear the window and bring it on top of the other windows */
    XClearWindow(dis, win);
    XMapRaised(dis, win);

    char *image32 = (char *)malloc(width * height * 4);
    window->ximage = XCreateImage(dis, window->vinfo.visual, window->vinfo.depth,
                                  ZPixmap, 0, image32,
                                  width, height, 32, 0);

    window->wmDeleteMessage = XInternAtom(dis, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dis, win, &window->wmDeleteMessage, 1);

    window->x11_fd = ConnectionNumber(dis);

    return window;
}

void window_destory(struct window_t *window)
{
    XFreeGC(window->dis, window->gc);
    XDestroyWindow(window->dis, window->win);
    XCloseDisplay(window->dis);
    free(window);
}

char *window_get_image_data(struct window_t *window)
{
    return window->ximage->data;
}

int window_connect_number(struct window_t *window)
{
    return window->x11_fd;
}

int window_update_image(struct window_t *window)
{
    return XPutImage(window->dis, window->win,
                     window->gc, window->ximage, 0, 0, 0, 0,
                     window->width, window->height);
}

void window_set_name(struct window_t *window, const char *name)
{
    XTextProperty tp;
    char *props[1] = {name};

    if (!XStringListToTextProperty(props, 1, &tp))
    {
        printf("Failed to set WMName");
        return;
    }

    XSetWMName(window->dis, window->win, &tp);
    XFree(tp.value);
}

int window_pending(struct window_t *window)
{
    return XPending(window->dis);
}

void window_next_event(struct window_t *window, struct meui_event_t *meui_event)
{
    XEvent event;
    XNextEvent(window->dis, &event);
    meui_event->type = MEUI_EVENT_NULL;

    if (event.type == ButtonPress)
    {
        switch (event.xbutton.button)
        {
        case Button1:
            *meui_event = (struct meui_event_t){
                .type = MEUI_EVENT_MOUSE_DOWN,
                .MOUSE_DOWN = {
                    .x = event.xbutton.x,
                    .y = event.xbutton.y,
                    .button = MEUI_MOUSE_PRIMARY_BUTTON,
                },
            };
            break;
        case Button2:
            *meui_event = (struct meui_event_t){
                .type = MEUI_EVENT_MOUSE_DOWN,
                .MOUSE_DOWN = {
                    .x = event.xbutton.x,
                    .y = event.xbutton.y,
                    .button = MEUI_MOUSE_AUXILIARY_BUTTON,
                },
            };

            break;
        case Button3:
            *meui_event = (struct meui_event_t){
                .type = MEUI_EVENT_MOUSE_DOWN,
                .MOUSE_DOWN = {
                    .x = event.xbutton.x,
                    .y = event.xbutton.y,
                    .button = MEUI_MOUSE_SECONDARY_BUTTON,
                },
            };
            break;
        case Button4:
            *meui_event = (struct meui_event_t){
                .type = MEUI_EVENT_MOUSE_WHEEL,
                .MOUSE_WHEEL = {
                    .deltaX = 0,
                    .deltaY = -30,
                },
            };
            break;
        case Button5:
            *meui_event = (struct meui_event_t){
                .type = MEUI_EVENT_MOUSE_WHEEL,
                .MOUSE_WHEEL = {
                    .deltaX = 0,
                    .deltaY = 30,
                },
            };
            break;
        default:
            printf("Unhandled event -> xbutton:%d\n", event.xbutton.button);
        }
    }
    else if (event.type == ButtonRelease)
    {
        switch (event.xbutton.button)
        {
        case Button1:
            *meui_event = (struct meui_event_t){
                .type = MEUI_EVENT_MOUSE_UP,
                .MOUSE_UP = {
                    .x = event.xbutton.x,
                    .y = event.xbutton.y,
                    .button = MEUI_MOUSE_PRIMARY_BUTTON,
                },
            };
            break;
        case Button2:
            *meui_event = (struct meui_event_t){
                .type = MEUI_EVENT_MOUSE_UP,
                .MOUSE_UP = {
                    .x = event.xbutton.x,
                    .y = event.xbutton.y,
                    .button = MEUI_MOUSE_AUXILIARY_BUTTON,
                },
            };

            break;
        case Button3:
            *meui_event = (struct meui_event_t){
                .type = MEUI_EVENT_MOUSE_UP,
                .MOUSE_UP = {
                    .x = event.xbutton.x,
                    .y = event.xbutton.y,
                    .button = MEUI_MOUSE_SECONDARY_BUTTON,
                },
            };
            break;
        case Button4:
            break;
        case Button5:
            break;
        default:
            printf("Unhandled event -> xbutton:%d\n", event.xbutton.button);
        }
    }
    else if (event.type == MotionNotify)
    {
        *meui_event = (struct meui_event_t){.type = MEUI_EVENT_MOUSE_MOVE, .MOUSE_MOVE = {event.xbutton.x, event.xbutton.y}};
    }
    else
    {
        printf("Unhandled event -> type:%d\n", event.type);
    }
    return;
}