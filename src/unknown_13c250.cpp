// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"

struct s_effect_owner;
void function_b7930(void *arg_1, long arg_2, long arg_3, s_effect_owner const *arg_4);
long __stdcall function_b7b40(void *arg_1);
void __stdcall function_b8ee0(long arg_1, long arg_2, long arg_3, long arg_4);
void function_b7680(long arg_1, real arg_2, real arg_3);
long function_cbe60(long arg_1);

struct s_13c250
{
    byte field_0[3];
    byte field_3;
    byte field_4[4];
    byte *field_8;
};

struct s_13c251
{
    byte field_0[0x10a];
    word field_10a0 : 1;
    word field_10a1 : 1;
    word field_10a2 : 1;
    word field_10a3 : 13;
};

__forceinline byte *function_13c251(s_record_pool *arg_1, long arg_2)
{
    return ((s_13c250 *)arg_1->data)[arg_2 & 0xffff].field_8;
}

static inline long local_player_first_index(void)
{
    long result = NONE;

    for (long i = 0; i < 4; i++)
    {
        if (g_4e8c20->entries[i] != NONE)
        {
            result = i;
            break;
        }
    }
    return result;
}

static inline long local_player_next(long index)
{
    long result = NONE;
    long start = index == NONE ? 0 : index + 1;

    for (long i = start; i < 4; i++)
    {
        if (g_4e8c20->entries[i] != NONE)
        {
            result = i;
            break;
        }
    }
    return result;
}

static inline bool local_player_valid(long local_player_index)
{
    bool result = false;
    if (local_player_index != NONE)
        result = g_4e8c20->entries[local_player_index] != NONE;
    return result;
}

struct s_player_13c250
{
    byte unknown000[0x2c];
    long unit_index;
    byte unknown030[0x21c - 0x30];
};

__forceinline bool function_13c253(s_13c251 const *arg_1)
{
    return TEST_FIELD_BIT(arg_1->field_10a2);
}

// @retail 0x13c250
void __stdcall function_13c250(long arg_1, long arg_2, long arg_3)
{
    volatile long local_2;
    long local_3[4];
    byte local_4[0xc4];
    if (arg_1 != NONE)
    {
        local_2 = 0;
        long local_6 = local_player_first_index();
        for (; local_6 != NONE; local_6 = local_player_next(local_6))
        {
            if (local_player_valid(local_6))
            {
                long local_8 = ((s_player_13c250 *)g_4e8c24->data)[g_4e8c20->entries[local_6] & 0xffff].unit_index;
                if (local_8 != NONE && !function_13c253((s_13c251 *)function_13c251(g_4e0300, local_8)))
                {
                    byte *local_9 = function_13c251(g_4e0300, local_8);
                    short local_10 = (signed char)local_9[0x212];
                    long local_11 = NONE;
                    if (local_10 != NONE)
                        local_11 = *(long *)(local_9 + local_10 * 4 + 0x218);
                    local_9 = function_13c251(g_4e0300, local_8);
                    short local_12 = (signed char)local_9[0x213];
                    long local_13 = NONE;
                    if (local_12 != NONE)
                        local_13 = *(long *)(local_9 + local_12 * 4 + 0x218);
                    long local_14 = function_cbe60(local_8);
                    if (local_11 != NONE)
                    {
                        long local_31 = local_2++;
                        local_3[local_31] = *(long *)function_13c251(g_4e0300, local_11);
                    }
                    if (local_13 != NONE)
                    {
                        long local_31 = local_2++;
                        local_3[local_31] = *(long *)function_13c251(g_4e0300, local_13);
                    }
                    if (local_14 != NONE)
                    {
                        long local_31 = local_2++;
                        local_3[local_31] = *(long *)function_13c251(g_4e0300, local_14);
                    }
                    break;
                }
            }
        }
        if (local_2 == 0)
        {
            byte *local_15 = (byte *)g_4e0350;
            if (*(long *)(local_15 + 0xf8) > 0)
            {
                byte *local_16 = *(byte **)(local_15 + 0xfc);
                long local_17 = *(long *)(local_16 + 0x2c);
                if (local_17 != NONE)
                {
                    long local_31 = local_2++;
                    local_3[local_31] = local_17;
                }
                local_17 = *(long *)(local_16 + 0x38);
                if (local_17 != NONE)
                {
                    long local_31 = local_2++;
                    local_3[local_31] = local_17;
                }
            }
            if (local_2 == 0)
            {
                s_13c250 *local_18 = &((s_13c250 *)g_4e0300->data)[arg_1 & 0xffff];
                if (local_18->field_3 == 0)
                {
                    byte *local_19 = (byte *)g_4e3b44[*(long *)local_18->field_8 & 0xffff].bytes;
                    long local_20 = *(long *)(local_19 + 0x1c0);
                    if (local_20 > 0)
                    {
                        byte *local_21 = *(byte **)(local_19 + 0x1c4) + 4;
                        do
                        {
                            long local_22 = *(long *)local_21;
                            if (local_22 != NONE)
                            {
                                long local_31 = local_2++;
                                local_3[local_31] = local_22;
                            }
                            local_21 += 8;
                            --local_20;
                        }
                        while (local_20);
                    }
                }
            }
        }
        long local_23 = NONE;
        long local_24 = 0x7fffffff;
        long local_25 = 0;
        while (local_25 < local_2)
        {
            long local_26 = local_3[local_25];
            long local_27 = *(long *)((byte *)g_4e3b44[local_26 & 0xffff].bytes + 0x288);
            long local_28;
            switch (local_27)
            {
            case 0x5000720:
                local_23 = local_26;
                goto local_29;
            case 0x500000a:
            case 0x5000722:
                local_28 = 2;
                break;
            case 0x6000721:
                local_28 = 1;
                break;
            case 0x700071e:
            case 0x700071f:
                local_28 = 3;
                break;
            default:
                local_28 = 4;
                break;
            }
            if (local_23 == NONE || local_28 < local_24)
            {
                local_24 = local_28;
                local_23 = local_26;
            }
            ++local_25;
        }
local_29:
        if (local_23 != NONE)
        {
            function_b7930(local_4, local_23, arg_1, 0);
            long local_30 = function_b7b40(local_4);
            if (local_30 != NONE)
            {
                function_b8ee0(arg_1, arg_2, local_30, arg_3);
                function_b7680(local_30, 1.0f, 0.0f);
            }
        }
    }
}
