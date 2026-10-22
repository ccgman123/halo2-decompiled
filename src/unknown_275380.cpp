// @flags /O2 /Gr /arch:SSE
#include "squads.h"
#include "unknown_272b70.h"
#include "unknown_20fe20.h"
#include "unknown_2551c0.h"
#include "globals.h"

struct s_location;
bool function_1e13f0(long arg_0);
bool actor_update_bsp_locations(long arg_0);
void function_1e22d0(long arg_0, bool arg_1);
void function_203fb0(long arg_0);
void function_203d70(long arg_0, short arg_1, bool arg_2);
void function_203ed0(long arg_0, bool arg_1);
void function_2011f0(long arg_0);
void function_cc590(long arg_0);
void function_b9a90(long arg_0);
void function_b7360(long arg_0);
void __stdcall function_b8540(long arg_0);
bool function_a7670(long object_index);
void object_dispatch_moved(long arg_0, long arg_1, long arg_2, long arg_3);
bool __stdcall function_b7430(long arg_0, point3f const *arg_1, vector3f const *arg_2, vector3f const *arg_3,
    s_location const *arg_4, bool arg_5, bool arg_6, bool arg_7, bool arg_8);

struct s_275381
{
    byte field_0[0xa];
    bool field_a;
    byte field_b[9];
    long field_14;
    long field_18;
    byte field_1c[4];
    long field_20;
    byte field_24[0xc];
    long field_30;
    long field_34;
    byte field_38[0x238-0x38];
    point3f field_238;
    byte field_244[0x888-0x244];
};
struct s_275382
{
    byte field_0[4];
    s_type_c3b527 field_4;
    byte field_14[0x64-0x14];
};
struct s_275383
{
    byte field_0[0x48];
    long field_48;
    s_275382 *field_4c;
    byte field_50[0x74-0x50];
};
struct s_275384 { byte field_0[0x164]; s_275383 *field_164; };

/* the squared distance between two points */
__forceinline real function_275388(point3f const *arg_0, point3f const *arg_1)
{
    real local_0 = arg_0->x - arg_1->x;
    real local_1 = arg_0->y - arg_1->y;
    real local_2 = arg_0->z - arg_1->z;
    return local_2*local_2 + local_1*local_1 + local_0*local_0;
}
__forceinline void function_275384(s_type_c3b527 const *arg_0, point3f *arg_1)
{
    if (arg_0->output_index == NONE || !function_2104b0(arg_0->output_index,&arg_0->point,arg_1))
        *arg_1 = arg_0->point;
}


// score 47 (was 47): every local of the outer frame sits 0xc bytes higher
// in retail's frame ([esp+0x2c] where ours has [esp+0x20]).
// @retail 0x275380
void function_275380(long arg_0)
{
    if (ai_index_get_type(arg_0) == _ai_index_type_squad || ai_index_get_type(arg_0) == _ai_index_type_squad_group)
    {
        s_ai_squad_iterator local_0;
        ai_squad_iterator_new_inline(&local_0, arg_0);
        while (ai_squad_iterator_next(&local_0))
        {
            s_275383 *local_1 = &((s_275384 *)g_4e0350)->field_164[local_0.squad_index & 0xffff];
            if (local_1->field_48 > 0)
            {
                long local_2 = 0;
                short local_3 = 0;
                dword local_4[1] = { 0 };
                long local_5[256];
                s_squad_actor_iterator local_20;
                squad_actors_begin(&local_20, local_0.squad_index);
                s_275381 *local_8;
                while ((local_8 = (s_275381 *)squad_actors_next(&local_20)) != NULL)
                {
                    long local_7 = local_20.actor_index;
                    if (!function_1e13f0(local_7) && local_8->field_18 != NONE)
                        local_5[local_2++] = local_7;
                }
                long local_6 = g_4f55d0->unknown14;
                while (local_6 != NONE)
                {
                    long local_7 = local_6;
                    s_275381 *local_8 = (s_275381 *)actor_datum_get(local_7);
                    if (!function_1e13f0(local_7) && local_8->field_18 != NONE && local_8->field_34 == local_0.squad_index)
                        local_5[local_2++] = local_7;
                    local_6 = local_8->field_20;
                }
                for (short local_7 = 0; local_7 < local_2; local_7++)
                {
                    long local_8 = local_5[local_7];
                    s_275381 *local_9 = (s_275381 *)actor_datum_get(local_8);
                    short local_10 = 0;
                    short local_11;
                    do
                    {
                        real local_12 = 3.402823466e38f;
                        local_11 = NONE;
                        for (short local_13 = 0; local_13 < local_1->field_48; local_13++)
                        {
                            if (!(local_4[local_13 >> 5] & (1UL << (local_13 & 31))))
                            {
                                s_type_c3b527 const *local_14 = &local_1->field_4c[local_13].field_4;
                                real local_19;
                                if (local_14->output_index == NONE)
                                {
                                    local_19 = function_275388(&local_9->field_238, &local_14->point);
                                }
                                else
                                {
                                    point3f local_15;
                                    if (!function_2104b0(local_14->output_index, &local_14->point, &local_15))
                                        local_15 = local_14->point;
                                    local_19 = function_275388(&local_9->field_238, &local_15);
                                }
                                if (local_12 > local_19)
                                {
                                    local_12 = local_19;
                                    local_11 = local_13;
                                }
                            }
                        }
                        if (local_11 != NONE) break;
                        local_4[0] = 0;
                        local_10++;
                    } while (local_10 < 2);
                    if (local_11 != NONE)
                    {
                        point3f local_12;
                        function_275384(&local_1->field_4c[local_11].field_4,&local_12);
                        s_handler_object_view *local_13 = handler_object_get(local_9->field_18);
                        if (*(long *)((byte *)local_13 + 0x14) != NONE)
                        {
                            if ((1UL << local_13->type) & 3) function_cc590(local_9->field_18);
                            else function_b9a90(local_9->field_18);
                        }
                        point3f local_14 = local_12;
                        long local_15 = local_9->field_18;
                        object_dispatch_moved(local_15,(long)&local_14,0,0);
                        if (!function_b7430(local_15,&local_14,NULL,NULL,NULL,true,true,false,false))
                        {
                            if (!function_a7670(local_15))
                                function_b8540(local_15);
                        }
                        function_b7360(local_15);
                        local_4[local_11 >> 5] |= 1UL << (local_11 & 31);
                        local_3++;
                        if (actor_update_bsp_locations(local_8))
                        {
                            if (local_9->field_34 == local_0.squad_index)
                            {
                                function_203fb0(local_8);
                                function_203d70(local_8,(short)local_0.squad_index,true);
                            }
                        }
                        else if (local_9->field_30 == local_0.squad_index)
                        {
                            local_9->field_34 = local_0.squad_index;
                            function_203ed0(local_8,false);
                            if (g_4f55d0->active)
                            {
                                s_275381 *local_16 = (s_275381 *)actor_datum_get(local_8);
                                local_16->field_20 = g_4f55d0->unknown14;
                                g_4f55d0->unknown14 = local_8;
                                local_16->field_a = true;
                                local_16->field_14 = g_510c54->game_time;
                                function_1e22d0(local_8,false);
                            }
                        }
                    }
                }
                if (local_3 > 0) function_2011f0(local_0.squad_index);
            }
        }
    }
}
