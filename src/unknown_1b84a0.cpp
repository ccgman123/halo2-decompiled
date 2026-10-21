// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "slot_handler.h"
#include "unknown_0259a0.h"

/* slot type 0x23 */

struct s_slot_23
{
	s_slot_header header;
	bool unknown0c;
	bool unknown0d;
	short unknown0e;
	byte unknown10[0x1b - 0x10];
	bool unknown1b;
	bool unknown1c;
	char unknown1d;
	byte unknown1e[2];
	s_type_c3b527 point;
	vector3f facing;
	byte unknown3c[0x40 - 0x3c];
};

short __stdcall function_1b84a0(long actor_index);
bool __stdcall function_1b85a0(long actor_index, s_slot *slot);
void __stdcall function_1b89d0(long actor_index, s_slot *slot);
void __stdcall function_1b8ae0(long actor_index, s_slot *slot);
bool function_25da00(s_prop_node_view *node);

// @retail 0x1b84a0
short __stdcall function_1b84a0(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (actor->prop_index != NONE && !actor->unknown223)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);
		s_prop_view_fields *view = prop_node_view(node);

		if (view && node->unknown24 >= 3 && (view->unknown70 == 1 || !function_25da00(node)))
			result = 3;
	}
	return result;
}

// @retail 0x1b8540
bool __stdcall function_1b8540(long actor_index, s_slot *slot)
{
	bool result = false;
	s_actor_view *actor = actor_get(actor_index);

	if (actor->prop_index != NONE && prop_node_get(actor->prop_index)->unknown27 < 2)
	{
		s_slot_23 *state = (s_slot_23 *)slot;

		state->unknown0c = false;
		state->unknown0d = false;
		state->unknown0e = 0;
		result = true;
	}
	return result;
}

real function_1f8940(long actor_index);
void *function_1e4db0(long actor_index);

/* the block of the actor's character tag function_1e4db0 returns */
struct s_character_db0_flags
{
	byte flags;
};

bool function_1f86f0(long index);
bool function_1f4f40(long actor_index, vector3f const *facing, short unknown, s_type_c3b527 const *point, bool face_prop);
void function_262800(long actor_index, s_reference reference, bool unknown);

/* slot type 0x23, update: once the actor stands at its prop's position, it
   turns to face it; then it lets the prop go */
// score 81 (was 81): retail reads the pool before the registers are pushed,
// ours reads it
// afterwards, so the prologue differs.
// @retail 0x1b89d0
void __stdcall function_1b89d0(long actor_index, s_slot *slot)
{
	long prop_index = actor_get(actor_index)->prop_index;
	s_slot_23 *state = (s_slot_23 *)slot;

	if (prop_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(prop_index);
		s_prop_view_fields *view = prop_node_view(node);

		if (view)
		{
			s_actor_view *actor = actor_get(actor_index);
			if (state->unknown0c)
				return;

			bool ready = state->unknown0d && function_1f86f0(actor_index);
			state->unknown0c = ready;
			if (ready)
			{
				if (!state->unknown1b || !state->unknown1d || state->unknown1c)
					return;

				function_1f4f40(actor_index, &state->facing, state->unknown1d, &state->point, true);
				state->unknown1c = true;
				state->unknown0c = false;
			}

			if (view->unknown70 != 0 || !function_25da00(node))
				return;

			function_262800(actor_index, actor->unknown418, false);
		}
	}

	state->unknown0c = true;
}

// score 41 (was 41): retail reads a different global and keeps the record
// count in EBP,
// ours uses ESI and another pool.
// @retail 0x1b8ae0
void __stdcall function_1b8ae0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->prop_index != NONE)
	{
		s_prop_view_fields *view = prop_node_view(prop_node_get(actor->prop_index));
		s_slot_23 *state = (s_slot_23 *)slot;

		actor->unknown4a2 = true;
		if (view->unknown06 != 2)
			state->unknown0e = g_510c54->field_2_3;
		else if (state->unknown0e > 0)
			state->unknown0e--;

		s_actor_view *current = actor_get(actor_index);
		if (current->unknown50c && current->unknown504 == 1 &&
			function_1f8940(actor_index) >= 2.0f && state->unknown0e <= 0)
		{
			actor->unknown41c = 3;
			actor->unknown420 = 0;
		}
		else if (view->unknown70 == 0)
		{
			actor->unknown420 = 2;
			actor->unknown41c = 4;
		}
		else if (view->unknown70 == 1)
		{
			point3f point;

			function_210850(&view->unknown78, &point);
			actor->unknown424.point = point;
			actor->unknown420 = 3;
			actor->unknown41c = 4;
		}

		if (actor->unknown229)
			actor->unknown482 = true;

		s_character_db0_flags *character = (s_character_db0_flags *)function_1e4db0(actor_index);

		if (character && (character->flags & 2) && actor->unknown086 <= 3)
			actor->unknown483 = true;
	}
}

// @retail 0x1b8c60
short __stdcall function_1b8c60(long actor_index, s_slot *slot, bool active)
{
	s_slot_23 *state = (s_slot_23 *)slot;
	short result = g_46fbe8;

	if (state->unknown0c)
		result = g_46fbe4;
	return result;
}

