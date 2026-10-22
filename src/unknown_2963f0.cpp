// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_2963F0.CPP: the search of a graph's weapon types with the "any"
   names as fallbacks */

#include "unknown_11c920.h"
#include "unknown_1dacb0.h"
#include "data_array.h"
#include "globals.h"
#include "slot_handler.h"
#include "object_iterator.h"
#include "unknown_1e46c0.h"
#include <float.h>
#include <string.h>


#define ANY_NAME 0x30000d9

struct s_scenario_identifier_ab;
void *__stdcall function_b7a40(s_scenario_identifier_ab const *identifier, long *index_out);

struct s_2960e0_counts
{
	byte unknown00[0x50];
	long first_count;
	byte unknown54[0xa8 - 0x54];
	long second_count;
};

__forceinline long combined_2960e0(s_2960e0_counts const *counts)
{
	return (word)(counts->first_count + counts->second_count);
}

// @retail 0x2960e0
long function_2960e0(s_2960e0_counts *counts, long *objects, long maximum_count)
{
	memset(objects, 0xff, maximum_count * sizeof(long));
	struct
	{
		s_object *object;
		s_type_f1af8e iterator;
	} state;
	function_bae80(&state.iterator, 0x40, 0);
	while ((state.object = function_baeb0(&state.iterator)) != NULL)
	{
		long index = NONE;
		if (function_b7a40((s_scenario_identifier_ab const *)((byte *)state.object + 0xa4), &index))
		{
			short slot = (short)index;
			if (slot >= maximum_count)
				break;
			objects[slot] = state.iterator.object_index;
		}
	}
	s_2960e0_counts const *base_counts = counts;
	function_bae80(&state.iterator, 0x80, 0);
	while ((state.object = function_baeb0(&state.iterator)) != NULL)
	{
		long index = NONE;
		if (function_b7a40((s_scenario_identifier_ab const *)((byte *)state.object + 0xa4), &index))
		{
			short slot = (short)(base_counts->first_count + (short)index);
			if (slot >= maximum_count)
				break;
			objects[slot] = state.iterator.object_index;
		}
	}
	short extra_count = *(short *)((byte *)g_4e0348 + 0x140);
	for (short i = 0; i < extra_count; i++)
	{
		short slot = (short)(combined_2960e0(counts) + i);
		if (slot >= maximum_count)
			break;
		objects[slot] = NONE;
	}
	return counts->first_count + counts->second_count + extra_count;
}

// @retail 0x2963f0
s_graph_weapon_type *graph_weapon_type_iterate(s_graph_weapon_type_iterator *iterator, long *found_mode,
	long *found_weapon_class, long *found_weapon_type)
{
	s_graph_weapon_type *result = NULL;

	while (!result)
	{
		if (iterator->step >= 8)
		{
			break;
		}
		if (!iterator->mode_entry)
		{
			if (iterator->step & 4)
			{
				if (iterator->mode == ANY_NAME)
				{
					goto mode_found;
				}
				*found_mode = ANY_NAME;
			}
			else
			{
				*found_mode = iterator->mode;
			}
			iterator->mode_entry = (s_graph_mode_entry *)function_1dd560((s_sorted_array *)&iterator->graph->mode_count,
				*found_mode, 0x14);
			iterator->class_entry = NULL;
		}
	mode_found:
		if (iterator->mode_entry)
		{
			if (!iterator->class_entry)
			{
				if (iterator->step & 2)
				{
					if (iterator->weapon_class == ANY_NAME)
					{
						goto class_found;
					}
					*found_weapon_class = ANY_NAME;
				}
				else
				{
					*found_weapon_class = iterator->weapon_class;
				}
				iterator->class_entry = (s_graph_mode_entry *)function_1dd560(
					(s_sorted_array *)&iterator->mode_entry->child_count, *found_weapon_class, 0x14);
			}
		class_found:
			if (iterator->class_entry)
			{
				if (iterator->step & 1)
				{
					if (iterator->weapon_type == ANY_NAME)
					{
						goto type_found;
					}
					*found_weapon_type = ANY_NAME;
				}
				else
				{
					*found_weapon_type = iterator->weapon_type;
				}
				result = (s_graph_weapon_type *)function_1dd560((s_sorted_array *)&iterator->class_entry->child_count,
					*found_weapon_type, 0x34);
			type_found:
				iterator->step++;
				if (!(iterator->step & 1))
				{
					iterator->class_entry = NULL;
					if (!(iterator->step & 2))
					{
						iterator->mode_entry = NULL;
					}
				}
			}
			else
			{
				iterator->step = (iterator->step + 2) & ~1;
				if (!(iterator->step & 2))
				{
					iterator->mode_entry = NULL;
				}
			}
		}
		else
		{
			iterator->step = (iterator->step + 4) & ~3;
			iterator->class_entry = NULL;
		}
	}
	return result;
}

