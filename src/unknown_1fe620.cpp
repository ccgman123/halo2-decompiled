#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"
#include "unknown_0259d0.h"
#include "unknown_20fe20.h"
#include <math.h>
// @flags /O2 /arch:SSE /Gr

bool function_1fdce0(long arg_0, short arg_1);
void function_1fe410(point3f *arg_0, real arg_1);

static __forceinline long real_to_long(real value);

// @retail 0x1fe620
void function_1fe620(long arg_0, byte const *arg_1, byte const *arg_2)
{
    bool local_0 = false;
    byte *local_1 = (byte *)actor_get(arg_0);
    if (local_1[0x70e] && !function_1fdce0(arg_0, *(short const *)(arg_2 + 0x7a)))
        local_1[0x70e] = false;
    local_1[0x70d] = local_1[0x70e];
    local_1[0x70e] = false;
    real timer_value = function_259d0(&g_4e7408->unknown0, __FILE__, __LINE__, *(real const *)(arg_1 + 0x20), *(real const *)(arg_1 + 0x24)) *
        (real)g_510c54->field_2_3;
    long timer_result;
    timer_result = real_to_long(timer_value);
    *(short *)(local_1 + 0x700) = (short)timer_result;
    *(long *)(local_1 + 0x718) = g_510c54->game_time;
    local_1[0x714] = local_1[0x5d0];
    *(real *)(local_1 + 0x7bc) = *(real const *)(arg_1 + 0x34);
    *(real *)(local_1 + 0x7c0) = 0.0f;
    if (*(real const *)(arg_1 + 0x30) > 0.0f)
        *(real *)(local_1 + 0x7c0) = *(real const *)(arg_1 + 0x30);
    if (local_1[0x70d] || local_1[0x70c])
    {
        if (*(real const *)(arg_2 + 0x84) > 0.0f)
            *(real *)(local_1 + 0x7c0) *= *(real const *)(arg_2 + 0x84);
        *(real *)(local_1 + 0x7bc) += *(real const *)(arg_2 + 0x88);
    }
    if (*(real const *)(arg_2 + 0x1c) > 0.0f && *(short *)(local_1 + 0x722) == 1)
    {
        s_prop_node_view *local_2 = prop_node_get(*(long *)(local_1 + 0x724));
        local_0 = local_2->unknown24 < 1 || local_2->unknown27 == 0;
    }
    s_type_c3b527 *local_3 = (s_type_c3b527 *)(local_1 + 0x744);
    point3f local_4;
    if (local_3->output_index == NONE ||
        !function_2104b0(local_3->output_index, &local_3->point, &local_4))
        local_4 = local_3->point;
    if (local_0)
        function_1fe410(&local_4, *(real const *)(arg_2 + 0x1c));
    vector3f local_5;
    vector3d_from_points3d((point3f *)(local_1 + 0x22c), &local_4, &local_5);
    vector3f local_6;
    local_6.i = local_5.j - local_5.k * 0.0f;
    local_6.j = local_5.k * 0.0f - local_5.i;
    local_6.k = local_5.i * 0.0f - local_5.j * 0.0f;
    real local_7 = (real)sqrt(local_6.k * local_6.k + local_6.j * local_6.j + local_6.i * local_6.i);
    if (!(fabs(local_7) < 0.0001f))
    {
        real local_8 = 1.0f / local_7;
        local_6.i *= local_8;
        local_6.j *= local_8;
        local_6.k *= local_8;
    }
    real local_9 = (real)sqrt(local_5.i * local_5.i + local_5.k * local_5.k + local_5.j * local_5.j);
    *(real *)(local_1 + 0x71c) = local_9;
    s_random_globals *local_10 = g_4e7408;
    dword local_11 = local_10->unknown0 * 1664525 + 1013904223;
    local_10->unknown0 = local_11;
    if (local_11 & 0x80000000)
    {
        local_6.i = 0.0f - local_6.i;
        local_6.j = 0.0f - local_6.j;
        local_6.k = 0.0f - local_6.k;
    }
    real local_12 = *(real const *)(arg_1 + 0xc);
    real first_limit = *(real const *)(arg_1 + 0x10);
    real second_limit = *(real const *)(arg_1 + 0x1c);
    real first_lower = 0.0f - first_limit;
    real second_lower = 0.0f - second_limit;
    real local_13 = function_259d0(&g_4e7408->unknown0, __FILE__, __LINE__, first_lower, first_limit);
    real local_14 = slot_random_range(second_lower, second_limit) + local_13;
    real local_15 = function_259d0(&g_4e7408->unknown0, __FILE__, __LINE__, *(real const *)(arg_1 + 0x14),
        *(real const *)(arg_1 + 0x18));
    real local_16 = *(real const *)(arg_1 + 0x3c);
    if (local_12 > 0.0f && local_9 > 0.0f && atan2(local_12, local_9) > local_16)
        local_12 = (real)(tan(local_16) * local_9);
    if (local_15 > 0.0f && local_9 > 0.0f && atan2(local_15, local_9) > local_16 * 2.0f)
        local_15 = (real)(tan(local_16 * 2.0f) * local_9);
    short local_17 = *(short *)(local_1 + 0x700);
    if (local_17 > 0 && *(real const *)(arg_1 + 0x38) > 0.0f)
    {
        real local_18 = g_510c54->rate * (real)local_17 * *(real const *)(arg_1 + 0x38);
        if (local_18 > 0.7853981852531433f)
            local_18 = 0.7853981852531433f;
        real local_19 = (real)(tan(local_18) * *(real *)(local_1 + 0x754));
        if (local_12 > local_19)
        {
            real local_20 = local_19 * 1.5f;
            if (local_20 > local_12)
            {
                real timer_value = local_12 / local_19 * (real)local_17;
                long timer_result;
                timer_result = real_to_long(timer_value);
                *(short *)(local_1 + 0x700) = (short)timer_result;
            }
            else
            {
                real timer_value = (real)local_17 * 1.5f;
                long timer_result;
                timer_result = real_to_long(timer_value);
                *(short *)(local_1 + 0x700) = (short)timer_result;
                local_15 *= local_20 / local_12;
                local_12 = local_20;
            }
        }
    }
    real local_21 = (real)cos(local_13);
    real local_22 = (real)sin(local_13);
    real local_23 = (real)cos(local_14);
    real local_24 = (real)sin(local_14);
    vector3f local_25;
    local_25.i = (local_6.i * local_21 + local_22 * 0.0f) * local_12;
    local_25.j = (local_6.j * local_21 + local_22 * 0.0f) * local_12;
    local_25.k = (local_6.k * local_21 + local_22) * local_12;
    vector3f local_26;
    local_26.i = 0.0f - (local_6.i * local_23 + local_24 * 0.0f) * local_15;
    local_26.j = 0.0f - (local_6.j * local_23 + local_24 * 0.0f) * local_15;
    local_26.k = -(local_6.k * local_23 + local_24) * local_15;
    if (*(short *)(local_1 + 0x700) > 0)
    {
        real local_27 = 1.0f / (real)*(short *)(local_1 + 0x700);
        local_26.i *= local_27;
        local_26.j *= local_27;
        local_26.k *= local_27;
    }
    *(s_type_c3b527 *)(local_1 + 0x768) = *(s_type_c3b527 *)(local_1 + 0x744);
    *(vector3f *)(local_1 + 0x784) = local_25;
    *(vector3f *)(local_1 + 0x790) = local_26;
    *(real *)(local_1 + 0x79c) = *(real *)(local_1 + 0x784) + local_4.x;
    *(real *)(local_1 + 0x7a0) = *(real *)(local_1 + 0x788) + local_4.y;
    *(real *)(local_1 + 0x7a4) = *(real *)(local_1 + 0x78c) + local_4.z;
}
