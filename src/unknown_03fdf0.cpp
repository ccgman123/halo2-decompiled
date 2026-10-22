#include "render_source_storage.h"
// @flags /O2 /arch:SSE /Gr
#include "material_records_storage.h"
/* UNKNOWN_03FDF0.CPP: cached light geometry and material pass dispatch */

#include "unknown_11c920.h"
#include "globals.h"
#include "index_cache_storage.h"
#include "unknown_0494b0.h"
#include <string.h>
#include <xtl.h>

struct s_record_source;
struct s_cache_record;
struct s_cache_record_state
{
    long count;
    dword unknown04;
    s_cache_record *records;
    void *buffer;
    byte unknown10;
    bool available;
    byte unknown12[2];
};
struct s_render_part_context
{
    long material;
    point3f position;
    real radius;
    byte active;
    byte unknown15[3];
    real values[4];
    real priority;
};
struct s_light_cone_ab { real values[5]; };
struct s_light_shape_ab
{
    long kind;
    union { struct { real radius_a, radius_b; } sphere; s_light_cone_ab cone; };
    bool ready;
    byte unknown19[3];
    vector3f left;
    real origin_offset;
    point3f origin;
    union
    {
        struct { real unused, radius_a, radius_b, radius; } sphere_render;
        struct
        {
            real unused;
            vector3f direction;
            real distance;
            real slope_x, slope_y, aspect;
            real width, height;
            real near_distance, far_distance;
            real far_width, far_height;
            point3f endpoint;
        } cone_render;
    };
};

/* The first cached record block precedes the existing metadata array.
   Later blocks occupy the preceding metadata entry's values storage. */

extern s_cache_record_state g_4b6280;
extern s_record_pool *g_4e030c;
extern dword g_4b843c;
extern long g_50943c, g_509440;
extern long g_467130, g_4858b4;
extern void *g_467134;
extern s_record_sources *g_467138;
extern byte g_485a75, g_485a76, g_4858bc;
extern long g_485898, g_4858b8;
extern real g_485adc, g_485ae0;
extern vector3f g_485950;
extern byte g_4ba022;
byte g_4ba018;

__declspec(noinline) void *function_449e0(short index, bool load, bool instance);
bool function_45ce0(short index, byte *out, short part, short transform_index);
bool function_460d0(dword const *mask, short index, long part_index);
void function_41040(short index, long part_index, long group, byte weight,
    s_render_part_context const *context, long material_override, bool force,
    dword and_mask, dword or_mask);
void function_14bc0(short target, short element, bool depth);
void function_15680(short mode);
void function_15370(short mode);
void function_44550(void);
void function_445d0(void);
void function_0496f0(void);
void __stdcall function_14b60(dword flags, dword color, real depth, byte stencil);
bool __stdcall function_44370(long mode);
void __stdcall function_174220(bool enabled);
bool function_c17f0(long light_index, s_light_shape_ab *shape, bool respect_engine);
void function_4a1e0(s_view_source *source, s_view_camera *camera, long resource_index,
    s_view_flags *flags, real unused_scale, bool enabled, real scale);

// score 54 (was 54): retail multiplies the index into edx and clears the globals before the
// loop, ours computes the index in eax and stores the table pointer later.
// @retail 0x402b0
void function_402b0(long cache_index, long mode)
{
    g_4c1a48[1].count = 0;
    g_4c1a48[1].first = 0;
    g_4c1a48[1].flags = 0;
    g_4c1a48[2].count = 0;
    g_4c1a48[2].first = 0;
    g_4c1a48[2].flags = 0;
    g_4c1a80 = 0;
    s_render_index_cache_block const *block = &g_4c62f8.records.blocks[cache_index + 1];
    if (block->count > 0)
    {
        long first = g_4c0b78.previous;
        g_4c0b78.current = first;
        s_44940_entry const *entries = block->entries;
        memcpy(g_4ba138 + first, entries, block->count * sizeof(s_44940_entry));
        g_4c0b78.current += block->count;
        dword selection = !mode ? ~0UL : mode == 2 ? 0x10 : 0x20;
        long n = 0;
        long index = first;
        while (n < block->count)
        {
            s_44940_entry const *entry = entries;
            if (entry->unknown00 & selection & 0x1fffff &&
                entry->unknown00 & 1 && entry->unknown10[0xa] > 0 && (entry->flags & 0x1f0) == 0x1f0)
            {
                byte weight = g_4c0b78.weights[index];
                long shape = entry->flags & 0xf;
                byte material_index = g_4c0b78.material_indices[(short)index];
                long material = NONE;
                if (material_index != 0xff)
                    material = g_4c0b78.materials[material_index];
                byte *geometry = (byte *)function_449e0((short)index, false, (bool)((entry->unknown00 >> 12) & 1));
                if (geometry)
                {
                    for (long part = 0; part < *(long *)geometry; ++part)
                    {
                        s_render_part_context context;
                        if (function_45ce0((short)index, (byte *)&context, (short)part, (short)shape) &&
                            function_460d0(*(dword const **)(entry->unknown10 + 4), (short)index, part))
                        {
                            long chosen = context.material;
                            if (material != NONE)
                                chosen = material;
                            if (chosen != NONE)
                                function_41040((short)index, part, 1, weight, &context, chosen, true, ~0UL, 0);
                        }
                    }
                }
            }
            ++n;
            ++index;
            ++entries;
        }
        g_4c1a80 = g_4c1a48[2].flags;
    }
}