/* Object, pool, and tree utilities. */

extern void *g_5047f4;

struct s_295970_state
{
	short field00;
	short field02;
	short field04;
	short field06;
	short field08;
	short field0a;
	long field0c;
	short field10;
	short counts[3];
	long indices[3];
};

// @retail 0x295970
void function_295970(void)
{
	s_295970_state *state = (s_295970_state *)g_5047f4;
	state->field00 = 0;
	state->field02 = 0;
	state->field08 = 0;
	state->field06 = 0;
	state->field0c = NONE;
	long i = 0;
	do
	{
		state->counts[i] = 0;
		state->indices[i] = NONE;
		i++;
	}
	while (i < 3);
}

char *function_11c9c0(char *buffer, long size, char const *format, ...);

// @retail 0x296270
s_record_pool *function_296270(char const *name, long size, long count)
{
	s_record_pool *result = NULL;
	char label[256];
	function_11c9c0(label, sizeof(label), "%s reference", name);
	long *local_0 = &size;
	size = *(volatile long *)local_0 + 12;
	if (size == 32)
		result = data_new(label, count, size, 5, g_510c2c);
	else
		result = data_new(label, count, size, 0, g_510c2c);
	return result;
}

struct s_296230_object
{
	byte unknown00[0xaa];
	byte type;
	byte unknownab[0x130 - 0xab];
	short field130;
	byte unknown132[0x1d4 - 0x132];
	short field1d4;
};

// @retail 0x296230
short function_296230(long object_index)
{
	short result = NONE;
	s_296230_object *object = (s_296230_object *)object_get(object_index);
	if (object->type == 6)
		result = object->field130;
	else if (object->type == 7)
		result = object->field1d4;
	return result;
}

struct s_296330_entry
{
	long unknown00;
	long key;
	long next;
};

// @retail 0x2962e0
void function_2962e0(s_record_pool *pool, long *head, long key, long size, void const *value)
{
	long index = *head;
	while (index != NONE)
	{
		s_296330_entry *entry = (s_296330_entry *)(pool->data + (index & 0xffff) * pool->size);
		if (entry->key == key)
		{
			memcpy(entry + 1, value, size);
			break;
		}
		index = entry->next;
	}
}

struct s_2959c0
{
	byte field_0[0x120];
	long field_120;
	real *field_124;
};

struct s_2959c1
{
	byte field_0[0xc8];
	long field_c8;
	s_2959c0 *field_cc;
};

bool unit_submit_entry_request(short fallback_index, short entry_index, long unit_index, long priority, void const *extra);
void function_295bd0(bool prefer_selected);

__forceinline long function_295a34(real arg_0)
{
	long local_0;
	__asm
	{
		fld arg_0
		fistp local_0
	}
	return local_0;
}

#pragma inline_depth(0)
PRIVATE __forceinline bool function_2959c1(short arg_0, short arg_1, long arg_2, long arg_3, void const *arg_4)
{
    return unit_submit_entry_request(arg_1, arg_0, arg_2, arg_3, arg_4);
}
#pragma inline_depth(255)

// @retail 0x2959c0
void function_2959c0()
{
	s_2959c1 *local_0 = (s_2959c1 *)g_4e034c;
	long local_1 = g_510c54->game_time;
	if (local_0->field_c8 > 0 && local_0->field_cc->field_120 > 0)
	{
		real *local_2 = local_0->field_cc->field_124;
		s_295970_state *local_3 = (s_295970_state *)g_5047f4;
		local_3->field02++;
		if (local_3->field02 > 5000)
			local_3->field02 = 5000;
		if (local_3->field0c != NONE)
        {
            long local_9 = function_295a34(g_510c54->field_2_3 * 10.0f);
            if (local_1 - local_3->field0c > local_9)
            {
                local_3->field0c = NONE;
                local_3->field08 = 0;
            }
        }
		volatile long local_8 = 3;
		long local_4 = 0;
		do
		{
			if (local_3->counts[local_4] > 0 && --local_3->counts[local_4] == 0)
			{
				long local_5 = actor_get(local_3->indices[local_4])->unknown018;
				if (local_5 != NONE)
				{
					if (local_3->field00 == 0)
						function_2959c1(NONE, 0x10, local_5, 0xe, NULL);
					else
						function_2959c1(NONE, 0x11, local_5, 0xe, NULL);
					local_3 = (s_295970_state *)g_5047f4;
				}
			}
			local_4++;
			local_8--;
		} while (local_8 != 0);
		short local_6;
		switch (local_3->field00)
		{
		case 0:
			if (local_3->field08 < 3)
				break;
			local_6 = 1;
			function_295bd0(false);
			goto local_7;
		case 1:
			if (local_3->field02 > function_295a34(g_510c54->field_2_3 * local_2[2]) ||
				local_3->field04 > function_295a34(g_510c54->field_2_3 * local_2[1]) ||
				(*(byte *)&local_3->field10 && local_3->field04 > function_295a34(g_510c54->field_2_3 * local_2[0])))
			{
				local_6 = 0;
				function_295bd0(true);
local_7:
				local_3->field00 = local_6;
				local_3->field02 = 0;
				local_3->field08 = 0;
				local_3->field04 = 0;
			}
			break;
		}
		if (*(byte *)&local_3->field10)
			local_3->field04 = 0;
		else
		{
			local_3->field04++;
			if (local_3->field04 > 5000)
				local_3->field04 = 5000;
		}
		*(byte *)&local_3->field10 = false;
	}
}

