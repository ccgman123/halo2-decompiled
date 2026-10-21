#include "unknown_11c920.h"
#include "effects.h"
#include <math.h>

// @flags /O2 /arch:SSE /Gr

struct s_247141
{
    short field_0;
    word field_2;
    long field_4;
    long field_8;
    byte field_c[0x40];
};

struct s_247142
{
    short field_0;
    union
    {
        word field_2;
        struct
        {
            word field_20 : 1;
            word : 2;
            word field_2_3 : 1;
            word field_2_4 : 1;
            word : 11;
        };
    };
    long field_4;
    real field_8;
    real field_c;
    byte field_10[0xc];
    point3f field_1c;
    vector3f field_28;
    real field_34;
    real field_38;
    long field_3c;
};

struct s_247143
{
    byte field_0[4];
    long field_4;
    byte field_8[0xb0];
};

struct s_247144
{
    real field_0;
    real field_4;
    real field_8;
    real field_c;
    real field_10;
    real field_14;
};

class c_247140
{
public:
    virtual void function_24714a() = 0;
    virtual void function_24714b() = 0;
    virtual void function_24714c() = 0;
    virtual void function_24714d() = 0;
    virtual void function_24714e() = 0;
    virtual void function_24714f() = 0;
    virtual void function_247150() = 0;
    virtual void function_247151() = 0;
    virtual void function_247152() = 0;
    virtual void function_247153() = 0;
    virtual void function_247154() = 0;
    virtual void function_247155() = 0;
    virtual void function_247156() = 0;
    virtual bool function_247157() = 0;
};

struct s_object_246eb0;
struct s_emitter_248340;
struct s_particle_entry_group_2ba;
void function_248340(s_particle_system_datum *arg_0, s_object_246eb0 *arg_1,
    s_emitter_248340 *arg_2);
void function_2ba3a0(s_particle_entry_group_2ba const *arg_0, long arg_1,
    void *arg_2, void const *arg_3, real arg_4);

struct s_247145
{
    byte field_0 : 1;
    byte : 2;
    byte field_3 : 1;
    byte : 2;
    byte field_6 : 1;
    byte : 1;
};

/* the same flags read as a word */
struct s_247146
{
    word field_0 : 1;
    word : 2;
    word field_3 : 1;
    word : 2;
    word field_6 : 1;
    word : 9;
};

// @retail 0x247140
void function_247140(s_247143 const *arg_0, s_247141 *arg_1,
    s_particle_system_datum *arg_2, byte *arg_3,
    s_effect_particle_system_definition const *arg_4, real arg_5, s_247144 *arg_6)
{
    s_247145 const *local_20 = (s_247145 const *)((byte const *)arg_2 + 0xc);
    s_247146 const *local_21 = (s_247146 const *)((byte const *)arg_2 + 0xc);
    byte *local_0 = arg_0->field_4 != NONE ? g_4e3b44[arg_0->field_4 & 0xffff].bytes : 0;
    c_247140 *local_1 = (c_247140 *)function_137bd0(arg_4->tag_index);
    if (TEST_FIELD_BIT(local_21->field_3))
    {
        long local_2 = arg_1->field_4;
        s_247142 *local_3 = 0;
        bool local_4 = false;
        while (local_2 != NONE)
        {
            s_247142 *local_5 = &((s_247142 *)g_51ec84->data)[local_2 & 0xffff];
            long local_6 = local_5->field_4;
            local_5->field_8 += local_5->field_c * arg_5;
            if (local_5->field_8 > 1.0f)
            {
                if (TEST_FIELD_BIT(local_21->field_6) && local_20->field_0)
                {
                    local_5->field_8 -= (real)real_truncate(local_5->field_8);
                }
                else if (((byte const *)arg_4)[0x17] & 1 && *(real *)(arg_3 + 0x1c) > 0.0f)
                {
                    local_5->field_8 -= (real)real_truncate(local_5->field_8);
                }
                else
                {
                    local_5->field_2 |= 1;
                    local_4 = true;
                }
            }
            if (!(local_5->field_2 & 9) && local_5->field_8 <= 1.0f)
            {
                if ((local_5->field_2 & 0x10) &&
                    !(local_5->field_28.i * local_5->field_28.i +
                    local_5->field_28.j * local_5->field_28.j +
                    local_5->field_28.k * local_5->field_28.k > 0.0625f))
                    local_5->field_2 |= 8;
                else
                {
                    local_5->field_1c.x += local_5->field_28.i * arg_5;
                    local_5->field_1c.y += local_5->field_28.j * arg_5;
                    local_5->field_1c.z += local_5->field_28.k * arg_5;
                    local_5->field_2 &= ~0x10;
                    local_5->field_38 += local_5->field_34 * arg_5;
                }
            }
            else if ((bool)((local_5->field_2 >> 3) & 1) && local_1->function_247157())
                local_5->field_2 |= 1;
            if (!(local_5->field_2 & 1))
            {
                local_3 = local_5;
                arg_6->field_0 = arg_6->field_0 > local_5->field_1c.x ? local_5->field_1c.x : arg_6->field_0;
                arg_6->field_8 = arg_6->field_8 > local_5->field_1c.y ? local_5->field_1c.y : arg_6->field_8;
                arg_6->field_10 = arg_6->field_10 > local_5->field_1c.z ? local_5->field_1c.z : arg_6->field_10;
                arg_6->field_4 = arg_6->field_4 > local_5->field_1c.x ? arg_6->field_4 : local_5->field_1c.x;
                arg_6->field_c = arg_6->field_c > local_5->field_1c.y ? arg_6->field_c : local_5->field_1c.y;
                arg_6->field_14 = arg_6->field_14 > local_5->field_1c.z ? arg_6->field_14 : local_5->field_1c.z;
            }
            else
            {
                function_248340(arg_2, (s_object_246eb0 *)local_5, (s_emitter_248340 *)arg_1);
                record_pool_release(g_51ec84, local_2);
                if (local_3)
                    local_3->field_4 = local_6;
                else
                    arg_1->field_4 = local_6;
            }
            local_2 = local_6;
        }
        if (local_0 && arg_1->field_4 != NONE)
            function_2ba3a0((s_particle_entry_group_2ba *)local_0, arg_1->field_4, arg_2, arg_3, arg_5);
        if (((byte const *)arg_4)[0x16] & 1 && local_4)
            arg_2->flag3 = false;
    }
}


