// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "slot_handler.h"
#include "unknown_2605d0.h"
#include "unknown_2626b0.h"

/* slot type 5, the slot test 0x6e and the slot group 0x6a */

struct s_slot_05
{
	s_slot_header header;
	byte unknown0c;
	bool unknown0d;
	byte unknown0e[2];
	short unknown10;
	bool unknown12;
	byte unknown13;
	s_reference reference;
	byte unknown18[0x40 - 0x18];
};

bool __stdcall function_1b1f70(long actor_index, s_slot *slot);

PRIVATE __forceinline void function_1b1eb2(long arg_0)
{
	s_actor_view *local_0 = actor_get(arg_0);
	*(bool volatile *)&local_0->unknown50c = false;
	*(long volatile *)&local_0->unknown5ac = NONE;
	*(short volatile *)&local_0->unknown5b0 = NONE;
	*(short volatile *)&local_0->unknown5b4 = 0;
	*(short volatile *)&local_0->unknown5b6 = 0;
	local_0->unknown4ac = 0;
	*(short volatile *)&local_0->unknown504 = 0;
}

// score 14 (was 14): ours keeps the scaled actor index in ECX and the actor pointer in EAX,
// retail keeps the index in EAX and the pointer in ECX.
// @retail 0x1b1eb0
bool __stdcall function_1b1eb0(long actor_index, s_slot *slot)
{
	s_slot_05 *state = (s_slot_05 *)slot;
	s_actor_view *actor = actor_get(actor_index);

	if (!state->unknown0d)
		state->unknown10 = 3;
	state->reference = actor->unknown418;
	actor->unknown3f2 = false;
	function_1b1eb2(actor_index);
	return true;
}

// @retail 0x1b1f30
short __stdcall function_1b1f30(long actor_index, s_slot *slot, bool active)
{
	s_slot_05 *state = (s_slot_05 *)slot;
	short result = g_46fbe8;

	if (state->unknown12 && actor_get(actor_index)->unknown504 == 2)
		result = g_46fbe4;
	return result;
}

// @retail 0x1b21f0
void __stdcall function_1b21f0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown086 > 1 && actor->prop_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);

		if (node->unknown24 >= 1 && node->unknown24 <= 2)
		{
			actor->unknown420 = 2;
			actor->unknown488 = true;
			actor->unknown41c = 3;
			return;
		}

		s_prop_view_fields *view = prop_node_view(node);

		if (view && (view->unknown00 >= 5 || view->unknown69 && view->unknown4c))
		{
			actor->unknown41c = 2;
			actor->unknown420 = 2;
		}
		actor->unknown484 = true;
		actor->unknown4a1 = true;
		return;
	}
	actor->unknown484 = true;
	actor->unknown4a1 = true;
}

// @retail 0x1b22e0
void __stdcall function_1b22e0(long actor_index, s_slot *slot, bool active)
{
	s_slot_05 *state = (s_slot_05 *)slot;

	if (!active || (state->reference.unknown2 & 0x8000))
		state->reference = g_470fa0;
}

// @retail 0x1b2300
short __stdcall function_1b2300(long actor_index, s_slot *slot)
{
	return g_46fbe4;
}

// @retail 0x1b2310
bool __stdcall function_1b2310(long actor_index, s_slot *slot)
{
	actor_get(actor_index)->unknown084 = 3;
	return true;
}

s_slot_handler_2 g_47dfd0 =
{
	{
		5, 2, NONE, -2, 0,
		actor_behavior_priority_minimum, function_1b1f30, function_1b1eb0, 0, NONE, {0},
		0, 0, 0, 0, function_1b22e0, 0, 0
	},
	(t_slot_proc)function_1b1f70, 0, function_1b21f0
};

s_slot_handler_0 g_47e01c =
{
	0x6e, 0, NONE, -2, 0, function_1b2300
};

/* the children of slot group 0x6a */
s_slot_child g_46f670[5] =
{
	{0x6e, 1, NONE, {0}, 0, 0, 0},
	{0x6d, 1, NONE, {0}, 0, 0, 0},
	{0x6b, 1, NONE, {0}, 0, 0, 0},
	{5, 1, NONE, {0}, 0, 0, 0},
	{2, 1, NONE, {0}, 0, 0, 0},
};

s_slot_handler_1 g_47e030 =
{
	{
		0x6a, 1, 0, -2, 0,
		actor_behavior_priority_minimum, function_1bced0, function_1b2310, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1a79e0, 5, g_46f670
};

bool function_262590(long actor_index, s_reference reference, bool unknown);
bool function_262890(long actor_index, s_reference reference);
bool function_1f8660(long actor_index);
real function_1e3920(long actor_index);

// @retail 0x1b1f70
bool __stdcall function_1b1f70(long actor_index, s_slot *slot)
{
    s_actor_view *actor = actor_get(actor_index);
    s_slot_05 *state = (s_slot_05 *)slot;
    if (actor->unknown007)
    {
        state->unknown10 = 1;
        return true;
    }
    if (!actor->unknown267)
    {
        bool allowed = actor->unknown328 <= 2;
        if (!state->unknown0c && state->unknown10 == 3)
        {
            if (!REFERENCE_EQUAL(actor->unknown418, g_470fa0))
                state->unknown0c = !function_262590(actor_index, actor->unknown418, allowed);
            else
                state->unknown0c = true;
        }
        if (actor->unknown270 == 4 && !function_1f8660(actor_index))
            state->unknown0c = true;
        else if (!state->unknown0c && !function_1f8660(actor_index) && state->unknown10 == 3 &&
            !REFERENCE_EQUAL(actor->unknown418, g_470fa0))
        {
            s_262b40_result *target = function_262b40(actor->unknown418);
            if (target && function_210b60((s_type_c3b527 *)target, &actor->position) > 2.0f * function_1e3920(actor_index))
                state->unknown0c = true;
        }
        if (actor->unknown040 && (state->unknown0c || !actor->unknown227))
        {
            long other_index = NONE;
            if (!state->unknown0d)
                *(short *)state->unknown0e = actor->unknown086 >= 2 ? 2 : 0xff;
            s_2605d0_request request;
            memset(&request, 0, sizeof(request));
            request.type = 4;
            request.unknown015 = true;
            request.unknown69a = *(short *)state->unknown0e;
            request.unknown698 = true;
            request.unknown69c = allowed;
            *((byte *)&request + 0x59) = true;
            byte *scratch = ai_scratch_buffer_get();
            bool unknown = false;
            s_reference reference = actor->unknown418;
            if (state->unknown0c)
            {
                reference = function_2605d0(actor_index, &request, 0, (long)&other_index, scratch, &unknown);
                if (!unknown)
                {
                    if (!REFERENCE_EQUAL(state->reference, g_470fa0) &&
                        function_262590(actor_index, state->reference, allowed) && !function_262890(actor_index, state->reference))
                        reference = state->reference;
                    else
                        state->reference = reference;
                }
            }
            if (!REFERENCE_EQUAL(function_2626b0(actor_index, reference, other_index, scratch, unknown, true), g_470fa0))
            {
                state->unknown0c = false;
                ai_scratch_buffer_release(scratch);
                return true;
            }
            if (actor->unknown5b4 >= 0x20 && REFERENCE_EQUAL(reference, state->reference))
                state->reference = g_470fa0;
            ai_scratch_buffer_release(scratch);
        }
    }
    return true;
}