// @retail 0x296330
void function_296330(s_record_pool *pool, long *head, long key)
{
	while (*head != NONE)
	{
		s_296330_entry *entry = (s_296330_entry *)(pool->data + (*head & 0xffff) * pool->size);
		if (entry->key == key)
		{
			long next = entry->next;
			record_pool_release(pool, *head);
			*head = next;
			break;
		}
		head = &entry->next;
	}
}

// @retail 0x296370
bool function_296370(long *head, s_record_pool *pool, long key)
{
	bool result = false;
	long index = *head;
	while (index != NONE)
	{
		s_296330_entry *entry = (s_296330_entry *)(pool->data + (index & 0xffff) * pool->size);
		if (entry->key == key)
		{
			result = true;
			break;
		}
		index = entry->next;
	}
	return result;
}

// @retail 0x2963b0
void function_2963b0(s_record_pool *pool, long index)
{
	while (index != NONE)
	{
		s_296330_entry *entry = (s_296330_entry *)(pool->data + (index & 0xffff) * pool->size);
		long next = entry->next;
		record_pool_release(pool, index);
		index = next;
	}
}

struct s_296520_node
{
	point2f normal;
	real distance;
	short children[2];
};

struct s_296520_tree
{
	long count;
	s_296520_node *nodes;
};

// @retail 0x296520
long function_296520(s_296520_tree *tree, long index, point2f const *point)
{
	while (!(index & 0x8000))
	{
		s_296520_node *node = &tree->nodes[index & 0x7fff];
		real distance = node->normal.y * point->y + node->normal.x * point->x - node->distance;
		index = node->children[distance >= 0.0f ? 1 : 0];
	}
	if (index != NONE)
		return index & 0x7fff;
	return NONE;
}

struct s_295bd0_flags
{
	dword other0 : 3;
	dword enabled : 1;
	dword other4 : 28;
};

struct s_295bd0_definition
{
	byte unknown00[8];
	long parent;
	byte unknown0c[0x34 - 0xc];
	long count;
	s_295bd0_flags *entries;
};

__forceinline s_295bd0_flags *flags_295bd0(long actor_index)
{
	long index = actor_get(actor_index)->unknown054;
	while (index != NONE)
	{
		s_295bd0_definition *definition = (s_295bd0_definition *)g_4e3b44[index & 0xffff].bytes;
		if (definition->count > 0)
			return definition->entries;
		index = definition->parent;
	}
	return NULL;
}

__forceinline long ticks_295bd0(real value)
{
	long result;
	__asm
	{
		fld value
		fistp result
	}
	return result;
}

__forceinline short find_295bd0(real const *scores, real value)
{
	for (short i = 0; i < 3; i++)
	{
		if (scores[i] == value)
			return i;
	}
	return NONE;
}

