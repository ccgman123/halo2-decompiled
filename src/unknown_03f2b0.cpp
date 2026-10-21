// @flags /O2 /Gr
#include "cluster_mask_storage.h"
#include "material_records_storage.h"
#include "target_cache_storage.h"
#include "shader_storage.h"
#include "projection_storage.h"
#include "view_context_storage.h"
/* UNKNOWN_03F2B0.CPP: visibility lists, resource prediction and index caches */

#include "unknown_11c920.h"
#include "unknown_044e90.h"
#include "visibility_storage.h"
#include "camera_storage.h"
#include "frame_storage.h"
#include "reset_storage.h"
#include "globals.h"
#include "index_cache_storage.h"
#include "depth_groups_storage.h"
#include "physical_memory.h"
#include <string.h>
#include <xtl.h>
#include "unknown_030290.h"

void *g_509438;

__declspec(noinline) void function_43990(void);

// @retail 0x43990
void function_43990(void)
{
	physical_memory_new_frame((s_physical_object *)g_509448);
}

extern D3DResource *g_509444;

// @retail 0x439d0
void *function_439d0(long index)
{
    s_physical_object *manager = (s_physical_object *)g_509448;
    s_physical_block *block = &((s_physical_block *)((s_physical_object *)g_509448)->blocks->data)[index & 0xffff];
    D3DResource *resource = g_509444;
    long offset = block->offset << manager->page_shift;
    block->time = manager->time;
    D3DDevice_FlushVertexCache();
    return (void *)((resource->Data | 0x80000000) + offset);
}

void function_1cdd0(real const *bounds, byte flags);
bool function_13d9f0(long type);

// @retail 0x4d7c0
void function_4d7c0(dword const *source, word const *type)
{
    (void)&type;
    long index = source[1];
    if (index != NONE)
    {
        byte *tag = g_4e3b44[index & 0xffff].bytes;
        if (*(long *)(tag + 0x14) > 0)
        {
            byte *entries = *(byte **)(tag + 0x28);
            long flags = *(word *)(entries + ((source[3] >> 9) & 0x1ff) * 0x5c + 0x1a);
            real const *bounds = *(real **)(tag + 0x18);
            if (function_13d9f0(*type))
                flags |= 1;
            function_1cdd0(bounds, (byte)flags);
            return;
        }
    }
    function_1cdd0(0, 0);
}

struct s_unknown_13bf00;
extern s_unknown_13bf00 *g_510c50;
extern byte g_509415;

__declspec(noinline)
// @retail 0x335c0
byte function_335c0(void)
{
	if (g_510c50 && ((byte *)g_510c50)[5])
		return g_509415;
	return true;
}

// @retail 0x46220
c_allocator *function_46220(void)
{
	return g_480118;
}


// @retail 0x3d3e0
short function_3d3e0(long object_index, bool cached)
{
	if (cached)
		return g_4c1bd4;
	long tag = ((s_render_object_header *)g_4e0300->data)[object_index & 0xffff].object[0];
	long next = *(long *)(g_4e3b44[tag & 0xffff].bytes + 0x38);
	return *(long *)(g_4e3b44[next & 0xffff].bytes + 4) != NONE;
}

struct s_word_bit_iterator
{
	short count;
	short index;
	dword remaining;
	dword const *words;
};

PRIVATE __forceinline long first_set_bit(dword mask)
{
	long result;
	__asm
	{
		mov ecx, -1
		bsf ecx, mask
		mov result, ecx
	}
	return result;
}

// @retail 0x44690
bool function_44690(s_word_bit_iterator *iterator, long *out)
{
	/* Retail keeps the output pointer on the stack. */
	(void)&out;
	while (!iterator->remaining)
	{
		if (++iterator->index >= iterator->count)
			break;
		iterator->remaining = iterator->words[iterator->index];
	}
	if (iterator->index < iterator->count)
	{
		long bit = first_set_bit(iterator->remaining);
		*out = iterator->index * 32 + bit;
		iterator->remaining &= ~(1 << bit);
		return true;
	}
	return false;
}

// @retail 0x3f2b0
void function_03f2b0(void)
{
	g_509438 = 0;
}

struct s_visible_index
{
	short index;
	byte unknown02[0x18];
};

struct s_visible_index_list
{
	byte unknown00[0xa6c];
	short count;
	s_visible_index entries[1];
};

class c_entry_list
{
public:
	c_entry_list(long maximum_count);
	bool add(long a, short b, long c, short d);
	short find(long a);
	void swap(short index0, short index1);

private:
	long maximum_count;
	word count;
	short *shorts_b;
	long *longs_a;
	long *longs_c;
	short *shorts_d;
};




struct s_parent_render_object
{
	byte unknown00[0x14];
	long parent;
	byte unknown18[0x92];
	byte attached;
};

// @retail 0x31c20
void function_31c20(s_bit_vector_pool *data, bool alternate, long object_index)
{
	s_bit_vector_pool *const *reference = &data;
	long flags = alternate ? 0x3000 : 0x1000;
	if (object_index != NONE)
	{
		bool again;
		do
		{
			again = false;
			s_parent_render_object *object = (s_parent_render_object *)((s_render_object_header *)g_4e0300->data)[object_index & 0xffff].object;
			if (object->attached && object->parent != NONE)
			{
				object_index = object->parent;
				again = true;
			}
		} while (again);
		(*reference)->lists[2]->add(object_index, flags, 0, NONE);
	}
}

// @retail 0x3f3f0
void function_3f3f0(void)
{
	s_visible_index_list *list = (s_visible_index_list *)g_547f88.context;
	for (long i = 0; i < list->count; ++i)
	{
		long index = list->entries[i].index;
		g_4c56c0[index >> 5] |= 1 << (index & 31);
	}
}


long g_50943c;
long g_509440;

// @retail 0x3fd70
void function_3fd70(void)
{
	long j = 0;
	g_50943c = j;
	for (long i = 0; i < 16; ++i)
		g_4c6b00[7].values[i] = NONE;
	for (; j < 8; ++j)
	{
		g_4c6b00[j].index = NONE;
		g_4c6b00[j].count = 0;
	}
	g_509440 = 0;
}

// @retail 0x40de0
long function_40de0(long index, bool first, bool second)
{
	/* Retail receives both boolean arguments on the stack. */
	bool const *first_reference = &first;
	bool const *second_reference = &second;
	long result = NONE;
	if (!index)
		result = 0;
	else if (*first_reference)
		result = 1;
	else if (*second_reference)
		result = 2;
	return result;
}