// @retail 0x404b0
bool function_404b0(long mode)
{
    for (long cache_index = 0; cache_index < g_509440; ++cache_index)
    {
        if (g_4c6b00[cache_index].count <= 0) continue;
        long light_index = g_4c6b00[cache_index].index;
        byte *light = g_4e030c->data + (light_index & 0xffff) * 0x110;
        byte *definition = g_4e3b44[*(long *)(light + 4) & 0xffff].bytes;
        s_light_shape_ab shape;
        function_c17f0(light_index, &shape, true);
        if (*(short *)(light + 0x54) != NONE)
        {
            long parent = *(long *)(light + 0x4c);
            while (parent != NONE)
                parent = *(long *)(*(byte **)(g_4e0300->data + (parent & 0xffff) * 12 + 8) + 0x14);
        }
        function_402b0(cache_index, mode);
        function_4a1e0((s_view_source *)&shape, (s_view_camera *)(light + 0x84), light_index,
            (s_view_flags *)definition, 1024.0f, false, 1.0f);
        g_4ba018 = true;
        dword flags = (short)shape.kind == 0 ? 1 : 0;
        if (g_485950.i > 0.0f || g_485950.j > 0.0f || g_485950.k > 0.0f) flags |= 2;
        else flags &= ~2UL;
        g_4ba014 = flags & ~0x3cUL;
        bool selected = (bool)((g_4c1a48[1].flags >> 11) & 1);
        if (selected)
        {
            g_467130 = 11; g_467134 = (void *)1; g_467138 = &g_4c1a48[1];
            g_4858b4 = g_4858b8;
            function_14bc0((short)g_4858b8, 0, true);
            function_15680(0);
            function_0496f0();
        }
        else g_467130 = 0;
        function_15370(0);
        if (selected) { function_44550(); function_445d0(); }
        selected = (bool)((g_4c1a48[1].flags >> 14) & 1);
        if (selected)
        {
            g_467130 = 14; g_467134 = (void *)1; g_467138 = &g_4c1a48[1];
            g_4858b4 = 1;
            function_14bc0(1, 0, true); function_15680(0);
        }
        else g_467130 = 0;
        function_15370(0);
        if (selected) { function_44550(); function_445d0(); }
        selected = (bool)((g_4c1a48[1].flags >> 12) & 1);
        if (selected)
        {
            g_467130 = 12; g_467134 = (void *)1; g_467138 = &g_4c1a48[1];
            g_4858b4 = g_4858b8;
            function_14bc0((short)g_4858b8, 0, true); function_15680(0);
        }
        else g_467130 = 0;
        function_15370(0);
        if (selected) { function_44550(); function_445d0(); }
        if ((g_4ba014 & 1) && (bool)((g_4ba014 >> 1) & 1))
        {
            selected = (bool)((g_4c1a48[1].flags >> 13) & 1);
            if (selected)
            {
                g_467130 = 13; g_467134 = (void *)1; g_467138 = &g_4c1a48[1];
                g_4858b4 = g_4858b8;
                function_14bc0((short)g_4858b8, 0, true); function_15680(0);
            }
            else g_467130 = 0;
            function_15370(0);
            if (selected) { function_44550(); function_445d0(); }
        }
        g_4858bc = false; g_4b843c = 0;
        D3DDevice_SetRenderState(D3DRS_STENCILENABLE, 0);
        D3DDevice_SetScissors(0, FALSE, 0);
        D3DDevice_SetDepthClipPlanes(g_485adc, g_485ae0, 2);
        function_14bc0((short)g_4858b8, 0, true);
        g_4ba018 = false; g_4ba014 = 0;
    }
    return true;
}

