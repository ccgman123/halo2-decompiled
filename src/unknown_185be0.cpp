#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1cec30.h"
#include "unknown_11cb00.h"
#include "unknown_11cc90.h"
#include <math.h>
#include <string.h>
// @flags /O2 /Ob1 /arch:SSE /Gr

struct s_186ab2
{
    long field_0;
    long field_4;
    long field_8;
    byte field_c[0xc];
    short field_18;
    byte field_1a[2];
    real field_1c;
    real field_20;
};
struct s_186ab3
{
    real field_0;
    real field_4;
    real field_8;
    real field_c;
    real field_10;
    real field_14;
    dword field_18;
    byte field_1c;
    byte field_1d[3];
    word field_20;
    byte field_22[6];
    s_186ab2 field_28;
};
struct s_186ab4
{
    void *field_0;
    byte *field_4;
};
struct s_185be0
{
    byte field_0[0x10];
    word field_10[0x10];
    real field_30;
    real field_34;
    real field_38;
    real field_3c;
    real field_40;
    real field_44;
    real field_48;
    byte field_4c;
    byte field_4d;
    bool field_4e;
    byte field_4f;
};
struct s_input_state
{
    byte unknown00[0x10];
    byte values[0x38];
};
struct s_185be6
{
    long field_0;
    byte field_4[8];
    word field_c;
    word field_e;
    byte field_10[0x2e - 0x10];
    short field_2e;
    byte field_30[4];
    vector3f field_34;
    long field_40;
    byte field_44[0x68 - 0x44];
    byte field_68;
    byte field_69[0x78 - 0x69];
    real field_78;
    real field_7c;
    byte field_80[8];
    byte field_88;
    byte field_89[5];
    byte field_8e;
    byte field_8f;
    byte field_90;
    byte field_91[2];
    byte field_93;
};
struct s_small_index;
struct s_havok_component;
struct s_unknown_13bf00;
struct s_player_action;
struct s_player_action_triggers;
extern byte g_4e61b9;
extern s_input_state g_4e61dc[3];
extern s_input_state g_4e630c;
extern s_unknown_13bf00 *g_510c50;
extern byte g_51ea18[];
real magnitude3d(vector3f const *arg_0);
real function_185b30(short arg_0, real const *arg_1, real arg_2);
real function_c8880(long arg_0, short arg_1);
short function_0b67a0(s_small_index const *arg_0);
bool function_e4050(long arg_0);
bool function_100f70(long arg_0);
vector2f *function_11df30(vector2f *arg_0, vector3f const *arg_1);
void __stdcall function_1a4900(long arg_0, s_186ab2 *arg_1, vector3f *arg_2);
void player_control_update_action_flags(s_player_action *arg_0, s_player_action_triggers const *arg_1);

__forceinline real function_185be1(real arg_0, real arg_1, real arg_2)
{
    if (arg_0 < arg_1) return arg_1;
    if (arg_0 > arg_2) return arg_2;
    return arg_0;
}
__forceinline byte *function_185be2(long arg_0)
{
    return *(byte **)(g_4e0300->data + (arg_0 & 0xffff) * 12 + 8);
}
__forceinline bool function_185be3(long arg_0)
{
    return arg_0 != NONE && ((1 << g_4e0300->data[(arg_0 & 0xffff) * 12 + 3]) & 1);
}
__forceinline void function_185be4(dword *arg_0, dword arg_1, bool arg_2)
{
    if (arg_2) *arg_0 |= arg_1;
    else *arg_0 &= ~arg_1;
}
__forceinline void function_185be5(s_186ab2 *arg_0)
{
    arg_0->field_1c = 0.0f;
    arg_0->field_20 = 0.0f;
    arg_0->field_0 = NONE;
    arg_0->field_4 = NONE;
    arg_0->field_8 = NONE;
    arg_0->field_18 = 0;
}

union s_185be7
{
    real field_0[2];
    vector2f field_8;
};

