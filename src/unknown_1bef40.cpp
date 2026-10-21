// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "slot_handler.h"
#include "unknown_0259d0.h"
#include "unknown_1fb7e0.h"
#include "unknown_2605d0.h"
#include "unknown_2626b0.h"
#include "unknown_20fe20.h"
#include "unknown_1e1f20.h"

extern "C" void _ReadWriteBarrier(void);

/* slot types 0xa and 0xb */

struct s_slot_0a
{
	s_slot_header header;
	s_reference reference;
	short unknown10;
	short unknown12;
	byte unknown14[0x40 - 0x14];
};

struct s_slot_0b
{
	s_slot_header header;
	word unknown0c;
	byte unknown0e[2];
	long unknown10;
	byte unknown14[0x40 - 0x14];
};

short actor_behavior_find_level(long owner_index, short type);
short __stdcall function_1b7920(long actor_index);

short __stdcall function_1bef40(long actor_index);
short __stdcall function_1bf0f0(long actor_index, s_slot *slot, bool active);
bool __stdcall function_1bf230(long actor_index, s_slot *slot);
bool __stdcall function_1bf4e0(long actor_index, s_slot *slot);
void __stdcall function_1c1520(long actor_index, s_slot *slot, long index);
void __stdcall function_1bf5c0(long actor_index, s_slot *slot);

/* the block of the actor's character tag function_1e4d10 returns */
struct s_character_d10
{
	byte unknown00[0x2c];
	real unknown2c;
};

void *function_1e4d10(long actor_index);
void function_1f86a0(long index);

short function_2684f0(s_actor_view *actor);
void function_e70b0(long unit_index, short *value);
long function_1469f0(real seconds);

#pragma intrinsic(_ReadWriteBarrier)
// score 18 (was 18): retail reads the slot reference before it fetches the
// actor, ours fetches the actor first, and g_46fbe8 is read as a byte here
// where retail reads it as a dword.
// @retail 0x1bf0f0
short __stdcall function_1bf0f0(long actor_index, s_slot *slot, bool active)
{
	s_slot_0a *state = (s_slot_0a *)slot;
	s_actor_view *actor = actor_get(actor_index);
	short result = g_46fbe8;
	_ReadWriteBarrier();
	if (REFERENCE_EQUAL(state->reference, g_470fa0))
	{
		function_1f86a0(actor_index);
	}
	else if (actor->unknown504 == 2)
	{
		short action;
		function_e70b0(actor->unknown018, &action);
		if (action == 5)
		{
			if (state->unknown12 > 0)
				state->unknown12--;
			else if (function_2684f0(actor) >= 10)
			{
				real delay = function_259d0(&g_4e7408->unknown0, __FILE__, __LINE__, 3.0f, 5.0f);
				real wait = function_259d0(&g_4e7408->unknown0, __FILE__, __LINE__, 0.0f, 0.5f);
				state->unknown10 = (short)function_1469f0(delay);
				state->unknown12 = (short)function_1469f0(wait);
				function_262800(actor_index, state->reference, false);
				function_1f86a0(actor_index);
				state->reference = g_470fa0;
			}
			else if (--state->unknown10 <= 0)
			{
				g_46eeb8[0x2b]->unknown8 = g_46f348;
				g_46eeb8[0xa]->unknown8 = g_46f348;
				result = g_46fbe4;
			}
		}
		else
		{
			result = g_46fbe4;
		}
	}
	return result;
}
#pragma function(_ReadWriteBarrier)

// @retail 0x1bef40
short __stdcall function_1bef40(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = g_46fbe4;

	if (actor->prop_index != NONE)
	{
		s_character_d10 *character = (s_character_d10 *)function_1e4d10(actor_index);

		if (character)
		{
			s_prop_node_view *node = prop_node_get(actor->prop_index);

			if (node->unknown24 >= 1 && node->unknown24 <= 2 && character->unknown2c * 2.0f > node->unknown28)
				result = 3;
		}
	}
	return result;
}