// @retail 0x40e10
long function_40e10(bool first, bool second)
{
	long result = 3;
	if (first)
		result = second ? 0 : 1;
	else if (second)
		result = 2;
	return result;
}

struct s_predicted_resource;
struct s_predicted_resource_block
{
	long count;
	s_predicted_resource *resources;
};

struct s_cluster_resources
{
	byte unknown00[0x84];
	s_predicted_resource_block resources;
	long link_count;
	short *links;
	byte unknown94[0x1c];
};

struct s_cluster_link
{
	short a;
	short b;
	byte unknown04[0x20];
};

struct s_cluster_resource_map
{
	byte unknown00[0x58];
	dword *visibility;
	byte unknown5c[4];
	s_cluster_link *links;
	byte unknown64[0x38];
	long count;
	s_cluster_resources *clusters;
};

bool function_16e5e0(s_predicted_resource_block const *block, short mode);

// @retail 0x3f450
void function_3f450(long cluster_index)
{
	if (cluster_index != NONE)
	{
		s_cluster_resource_map *map = (s_cluster_resource_map *)g_4e0348;
		dword const *visible = map->visibility + ((map->count + 31) >> 5) * (short)cluster_index;
		function_16e5e0(&map->clusters[cluster_index].resources, 0);
		for (long i = 0; i < map->count; ++i)
		{
			if (visible[i >> 5] & (1 << (i & 31)))
				function_16e5e0(&map->clusters[i].resources, 0);
		}
	}
}

// @retail 0x3f500
void function_3f500(long cluster_index)
{
	if (cluster_index != NONE)
	{
		s_cluster_resource_map *map = (s_cluster_resource_map *)g_4e0348;
		dword const *visible = map->visibility + ((map->count + 31) >> 5) * (short)cluster_index;
		function_16e5e0(&map->clusters[cluster_index].resources, 1);
		for (long i = 0; i < map->count; ++i)
		{
			if (g_4c56c0[i >> 5] & (1 << (i & 31)))
			{
				s_cluster_resources *cluster = &map->clusters[i];
				for (long j = 0; j < cluster->link_count; ++j)
				{
					s_cluster_link *link = &map->links[cluster->links[j]];
					short adjacent = link->a == i ? link->b : link->a;
					if ((visible[adjacent >> 5] & (1 << (adjacent & 31))) &&
						!(g_4c56c0[adjacent >> 5] & (1 << (adjacent & 31))))
					{
						cluster = &map->clusters[adjacent];
						function_16e5e0(&cluster->resources, 1);
					}
				}
			}
		}
	}
}

#include "geometry_cache.h"

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);

// @retail 0x2d000
long function_2d000(long object_index, long tag, long instance)
{
	long result = 3;
	byte *map;
    byte *definition;
    if (g_4e0344 && g_4e0344->count > 0 &&
        (map = (byte *)g_4e0348) != NULL &&
        (definition = (byte *)g_4e0344->bsp,
		*(long *)(definition + 0x1c) != NONE &&
		*(long *)(definition + 4) == *(long *)(map + 8)))
	{
		if (tag != NONE)
		{
			if (instance == 0x7ff)
				result = 2;
			else
			{
				byte *data = g_4e3b44[tag & 0xffff].bytes;
				byte *tags = *(byte **)(definition + 0x5c);
				if (*(long *)(data + 8) == *(long *)(tags + instance * 12 + 8))
				{
					byte *mappings = *(byte **)(definition + 0x64);
					long section = *(word *)(mappings + instance * 12 + 2);
					byte *sections = *(byte **)(definition + 0x44);
					byte *section_record = sections + section * 0x38;
					if (function_12de70((s_geometry_block_info *)(section_record + 0xc), 3))
						result = 1;
				}
			}
		}
		else if (instance != 0x7ff)
		{
			byte *mappings = *(byte **)(definition + 0x54);
			long section = *(word *)(mappings + instance * 12 + 2);
			byte *sections = *(byte **)(definition + 0x44);
			byte *section_record = sections + section * 0x38;
			if (function_12de70((s_geometry_block_info *)(section_record + 0xc), 3))
			{
				byte *instances = *(byte **)((byte *)g_4e0348 + 0x144);
				result = *(short *)(instances + instance * 0x58 + 0x56) != 0;
			}
		}
        else
            result = 0;
	}
	else if (tag != NONE && object_index != NONE)
	{
		byte *object = (byte *)function_badc0(object_index, NONE);
		if (object && (object[7] & 1))
			result = 2;
	}
	return result;
}

struct s_geometry_section
{
    byte unknown00[0x38];
    s_geometry_block_info block;
};

struct s_geometry_sections
{
    byte unknown00[0x1c];
    long count;
    byte unknown20[8];
    s_geometry_section *sections;
};

// @retail 0x3e320
bool function_3e320(long tag, byte const *indices)
{
    (void)&indices;
    s_geometry_sections *data = (s_geometry_sections *)g_4e3b44[tag & 0xffff].bytes;
    bool result = true;
    for (long i = 0; i < data->count; ++i)
    {
        long index = indices[i];
        if (index != 255)
        {
            byte *section = (byte *)data->sections + index * sizeof(s_geometry_section);
            if (function_12de70((s_geometry_block_info *)(section + 0x38), 3) == false)
                result = false;
        }
    }
    return result;
}

struct s_decal_globals;
extern s_decal_globals *g_4ea94c;
extern long g_4ba038;
bool g_4c8780;
dword g_4c8784;
long g_4c8788, g_4c878c;
long g_4c8790;
void function_144f0(long tag, short stage, long fallback, short fallback_index, short index, real priority);
void function_1cf50(void);

