/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

int check_for_xshm(Display *display);
XImage *alloc_xshm_image(Display *dpy, XVisualInfo *vinfo, XShmSegmentInfo *shminfo, int width, int height);
void destroy_xshm_image(Display *dpy, XImage *img, XShmSegmentInfo *shminfo);