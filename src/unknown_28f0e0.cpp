#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "vector_products.h"
#include "globals.h"
#include "slot_handler.h"
#include "object_default_placement.h"
// @flags /O2 /Ob1 /arch:SSE /Gr

real function_30bf0(vector3f *arg_0);
void __stdcall function_b93b0(long arg_0, long arg_1, short arg_2);

struct s_28f0e0
{
    word field_0;
    byte field_2[0x3c - 2];
    vector3f field_3c;
    byte field_48[0x18];
    point3f field_60;
};
struct s_28f0e1 { byte field_0[0x134]; long field_134; byte field_138[2]; short field_13a; };
struct s_28f0e2 { byte field_0[0xa4]; bool field_a4; byte field_a5[3]; long field_a8; long field_ac; };

// @retail 0x28f0e0
bool function_28f0e0(vector3f const *arg_0, s_28f0e0 const *arg_1, long arg_2, long arg_3)
{
    s_28f0e1 *local_0 = (s_28f0e1 *)object_get(arg_2);
    vector3f local_1;
    cross3f(&arg_1->field_3c, arg_0, &local_1);
    vector3f tangent;
    vector3f const *local_2;
    vector3f const *local_3;
    if (normalize_inline(&local_1) > 0.0f)
    {
        cross3f(&local_1, &arg_1->field_3c, &tangent);
        if (function_30bf0(&tangent) > 0.0f)
        {
            local_2 = &tangent;
            local_3 = &arg_1->field_3c;
            goto local_6;
        }
    }
    local_2 = NULL;
    local_3 = NULL;
local_6:
    function_b75a0(arg_2, &arg_1->field_60, local_2, local_3, NULL, false);
    function_b93b0(arg_3, arg_2, (short)(arg_1->field_0));
    s_object_header_view *header = &((s_object_header_view *)g_4e0300->data)[arg_2 & 0xffff];
    header->flags &= ~0x20;
    if (local_0->field_134 == 0)
    {
        s_28f0e2 *local_5 = (s_28f0e2 *)((byte *)local_0 + local_0->field_13a);
        if (local_5)
        {
            local_5->field_a4 = true;
            local_5->field_ac = 0x6000086;
            local_5->field_a8 = 0x60005b6;
        }
    }
    return true;
}