struct s_particle_emitter_datum;
void function_2483b0(s_particle_emitter_datum *arg_0);
extern dword g_4ba034;

struct s_248751
{
    byte field_0;
    byte field_1;
    word field_2;
    real field_4;
    real field_8;
};

class c_248750
{
public:
    virtual void function_248750() = 0;
    virtual void function_248751() = 0;
    virtual void function_248752() = 0;
    virtual void function_248753() = 0;
    virtual void function_248754() = 0;
    virtual void function_248755() = 0;
    virtual void function_248756() = 0;
    virtual void function_248757() = 0;
    virtual void function_248758() = 0;
    virtual void function_248759() = 0;
    virtual void function_24875a() = 0;
    virtual void function_24875b() = 0;
    virtual void function_24875c() = 0;
    virtual void function_24875d() = 0;
    virtual void function_24875e() = 0;
    virtual void function_24875f() = 0;
    virtual void function_248760() = 0;
    virtual void function_248761() = 0;
    virtual void function_248762() = 0;
    virtual real function_248763() = 0;
};

PRIVATE __forceinline real function_248765(byte const *arg_1)
{
    s_248751 const *arg_0 = *(s_248751 const *const *)(arg_1 + 4);
    real local_0;
    if (!(arg_0->field_1 >> 4))
        local_0 = arg_0->field_4;
    else
        local_0 = 0.0f;
    return local_0;
}

PRIVATE __forceinline real function_248766(byte const *arg_1)
{
    s_248751 const *arg_0 = *(s_248751 const *const *)(arg_1 + 4);
    real local_0;
    if (!(arg_0->field_1 >> 4))
        local_0 = arg_0->field_8;
    else
        local_0 = 1.0f;
    return local_0;
}

class c_248767 : public s_effect_particle_system_definition
{
public:
    void function_248750(s_particle_system_datum *arg_2, real arg_3, byte *arg_1);
};

// @retail 0x248750
void c_248767::function_248750(s_particle_system_datum *arg_2, real arg_3, byte *arg_1)
{
    s_effect_particle_system_definition const *arg_0 = this;
    real local_0 = 0.0f;
    s_247144 local_1;
    local_1.field_0 = local_1.field_4 = *(real *)(arg_1 + 0x10);
    local_1.field_8 = local_1.field_c = *(real *)(arg_1 + 0x14);
    local_1.field_10 = local_1.field_14 = *(real *)(arg_1 + 0x18);
    long local_2 = 0;
    long local_3 = *(long *)(arg_1 + 4);
    while (local_3 != NONE)
    {
        s_247141 *local_4 = &((s_247141 *)g_51ec88->data)[local_3 & 0xffff];
        if (local_2 < arg_0->unknown30)
        {
            s_247143 const *local_5 = &(*(s_247143 const **)arg_0->unknown34)[local_2];
            real local_8 = function_248765((byte const *)local_5 + 0x50);
            real local_9 = function_248766((byte const *)local_5 + 0x50);
            real local_10 = local_8 > local_9 ? local_8 : local_9;
            if (local_10 > local_0)
                local_0 = local_10;
        }
        function_247140(&(*(s_247143 const **)arg_0->unknown34)[local_2], local_4, arg_2, arg_1, arg_0, arg_3, &local_1);
        local_3 = local_4->field_8;
        ++local_2;
    }
    if (*(long *)(arg_1 + 0x30) + 1 < (long)g_4ba034 && !TEST_FIELD_BIT(arg_0->flag1))
    {
        for (long local_11 = *(long *)(arg_1 + 4); local_11 != NONE; )
        {
            s_247141 *local_12 = &((s_247141 *)g_51ec88->data)[local_11 & 0xffff];
            function_2483b0((s_particle_emitter_datum *)local_12);
            local_11 = local_12->field_8;
        }
    }
    *(real *)(arg_1 + 0x20) = (local_1.field_0 + local_1.field_4) * 0.5f;
    *(real *)(arg_1 + 0x24) = (local_1.field_8 + local_1.field_c) * 0.5f;
    *(real *)(arg_1 + 0x28) = (local_1.field_10 + local_1.field_14) * 0.5f;
    vector3f local_13;
    local_13.i = local_1.field_4 - *(real *)(arg_1 + 0x20);
    local_13.j = local_1.field_c - *(real *)(arg_1 + 0x24);
    local_13.k = local_1.field_14 - *(real *)(arg_1 + 0x28);
    c_248750 *local_14 = (c_248750 *)function_137bd0(arg_0->tag_index);
    *(real *)(arg_1 + 0x2c) = local_14->function_248763() * local_0 +
        (real)sqrt(local_13.i * local_13.i + local_13.j * local_13.j + local_13.k * local_13.k);
}