PRIVATE __forceinline void function_1befd1(long arg_0)
{
    s_actor_view *local_0 = actor_get(arg_0);
    *(bool volatile *)&local_0->unknown50c = false;
    *(short volatile *)&local_0->unknown5b4 = 0;
    *(short volatile *)&local_0->unknown5b6 = 0;
    *(long volatile *)&local_0->unknown5ac = NONE;
    *(short volatile *)&local_0->unknown5b0 = NONE;
    local_0->unknown4ac = 0;
    local_0->unknown504 = 0;
}

static __forceinline long real_to_long(real value);

// score 42 (was 42): retail sets up its frame with a single push where ours
// reserves 0x14 bytes, and the random seed it passes to function_259d0 comes
// from a different global than ours.
// @retail 0x1befd0
bool __stdcall function_1befd0(long actor_index, s_slot *slot)
{
	struct s_1befd4 { double field_0; double field_8; } local_0;
	s_slot_0a *state = (s_slot_0a *)slot;
	local_0.field_0 = function_259d0(&g_4e7408->unknown0, __FILE__, __LINE__, 3.0f, 5.0f);
	local_0.field_8 = function_259d0(&g_4e7408->unknown0, __FILE__, __LINE__, 0.0f, 0.5f);
	real ticks;
	long rounded;

	function_1befd1(actor_index);
	state->reference = g_470fa0;

	ticks = g_510c54->field_2_3 * local_0.field_0;
	rounded = real_to_long(ticks);
	state->unknown10 = (short)rounded;

	ticks = g_510c54->field_2_3 * local_0.field_8;
	rounded = real_to_long(ticks);
	state->unknown12 = (short)rounded;
	return true;
}

// @retail 0x1bf230
bool __stdcall function_1bf230(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_0a *state = (s_slot_0a *)slot;
	bool result = true;

	if (!REFERENCE_EQUAL(state->reference, g_470fa0) && actor->unknown5b4 > 0)
		state->reference = g_470fa0;
	if (actor->unknown040)
	{
		long other_index = NONE;

		result = false;
		if (REFERENCE_EQUAL(state->reference, g_470fa0))
		{
			byte *scratch = ai_scratch_buffer_get();
			s_2605d0_request request;
			bool unknown;

			memset(&request, 0, sizeof(request));
			unknown = false;
			request.type = 7;
			request.unknown008 = 5.0f;
			request.unknown00c = 5.0f;
			request.unknown056 = true;
			request.unknown057 = true;
			request.unknown010 = 10.0f;
			state->reference = function_2605d0(actor_index, &request, 0, (long)&other_index, scratch, &unknown);
			ai_scratch_buffer_release(scratch);
			if (REFERENCE_EQUAL(state->reference, g_470fa0))
				goto done;
		}
		state->reference = function_2626b0(actor_index, state->reference, other_index, NULL, false, true);
		result = !REFERENCE_EQUAL(state->reference, g_470fa0);
	}
done:
	return result;
}

// @retail 0x1bf360
void __stdcall function_1bf360(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->prop_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);

		if (node->unknown24 >= 1 && node->unknown24 <= 2)
		{
			actor->unknown41c = 3;
			actor->unknown420 = 2;
			actor->unknown488 = true;
		}
	}
}

// @retail 0x1bf3d0
void __stdcall function_1bf3d0(long actor_index, s_slot *slot, bool active)
{
	s_slot_0a *state = (s_slot_0a *)slot;

	if (!active || (state->reference.unknown2 & 0x8000))
		state->reference = g_470fa0;
}

// @retail 0x1bf3f0
bool function_1bf3f0(long actor_index, long other_index, short type)
{
	bool result = false;
	s_actor_view *actor = actor_get(actor_index);

	if (other_index != NONE)
	{
		s_actor_view *other = actor_get(other_index);

		if (other->prop_index != NONE && actor->prop_index != NONE &&
			prop_node_get(actor->prop_index)->unknown08 == prop_node_get(other->prop_index)->unknown08 &&
			actor_behavior_find_level(other_index, type) != NONE)
		{
			result = true;
		}
	}
	return result;
}

// @retail 0x1bf480
short __stdcall function_1bf480(long actor_index)
{
	short result = 0;
	s_actor_view *actor = actor_get(actor_index);

	if (function_1b7920(actor_index) && function_1bf3f0(actor_index, actor->unknown3cc, actor->unknown3d0))
		result = 3;
	return result;
}

