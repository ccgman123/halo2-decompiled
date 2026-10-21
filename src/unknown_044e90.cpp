// @flags /O2 /Gr /arch:SSE
#include "unknown_044e90.h"
#include "normalize3d.h"

// @retail 0x44e90
void __stdcall function_44e90(short entry_index, short section_index)
{
    s_visibility_query_workspace workspace;
    s_44940_entry *entry = &g_4ba138[entry_index];
    byte *geometry;
    if (entry->tag != NONE)
    {
        byte *definition = g_4e3b44[entry->tag & 0xffff].bytes;
        byte *sections = *(byte **)(definition + 0x28);
        geometry = *(byte **)(sections + ((entry->flags >> 9) & 0x1ff) * 0x5c + 0x34);
    }
    else
    {
        byte *structure = (byte *)g_4e0348;
        byte *section;
        if (!((entry->unknown00 >> 12) & 1))
            section = *(byte **)(structure + 0xa0) + ((entry->flags >> 9) & 0x1ff) * 0xb0;
        else
        {
            byte *instances = *(byte **)(structure + 0x144);
            short index = *(short *)(instances + ((entry->flags >> 18) & 0x7ff) * 0x58 + 0x34);
            section = *(byte **)(structure + 0x13c) + index * 0xc8;
        }
        geometry = *(byte **)(section + 0x50);
    }
    byte *views = *(byte **)g_4c0b78.state->unknown00;
    byte *view_ranges = views + section_index * 0x1a + 0xa6e;
    if (*(long *)(geometry + 0x30) > 0) {
        bool parts = !g_4c0b78.state->active && (entry->unknown00 & 0x1000) &&
            *(long *)(geometry + 0x30) > 2 * *(long *)(geometry + 8);
        byte *query_data = *(byte **)(geometry + 0x2c);
        if (parts)
            query_data += (*(long *)(query_data + 0x20) + 15) & ~15;
        workspace.first.list.indices = workspace.first.indices;
        workspace.first.list.count = 0;
        workspace.first.list.capacity = (long)0x80000400;
        workspace.second.list.indices = workspace.second.indices;
        workspace.second.list.count = 0;
        workspace.second.list.capacity = (long)0x80000400;
        workspace.third.list.indices = workspace.third.indices;
        workspace.third.list.count = 0;
        workspace.third.list.capacity = (long)0x80000800;
        workspace.fourth.list.indices = workspace.fourth.indices;
        workspace.fourth.list.count = 0;
        workspace.fourth.list.capacity = (long)0x80000800;
        __declspec(align(16)) plane3f planes[6];
        s_visibility_plane_query query = { 6, planes };
        for (long view = 0; view < *(short *)views; ++view)
        {
            for (long i = 0; i < ((short *)view_ranges)[view + 1]; ++i)
            {
                long frustum_index = *(short *)(view_ranges + 0xe + view * 2) + i;
                plane3f const *source = (plane3f *)(views + 0x1974 + frustum_index * 0x108 + 0x78);
                transform4x3f const *transform = *(transform4x3f **)(entry->unknown10 + 0xc);
                long j = 0;
                do
                {
                    plane3f p = source[j];
                    if (transform)
                    {
                        if (transform->scale != 0.0f)
                        {
                            p.d -= transform->position.y * p.j + transform->position.z * p.k + transform->position.x * p.i;
                            if (transform->scale != 1.0f)
                                p.d /= transform->scale;
                        }
                        vector3f n = p.n;
                        if (transform->scale != 1.0f)
                        {
                            real inverse = 1.0f / transform->scale;
                            n.i *= inverse; n.j *= inverse; n.k *= inverse;
                        }
                        p.i = transform->forward.k * n.k + transform->forward.j * n.j + transform->forward.i * n.i;
                        p.j = transform->left.k * n.k + transform->left.j * n.j + transform->left.i * n.i;
                        p.k = transform->up.k * n.k + transform->up.j * n.j + transform->up.i * n.i;
                    }
                    p.d = -p.d;
                    planes[j] = p;

                    ++j;
                } while (j < 6);
                function_2dd930(query_data, &query, &workspace.first.list, &workspace.second.list);
            }
            if (parts)
                function_45a80(entry_index, true, (s_geometry_visibility_list *)&workspace.second.list,
                    (s_geometry_visibility_list *)&workspace.first.list);
            else
                function_458d0(entry_index, true, (s_geometry_visibility_list *)&workspace.second.list,
                    (s_geometry_visibility_list *)&workspace.first.list);
        }
        if (workspace.fourth.list.capacity >= 0)
            g_480118->allocate((long)workspace.fourth.list.indices, (workspace.fourth.list.capacity & 0x7fffffff) * 4, 0x12);
        if (workspace.third.list.capacity >= 0)
            g_480118->allocate((long)workspace.third.list.indices, (workspace.third.list.capacity & 0x7fffffff) * 4, 0x12);
        if (workspace.first.list.capacity >= 0)
            g_480118->allocate((long)workspace.first.list.indices, (workspace.first.list.capacity & 0x7fffffff) * 4, 0x12);
        if (workspace.second.list.capacity >= 0)
            g_480118->allocate((long)workspace.second.list.indices, (workspace.second.list.capacity & 0x7fffffff) * 4, 0x12);
    } else {
        byte *tag = g_4e3b44[entry->tag & 0xffff].bytes;
        long object_index = **(long **)entry->unknown10;
        byte *object = (byte *)((s_render_object_header *)g_4e0300->data)[object_index & 0xffff].object;
        long node_count = *(short *)(object + 0x114) / sizeof(transform4x3f);
        transform4x3f const *nodes = (transform4x3f *)(object + *(short *)(object + 0x116));
        for (long i = 0; i < node_count; ++i)
            function_142a60(&nodes[i], (transform4x3f *)(*(byte **)(tag + 0x4c) + i * 0x60 + 0x28), &workspace.transforms[i]);
        dword *visible = *(dword **)(entry->unknown10 + 4);
        for (long part = 0; part < *(long *)(geometry + 8); ++part)
        {
            dword bit = 1 << (part & 31);
            if (!(visible[part >> 5] & bit))
            {
                word bound_index = *(word *)(*(byte **)(geometry + 0xc) + part * 8 + 4);
                byte *bound = *(byte **)(geometry + 0x14) + bound_index * 20;
                transform4x3f const *transform = &workspace.transforms[bound[0x10]];
                point3f local = *(point3f *)bound;
                if (transform->scale != 1.0f)
                {
                    vector3f_scale((vector3f const *)&local, transform->scale, (vector3f *)&local);
                }
                point3f center;
                center.x = transform->up.i * local.z + transform->left.i * local.y + transform->forward.i * local.x + transform->position.x;
                center.y = transform->up.j * local.z + transform->left.j * local.y + transform->forward.j * local.x + transform->position.y;
                center.z = transform->up.k * local.z + transform->left.k * local.y + transform->forward.k * local.x + transform->position.z;
                bool contained = false;
                if (function_165010((s_frustum_set_view *)views, section_index, &center, *(real *)(bound + 0xc), &contained))
                {
                    entry->unknown00 |= 1;
                    visible[part >> 5] |= bit;
                }
            }
        }
    }
}
