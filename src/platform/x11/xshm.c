#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#include <stdio.h>

// https://www.ssec.wisc.edu/~billh/bp/xshm.c

/*
 * Check if the X Shared Memory extension is available.
 * Return:  0 = not available
 *          1 = shared XImage support available
 *          2 = shared Pixmap support available also
 */
int check_for_xshm(Display *display)
{
    int major, minor, ignore;
    Bool pixmaps;

    if (XQueryExtension(display, "MIT-SHM", &ignore, &ignore, &ignore))
    {
        if (XShmQueryVersion(display, &major, &minor, &pixmaps) == True)
        {
            return (pixmaps == True) ? 2 : 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

/*
 * Allocate a shared memory XImage.
 */
XImage *alloc_xshm_image(Display *dpy, XVisualInfo *vinfo, XShmSegmentInfo *shminfo,
                         int width, int height, int bytes_per_line)
{
    Status XShm = XShmQueryExtension(dpy);
    if (!XShm)
    {
        printf("shared memory extension not available\n");
        return NULL;
    }

    XImage *img = XShmCreateImage(dpy, vinfo->visual, vinfo->depth,
                                  ZPixmap, 0, shminfo,
                                  width, height);
    shminfo->shmid = shmget(IPC_PRIVATE, height * img->bytes_per_line, IPC_CREAT | 0777);

    if (shminfo->shmid < 0)
    {
        printf("Faile to call shmget");
        XDestroyImage(img);
        return NULL;
    }

    shminfo->shmaddr = img->data = (char *)shmat(shminfo->shmid, 0, 0);

    if (shminfo->shmaddr == (char *)-1)
    {
        printf("shmat failed\n");
        XDestroyImage(img);
        return NULL;
    }

    shminfo->readOnly = False;

    XShmAttach(dpy, shminfo);
    XSync(dpy, False);

    return img;
}

void destroy_xshm_image(Display *dpy, XImage *img, XShmSegmentInfo *shminfo)
{
    XShmDetach(dpy, shminfo);
    XDestroyImage(img);
    shmdt(shminfo->shmaddr);
}