#pragma inline_depth(0)
// @retail 0x44040
void __stdcall function_44040(long cell, short mask)
{
    long handle = ((long *)g_4ea94c)[((short)g_4c8790 << 9) + (short)cell];
    if (handle != NONE)
    {
        do
        {
            byte *entry = (byte *)g_4ea950->data + (handle & 0xffff) * 0x40;
            byte *group = (byte *)g_4ea950->data + (*(long *)(entry + 0x38) & 0xffff) * 0x40;
            if (*(short *)(group + 8) != (short)g_4ba038)
            {
                byte *definition = g_4e3b44[*(long *)(entry + 0x2c) & 0xffff].bytes + 0x6c;
                *(short *)(group + 8) = (short)g_4ba038;
                long tag = *(long *)(definition + 0x20);
                if (g_4c878c != tag || g_4c8788 != *(signed char *)(entry + 0x29))
                {
                    long bitmap = *(signed char *)(entry + 0x29);
                    g_4c8788 = bitmap;
                    g_4c878c = tag;
                    function_144f0(tag, 0, 1, (short)bitmap, 0, 0.0f);
                    function_1cf50();
                }
                dword color = *(dword *)(entry + 0x24);
                if (g_4c8784 != color || g_4c8780)
                {
                    g_4c8784 = color;
                    g_4c8780 = false;
                    D3DDevice_SetVertexData4ub(10, (byte)(color >> 16), (byte)(color >> 8), (byte)color, (byte)(color >> 24));
                    #pragma inline_depth(1)
                    D3DDevice_SetVertexDataColor(11, 0);
                    #pragma inline_depth(0)
                }
                long block_index = *(long *)(entry + 4) & 0xffff;
                long vertex_count = *(short *)(entry + 0x2a) * 4;
                s_physical_object *manager = (s_physical_object *)g_509448;
                s_physical_block *block = (s_physical_block *)manager->blocks->data + block_index;
                dword first_vertex = ((dword)block->offset << manager->page_shift) >> 4;
                D3DDevice_DrawVertices(D3DPT_QUADLIST, first_vertex, vertex_count);
            }
            handle = *(long *)(entry + 0x34);
        }
        while (handle != NONE);
    }
}
#pragma inline_depth()

extern byte *g_4858c4;
long g_485a2c, g_485a64, g_485a68;

// @retail 0x34060
long function_34060(long mode, dword flags)
{
    (void)&flags;
    switch (mode)
    {
    case 0: return g_485a2c;
    case 1: return g_485a2c;
    case 2: return *(short *)(g_4858c4 + 0x8a);
    case 3: return *(short *)(g_4858c4 + 0x80);
    case 4: return g_485a64;
    case 5: return g_485a68;
    case 6:
        if (!(*g_4858c4 & 2) && (bool)((g_4ba014 >> 1) & 1) && (flags & 0x2000))
            return 1;
        return 0;
    case 7: return !((*(dword *)g_4858c4 >> 1) & 1);
    default: __assume(0);
    }
}

struct s_bsp3d;
long function_14a280(s_bsp3d *bsp, long index, point3f *point);

struct s_leaf_cluster
{
    short cluster;
    byte unknown02[6];
};

struct s_leaf_cluster_map
{
    byte unknown00[0x18];
    s_bsp3d *bsp;
    byte unknown1c[0x10];
    long leaf_count;
    s_leaf_cluster *leaves;
    byte unknown34[0x68];
    long cluster_count;
};

// @retail 0x2b720
bool function_2b720(point3f *point, long *cluster, long *leaf)
{
    long *const *cluster_reference = &cluster;
    s_leaf_cluster_map *map = (s_leaf_cluster_map *)g_4e0348;
    bool result = false;
    long index = function_14a280(map->bsp, 0, point);
    if (index != NONE)
    {
        *leaf = index;
        **cluster_reference = map->leaves[index].cluster;
        result = true;
    }
    else if (*leaf < 0 || *leaf >= map->leaf_count || **cluster_reference < 0 || **cluster_reference >= map->cluster_count)
    {
        *leaf = NONE;
        **cluster_reference = NONE;
    }
    return result;
}

long function_baf80(long object_index);
long function_155760(long index);

struct s_inactive_object_header
{
    byte unknown00[3];
    byte inactive;
    byte unknown04[4];
    byte *object;
};

// @retail 0x3e9c0
bool function_3e9c0(long object_index)
{
    bool result = false;
    if (object_index != NONE)
    {
        long index = g_4b9ed8;
        if (index != NONE && !function_155760(index))
        {
            long parent = function_baf80(object_index);
            s_inactive_object_header *header = &((s_inactive_object_header *)g_4e0300->data)[parent & 0xffff];
            if (!header->inactive)
            {
                byte *object = header->object;
                s_index_table *table = g_4e8c20;
                s_index_table *const *table_reference = &table;
                long entry = (*table_reference)->entries[index];
                if (*(long *)(object + 0x13c) == entry)
                    result = true;
            }
        }
    }
    return result;
}

extern s_record_pool *g_4e030c;

struct s_render_entry_110
{
    long unknown00;
    long tag;
    byte unknown08[0x4c - 8];
    long object;
    long unknown50;
    short index;
    byte unknown56[0x110 - 0x56];
};

// @retail 0x31520
bool function_31520(long index)
{
    bool result = false;
    if (index != NONE)
    {
        s_render_entry_110 *entry = &((s_render_entry_110 *)g_4e030c->data)[index & 0xffff];
        byte *tag = g_4e3b44[entry->tag & 0xffff].bytes;
        bool active = false;
        if (entry->index != NONE)
            active = function_3e9c0(entry->object);
        dword flags = *(dword *)tag;
        if ((!(flags & 0x20) || active) && (!(flags & 0x40) || !active))
            result = true;
    }
    return result;
}

// @retail 0x4c0d0
void function_4c0d0(long object_index, byte *output, long mode)
{
    (void)&mode;
    byte *object = (byte *)((s_render_object_header *)g_4e0300->data)[object_index & 0xffff].object;
    byte *definition = g_4e3b44[*(long *)object & 0xffff].bytes;
    byte *model = g_4e3b44[*(long *)(definition + 0x38) & 0xffff].bytes;
    if ((signed char)object[0xb1] != -1 && mode == 1)
    {
        for (long i = 0; i < *(long *)(model + 0x70); ++i)
        {
            byte *variant = *(byte **)(model + 0x54) + (signed char)object[0xb1] * 0x38;
            signed char index = (signed char)variant[i + 4];
            if (index != -1)
                ((short *)(output + 0xc2))[i] = *(short *)(*(byte **)(variant + 0x18) + index * 20 + 0x10);
            else
                ((short *)(output + 0xc2))[i] = 0;
        }
    }
    else
        memset(output + 0xc2, 0, 0xa0);
}

#include "unknown_123b30.h"

