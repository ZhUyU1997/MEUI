// https://github.com/xkbcommon/libxkbcommon/blob/master/src/keysym.c

#include "xkbcommon/xkbcommon.h"
#include "xkbcommon/xkbcommon-keysyms.h"

#include "gen/ks_table.h"
#include "common/sizes.h"

#include <assert.h>
#include <string.h>

static inline const char *get_name(const struct name_keysym *entry)
{
    return keysym_names + entry->offset;
}

const char *xkb_keysym_to_dom_key(xkb_keysym_t ks)
{
    if ((ks & ((unsigned long)~0x1fffffff)) != 0)
    {
        return "Unidentified";
    }

    int32_t lo = 0, hi = ARRAY_SIZE(keysym_to_name) - 1;
    while (hi >= lo)
    {
        int32_t mid = (lo + hi) / 2;
        if (ks > keysym_to_name[mid].keysym)
        {
            lo = mid + 1;
        }
        else if (ks < keysym_to_name[mid].keysym)
        {
            hi = mid - 1;
        }
        else
        {
            return get_name(&keysym_to_name[mid]);
        }
    }

    /* Unnamed Unicode codepoint. */
    if (ks >= 0x01000100 && ks <= 0x0110ffff)
    {
        return "Unidentified";
    }

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    return "Unidentified";
}