// @retail 0x3fdf0
bool function_3fdf0(long mode, bool single_pass)
{
    (void)&single_pass;
    g_4b6280.unknown04 = g_4b6280.count;
    dword selection = !mode ? ~0UL : mode == 2 ? 0x10 : 0x20;
    g_4c1a48[0].unknown10 = g_4c1a48[0].flags;
    g_4c1a48[0].flags = 0;
    g_4c1a48[0].first = g_4c1a48[0].count;
    function_174220(true);
    for (long n = 0; n < g_50943c; ++n)
    {
        short index_bits = ((short const *)g_4c6b00[7].values)[n];
        if (index_bits == NONE) continue;
        long index = (short)index_bits;
        s_44940_entry const *entry = &g_4ba138[index];
        if (!((entry->unknown00 & selection) & 0x1fffff) || !(entry->unknown00 & 1) ||
            !entry->unknown10[0xa] || (entry->flags & 0x1f0) != 0x1f0) continue;
        long transform_index = entry->flags & 0xf;
        byte weight = g_4c0b78.weights[index];
        byte material_index = g_4c0b78.material_indices[index];
        long material = NONE;
        if (material_index != 0xff)
            material = g_4c0b78.materials[material_index];
        byte *geometry = (byte *)function_449e0((short)index_bits, false, (bool)((entry->unknown00 >> 12) & 1));
        if (!geometry) continue;
        for (long part = 0; part < *(long *)geometry; ++part)
        {
            s_render_part_context context;
            if (function_45ce0((short)index_bits, (byte *)&context, (short)part, (short)transform_index) &&
                function_460d0(*(dword const **)(entry->unknown10 + 4), index, part))
            {
                long chosen = context.material;
                if (material != NONE) chosen = material;
                if (chosen != NONE) function_41040(index, part, 0, weight, &context, chosen, false, ~0UL, 0);
            }
        }
    }
    bool selected;
    if (single_pass)
    {
        selected = (bool)((g_4c1a48[0].flags >> 16) & 1);
        if (selected)
        {
            g_467130 = 16; g_467134 = 0; g_467138 = &g_4c1a48[0];
            g_4858b4 = g_4858b8;
            function_14bc0((short)g_4858b8, 0, true); function_15680(0);
        }
        else g_467130 = 0;
        function_15370(0);
        if (selected) { function_44550(); function_445d0(); }
    }
    else
    {
        selected = (bool)((g_4c1a48[0].flags >> 1) & 1);
        if (selected)
        {
            g_467130 = 1; g_467134 = 0; g_467138 = &g_4c1a48[0]; g_4858b4 = 1;
            function_14bc0(1, 0, true);
            if (!g_485a75 && !g_485a76) function_14b60(1, 0, 0.0f, 0);
            function_15680(g_485898 >= 4 && g_485898 <= 7 ? 0 : 1);
        }
        else g_467130 = 0;
        function_15370(1);
        if (selected) { function_44550(); function_445d0(); }
        selected = (bool)((g_4c1a48[0].flags >> 3) & 1);
        if (selected)
        {
            g_467130 = 3; g_467134 = 0; g_467138 = &g_4c1a48[0]; g_4858b4 = g_4858b8;
            function_14bc0((short)g_4858b8, 0, true);
            function_15680(g_485898 >= 4 && g_485898 <= 7 ? 0 : 1);
        }
        else g_467130 = 0;
        function_15370(1);
        if (selected) { function_44550(); function_445d0(); }
        selected = (bool)((g_4c1a48[0].flags >> 8) & 1) && g_4ba022;
        if (selected)
        {
            g_467130 = 8; g_467134 = 0; g_467138 = &g_4c1a48[0]; g_4858b4 = g_4858b8;
            function_14bc0((short)g_4858b8, 0, true); function_15680(0);
        }
        else g_467130 = 0;
        function_15370(0);
        if (selected) { function_44550(); function_445d0(); }
        selected = (bool)((g_4c1a48[0].flags >> 20) & 1);
        if (selected)
        {
            g_467130 = 20; g_467134 = 0; g_467138 = &g_4c1a48[0]; g_4858b4 = g_4858b8;
            function_14bc0((short)g_4858b8, 0, true); function_15680(0);
        }
        else g_467130 = 0;
        function_15370(0);
        if (selected) { function_44550(); function_445d0(); }
        selected = (bool)((g_4c1a48[0].flags >> 7) & 1);
        if (selected)
        {
            g_467130 = 7; g_467134 = 0; g_467138 = &g_4c1a48[0]; g_4858b4 = g_4858b8;
            function_14bc0((short)g_4858b8, 0, true); function_15680(0);
        }
        else g_467130 = 0;
        function_15370(0);
        if (selected) { function_44550(); function_445d0(); }
        if (function_44370(2)) { function_44550(); function_445d0(); }
    }
    g_4c1a48[0].flags = g_4c1a48[0].unknown10;
    g_4c1a48[0].first = 0;
    return true;
}
