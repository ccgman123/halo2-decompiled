// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_02CBF0.CPP: adding an entry to the visibility list (built with SSE,
   apart from its neighbours in unknown_03f2b0.cpp) */

#include "cluster_mask_storage.h"
#include "material_records_storage.h"
#include "target_cache_storage.h"
#include "shader_storage.h"
#include "projection_storage.h"
#include "view_context_storage.h"
#include "unknown_11c920.h"
#include "visibility_storage.h"
#include "camera_storage.h"
#include "frame_storage.h"
#include "reset_storage.h"
#include "globals.h"
#include "index_cache_storage.h"
#include "depth_groups_storage.h"
#include "physical_memory.h"
#include <string.h>
#include <xtl.h>
#include "unknown_030290.h"




struct s_bit_vector_pool;
dword *function_1332b0(long bit_count, s_bit_vector_pool *data);
long function_016ae0(real value);
void function_30290(point3f const *point, s_view const *view, s_camera const *camera, real radius, box2f *bounds);
extern byte g_509410, g_509411;

void *function_44940(short index, bool instance);
long function_2d000(long object_tag, long tag, long index);
__declspec(noinline) void *function_449e0(short index, bool load, bool instance);

/* retail inlines 0x16ae0's body at the first two uses in 0x2cbf0 */
static __forceinline long floor_to_long_inline(real value)
{
	__asm {
		movss xmm0, value
		cvttss2si eax, xmm0
		cvtsi2ss xmm1, eax
		cmpneqss xmm1, xmm0
		cmpltss xmm0, g_45dbd8
		andps xmm0, xmm1
		movmskps ecx, xmm0
		sub eax, ecx
	}
}

// @retail 0x2cbf0
short function_2cbf0(long geometry, long category, short instance, dword flags,
    long tag, dword key, long material, byte *object, short mask, byte mode,
    void *context, long object_index, real intensity, long group, long group_type,
    bool outside, bool whole, bool first_component, real first_opacity,
    real second_opacity, real depth, byte level, point3f const *position, real radius)
{
    long index = g_4c0b78.current++;
    long result = index;
    if (index >= 850 || geometry == NONE)
    {
        --g_4c0b78.current;
        result = NONE;
    }
    else
    {
        s_44940_entry *entry = &g_4ba138[(short)index];
        entry->tag = tag;
        entry->unknown08 = key;
        entry->unknown00 = (entry->unknown00 & 0xe0000000) | (flags & 0x1fffff) | (((dword)category & 0xff) << 21);
        *(byte **)entry->unknown10 = object;
        entry->unknown10[10] = mode;
        *(void **)(entry->unknown10 + 12) = context;
        entry->flags = (entry->flags & 0xe0000000) |
            (((((instance & 0x7ff) << 9) | (geometry & 0x1ff)) << 5) | (group & 0x1f)) << 4 |
            (material & 0xf);
        entry->unknown10[11] = (entry->unknown10[11] & 0xf0) | (level & 15);
        long group_index = (entry->flags >> 4) & 31;
        if (group_index != 31)
        {
            if ((dword)g_4c0b78.entry_count <= (dword)(group_index + 1))
                g_4c0b78.entry_count = group_index + 1;
            ((s_42760_entry *)g_4c0b78.entries)[(entry->flags >> 4) & 31].type = group_type;
        }
        group_index = (entry->flags >> 4) & 31;
        if (group_index != 31 && object_index != NONE)
        {
            s_42760_entry *group_entry = &((s_42760_entry *)g_4c0b78.entries)[group_index];
            group_entry->depth = depth;
            group_entry->position = *position;
            group_entry->type = group_type;
            if (outside || g_509410)
            {
                ((box2f *)group_entry->unknown10)->x0 = -3.4028234663852886e+38f;
                ((box2f *)group_entry->unknown10)->y0 = -3.4028234663852886e+38f;
                ((box2f *)group_entry->unknown10)->x1 = 3.4028234663852886e+38f;
                ((box2f *)group_entry->unknown10)->y1 = 3.4028234663852886e+38f;
            }
            else if (whole || g_509411)
            {
                ((box2f *)group_entry->unknown10)->x0 = 3.4028234663852886e+38f;
                ((box2f *)group_entry->unknown10)->y0 = 3.4028234663852886e+38f;
                ((box2f *)group_entry->unknown10)->x1 = -3.4028234663852886e+38f;
                ((box2f *)group_entry->unknown10)->y1 = -3.4028234663852886e+38f;
            }
            else
                function_30290(position, (s_view const *)&g_4b9da0, &g_4b9e14, radius, (box2f *)group_entry->unknown10);
        }
        *(short *)(entry->unknown10 + 8) = mask;
        g_4c0b78.unknown0c[850 + index] = 0xff;
        if (object_index != NONE && g_4c0b78.count < 192)
        {
            g_4c0b78.unknown0c[850 + index] = (byte)g_4c0b78.count;
            ((long *)(g_4c0b78.unknown0c + 1700))[g_4c0b78.count] = object_index;
            ++g_4c0b78.count;
        }
        real scaled = intensity * 0.25f;
        long value;
        if (floor_to_long_inline(scaled) < 0) value = 0;
        else if (floor_to_long_inline(scaled) > 255) value = 255;
        else value = function_016ae0(scaled);
        g_4c0b78.unknown0c[index] = (byte)value;
        byte *target = *(byte **)entry->unknown10;
        if (target && object_index != NONE)
        {
            if (first_component)
            {
                long amount = (long)(first_opacity * 256.0f);
                if (amount < 0) amount = 0;
                else if (amount > 255) amount = 255;
                target[0x76] = (byte)amount;
            }
            else
            {
                long amount = (long)(second_opacity * 256.0f);
                if (amount < 0) amount = 0;
                else if (amount > 255) amount = 255;
                target[0x77] = (byte)amount;
            }
        }
        byte *section = (byte *)function_44940((short)index, (bool)((flags >> 12) & 1));
        entry->flags = (entry->flags & 0x1fffffff) | ((dword)*(word *)(section + 0x14) << 29);
        long object_tag = *(byte **)entry->unknown10 ? **(long **)entry->unknown10 : NONE;
        long kind = function_2d000(object_tag, entry->tag, (entry->flags >> 18) & 0x7ff);
        entry->unknown00 = (entry->unknown00 & 0x1fffffff) | ((dword)kind << 29);
        if (!function_449e0((short)index, entry->tag == NONE, (bool)((entry->unknown00 >> 12) & 1)))
        {
            --g_4c0b78.current;
            result = NONE;
        }
        else
        {
            s_2cb30_state *state = g_4c0b78.state;
            if (!state->active)
                ++g_4c0b78.previous;
            if (entry->unknown00 & 0x40)
            {
                section = (byte *)function_44940((short)index, (bool)((entry->unknown00 >> 12) & 1));
                *(void **)(entry->unknown10 + 4) = NULL;
                word bits = *(word *)(section + 0x24);
                if (bits > 0)
                    *(void **)(entry->unknown10 + 4) = function_1332b0(bits, (s_bit_vector_pool *)state);
                if (!*(void **)(entry->unknown10 + 4))
                    entry->unknown00 = (entry->unknown00 & ~0x40) | 1;
            }
        }
    }
    return (short)result;
}