extern double g_4ba040;
void function_3d270(void);
extern dword g_4ba034;
long g_4ba038, g_4ba03c;
long g_4ba054[4], g_4ba064[4];
struct s_fade_record;
extern s_fade_record *g_50942c;
void function_3b8a0(void);
void function_0167a0(void);

// @retail 0x2b4a0
void function_2b4a0(void)
{
    g_4ba034 = 0;
    g_4ba040 = 0.0;
    g_4ba038 = 0;
    g_4ba03c = 0;
    memset(g_4ba054, 0xff, sizeof(g_4ba054));
    memset(g_4ba064, 0xff, sizeof(g_4ba064));
    function_3b8a0();
    function_3d270();
    function_0167a0();
    g_50942c = (s_fade_record *)function_123d40(0, 0, 0x18);
}

extern real g_45dd38, g_45dd44;

// @retail 0x3f220
point3f *function_3f220(dword a, dword b, dword c, point3f *out)
{
    dword const *reference = &c;
    point3f value;
    real x, y, z;
    x = (real)a * 8.0f;
    value.x = x * g_45dd38;
    y = (real)b * 8.0f;
    value.y = y * g_45dd38;
    z = (real)*reference * 8.0f;
    value.z = z * g_45dd44;
    *out = value;
    return out;
}





// @retail 0x2cb30
void function_2cb30(void)
{
	if (!g_4c0b78.state->active)
	{
		s_2cb30_entry empty = {};
		g_4c0b78.previous = 0;
		g_4c0b78.current = 0;
		g_4c0b78.count = 0;
		g_4c0b78.entries[0] = empty;
		// Retail's forward copy propagates the cleared first entry through the array.
		memcpy(g_4c0b78.entries + 1, g_4c0b78.entries, sizeof(g_4c0b78.entries) - sizeof(g_4c0b78.entries[0]));
		g_4c0b78.entry_count = 0;
	}
	else
		g_4c0b78.current = g_4c0b78.previous;
}



// @retail 0x44940
void *function_44940(short index, bool instance)
{
	(void)&instance;
	s_44940_entry *entry = &g_4ba138[index];
	if (entry->tag != NONE)
	{
		byte *definition = g_4e3b44[entry->tag & 0xffff].bytes;
		byte *sections = *(byte **)(definition + 0x28);
		return sections + ((entry->flags >> 9) & 0x1ff) * 0x5c + 4;
	}
	byte *structure = (byte *)g_4e0348;
	if (!instance)
		return *(byte **)(structure + 0xa0) + ((entry->flags >> 9) & 0x1ff) * 0xb0;
	byte *instances = *(byte **)(structure + 0x144);
	short definition_index = *(short *)(instances + ((entry->flags >> 18) & 0x7ff) * 0x58 + 0x34);
	return *(byte **)(structure + 0x13c) + definition_index * 0xc8;
}

__declspec(noinline) void *function_449e0(short index, bool load, bool instance);

// @retail 0x449e0
void *function_449e0(short index, bool load, bool instance)
{
	bool const *instance_reference = &instance;
	s_44940_entry *entry = &g_4ba138[index];
	void *result = NULL;
	if (entry->tag != NONE)
	{
		byte *definition = g_4e3b44[entry->tag & 0xffff].bytes;
		byte *section = *(byte **)(definition + 0x28) + ((entry->flags >> 9) & 0x1ff) * 0x5c;
		if (!load || function_12de70((s_geometry_block_info *)(section + 0x38), 3))
			result = *(void **)(section + 0x34);
	}
	else
	{
		byte *structure = (byte *)g_4e0348;
		if (!*instance_reference)
		{
			byte *section = *(byte **)(structure + 0xa0) + ((entry->flags >> 9) & 0x1ff) * 0xb0;
			if (!load || function_12de70((s_geometry_block_info *)(section + 0x28), 3))
				result = *(void **)(section + 0x50);
		}
		else
		{
			byte *instances = *(byte **)(structure + 0x144);
			short definition_index = *(short *)(instances + ((entry->flags >> 18) & 0x7ff) * 0x58 + 0x34);
			byte *section = *(byte **)(structure + 0x13c) + definition_index * 0xc8;
			if (!load || function_12de70((s_geometry_block_info *)(section + 0x28), 3))
				result = *(void **)(section + 0x50);
		}
	}
	return result;
}

PRIVATE __forceinline byte *visible_entry_geometry(short index)
{
	s_44940_entry *entry = &g_4ba138[index];
	if (entry->tag != NONE)
	{
		byte *definition = g_4e3b44[entry->tag & 0xffff].bytes;
		byte *sections = *(byte **)(definition + 0x28);
		return *(byte **)(sections + ((entry->flags >> 9) & 0x1ff) * 0x5c + 0x34);
	}
	byte *structure = (byte *)g_4e0348;
	byte *data;
	if (!((byte)(entry->unknown00 >> 12) & 1))
	{
		byte *section = *(byte **)(structure + 0xa0) + ((entry->flags >> 9) & 0x1ff) * 0xb0;
		data = *(byte **)(section + 0x50);
	}
	else
	{
		byte *instances = *(byte **)(structure + 0x144);
		short definition_index = *(short *)(instances + ((entry->flags >> 18) & 0x7ff) * 0x58 + 0x34);
		data = (byte *)*(dword const *)(*(byte **)(structure + 0x13c) + definition_index * 0xc8 + 0x50);
	}
	return data;
}

struct s_render_part_context;
bool function_45ce0(short index, byte *out, short part, short transform_index);
void function_41040(short index, long part_index, long group, byte weight,
    s_render_part_context const *context, long material_override, bool force,
    dword and_mask, dword or_mask);

// @retail 0x458d0
void function_458d0(short index, bool ranges, s_geometry_visibility_list const *first, s_geometry_visibility_list const *second)
{
	short const *index_reference = &index;
	byte *geometry = visible_entry_geometry(*index_reference);
	s_44940_entry *entry = &g_4ba138[*index_reference];
	dword *mask = *(dword **)(entry->unknown10 + 4);
	if (first->count > 0 || second->count > 0)
		entry->unknown00 |= 1;
	for (long i = 0; i < first->count; ++i)
	{
		if (!ranges)
		{
			dword bit = (word)first->indices[i];
			mask[bit >> 5] |= 1 << (bit & 31);
		}
		else
		{
			word end = (*(word **)(geometry + 0x34))[(short)first->indices[i + 1] + *(long *)(geometry + 8)];
			for (long j = (*(word **)(geometry + 0x34))[(short)first->indices[i] + *(long *)(geometry + 8)]; j <= end; ++j)
			{
				dword bit = (*(word **)(geometry + 0x34))[(short)j];
				mask[bit >> 5] |= 1 << (bit & 31);
			}
			++i;
		}
	}
	for (long i = 0; i < second->count; ++i)
	{
		dword bit = (word)second->indices[i];
		mask[bit >> 5] |= 1 << (bit & 31);
	}
}