#pragma inline_depth(1)
// @retail 0x185be0
void __stdcall function_185be0(long arg_0, long arg_1, real arg_2, real arg_3, s_186ab4 *arg_4, s_186ab3 *arg_5)
{
    byte *local_0 = *(byte **)((byte *)g_4e034c + 0xf4);
    s_185be6 *local_1 = (s_185be6 *)((byte *)g_4ed284 + 0x14) + arg_1;
    s_185be0 local_2;
    memset(&local_2, 0, sizeof(local_2));
    memset(arg_5, 0, sizeof(*arg_5));
    function_185be5(&arg_5->field_28);
    if (arg_0 != NONE && g_4e61cc[(short)arg_0])
    {
        arg_4->field_0 = g_4e61b9 ? &g_4e630c : &g_4e61dc[(short)arg_0];
        byte local_3[16];
        word local_4[16];
        memset(local_3, 0, sizeof(local_3));
        memset(local_4, 0, sizeof(local_4));
        s_185be7 local_5;
        local_5.field_0[0] = *(real *)(g_51ea18 + arg_0 * 0x1c) * 0.01745329238474369f;
        local_5.field_0[1] = *(real *)(g_51ea18 + arg_0 * 0x1c + 4) * 0.01745329238474369f;
        local_2 = ((s_185be0 *)(g_51ea18 + 0x70))[arg_0];
        long local_7 = local_1->field_0;
        if (local_7 != NONE)
        {
            byte *local_8 = function_185be2(local_7);
            long local_9 = *(long *)(local_8 + 0x14);
            short local_10 = *(short *)(local_8 + 0x1fc);
            if (local_9 != NONE && local_10 != NONE)
            {
                byte *local_11 = function_185be2(local_9);
                byte *local_12 = (byte *)g_4e3b44[*(long *)local_11 & 0xffff].data;
                byte *local_13 = *(byte **)(local_12 + 0x1cc) + local_10 * 0xb0;
                real local_14 = *(real *)(local_13 + 0x54);
                real local_15 = *(real *)(local_13 + 0x58) - local_14;
                real local_16 = 0.0f;
                if (local_15 > 0.0f)
                    local_16 = function_185be1((magnitude3d((vector3f *)(local_11 + 0x88)) - local_14) / local_15, 0.0f, 1.0f);
                if (*(real *)(local_13 + 0x5c) != 0.0f)
                    local_16 = (real)pow(local_16, *(real *)(local_13 + 0x5c));
                if (*(real *)(local_13 + 0x44) > 0.0f || *(real *)(local_13 + 0x48) > 0.0f)
                    local_5.field_0[0] = ((*(real *)(local_13 + 0x48) - *(real *)(local_13 + 0x44)) * local_16 + *(real *)(local_13 + 0x44)) * 0.01745329238474369f;
                if (*(real *)(local_13 + 0x4c) > 0.0f || *(real *)(local_13 + 0x50) > 0.0f)
                    local_5.field_0[1] = ((*(real *)(local_13 + 0x50) - *(real *)(local_13 + 0x4c)) * local_16 + *(real *)(local_13 + 0x4c)) * 0.01745329238474369f;
            }
        }
        arg_5->field_0 = local_2.field_38;
        arg_5->field_4 = local_2.field_3c;
        if (!(g_4ed284->flags10 & 1) && (!g_510c54->active || !g_510c54->unknown01))
        {
            real local_8 = (real)fabs(local_2.field_44);
            real local_9 = (real)fabs(local_2.field_40);
            real local_10 = 1.0f;
            if (local_8 > 0.1f && local_9 > 0.1f)
            {
                if (local_8 > local_9) { local_9 /= local_8; local_8 = 1.0f; }
                else { local_8 /= local_9; local_9 = 1.0f; }
                local_10 = (real)sqrt(local_9 * local_9 + local_8 * local_8);
            }
            real local_11 = function_185be1(local_10 * local_2.field_40, -1.0f, 1.0f);
            real local_12 = function_185be1(local_10 * local_2.field_44, -1.0f, 1.0f);
            s_185be7 local_13;
            local_13.field_0[0] = function_185b30(*(short *)(local_0 + 0x74), *(real **)(local_0 + 0x78), local_11) * local_5.field_0[0];
            local_13.field_0[1] = function_185b30(*(short *)(local_0 + 0x74), *(real **)(local_0 + 0x78), local_12) * local_5.field_0[1];
            if (fabs(local_11) >= *(real *)(local_0 + 0x48))
            {
                real local_15 = function_185be1(local_1->field_78 / *(real *)(local_0 + 0x4c), 0.0f, 1.0f);
                local_13.field_0[0] *= (*(real *)(local_0 + 0x50) - 1.0f) * local_15 + 1.0f;
                local_1->field_78 += arg_2;
            }
            else local_1->field_78 = 0.0f;
            if (fabs(local_12) >= *(real *)(local_0 + 0x48))
            {
                real local_15 = function_185be1(local_1->field_7c / *(real *)(local_0 + 0x54), 0.0f, 1.0f);
                local_13.field_0[1] *= (*(real *)(local_0 + 0x58) - 1.0f) * local_15 + 1.0f;
                local_1->field_7c += arg_2;
            }
            else local_1->field_7c = 0.0f;
            if (local_1->field_0 != NONE && local_1->field_2e != NONE)
            {
                real local_15 = 1.0f / function_c8880(local_1->field_0, local_1->field_2e);
                local_13.field_0[0] *= local_15;
                local_13.field_0[1] *= local_15;
            }
            if (local_1->field_0 != NONE)
            {
                real local_15 = 1.0f - *(real *)(function_185be2(local_1->field_0) + 0x2e4) *
                    *(real *)(*(byte **)((byte *)g_4e034c + 0x134) + 0x7c);
                local_13.field_0[0] *= local_15;
                local_13.field_0[1] *= local_15;
            }
            vector3f local_15;
            function_1a4900(arg_1, &arg_5->field_28, &local_15);
            local_1->field_68 = local_15.k > 0.0f;
            real local_16 = local_13.field_0[0];
            real local_17 = local_13.field_0[1];
            if (local_1->field_68 && arg_2 > 0.0f && (fabs(local_13.field_0[0]) > 0.0001f || fabs(local_13.field_0[1]) > 0.0001f ||
                fabs(arg_5->field_0) > 0.0001f || fabs(arg_5->field_4) > 0.0001f))
            {
                real local_18 = arg_3 / arg_2;
                real local_19 = function_185be1(*(real *)local_0, 0.0f, 1.0f) * local_15.k;
                real local_20 = function_185be1(*(real *)(local_0 + 4), 0.0f, 1.0f) * local_15.k;
                local_16 = function_185be1(local_15.i * local_18, -3.1415927410125732f, 3.1415927410125732f) * local_20 + (1.0f - local_19) * local_13.field_0[0];
                local_17 = function_185be1(local_15.j * local_18, -1.5707963705062866f, 1.5707963705062866f) * local_20 + (1.0f - local_19) * local_13.field_0[1];
            }
            arg_5->field_10 = local_16 * arg_2;
            arg_5->field_14 = local_17 * arg_2;
            *(real *)((byte *)arg_5 + 0x24) = local_2.field_48;
            if (function_185be3(local_1->field_0))
            {
                long local_18 = *(long *)(function_185be2(local_1->field_0) + 0x3e4);
                if (local_18 != NONE)
                {
                    long local_19 = *(long *)(function_185be2(local_18) + 0xb4);
                    if (local_19 != NONE)
                    {
                        s_havok_component *local_20 = (s_havok_component *)(g_51e9b8->data + (local_19 & 0xffff) * 0xa0);
                        short local_24 = function_0b67a0((s_small_index *)local_20);
                        if (local_24 != NONE)
                        {
                            transform4x3f local_21;
                            local_20->rigid_body_matrix_get(local_24, &local_21);
                            local_15 = *(vector3f *)((byte *)&local_21 + 4);
                            if (local_18 == local_1->field_40)
                            {
                                function_11df30(&local_5.field_8, &local_15);
                                function_11df30(&local_13.field_8, &local_1->field_34);
                                arg_5->field_10 += local_5.field_8.i - local_13.field_8.i;
                                arg_5->field_14 += local_5.field_8.j - local_13.field_8.j;
                            }
                        }
                    }
                    local_1->field_34 = local_15;
                }
                local_1->field_40 = local_18;
            }
        }
        else
        {
            arg_5->field_10 = 0.0f;
            arg_5->field_14 = 0.0f;
            *(real *)((byte *)arg_5 + 0x24) = 0.0f;
        }
        word local_8 = local_1->field_e & local_1->field_c;
        if (local_8)
        {
            long local_9 = 0;
            dword mask = 1;
            do
            {
                if ((local_8 & mask) && !local_2.field_0[local_9])
                {
                    local_1->field_c &= ~mask;
                    local_1->field_e &= ~mask;
                }
                mask <<= 1;
            } while (++local_9 < 16);
        }
        local_8 = local_1->field_c;
        for (long local_24 = 0; local_24 < 16; local_24++)
            if (!(local_8 & (1 << local_24))) { local_3[local_24] = local_2.field_0[local_24]; local_4[local_24] = local_2.field_10[local_24]; }
        bool local_9 = function_185be3(local_1->field_0) && function_e4050(local_1->field_0);
        if (local_3[10] == 1)
        {
            if (local_9 || arg_5->field_4 * arg_5->field_4 + arg_5->field_0 * arg_5->field_0 < 0.25f) local_1->field_88 = 1;
        }
        else if (!local_3[10]) local_1->field_88 = 0;
        function_185be4(&arg_5->field_18, 1, local_1->field_88 != 0);
        bool local_10 = local_3[11] == 1;
        if (local_10 && local_1->field_2e == NONE && arg_4->field_0)
        {
            long local_11 = NONE;
            if (local_1->field_0 != NONE)
            {
                byte *local_12 = function_185be2(local_1->field_0);
                short local_13 = *(char *)(local_12 + 0x212);
                if (local_13 != NONE) local_11 = *(long *)(local_12 + local_13 * 4 + 0x218);
            }
            if (((byte *)arg_4->field_0)[6] > 0x7f || ((byte *)arg_4->field_0)[7] > 0x7f) local_10 = false;
            if (local_11 != NONE && !function_100f70(local_11) && local_2.field_44 * local_2.field_44 + local_2.field_40 * local_2.field_40 > 0.64000004529953f) local_10 = false;
        }
        function_185be4((dword *)&arg_5->field_1c, 4, local_10);
        function_185be4((dword *)&arg_5->field_1c, 8, local_3[11] != 0);
        arg_5->field_8 = local_2.field_4e ? local_2.field_30 : local_2.field_34;
        arg_5->field_c = local_2.field_4e ? local_2.field_34 : local_2.field_30;
        long local_11 = local_3[local_2.field_4c];
        byte *local_12 = &local_3[local_2.field_4c];
        if (local_1->field_0 != NONE && function_185be2(local_1->field_0)[0x212] != 0xff && function_185be2(local_1->field_0)[0x213] != 0xff)
        {
            *local_12 = 0;
            local_1->field_90 = local_11 > 0;
        }
        else if (local_1->field_90)
        {
            if (local_11 > 0) { local_11 = 0; *local_12 = 0; }
            else local_1->field_90 = 0;
        }
        bool selected_active = local_11 != 0;
        function_185be4(&arg_5->field_18, 0x10000, local_3[7] != 0);
        function_185be4(&arg_5->field_18, 0x200000, selected_active);
        function_185be4(&arg_5->field_18, 0x20000, selected_active);
        if (g_4e6948->state != 2) function_185be4(&arg_5->field_18, 0x20000000, local_3[local_2.field_4d] != 0);
        function_185be4(&arg_5->field_18, 0x800, selected_active);
        function_185be4(&arg_5->field_18, 0x4000, selected_active);
        function_185be4(&arg_5->field_18, 0x4000000, local_3[6] != 0);
        function_185be4(&arg_5->field_18, 0x80, local_3[12] != 0);
        function_185be4(&arg_5->field_18, 0x100, local_3[13] != 0);
        bool action_active = local_3[5] != 0;
        function_185be4(&arg_5->field_18, 4, action_active);
        function_185be4(&arg_5->field_18, 0x100000, action_active);
        function_185be4(&arg_5->field_18, 0x2000000, action_active);
        local_1->field_93 = local_3[0] && ((g_510c50 && ((byte *)g_510c50)[5]) || local_1->field_93);
        function_185be4(&arg_5->field_18, 2, local_3[0] && !local_1->field_93);
        function_185be4(&arg_5->field_18, 0x10, local_3[4] != 0);
        function_185be4(&arg_5->field_18, 0x20, local_3[4] != 0);
        function_185be4(&arg_5->field_18, 0x40, local_3[7] != 0);
        function_185be4((dword *)&arg_5->field_1c, 2, local_3[1] == 1);
        function_185be4((dword *)&arg_5->field_1c, 0x10, local_3[14] != 0);
        function_185be4((dword *)&arg_5->field_1c, 0x20, local_3[15] != 0);
        if (local_3[14] == 1) ((byte *)arg_5)[0x21] |= 1;
        else ((byte *)arg_5)[0x21] &= ~1;
        s_input_state *local_14 = NULL;
        if (g_4e61cc[(short)arg_0]) local_14 = g_4e61b9 ? &g_4e630c : &g_4e61dc[(short)arg_0];
        if (local_14 && ((byte *)local_14)[0x19]) *(dword *)&arg_5->field_1c |= 0x40;
        long local_15 = (long)(*(real *)(*(byte **)((byte *)g_4e034c + 0xf4) + 0x7c) * 1000.0f);
        if (local_3[2] > 0) ((byte *)arg_5)[0x20] |= 1; else ((byte *)arg_5)[0x20] &= ~1;
        if (local_4[2] >= local_15) ((byte *)arg_5)[0x20] |= 2; else ((byte *)arg_5)[0x20] &= ~2;
        if (local_3[3] > 0) ((byte *)arg_5)[0x20] |= 0x10; else ((byte *)arg_5)[0x20] &= ~0x10;
        if (local_4[3] >= local_15) arg_5->field_20 |= 0x20; else ((byte *)arg_5)[0x20] &= ~0x20;
        if (local_3[3] > 0) local_1->field_8e = 1;
        else
        {
            if (local_1->field_8e && !local_1->field_8f) *(dword *)&arg_5->field_1c |= 1;
            local_1->field_8e = 0;
            local_1->field_8f = 0;
        }
    }
    player_control_update_action_flags((s_player_action *)arg_5, (s_player_action_triggers *)&local_2);
}
#pragma inline_depth(255)
