#include "unknown_440dd8.h"
#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0820f0.h"
// @flags /O2 /Gr
#include "counters_storage.h"

struct s_structure_bsp_reference;
struct s_bsp3d;
struct s_slot_entry_list;
extern s_bsp3d *g_4e033c;
extern s_slot_entry_list *g_4e0340;
extern s_connection_counter g_4e6398;

void function_d4e60(short arg_0);
void function_137da0();
void function_137d40();
bool cache_files_load_structure_bsp(s_structure_bsp_reference *arg_0);
void cache_files_unload_structure_bsp(s_structure_bsp_reference *arg_0);
void function_16e2c0(long arg_0);

struct s_11c1b0
{
    void (__stdcall *field_0)(short);
    dword field_4[8];
};

#define g_440df0 (*( s_11c1b0 const (*)[68])((byte const *)g_440dd8 + 0x18))

// @retail 0x11c1b0 standard
bool __stdcall function_11c1b0(short arg_0, bool arg_1)
{
    short local_0 = g_4e6948->index;
    bool local_1 = false;
    short local_7;
    if (arg_0 != NONE && (arg_0 < 0 || arg_0 >= *(long *)((byte *)g_4e0350 + 0x210)))
        goto finish;
    if (g_4686c4 == arg_0 && local_0 == arg_0)
        return true;
    for (dword local_2 = 0; local_2 < 68; local_2++)
    {
        if (g_440df0[local_2].field_0)
            g_440df0[local_2].field_0(arg_0);
    }
    local_7 = local_0;
    if (local_0 != NONE && local_0 != arg_0)
    {
        function_137da0();
        local_7 = g_4e6948->index;
    }
    if (g_4686c4 != NONE && g_4686c4 != arg_0)
    {
        if (arg_1)
        {
            byte *local_3 = *(byte **)((byte *)g_4e0350 + 0x214) + g_4686c4 * 0x44;
            cache_files_unload_structure_bsp((s_structure_bsp_reference *)local_3);
        }
        g_4686c4 = NONE;
        g_4e0348 = NULL;
        g_4e0344 = NULL;
        g_4e0340 = NULL;
        g_4e033c = NULL;
    }
    local_1 = true;
    if (g_4686c4 != arg_0)
    {
        byte *local_4 = *(byte **)((byte *)g_4e0350 + 0x214) + arg_0 * 0x44;
        g_4e0348 = NULL;
        g_4e0344 = NULL;
        g_4e0340 = NULL;
        g_4e033c = NULL;
        g_4686c4 = arg_0;
        if (cache_files_load_structure_bsp((s_structure_bsp_reference *)local_4))
        {
            function_16e2c0(*(long *)(local_4 + 0x14));
            long local_6 = *(long *)(local_4 + 0x1c);
            if (local_6 != NONE)
                g_4e0344 = (s_structure_bsp_globals *)g_4e3b44[local_6 & 0xffff].bytes;
            else
                g_4e0344 = NULL;
            g_4e0348 = (s_match_globals *)g_4e3b44[*(long *)(local_4 + 0x14) & 0xffff].bytes;
            g_4e0340 = *(s_slot_entry_list **)((byte *)g_4e0348 + 0x18);
            g_4e033c = (s_bsp3d *)g_4e0340;
        }
        else
        {
            g_4686c4 = NONE;
            local_1 = false;
            goto local_5;
        }
    }
    if (local_7 != arg_0)
        function_137d40();
local_5:
    g_4e6398 = g_485ab0;
finish:
    return local_1;
}