s_slot_handler_2 g_47e908 =
{
	{
		0x23, 2, 0, -2, 0,
		function_1b84a0, function_1b8c60, function_1b8540, slot_proc_nothing, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	(t_slot_proc)function_1b85a0, function_1b89d0, function_1b8ae0
};

void *function_1e4be0(long actor_index);
bool __stdcall function_1ac610(long actor_index, point3f const *point,
    s_object_marker *markers, short *indices);
short __stdcall function_272af0(s_match_globals *structure, point3f const *point);
struct s_1f4a20_entry;
struct s_1f4a20_source;
bool function_1f4a20(s_1f4a20_entry const *entries, long actor_index, s_reference reference, s_1f4a20_source const *source,
    bool alternate, s_type_c3b527 *point, vector3f *facing,
    signed char *type, bool *available);

// score 323 (was 323): the size we use is 0x1634 where retail uses 0x162c,
// and the index is
// scaled in EAX here and in ESI there, so the loop is a different length.
// @retail 0x1b85a0
bool __stdcall function_1b85a0(long actor_index, s_slot *slot)
{
    volatile bool result = true;
    s_actor_view *actor = actor_get(actor_index);
    if (actor->prop_index == NONE)
        return false;
    if (!*((bool *)actor + 0x40))
        return result;

    s_prop_node_view *node = prop_node_get(actor->prop_index);
    s_prop_view_fields *view = prop_node_view(node);
    byte *character = (byte *)function_1e4be0(actor_index);
    s_slot_23 *state = (s_slot_23 *)slot;
    state->unknown1b = false;
    state->unknown1c = false;
    state->unknown1d = 0;
    bool has_markers = false;
    s_object_marker markers[32];
    short marker_indices[32];
    if (!*((bool *)actor + 0x3c) && actor->unknown227)
        has_markers = function_1ac610(actor_index, &prop_node_state(node)->position,
            markers, marker_indices);

    s_2605d0_request request;
    memset(&request, 0, sizeof(request));
    request.type = 3;
    real distance;
    if (view && view->unknown70 == 1)
    {
        function_210850(&view->unknown78, (point3f *)((byte *)&request + 0x24));
        short sector = *(short *)((byte *)view + 0x84) == NONE ?
            *(short *)((byte *)view + 0x72) :
            function_272af0(g_4e0348, (point3f *)((byte *)&request + 0x24));
        *((bool *)&request + 0x20) = true;
        *(s_type_c3b527 *)((byte *)&request + 0x30) = view->unknown78;
        *(long *)((byte *)&request + 0x40) = *(long *)((byte *)view + 0x74);
        *(short *)((byte *)&request + 0x44) = sector;
        distance = function_210ac0(&view->unknown78, &actor->position);
    }
    else
    {
        *((bool *)&request + 0x52) = false;
        distance = node->unknown28;
    }
    byte *movement = (byte *)function_1e4db0(actor_index);
    long minimum;
    if (movement)
    {
        real limit = *(real *)(movement + 0xc);
        minimum = (long)(limit > distance ? distance : limit);
        *(long *)((byte *)&request + 0x684) = minimum;
        *(long *)((byte *)&request + 0x688) = (long)*(real *)(movement + 0x10);
    }
    else
    {
        minimum = 0;
        *(long *)((byte *)&request + 0x684) = 0;
        *(long *)((byte *)&request + 0x688) = 100;
    }
    *(real *)((byte *)&request + 0x48) = 2.0f;
    *((bool *)&request + 0x46) = true;
    *(real *)((byte *)&request + 0x4c) = (real)minimum;
    *((bool *)&request + 0x50) = true;
    *((bool *)&request + 0x59) = character && (*character & 1);
    *(s_object_marker **)((byte *)&request + 0x64) = has_markers ? markers : NULL;
    if (*(real *)((byte *)state + 0x10) != 0.0f)
        *(real *)((byte *)&request + 0x1c) = *(real *)((byte *)state + 0x10);
    if (actor->unknown50c && actor_get(actor_index)->unknown504 != 2 &&
        !REFERENCE_EQUAL(actor->unknown418, g_470fa0))
    {
        *((bool *)&request + 0x692) = true;
        *(s_reference *)((byte *)&request + 0x694) = actor->unknown418;
    }

    byte *scratch = ai_scratch_buffer_get();
    s_261d20_entry entry;
    long other_actor;
    bool unknown;
    s_reference reference = function_261280((s_prop_search *)&request, actor_index,
        &entry, &other_actor, scratch, &unknown);
    if (!REFERENCE_EQUAL(reference, g_470fa0) && *(short *)((byte *)&entry + 8) <= 1)
    {
        s_262b40_result *location = function_262b40(reference);
        bool available = false;
        bool success = false;
        if (*(short *)((byte *)&entry + 0x5c) != 0 &&
            *(long *)((byte *)location + 0x14) != NONE &&
            *(long *)((byte *)location + 0x14) != 0xffff)
        {
            success = function_1f4a20((s_1f4a20_entry const *)&entry, actor_index, reference,
                (s_1f4a20_source const *)scratch, true, &state->point, &state->facing,
                (signed char *)&state->unknown1d, &available);
            if (success)
            {
                state->unknown1b = true;
                state->unknown1c = false;
            }
            else if (available)
                reference = g_470fa0;
        }
        if (!available && !success)
            reference = function_2626b0(actor_index, reference, other_actor, scratch, unknown, true);
        if (!REFERENCE_EQUAL(reference, g_470fa0))
        {
            state->unknown0d = true;
            actor->unknown4ae = true;
            ai_scratch_buffer_release(scratch);
            return result;
        }
    }
    state->unknown0c = true;
    g_46eeb8[0x23]->unknown8 = g_46f348;
    ai_scratch_buffer_release(scratch);
    return result;
}