PRIVATE __forceinline void mark_visible_part(byte *geometry, long index, dword *mask)
{
	byte *part = *(byte **)(geometry + 4) + index * 0x48;
	for (long bit = *(short *)(part + 0xa); bit < *(short *)(part + 0xa) + *(short *)(part + 0xc); ++bit)
		mask[bit >> 5] |= 1 << (bit & 31);
}

// @retail 0x45a80
void function_45a80(short index, bool ranges, s_geometry_visibility_list const *first, s_geometry_visibility_list const *second)
{
	short const *index_reference = &index;
	byte *geometry = visible_entry_geometry(*index_reference);
	s_44940_entry *entry = &g_4ba138[*index_reference];
	dword *mask = *(dword **)(entry->unknown10 + 4);
	if (first->count > 0 || second->count > 0)
		entry->unknown00 |= 1;
	for (long i = 0; i < first->count; ++i)
	{
		if (!ranges)
			mark_visible_part(geometry, (word)first->indices[i], mask);
		else
		{
			word end = (*(word **)(geometry + 0x34))[(short)first->indices[i + 1] + *(long *)geometry + 2 * *(long *)(geometry + 8)];
			for (long j = (*(word **)(geometry + 0x34))[(short)first->indices[i] + *(long *)geometry + 2 * *(long *)(geometry + 8)]; j <= end; ++j)
			{
				long part = (*(word **)(geometry + 0x34))[(short)j + 2 * *(long *)(geometry + 8)];
				mark_visible_part(geometry, part, mask);
			}
			++i;
		}
	}
	for (long i = 0; i < second->count; ++i)
		mark_visible_part(geometry, second->indices[i], mask);
}




void function_3d600(void);

struct s_planar_camera_source
{
	point3f origin;
	vector3f normal;
	vector3f horizontal;
	byte unknown24[0x40 - 0x24];
	real offset;
	real depth;
};
struct s_planar_camera;
struct s_timed_effect_globals;
void function_441b0(s_planar_camera_source const *source, s_planar_camera *state, byte const *view);
s_timed_effect_globals *function_01fd20(s_timed_effect_globals *result);
s_bit_vector_pool *function_1320f0(long mode, long pass, bool enabled, long extra,
	byte const *camera, byte const *plane, real distance, void *filter, real offset, long identifier);
void function_133390(s_bit_vector_pool const *data);
void __stdcall function_133520(s_bit_vector_pool *data);
void function_133720(s_bit_vector_pool *data, bool enabled);
void function_44720(void);
void function_2cb30(void);

// @retail 0x2cbc0
void __stdcall function_2cbc0(bool enabled)
{
	s_bit_vector_pool *current = (s_bit_vector_pool *)g_4c0b78.state;
	function_2cb30();
	function_133390(current);
	function_133520(current);
	function_133720(current, enabled);
}

// @retail 0x3f2c0
void function_3f2c0(s_planar_camera_source const *source, byte const *view, long object_index, long identifier)
{
	byte camera[0x1bc]; // a whole s_planar_camera: function_441b0 fills 0x1bc bytes
	real offset = 0.0f;
	function_441b0(source, (s_planar_camera *)camera, view);
	byte *effect = (byte *)function_01fd20(0);
	bool enabled = effect && effect[0x3c];
	if (enabled)
	{
		offset = *(real *)(effect + 0x40) - *(real *)(camera + 0x70);
	}
	function_3d600();
	s_bit_vector_pool *data = function_1320f0(0, 1, enabled, 0, camera,
		camera + 0x60, 1024.0f, NULL, offset, g_4b9ee4);
	if (object_index != NONE)
	{
		struct s_entry_list_view
		{
			long maximum;
			word count;
			short *shorts_b;
			long *longs_a;
			long *longs_c;
			short *shorts_d;
		};
		s_entry_list_view *list = (s_entry_list_view *)data->lists[2];
		bool found = false;
		for (long i = 0; i < (short)list->count && !found; ++i)
			found = list->longs_a[(short)i] == object_index;
		if (!found && list->count < list->maximum - 1)
		{
			list->longs_a[list->count] = object_index;
			list->shorts_b[list->count] = 0;
			list->longs_c[list->count] = 0;
			list->shorts_d[list->count] = NONE;
			++list->count;
		}
	}
	function_2cbc0(true);
	function_44720();
}

extern byte g_4ba025;
void __stdcall function_3f830(byte const *tree, point3f const *center);
long function_155760(long index);
void function_3f3f0(void);

// @retail 0x2b990
void function_2b990(long player_index)
{
	byte const *tree = *(byte **)((byte *)g_4e0348 + 0x238);
	long object_index;
	if (player_index != NONE)
	{
		object_index = g_4e8c20->entries[player_index];
		if (object_index != NONE && !function_155760(player_index))
		{
			object_index = *(long *)((byte *)g_4e8c24->data + (object_index & 0xffff) * 0x21c + 0x2c);
			goto object_selected;
		}
	}
	object_index = NONE;
object_selected:
	function_3f2c0((s_planar_camera_source const *)&g_camera_storage, (byte const *)&g_projection_storage, object_index, g_4b9ee4);
	function_3f3f0();
	if (g_4ba025)
		function_3f830(tree, &g_camera_storage.field_0);
}