// @retail 0x295bd0
void function_295bd0(bool prefer_selected)
{
	short count = 0;
	real minimum = 0.0f;
	real scores[3];
	memset(scores, 0, sizeof(scores));
	long indices[3];
	memset(indices, 0xff, sizeof(indices));
	s_actor_iterator iterator;
	function_x66da2b(&iterator, true);
	s_actor_view *actor;
	while ((actor = (s_actor_view *)function_1e46c0(&iterator)) != NULL)
	{
		s_295bd0_flags *flags = flags_295bd0(iterator.actor_index);
		if (!flags)
			continue;
		bool enabled = (bool)flags->enabled;
		// Preserve the retail temporary store without reloading it for the test.
		volatile bool flag_copy = enabled;
		if (enabled && actor->unknown018 != NONE)
		{
			real score = 0.0f;
			long prop_index = *(long *)((byte *)actor + 0x338);
			if (prop_index != NONE)
			{
				s_prop_node_view *prop = prop_node_get(prop_index);
				if (prop->unknown28 < 25.0f)
				{
					if (prefer_selected && (prop->unknown24 < 1 || prop->unknown24 > 2))
						score = 24.0f;
					score += 25.0f - prop->unknown28;
				}
			}
			if (score > minimum)
			{
				short slot;
				if (count < 3)
					slot = count;
				else
					slot = find_295bd0(scores, minimum);
				if (slot != NONE)
				{
					indices[slot] = iterator.actor_index;
					scores[slot] = score;
					if (count < 3)
						count++;
					if (count >= 3)
					{
						minimum = FLT_MAX;
						real const *current = scores;
						long remaining = 3;
						do
						{
							if (minimum > *current)
								// Retail assigns the candidate score here, not *current.
								minimum = score;
							current++;
						}
						while (--remaining);
					}
				}
			}
		}
	}
	s_295970_state *state = (s_295970_state *)g_5047f4;
	long i = 0;
	do
	{
		state->counts[i] = 0;
		state->indices[i] = NONE;
		i++;
	}
	while (i < 3);
	if (count > 0)
	{
		s_game_time_globals *clock = g_510c54;
		dword *seed = &g_4e7408->unknown0;
		for (short i = 0; i < count; i++)
		{
			*seed = 1664525 * *seed + 1013904223;
			real time = (real)(*seed >> 16) * (1.f / 65535.f);
			time *= 1.3f;
			time += 0.2f;
			state->counts[i] = (short)ticks_295bd0(time * clock->field_2_3);
			state->indices[i] = indices[i];
		}
	}
}

struct s_295e60_object
{
	long definition_index;
	dword flags;
	byte unknown008[0x130 - 8];
	short field_130;
	byte unknown132[0x1d4 - 0x132];
	short field_1d4;
};

struct s_295e60_entry
{
	short field_0;
	short value;
};

struct s_295e60_placement
{
	byte unknown00[0x40];
	long count_40;
	s_295e60_entry *entries_44;
	byte unknown48[8];
	long count_50;
	s_295e60_entry *entries_54;
};

struct s_scenery_placement;
short __fastcall function_10a040(long definition_index, s_scenery_placement *placement);
inline void function_b8b70(long object_index);

// @retail 0x295e60
void function_295e60(short arg_0)
{
	struct
	{
		s_295e60_object *object;
		s_type_f1af8e iterator;
	} state;
	function_bae80(&state.iterator, 0x40, 0);
	while ((state.object = (s_295e60_object *)function_baeb0(&state.iterator)) != NULL)
	{
		s_295e60_placement *placement = (s_295e60_placement *)function_b7a40((s_scenario_identifier_ab const *)((byte *)state.object + 0xa4), NULL);
		dword *flags = &state.object->flags;
		*flags &= ~0x400000;
		state.object->field_130 = NONE;
		if (placement)
		{
			switch (function_10a040(state.object->definition_index, (s_scenery_placement *)placement))
			{
			case 0:
			case 3:
				*flags |= 0x800000;
				break;
			case 1:
				if (arg_0 < placement->count_50)
				{
					s_295e60_entry *entry = &placement->entries_54[arg_0];
					state.object->field_130 = entry->value;
					if (entry->value != NONE)
						*flags |= 0x400000;
				}
				*flags |= 0x800000;
				break;
			}
		}
		function_b8b70(state.iterator.object_index);
	}
	function_bae80(&state.iterator, 0x80, 0);
	while ((state.object = (s_295e60_object *)function_baeb0(&state.iterator)) != NULL)
	{
		s_295e60_placement *placement = (s_295e60_placement *)function_b7a40((s_scenario_identifier_ab const *)((byte *)state.object + 0xa4), NULL);
		dword *flags = &state.object->flags;
		*flags &= ~0x400000;
		state.object->field_1d4 = NONE;
		if (placement)
		{
			short mode = 3;
			if (state.object->definition_index != NONE)
			{
				byte *definition = g_4e3b44[state.object->definition_index & 0xffff].bytes;
				switch (*(short *)(definition + 0x130))
				{
				case 0: mode = 2; break;
				case 1: mode = 1; break;
				case 2: mode = 0; break;
				default: mode = 3; break;
				}
			}
			switch (mode)
			{
			case 0:
			case 3:
				*flags |= 0x800000;
				break;
			case 1:
				if (arg_0 < placement->count_40)
				{
					s_295e60_entry *entry = &placement->entries_44[arg_0];
					state.object->field_1d4 = entry->value;
					if (entry->value != NONE)
						*flags |= 0x400000;
				}
				*flags |= 0x800000;
				break;
			}
		}
		function_b8b70(state.iterator.object_index);
	}
}
