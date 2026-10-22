// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "slot_handler.h"
#include "ai_actor.h"

/* slot type 0x59's evaluate callback: once the actor has reached the other
   actor it follows (+0x10) and both are close enough for its vehicle, it
   hands over to slot type 0x4c, boarding the vehicle */

struct s_slot_59_view
{
	s_slot_header header;
	real unknown0c;
	long unknown10;
	bool unknown14;
	bool unknown15;
	bool unknown16;
	bool unknown17;
	byte unknown18[0x40 - 0x18];
};

/* the state slot type 0x4c starts from */
struct s_slot_4c_request
{
	s_slot_header header;
	byte unknown0c[4];
	long element_index;
	byte unknown14[0x1c - 0x14];
	long unknown1c;
	short seat_index;
	byte flags;
	byte unknown23;
	short unknown24;
	byte unknown26[2];
	real unknown28;
	real unknown2c;
	byte unknown30[0x40 - 0x30];
};

/* the actor's entry for its vehicle (function_1e5450) */
struct s_vehicle_element_59
{
	byte unknown00[0x14];
	real unknown14;
};

short __stdcall function_1bc6d0(long actor_index);
bool __stdcall function_1f4280(long actor_index);
short __stdcall function_1bc850(long actor_index, s_slot *slot, s_slot *next);

// @retail 0x1bc850
short __stdcall function_1bc850(long actor_index, s_slot *slot, s_slot *next)
{
	short result = g_46fbe8;
	s_slot_59_view *state = (s_slot_59_view *)slot;
	s_actor_view *actor = actor_get(actor_index);

	if (state->unknown10 == NONE || !function_1bc6d0(actor_index))
		result = g_46fbe4;
	else if (state->unknown17)
	{
		s_vehicle_element_59 *element = (s_vehicle_element_59 *)function_1e5450(actor_index, ai_object_get(actor->unknown26c)->definition_index);
		if (element)
		{
			if (distance3d(&actor->position, &actor_get(state->unknown10)->position) < element->unknown14 * 2.0f)
			{
				s_slot_4c_request *request = (s_slot_4c_request *)next;

				function_1f4280(actor_index);
				long vehicle_index = actor->unknown26c;
				request->unknown28 = 11.0f;
				request->seat_index = NONE;
				request->unknown2c = 14.0f;
				request->unknown1c = vehicle_index;
				request->flags |= 0x29;
				result = 0x4c;
			}
		}
		else
			result = g_46fbe4;
	}
	return result;
}