// @retail 0x3d4f0
void function_3d4f0(bool cached, short cache_index, long *flags,
	long *source_object, long *tag, transform4x3f **transforms, dword *count, long object_index)
{
	short const *cache_reference = &cache_index;
	(void)&flags;
	(void)&source_object;
	(void)&tag;
	(void)&transforms;
	dword *const *count_reference = &count;
	if (cached)
	{
		s_3d4f0_entry *entry = &g_4c1bd8[*cache_reference];
		byte *definition = g_4e3b44[entry->tag & 0xffff].bytes;
		*tag = entry->tag;
		*transforms = entry->transforms;
		dword n = *(dword *)(definition + 0x48);
		if (n > 64) n = 64;
		**count_reference = n;
		*flags = entry->flags;
		*source_object = entry->object_index;
	}
	else
	{
		long offset = (object_index & 0xffff) * sizeof(s_render_object_header);
		byte *object = *(byte **)(offset + (dword)g_4e0300->data + 8);
		byte *definition = g_4e3b44[*(long *)object & 0xffff].bytes;
		byte *model = g_4e3b44[*(long *)(definition + 0x38) & 0xffff].bytes;
		long model_tag = *(long *)(model + 4);
		byte *geometry = g_4e3b44[model_tag & 0xffff].bytes;
		*tag = model_tag;
		object = *(byte **)(offset + (dword)g_4e0300->data + 8);
		**count_reference = (dword)(long)*(short *)(object + 0x114) / sizeof(transform4x3f);
		*transforms = (transform4x3f *)(object + *(short *)(object + 0x116));
		**count_reference = *(dword *)(geometry + 0x48);
		*flags = 0;
		*source_object = object_index;
	}
}

// @retail 0x460d0
bool function_460d0(dword const *mask, short index, long part_index)
{
	// The render-entry index is passed on the stack in retail.
	short const *index_reference = &index;
	dword flags = g_4ba138[*index_reference].unknown00;
	s_44940_entry *entry = &g_4ba138[*index_reference];
	bool result = true;
	if (flags & 0x40)
	{
		byte *data;
		result = false;
		if (entry->tag != NONE)
		{
			byte *definition = g_4e3b44[entry->tag & 0xffff].bytes;
			byte *section = *(byte **)(definition + 0x28) + ((entry->flags >> 9) & 0x1ff) * 0x5c;
			data = *(byte **)(section + 0x34);
		}
		else
		{
			byte *structure = (byte *)g_4e0348;
			if (!((byte)(flags >> 12) & 1))
			{
				byte *section = *(byte **)(structure + 0xa0) + ((entry->flags >> 9) & 0x1ff) * 0xb0;
				data = *(byte **)(section + 0x50);
			}
			else
			{
				byte *instances = *(byte **)(structure + 0x144);
				short definition_index = *(short *)(instances + ((entry->flags >> 18) & 0x7ff) * 0x58 + 0x34);
				data = (byte *)*(dword const *)(*(byte **)(structure + 0x13c) + definition_index * 0xc8 + 0x50);
			}
		}
		byte *part = *(byte **)(data + 4) + part_index * 0x48;
		long first = *(short *)(part + 0xa);
		long count = *(short *)(part + 0xc);
		for (long i = first; i < first + count && !result; ++i)
			result = (mask[i >> 5] & (1 << (i & 31))) != 0;
	}
	return result;
}

/* the render model sections, groups and variants, as read here */
struct s_4c2b0_section
{
    byte unknown00[0x38];
    s_geometry_block_info block;
};

struct s_4c2b0_variant
{
    byte unknown00[4];
    short section_indices[6];
};

struct s_4c2b0_group
{
    byte unknown00[8];
    long variant_count;
    s_4c2b0_variant *variants;
};

struct s_4c2b0_definition
{
    byte unknown00[0x1c];
    long group_count;
    s_4c2b0_group *groups;
    long section_count;
    s_4c2b0_section *sections;
};

// @retail 0x4c2b0
bool function_4c2b0(long tag, byte const *wanted, signed char *current, long level,
    bool request, signed char *sections, bool *fallback)
{
    (void)&wanted; (void)&current; (void)&level;
    (void)&request; (void)&sections; (void)&fallback;
    s_4c2b0_definition *definition = (s_4c2b0_definition *)g_4e3b44[tag & 0xffff].bytes;
    *fallback = false;
    word available = 0;
    bool result = true;
    for (long i = 0; i < definition->group_count; ++i)
    {
        s_4c2b0_group *group = &definition->groups[i];
        long selection = wanted[i];
        if (selection != NONE && selection < group->variant_count)
        {
            s_4c2b0_section *section = &definition->sections[group->variants[selection].section_indices[level]];
            if (request)
            {
                if (function_12de70(&section->block, 3)) available |= 1 << i;
            }
            else
            {
                if (function_12de70(&section->block, 0)) available |= 1 << i;
            }
        }
    }
    for (long i = 0; i < definition->group_count; ++i)
    {
        s_4c2b0_group *group = &definition->groups[i];
        if (available & (1 << i))
        {
            sections[i] = (signed char)group->variants[(signed char)wanted[i]].section_indices[level];
            current[i] = wanted[i];
        }
        else if (wanted[i] != 0xff)
        {
            if (current[i] != -1)
            {
                s_4c2b0_variant *variant = &group->variants[current[i]];
                s_4c2b0_section *section = &definition->sections[variant->section_indices[level]];
                if (function_12de70(&section->block, 0))
                {
                    sections[i] = (signed char)variant->section_indices[level];
                    *fallback = true;
                }
                else result = false;
            }
            else
            {
                *fallback = true;
                sections[i] = -1;
            }
        }
        else sections[i] = -1;
    }
    return result;
}

struct s_light_shape_ab;
bool function_c3140(long index);
bool function_c17f0(long light_index, s_light_shape_ab *shape, bool respect_engine);

// @retail 0x31590
bool function_31590(long index, s_light_shape_ab *shape)
{
    volatile bool enabled = false;
    bool result = false;
    bool *result_reference = &result;
    if (index != NONE && function_c3140(index))
    {
        s_render_entry_110 *entry = &((s_render_entry_110 *)g_4e030c->data)[index & 0xffff];
        byte *definition = g_4e3b44[entry->tag & 0xffff].bytes;
        bool visible;
        if (function_31520(index))
        {
            *result_reference = function_c17f0(index, shape, true);
            visible = true;
            if ((*(dword *)definition & 0x400) && g_485602 != 2)
                visible = false;
        }
        else
            visible = enabled;
        *result_reference &= visible;
        *result_reference &= true;
        *result_reference &= *(short *)((byte *)entry + 0x48) != NONE ||
            ((*(dword *)definition & 0x20) && (*(dword *)definition & 0x40000));
    }
    else
        return false;
    return *result_reference;
}

