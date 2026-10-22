#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"
#include "unknown_11cc90.h"
#include <math.h>
// @flags /O2 /arch:SSE /Gr

real function_11ce20(vector3f const *arg_0, vector3f const *arg_1);

struct s_29b2e0
{
    byte field_0[0x30];
    real field_30;
    real field_34;
    real field_38;
};

PRIVATE __forceinline real function_29b32b(vector3f *arg_0, real *length = NULL)
{
    real local_0 = (real)sqrt(arg_0->i * arg_0->i + arg_0->j * arg_0->j);
    arg_0->k = 0.0f;
    if (!(0.0001f > fabs(local_0)))
    {
        arg_0->i = (1.0f / local_0) * arg_0->i;
        real local_1 = 1.0f / local_0;
        arg_0->j *= local_1;
        arg_0->k *= local_1;
        if (length) *length = local_0;
        return local_0;
    }
    if (length) *length = 0.0f;
    return 0.0f;
}

PRIVATE __forceinline real function_29b45d(point2f *arg_0)
{
    real local_0 = (real)sqrt(arg_0->x * arg_0->x + arg_0->y * arg_0->y);
    if (!(0.0001f > fabs(local_0)))
    {
        real local_1 = 1.0f / local_0;
        arg_0->x *= local_1;
        arg_0->y *= local_1;
        return local_0;
    }
    return 0.0f;
}

PRIVATE __forceinline real function_29b4c0(real arg_0, real arg_1, real arg_2)
{
    real result = arg_0;
    if (arg_1 > arg_0)
        result = arg_1;
    else if (arg_0 > arg_2)
        result = arg_2;
    return result;
}

PRIVATE __forceinline void function_29b6c0(vector3f const *arg_0,
    vector3f const *arg_1, real arg_2, vector3f *arg_3)
{
    real local_0 = (real)sin(arg_2);
    real local_1 = (real)cos(arg_2);
    real local_2 = (arg_1->i * arg_0->i + arg_1->k * arg_0->k + arg_0->j * arg_1->j) * (1.0f - local_1);
    vector3f local_3;
    local_3.i = arg_1->k * arg_0->j - arg_1->j * arg_0->k;
    local_3.j = arg_1->i * arg_0->k - arg_1->k * arg_0->i;
    local_3.k = arg_0->i * arg_1->j - arg_1->i * arg_0->j;
    real local_4 = arg_1->i * local_2 + local_1 * arg_0->i - local_3.i * local_0;
    real local_5 = local_1 * arg_0->j + local_2 * arg_1->j - local_3.j * local_0;
    real local_6 = arg_1->k * local_2 + local_1 * arg_0->k - local_3.k * local_0;
    arg_3->i = local_4;
    arg_3->j = local_5;
    arg_3->k = local_6;
}

// @retail 0x29b2e0
void function_29b2e0(long arg_0, long arg_1, vector3f const *arg_2,
    vector3f const *arg_3, real arg_4, vector3f *arg_5)
{
    s_actor_view *local_0 = actor_get(arg_0);
    s_slot_object_view *local_1 = object_get(arg_1);
    s_29b2e0 const *local_2 = (s_29b2e0 const *)function_1e5450(arg_0, *(long *)local_1);
    vector3f local_3 = *arg_3;
    real local_4;
    function_29b32b(&local_3, &local_4);
    vector3f local_5 = *arg_2;
    point2f local_6 = { arg_2->i, arg_2->j };
    if (function_29b32b(&local_5) == 0.0f) local_5 = *arg_2;
    vector3f local_7;
    real local_8 = 0.0f;
    real local_9 = 0.0f;
    if (local_4 > 0.0f && function_29b45d(&local_6) != 0.0f)
    {
        real local_10 = function_29b4c0(local_3.i * local_6.x + local_3.j * local_6.y, -1.0f, 1.0f);
        local_8 = (real)acos(function_29b4c0(local_10, -1.0f, 1.0f));
        if (local_3.j * local_6.x - local_3.i * local_6.y < 0.0f) local_8 = -local_8;
        if (local_2->field_38 > 0.0f) local_8 *= local_2->field_38;
        real local_11 = function_11ce20(arg_3, &local_3);
        if (arg_3->k < 0.0f) local_11 = -local_11;
        real local_12 = function_11ce20(&local_5, arg_2);
        if (arg_2->k < 0.0f) local_12 = -local_12;
        local_9 = local_11 - local_12;
        real local_13 = local_2->field_30 > 0.0f ? local_2->field_30 * 0.01745329238474369f : 1.5707963705062866f;
        local_8 = function_29b4c0(local_8, -local_13, local_13);
        local_9 = function_29b4c0(local_9, -local_13, local_13);
        real local_14 = local_2->field_34 > 0.0f ? local_2->field_34 * 0.01745329238474369f : 6.2831854820251465f;
        real local_15 = *(real *)((byte *)local_0 + 0x658);
        real local_16 = local_8 - local_15;
        if (!(0.0f > local_15 * local_16)) local_14 *= arg_4;
        local_8 = local_15 + function_29b4c0(local_16, -local_14, local_14);
        local_14 = local_2->field_34 > 0.0f ? local_2->field_34 * 0.01745329238474369f : 6.2831854820251465f;
        local_15 = *(real *)((byte *)local_0 + 0x660);
        local_16 = local_9 - local_15;
        if (!(0.0f > local_15 * local_16)) local_14 *= arg_4;
        local_9 = local_15 + function_29b4c0(local_16, -local_14, local_14);
        vector3f rotation_input = *arg_2;
        function_29b6c0(&rotation_input, g_4687b0, local_8, &local_7);
        vector3f const *local_17 = (vector3f const *)((byte *)local_1 + 0x7c);
        vector3f local_18;
        local_18.i = local_17->k * local_1->forward.j - local_17->j * local_1->forward.k;
        local_18.j = local_1->forward.k * local_17->i - local_1->forward.i * local_17->k;
        local_18.k = local_1->forward.i * local_17->j - local_1->forward.j * local_17->i;
        function_29b6c0(&local_7, &local_18, local_9, &local_7);
    }
    else
        local_7 = *(vector3f *)((byte *)local_0 + 0x290);
    *(real *)((byte *)local_0 + 0x65c) = local_8;
    *(real *)((byte *)local_0 + 0x664) = local_9;
    *arg_5 = local_7;
}

// @retail 0x29b990
void function_29b990(long arg_0, long arg_1, vector3f const *arg_2,
    vector3f const *arg_3, vector3f *arg_4)
{
    s_actor_view *local_0 = actor_get(arg_0);
    vector3f local_1;
    if (*(bool *)((byte *)local_0 + 0x4d5))
        local_1 = *(vector3f *)((byte *)local_0 + 0x4d8);
    else if (*(bool *)((byte *)local_0 + 0x5d1))
        local_1 = *(vector3f *)((byte *)local_0 + 0x5f8);
    else if (arg_3)
        local_1 = *arg_3;
    else
        local_1 = *(vector3f *)((byte *)local_0 + 0x290);
    if (function_29b32b(&local_1) == 0.0f)
        local_1 = *(vector3f *)((byte *)local_0 + 0x290);
    function_29b2e0(arg_0, arg_1, arg_2, &local_1, 1.0f, arg_4);
}