// @retail 0x1bf4e0
bool __stdcall function_1bf4e0(long actor_index, s_slot *slot)
{
	bool result = false;
	s_actor_view *actor = actor_get(actor_index);

	if (function_1bf3f0(actor_index, actor->unknown3cc, actor->unknown3d0))
	{
		s_slot_0b *state = (s_slot_0b *)slot;
		short type;

		state->unknown10 = actor->unknown3cc;
		state->unknown0c = actor->unknown3d0;
		type = actor->unknown3d2;
		actor->unknown3cc = NONE;
		if (type != NONE)
			function_1fb7e0(type, actor_index, NULL, actor_get(state->unknown10)->unknown018, NONE);
		function_1f86a0(actor_index);
		result = true;
	}
	return result;
}

// @retail 0x1bf590
short __stdcall function_1bf590(long actor_index, s_slot *slot, bool active)
{
	s_slot_0b *state = (s_slot_0b *)slot;
	short result = g_46fbe4;

	if (function_1bf3f0(actor_index, state->unknown10, state->unknown0c))
		result = g_46fbe8;
	return result;
}

/* how the actor's character uses its weapon, by difficulty */
struct s_weapon_difficulty_entry_0b
{
	long unknown0;
	long unknown4;
	long unknown8;
};

struct s_character_weapon_0b
{
	byte unknown00[0x98];
	s_weapon_difficulty_entry_0b difficulty[3];
};

/* aims the actor at its prop: held props are only approached */
// @retail 0x1bf5c0
void __stdcall function_1bf5c0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	s_prop_node_view *node = prop_node_get(actor->prop_index);
	s_prop_state_view *s_type_5cfb45 = prop_node_state(node);
	s_prop_view_fields *view = prop_node_view(node);
	long weapon_index = function_1e1f20(actor_index);

	if (node->unknown24 >= 1 && node->unknown24 <= 2)
	{
		actor->unknown488 = true;
		actor->unknown41c = 4;
		actor->unknown420 = 2;
	}
	else if (view)
	{
		s_type_c3b527 point = view->unknown18;
		point3f position;

		point.point.z += s_type_5cfb45->unknown38 - s_type_5cfb45->position.z;
		function_210850(&point, &position);
		actor->unknown41c = 3;
		actor->unknown420 = 3;
		actor->unknown424.point = position;
		actor->unknown430 = 2;
		actor->unknown434 = 3;
		actor->unknown438.point = position;
		actor->unknown444 = NONE;
		actor->unknown488 = true;
		actor->unknown4a0 = true;
		actor->unknown48c = true;
		actor->unknown490_point = point;
		if (weapon_index != NONE)
		{
			s_character_weapon_0b *weapon = (s_character_weapon_0b *)function_1e5280(actor_index, object_get(weapon_index)->tag_index);

			if (weapon)
			{
				s_weapon_difficulty_entry_0b *entry;
				short local_0 = g_4e6948->state == 1 ? g_4e6948->difficulty : 1;

				switch (local_0)
				{
				case 2:
					entry = &weapon->difficulty[1];
					break;
				case 3:
					entry = &weapon->difficulty[2];
					break;
				default:
					entry = &weapon->difficulty[0];
					break;
				}
				actor->unknown710 = entry->unknown4;
			}
		}
	}
}

s_slot_handler_2 g_47ee18 =
{
	{
		0xa, 2, 0x7ff, -2, 0,
		function_1bef40, function_1bf0f0, function_1befd0, slot_proc_nothing, NONE, {0},
		0, 0, 0, 0, function_1bf3d0, 0, 0
	},
	(t_slot_proc)function_1bf230, 0, function_1bf360
};

s_slot_handler_2 g_47ee68 =
{
	{
		0xb, 2, 0, -2, 0,
		function_1bf480, function_1bf590, function_1bf4e0, 0, NONE, {0},
		function_1c1520, 0, 0, 0, 0, 0, 0
	},
	0, 0, function_1bf5c0
};
