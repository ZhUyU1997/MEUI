/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <string.h>

const char *fileext(const char *filename)
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