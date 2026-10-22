// @flags /O1 /Oi /Oy- /arch:SSE /Gr
#include "view_context_storage.h"
#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "unknown_13927e.h"
#include "globals.h"
#include <string.h>

struct s_510c4c;
extern s_510c4c *g_510c4c;
extern point3f *g_468720;
extern real g_4e69c0[4];

struct s_4e6950
{
    s_color_bits field_0;
    point3f field_c;
    real value18;
    real value1c;
    real value20;
    real value24;
    byte unknown28[0x70 - 0x28];
    real user_values[4];
};
extern s_4e6950 g_4e6950;

struct s_interface_pulse
{
    char delay;
    char ticks;
    char repeats;
    byte unknown03;
};

struct s_float_rect
{
    real x0;
    real x1;
    real y0;
    real y1;
};

struct s_widget_quad_2b11;
void function_1396c7(long arg_0, point2f *arg_1);
real function_200891(s_interface_pulse const *arg_0, bool arg_1);
void function_22a664(s_widget_quad_2b11 const *arg_0, s_float_rect const *arg_1,
    long arg_2, long arg_3, long arg_4);

// @retail 0x200462
void function_200462(long arg_0)
{
    s_interface_pulse *local_0 = (s_interface_pulse *)((byte *)g_510c4c + arg_0 * 0x6c + 0x2c);
    byte *local_1 = *(byte **)((byte *)g_510c94 + 0x404);
    real local_2 = 0.0f;
    dword local_3 = 0;
    long entry_index;
    for (entry_index = 0; entry_index < 9; entry_index++)
    {
        byte *local_5 = local_1 + entry_index * 0x1c;
        s_interface_pulse *local_6 = &local_0[entry_index];
        if (*(long *)(local_5 + 4) == NONE || *(long *)(local_5 + 0xc) == NONE ||
            (!local_6->delay && !local_6->ticks))
            continue;
        byte *local_7 = g_4e3b44[*(long *)(local_5 + 4) & 0xffff].bytes;
        short local_8 = *(short *)(local_5 + 0x10);
        long sprite_index = local_8;
        real local_9 = 1.0f;
        if (local_8 < 0)
            continue;
        if (local_8 < *(long *)(local_7 + 0x3c))
        {
            byte *local_10 = *(byte **)(local_7 + 0x40) + local_8 * 0x3c;
            sprite_index = *(short *)(local_10 + 0x20);
            if (*(long *)(local_10 + 0x34) > 0)
            {
                byte *local_11 = *(byte **)(local_10 + 0x38);
                local_9 = *(real *)(local_11 + 0xc) - *(real *)(local_11 + 8);
            }
        }
        else if (sprite_index || *(long *)(local_7 + 0x3c) || *(long *)(local_7 + 0x44) <= 0)
            continue;
        else
            sprite_index = 0;
        if (sprite_index != NONE)
        {
            local_3 |= 1 << entry_index;
            real local_12 = function_200891(local_6, true);
            byte *local_13 = *(byte **)(local_7 + 0x48) + sprite_index * 0x74;
            local_2 += local_12 * ((real)*(short *)(local_13 + 4) * local_9);
        }
    }
    point2f local_14;
    function_1396c7(4, &local_14);
    local_14.x -= local_2 * 0.5f;
    local_14.y += 50.0f;
    for (entry_index = 0; entry_index < 9; entry_index++)
    {
        byte *local_16 = *(byte **)((byte *)g_510c94 + 0x404) + entry_index * 0x1c;
        if (!(local_3 & (1 << entry_index)))
            continue;
        byte *local_17 = g_4e3b44[*(long *)(local_16 + 4) & 0xffff].bytes;
        long local_18 = *(short *)(local_16 + 0x10);
        s_float_rect local_19 = {0.0f, 1.0f, 0.0f, 1.0f};
        if (local_18 < 0)
            continue;
        if (local_18 < *(long *)(local_17 + 0x3c))
        {
            byte *local_20 = *(byte **)(local_17 + 0x40) + local_18 * 0x3c;
            local_18 = *(short *)(local_20 + 0x20);
            if (*(long *)(local_20 + 0x34) > 0)
                local_19 = *(s_float_rect *)(*(byte **)(local_20 + 0x38) + 8);
        }
        else if (local_18 || *(long *)(local_17 + 0x3c) || *(long *)(local_17 + 0x44) <= 0)
            continue;
        else
            local_18 = 0;
        if (local_18 == NONE)
            continue;
        byte *local_21 = *(byte **)(local_17 + 0x48) + local_18 * 0x74;
        s_interface_pulse *local_22 = &local_0[entry_index];
        real local_23 = function_200891(local_22, true);
        real local_24 = (local_19.x1 - local_19.x0) * (real)*(short *)(local_21 + 4) * 0.5f;
        real local_25 = (local_19.y1 - local_19.y0) * (real)*(short *)(local_21 + 6) * 0.5f;
        real local_26 = local_24 * local_23;
        local_14.x += local_26;
        point2f local_27[4];
        local_27[0].x = local_14.x - local_24;
        local_27[0].y = local_14.y - local_25;
        local_27[1].x = local_14.x + local_24;
        local_27[1].y = local_14.y - local_25;
        local_27[2].x = local_14.x + local_24;
        local_27[2].y = local_14.y + local_25;
        local_27[3].x = local_14.x - local_24;
        local_27[3].y = local_14.y + local_25;
        g_4e6950.field_0 = *function_13927e(g_4b9ed8);
        g_4e6950.field_c = *g_468720;
        g_4e6950.value18 = 0.0f;
        g_4e6950.value20 = (real)local_22->delay * g_510c54->rate * 2.0f;
        g_4e6950.value1c = function_200891(local_22, false) * g_4e69c0[g_4b9ed8];
        function_22a664((s_widget_quad_2b11 const *)local_27, &local_19,
            *(long *)(local_16 + 4), local_18, *(long *)(local_16 + 0xc));
        local_14.x += local_26;
    }
}