#include <math.h>
struct s_camera;
struct s_tag_data;
real function_50650(real cosine);
real function_13bb90(s_tag_data const *function, real input, real range);
long function_30830(point3f const *a, s_camera const *camera, vector3f const *d,
    point2f const *scale, bool perspective, bool negate, point2f *out);

void function_4ff50(byte const *definition, byte const *state, bool facing, real time, real cosine, real *strength, real *horizontal, real *vertical, real *out);


struct s_137801;
long function_137650(long model_index, long lod, byte const *permutations);
void function_137800(long object_index, long model_index, transform4x3f const *nodes,
    byte const *permutations, long lod, bool alternate, s_137801 *result);

struct s_matrix_workspace
{
    long size;
    byte data[0x27000];
};
s_matrix_workspace g_487b18;

/* the handle of `bytes` (rounded up to dwords) of the frame's render memory, or NONE when it is full */
static inline long render_memory_allocate(long bytes)
{
    long result = NONE;
    long whole_size = bytes;
    long remainder = bytes % 4;
    if (remainder)
        whole_size = bytes - remainder + 4;
    long size = g_487b18.size + whole_size;
    if (size <= 0x27000)
    {
        result = g_487b18.size & 0x0fffffff;
        g_487b18.size = size;
    }
    return result;
}

// @retail 0x3e380
long function_3e380(long model_index, byte lod, transform4x3f const *nodes,
    byte const *permutations, long *node_count, long extra_bytes)
{
    long count = function_137650(model_index, lod, permutations);
    *node_count = count;
    long bytes = 0;
    if (count > 0)
        bytes = count * 48 + 0x44;
    long result = render_memory_allocate(bytes + extra_bytes);
    if (result != NONE)
    {
        s_137801 *out = NULL;
        if ((result & 0x80000000) == 0)
            out = (s_137801 *)(g_487b18.data + (result & 0x0fffffff));
        function_137800(NONE, model_index, nodes, permutations, lod, false, out);
    }
    return result;
}

#include "unknown_030290.h"

struct s_bit_vector_pool;
dword *function_1332b0(long bit_count, s_bit_vector_pool *data);
long function_016ae0(real value);
void function_30290(point3f const *point, s_view const *view, s_camera const *camera, real radius, box2f *bounds);
byte g_509410, g_509411;


void __stdcall function_44e90(short entry_index, short section_index);


transform4x3f *function_b8c00(long object_index, long *node_count);
point3f *function_142700(transform4x3f const *matrix, point3f const *point, point3f *out);
struct s_visibility_sphere_query
{
    point3f center;
    real radius;
};
void __cdecl function_2ddbd0(void *data, s_visibility_sphere_query const *query,
    s_visibility_query_list *second, s_visibility_query_list *first);

signed char *function_3d430(long object_index);
__declspec(noinline) void function_4c640(long object_index, signed char value);

extern s_record_pool *g_509434;
extern long g_4ba050;
long __stdcall function_3ddd0(long object_index);
extern long g_4ba01c;
extern bool g_4ba019;
extern byte *g_485a80;
bool object_or_parent_hidden(long object_index);
bool __stdcall function_bab40(long object_index, long name, real *value);

long function_4c450(long object_index, real distance, real const *definition, real *fraction);

short function_4bcc0(byte *output, long object_index, real distance, long override,
    bool *cached, bool force, signed char *level);

// @retail 0x4c190
bool function_4c190(long *count, long object_index, bool defaults, long tag,
    short *level, signed char fallback_level, signed char *sections, bool *fallback)
{
    (void)&object_index; (void)&defaults; (void)&tag;
    (void)&level; (void)&fallback_level; (void)&sections; (void)&fallback;
    byte *definition = g_4e3b44[tag & 0xffff].bytes;
    byte zero[16];
    byte const *wanted;
    signed char *current;
    if (!defaults)
    {
        byte *object = (byte *)((s_render_object_header *)g_4e0300->data)[object_index & 0xffff].object;
        *count = *(short *)(object + 0x118);
        wanted = object + *(short *)(object + 0x11a);
        *count = *count / 10;
        current = function_3d430(object_index);
    }
    else
    {
        *(long *)(zero + 0) = 0;
        *(long *)(zero + 4) = 0;
        *(long *)(zero + 8) = 0;
        *(long *)(zero + 12) = 0;
        *count = *(long *)(definition + 0x1c);
        current = (signed char *)zero;
        wanted = zero;
    }
    short requested_level = *level;
    bool result = function_4c2b0(tag, wanted, current, requested_level, true, sections, fallback);
    if (!result && fallback_level != -1)
    {
        result = function_4c2b0(tag, wanted, current, fallback_level, false, sections, fallback);
        if (result) *level = fallback_level;
    }
    else
        function_4c640(object_index, (signed char)(byte)*level);
    if (!result)
        function_4c640(object_index, -1);
    return result;
}

extern byte g_4670bc;
extern long g_4858b8;
extern D3DPIXELSHADERDEF g_484f68;
extern D3DResource *g_509444;
struct s_render_reset_state;
struct s_shader_cache;
struct s_coefficient_layout;
struct s_lighting_record;
bool __stdcall function_23690(s_coefficient_layout const *layout, short order, long object_index,
    s_137801 const *vertices, long count, byte const *indices, long tag);
bool function_3d480(long object_index, long count, transform4x3f const *matrices, transform4x3f *out);
void function_3dd10(long object_index, bool force);
s_lighting_record *function_3db00(long object_index, real priority);

struct s_batch_cache_4b5d0
{
    byte unknown00[8];
    long handle;
    dword stamp;
    byte lighting[0x54];
};

struct s_133f70;

