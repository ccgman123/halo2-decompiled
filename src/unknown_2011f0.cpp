// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include "squads.h"
#include <string.h>

void __stdcall function_201100(long arg_0, dword *arg_1);
void __stdcall function_202e90(long arg_0, short arg_1, long arg_2);
bool function_204390(long arg_0);
void function_203360(long arg_0);
void function_201ea0(void);
void function_268840(void);

// @retail 0x2011f0
void function_2011f0(long arg_0)
{
    byte *local_2 = g_51e9d8->data + (arg_0 & 0xffff) * 0x98;
    byte *local_1 = *(byte **)((byte *)g_4e0350 + 0x164) + (arg_0 & 0xffff) * 0x74;
    if (!(*(word *)(local_2 + 2) & 4))
    {
        byte *local_4 = NULL;
        dword local_3[4];
        memset(local_3, 0, sizeof(local_3));
        *(word *)(local_2 + 2) |= 4;
        word local_5 = *(word *)(local_1 + 0x26);
        if (local_5 != (word)NONE)
        {
            local_4 = g_51e9dc->data + local_5 * 0x38;
            if (!local_4[0x23])
                function_201100((short)local_5, local_3);
        }
        short local_6 = *(short *)(local_1 + 0x42);
        if (local_6 >= 0 && local_6 < *(long *)((byte *)g_4e0350 + 0x240))
            function_202e90(arg_0, local_6, true);
        else if (*(short *)(local_2 + 0x2a) == NONE && local_4 && local_4[0x23])
        {
            short local_7 = *(short *)(local_4 + 0x14);
            if (local_7 != NONE)
                function_202e90(arg_0, local_7, false);
        }
    }
    if (g_4e0348 && function_204390(arg_0))
        *(short *)(g_51e9d8->data + (arg_0 & 0xffff) * 0x98 + 0x7e) = g_4686c4;
    else
        *(short *)(g_51e9d8->data + (arg_0 & 0xffff) * 0x98 + 0x7e) = NONE;
    function_203360(arg_0);
    function_201ea0();
    function_268840();
}