// @retail 0x4b5d0
void function_4b5d0(bool force, long override, long object_index, real distance, real priority,
    s_133f70 *record, point3f const *unused, bool project_nodes)
{
    byte *output = (byte *)record;
    transform4x3f projected[255];
    long failed = 0;
    signed char initial_level = -1;
    long entries = function_4bcc0(output, object_index, distance, override,
        (bool *)(output + 0x168), force, &initial_level);
    *(short *)(output + 0x164) = initial_level;
    if (output[0x168] && g_4b9ed8 != NONE && g_4ed284->entries[g_4b9ed8].index != NONE)
        entries = 0;
    *(short *)(output + 0x162) = 0;
    s_render_object_header *header = &((s_render_object_header *)g_4e0300->data)[object_index & 0xffff];
    short type = NONE;
    if (header->unknown00[3] == 6) type = *(short *)((byte *)header->object + 0x134);
    *(short *)(output + 0x166) = type;
    for (long index = 0; index < entries; ++index)
    {
        long slot = index - failed;
        long flags;
        transform4x3f *nodes;
        dword node_count;
        long *source_object = (long *)(output + 0x30) + slot;
        long *tag = (long *)(output + 0x58) + slot;
        long *count = (long *)(output + 0x14) + slot;
        long *handle = (long *)output + slot;
        s_batch_cache_4b5d0 **cache_slot = (s_batch_cache_4b5d0 **)(output + 0x44) + slot;
        bool success = true;
        function_3d4f0(*(bool *)(output + 0x168), (short)index, &flags, source_object,
            tag, &nodes, &node_count, object_index);
        /* the level the object's cache entry was last drawn at (-1 without a mode) */
        long drawn_object = *source_object;
        signed char previous = -1;
        if (g_4ba050 >= 0 && g_4ba050 < 4)
        {
            byte *levels = NULL;
            long cache_index = function_3ddd0(drawn_object);
            if (cache_index != NONE)
                levels = g_509434->data + (cache_index & 0xffff) * 0x100 + 0xbc;
            previous = (signed char)levels[g_4ba050];
        }
        function_3d430(*source_object);
        s_batch_cache_4b5d0 *cache = NULL;
        long cache_index = function_3ddd0(*source_object);
        if (cache_index != NONE)
            cache = (s_batch_cache_4b5d0 *)(g_509434->data + (cache_index & 0xffff) * 0x100 + 0x44);
        *cache_slot = cache;
        output[slot + 0x6c] = (byte)(flags & 1);
        bool reuse = previous != -1 && *cache_slot && (*cache_slot)->handle != NONE &&
            (*cache_slot)->stamp == ((g_4ba034 & 0x3fffffff) | ((dword)g_4ba050 << 30));
        if (reuse)
            *(short *)(output + 0x164) = previous;
        bool fallback;
        signed char *sections = (signed char *)(output + 0x71 + slot * 16);
        long built = function_4c190(count, object_index, *(bool *)(output + 0x168), *tag,
            (short *)(output + 0x164), previous, sections, &fallback);
        reuse &= !fallback;
        bool level_changed = *(short *)(output + 0x164) != initial_level;
        if (reuse && level_changed) reuse = false;
        if ((index > 0 && *(short *)(output + 0x164) != initial_level) || !built)
        {
            ++failed;
            continue;
        }
        long total = function_137650(*tag, *(short *)(output + 0x164), (byte const *)sections);
        output[slot + 0x28] = (byte)total;
        *handle = NONE;
        if (!reuse || output[0x168])
        {
            long bytes = 0;
            if (total > 0)
                bytes = total * 48 + 0x44;
            *handle = render_memory_allocate(bytes);
            if (*handle != NONE)
            {
                if (*cache_slot && !output[0x168] && !fallback && !level_changed)
                {
                    (*cache_slot)->stamp = (g_4ba034 & 0x3fffffff) | ((dword)g_4ba050 << 30);
                    (*cache_slot)->handle = *handle;
                }
                if (project_nodes)
                {
                    function_3d480(object_index, node_count, nodes, projected);
                    nodes = projected;
                }
                s_137801 *result = NULL;
                if (*handle != NONE && (*handle & 0x80000000) == 0)
                    result = (s_137801 *)(g_487b18.data + (*handle & 0x0fffffff));
                function_137800(object_index, *tag, nodes, (byte const *)sections,
                    *(short *)(output + 0x164), *(bool *)(output + 0x168), result);
            }
            else
                success = false;
        }
        if (!reuse && success)
        {
            if (*(dword *)(output + 0x16c) & 0x40)
            {
                byte *definition = g_4e3b44[*tag & 0xffff].bytes;
                s_coefficient_layout const *layout = *(s_coefficient_layout **)(definition + 0x78);
                s_137801 const *vertices = NULL;
                if (*handle != NONE && (*handle & 0x80000000) == 0)
                    vertices = (s_137801 const *)(g_487b18.data + (*handle & 0x0fffffff));
                success = function_23690(layout, *(short *)(output + 0x164), *source_object,
                    vertices, *count, (byte const *)sections, *tag);
            }
            function_3dd10(*source_object, false);
            if (*cache_slot)
                memcpy((*cache_slot)->lighting, function_3db00(*source_object, priority), 0x54);
        }
        else if (reuse && success && !output[0x168] && g_4ba050 != NONE)
            *handle = (*cache_slot)->handle;
        if (success)
        {
            ++*(short *)(output + 0x162);
            byte *object = (byte *)((s_render_object_header *)g_4e0300->data)[*source_object & 0xffff].object;
            long cache_index = *(long *)(object + 0xcc);
            if (cache_index != NONE)
            {
                byte *cache = g_509434->data + (cache_index & 0xffff) * 0x100;
                if (*(long *)(cache + 4) == *source_object) *(dword *)(cache + 0xc) = g_4ba034;
            }
        }
        else ++failed;
    }
}
struct s_type_7ba8e9;
void function_16b10(s_render_reset_state *state);
s_type_7ba8e9 *function_137550(long tag, short bitmap);
bool function_14390(short stage, s_type_7ba8e9 *bitmap, real priority);
void function_14bc0(short target, short element, bool depth);
void function_0222d0(D3DRENDERSTATETYPE state, dword value);
bool function_1ccf0(D3DPIXELSHADERDEF const *program);
dword function_1cc30(long index);
void function_1c620(s_shader_cache *state, long a, long b, long c, byte const *descriptor);
void __stdcall function_1c710(void *state);

void __stdcall function_43a40(long mode);

struct s_masked_list;
typedef void (__stdcall *masked_list_proc)(long value, short mask);
extern dword g_4b8340;
void masked_list_iterate(s_masked_list const *list, long mask, masked_list_proc proc);

// @retail 0x2c370
void function_2c370(long mode)
{
    function_43a40(mode);
    masked_list_iterate(g_547f94, 0xffff, function_44040);
    g_4b8340 = 0;
    D3DDevice_SetRenderState(D3DRS_SOLIDOFFSETENABLE, 0);
}

s_target_cache_storage g_target_cache_storage;

s_material_records_storage g_material_records_storage;
