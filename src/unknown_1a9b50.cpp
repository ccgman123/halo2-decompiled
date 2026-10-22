#include <string.h>
// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1A9B50.CPP: the slot handlers of types 0x42, 0x18, 0x19, 0x10 and
   0x11 (0x47f7f0, 0x47da7c..0x47db48) */

#include "unknown_11c920.h"
#include "slot_handler.h"
#include "ai_actor.h"

/* the actor's tag entry function_1e4f90 returns (0x40 bytes) */
struct s_actor_tag_entry
{
	byte flags;
	byte unknown01[3];
	real unknown04;
	real unknown08;
	real unknown0c;
	byte unknown10[0x18 - 0x10];
	real unknown18;
	real unknown1c;
	real unknown20;
	real unknown24;
	byte unknown28[0x34 - 0x28];
	real unknown34;
	byte unknown38[0x40 - 0x38];
};

/* the state of the slots of types 0x10 and 0x11 */
struct s_slot_10
{
	s_slot_header header;
	long time;
	short next_type;
	bool unknown12;
	byte unknown13[0x17 - 0x13];
	bool unknown17;
	bool unknown18;
	byte unknown19;
	short unknown1a;
	s_type_c3b527 unknown1c;
	byte unknown2c[0x40 - 0x2c];
};

struct s_prop_datum_view
{
	byte unknown00[0x27];
	char unknown27;
	real unknown28;
	byte unknown2c[0x3c - 0x2c];
};

struct s_prop_state_64
{
	byte unknown00[0x64];
	bool unknown64;
};

real function_259a0(dword *seed);
void *function_1e4f90(long actor_index);
real function_1c9ee0(real fraction);
bool __stdcall function_1aa750(long actor_index, s_slot *slot, bool flag);
short __stdcall function_1a9ec0(long actor_index);
bool __stdcall function_1aa0d0(long arg_0, s_slot *arg_1);
bool __stdcall function_1aab50(long arg_0, s_slot *arg_1);
void __stdcall function_1c04c0(long actor_index, s_slot *slot);

static inline bool actor_function_50c(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	return actor->unknown50c && actor->unknown504 == 1;
}

/* ---- slot type 0x42 ---- */

// @retail 0x1a9b50
short __stdcall function_1a9b50(long actor_index, s_slot *slot)
{
	function_1e4650(actor_index, true);
	return g_46fbe4;
}

/* ---- slot type 0x18 ---- */

// @retail 0x1a9bc0
short __stdcall function_1a9bc0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown07c != NONE)
	{
		long other_index = element_502420_get(actor->unknown07c)->first_actor_index;
		s_actor_view *other;
		do
		{
			if (other_index == NONE)
			{
				function_1e4650(actor_index, true);
				break;
			}
			other = actor_get(other_index);
			other_index = other->next_index;
		} while (other == actor || other->unknown004 != actor->unknown004);
	}
	return g_46fbe4;
}

/* ---- slot type 0x19 ---- */

// @retail 0x1a9c50
short __stdcall function_1a9c50(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown358 != 0 && actor->unknown35e)
	{
		function_1e4650(actor_index, true);
		if (!actor->unknown226)
		{
			dword *seed = &g_4e7408->unknown0;
			*seed = 1664525 * *seed + 1013904223;
			if (0.5f > (real)(*seed >> 16) * (1.f / 65535.f))
				actor->unknown226 = true;
		}
	}
	return g_46fbe4;
}

/* ---- slot types 0x10 and 0x11 ---- */

// @retail 0x1a9cf0
short __stdcall function_1a9cf0(long actor_index, s_slot *slot, bool active)
{
	short result = g_46fbe8;
	s_slot_10 *state = (s_slot_10 *)slot;
	short type = state->next_type;

	if (type >= 0 && type < k_slot_type_count)
	{
		s_slot_handler *handler = g_46eeb8[type];
		if (handler->unknown8 != g_46f348 &&
			(handler->mask & g_4ee4ec) == g_4ee4ec &&
			(g_557c40[type >> 5] & (1 << (type & 31))) != 0)
		{
			result = state->next_type;
			if (result != g_46fbe8)
				goto local_0;
		}
	}
	if (g_46eeb8[state->header.type]->unknown8 == g_46f348 || state->unknown12)
		result = g_46fbe4;
local_0:
	return result;
}

void function_1f86a0(long index);
byte function_1f8640(long index);
bool __stdcall function_1f4280(long actor_index);
void function_265c30(long prop_index, long actor_index, bool unknown);

/* stops the actor's prop approach once the prop is out of reach */
// @retail 0x1a9d80
void function_1a9d80(long actor_index, real distance, bool *approaching, bool *stopped)
{
	s_actor_view *actor = actor_get(actor_index);

	if (!actor->unknown007 && actor->unknown040)
	{
		*stopped = false;
		if (*approaching)
		{
			function_1f86a0(actor_index);
			if (!function_1f4810(actor_index, actor->prop_index, distance, 1))
			{
				*stopped = true;
				*approaching = false;
				function_1f4280(actor_index);
				actor_get(actor_index)->unknown040 = false;
			}
		}
	}
}

// @retail 0x1a9e00
void function_1a9e00(long actor_index, real distance, bool force)
{
	if (function_1e2030(actor_index))
	{
		s_actor_view *actor = actor_get(actor_index);

		if (!actor->unknown007 && actor->unknown040)
		{
			long prop_index = actor->prop_index;

			if (prop_index != NONE)
			{
				short *view = (short *)function_25d700(prop_index);

				if (view && *view >= 6)
				{
					bool close = false;

					if (prop_node_get(prop_index)->unknown28 > distance &&
						(force || !function_1f8640(actor_index) || actor->unknown524 > distance))
					{
						close = true;
					}
					function_265c30(prop_index, actor_index, close);
				}
			}
		}
	}
}
// @retail 0x1aa6e0
void __stdcall function_1aa6e0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown41c = 4;
	actor->unknown420 = 2;
	s_actor_view *local_0 = actor_get(actor_index);
	if (local_0->unknown50c)
	{
		if (local_0->unknown504 == 1)
		{
			actor->unknown3e0 = 0;
			actor->unknown44d = true;
		}
		else
			actor->unknown3e0 = 0;
	}
	else
		actor->unknown3e0 = 0;
}

// @retail 0x1aa990
bool __stdcall function_1aa990(long actor_index, s_slot *slot)
{
	bool result = false;

	if (!actor_get(actor_index)->unknown264)
	{
		result = function_1aa750(actor_index, slot, false);
		if (result)
		{
			s_actor_tag_entry *entry = (s_actor_tag_entry *)function_1e4f90(actor_index);
			if (entry && (entry->flags & 2))
				function_1e4650(actor_index, true);
		}
	}
	return result;
}

// score 17 (was 17): retail loads the prop pool pointer into EAX, one byte
// shorter than our ECX, and passes entry->unknown24 in ECX where we pass it in
// EDX; the one-byte difference moves every later jump target.
// @retail 0x1aa9f0
short __stdcall function_1aa9f0(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	s_ai_object *unit = ai_object_get(actor->unknown018);
	s_actor_tag_entry *entry = (s_actor_tag_entry *)function_1e4f90(actor_index);
	short result = 0;

	if (!(unit->flags19 & 1) && entry && actor->prop_index != NONE &&
		entry->unknown24 >= g_45dbd8 && !actor->unknown229 &&
		(actor->times[4] == NONE || (real)(g_510c54->game_time - actor->times[4]) * g_510c54->rate > entry->unknown34) &&
		!function_110ab0(actor->unknown018))
	{
		s_prop_datum_view *prop = (s_prop_datum_view *)prop_node_get(actor->prop_index);
		s_prop_state_64 *state = (s_prop_state_64 *)function_25d690((s_prop_datum *)prop);
		if (prop->unknown27 >= 1 && (state->unknown64 || prop->unknown28 > entry->unknown1c) && entry->unknown20 > prop->unknown28)
		{
			if (state->unknown64 && !function_1e2030(actor_index))
				result = 4;
			else if (function_1c9ee0(entry->unknown24) > function_259a0(&g_4e7408->unknown0))
				result = 3;
		}
	}
	return result;
}

// @retail 0x1ab2c0
void __stdcall function_1ab2c0(long actor_index, s_slot *slot)
{
	s_slot_10 *state = (s_slot_10 *)slot;

	if (state->unknown17 && !state->unknown12 && !actor_get(actor_index)->unknown264)
		state->unknown1a++;
}

// @retail 0x1ab300
void __stdcall function_1ab300(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_10 *state = (s_slot_10 *)slot;

	function_1aa6e0(actor_index, slot);
	if (state->unknown18)
	{
		actor->unknown464 = true;
		actor->unknown465 = true;
		actor->unknown466 = true;
		((dword *)&actor->unknown468)[0] = ((dword *)&state->unknown1c)[0];
		((dword *)&actor->unknown468)[1] = ((dword *)&state->unknown1c)[1];
		((dword *)&actor->unknown468)[2] = ((dword *)&state->unknown1c)[2];
		((dword *)&actor->unknown468)[3] = ((dword *)&state->unknown1c)[3];
		state->unknown17 = true;
		state->unknown18 = false;
		state->unknown1a = 0;
	}
}

// @retail 0x1ab380
bool __stdcall function_1ab380(long actor_index, s_slot *slot)
{
	return function_1aa750(actor_index, slot, true);
}

// @retail 0x1ab3a0
void __stdcall function_1ab3a0(long actor_index, s_slot *slot)
{
	actor_get(actor_index)->times[4] = g_510c54->game_time;
}

/* ---- the handlers ---- */

s_slot_handler_0 g_47f7f0 =
{
	0x42, 0, 0, -2, 0, function_1a9b50
};

s_slot_handler_0 g_47da7c =
{
	0x18, 0, 0x7ff, -2, 0, function_1a9bc0
};

s_slot_handler_0 g_47da90 =
{
	0x19, 0, 0x7ff, -2, 0, function_1a9c50
};

s_slot_handler_2 g_47daa8 =
{
	{
		0x10, 2, 0, -2, 0,
		function_1a9ec0, function_1a9cf0, function_1aa990, function_1c04c0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	(t_slot_proc)function_1aa0d0, 0, function_1aa6e0
};

s_slot_handler_2 g_47daf8 =
{
	{
		0x11, 2, 0, -2, 0,
		function_1aa9f0, function_1a9cf0, function_1ab380, function_1ab3a0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	(t_slot_proc)function_1aab50, function_1ab2c0, function_1ab300
};

struct s_1aa0d0
{
	byte field_0[0xc];
	long field_c;
	short field_10;
	bool field_12;
	bool field_13;
	bool field_14;
	char field_15;
	bool field_16;
	bool field_17;
	bool field_18;
	byte field_19[0x2c - 0x19];
	bool field_2c;
	bool field_2d;
	bool field_2e;
	byte field_2f;
	short field_30;
	short field_32;
	real field_34;
	real field_38;
	byte field_3c[4];
};

struct s_1aa0d1
{
	byte field_0[0xc];
	real field_c;
	real field_10;
	real field_14;
};

struct s_1aa0d2
{
	byte field_0[4];
	point3f field_4;
	byte field_10[0x1c - 0x10];
	vector3f field_1c;
};

bool function_10f340(long arg_0, long arg_1, long arg_2);
bool unit_submit_entry_request(short fallback_index, short entry_index, long unit_index, long priority, void const *extra);
long function_1469f0(real arg_0);
real function_30bf0(vector3f *arg_0);
real normalize2d(point2f *arg_0);
extern point2f *g_468778;

// @retail 0x1aa0d0
bool __stdcall function_1aa0d0(long arg_0, s_slot *arg_1)
{
	bool result = false;
	s_1aa0d1 *local_1 = (s_1aa0d1 *)function_1e4f90(arg_0);
	s_1aa0d0 *local_2 = (s_1aa0d0 *)arg_1;
	s_actor_view *local_0 = actor_get(arg_0);
	if (local_1 && !local_2->field_13 && !local_2->field_14 && !local_2->field_2d)
	{
		s_prop_node_view *local_3 = NULL;
		if (local_0->prop_index != NONE)
		{
			real local_4 = (real)g_510c54->game_time;
			real local_5 = 3.402823466e+38f;
			local_3 = prop_node_get(local_0->prop_index);
			if (!local_2->field_12)
			{
				if (!local_0->unknown225 && !function_1e2030(arg_0))
					local_5 = local_1->field_10;
				if (!local_0->unknown225 && function_1e2030(arg_0))
				{
					if ((local_3->unknown28 > local_5 && function_1e2030(arg_0)) ||
						local_4 >= (real)(local_2->field_c + function_1469f0(local_1->field_14)))
						local_2->field_14 = true;
					else
						local_2->field_2c = true;
				}
				else
				{
					local_2->field_14 = false;
					local_2->field_2c = true;
				}
			}
		}
		else
		{
			local_2->field_2c = false;
			local_2->field_14 = true;
		}
		if (local_2->field_12)
			local_2->field_13 = !function_110ab0(local_0->unknown018);
		else if (local_3)
		{
			s_1aa0d2 *local_6 = (s_1aa0d2 *)function_25d690((s_prop_datum *)local_3);
			s_prop_view_fields *local_7 = (s_prop_view_fields *)function_25d740((s_prop_node *)local_3);
			vector3f const *local_8 = &local_7->unknown2c;
			if (!function_110ab0(local_0->unknown018))
			{
				vector3f local_9;
				if (-0.4f > local_8->k * local_0->unknown290.k + local_8->j * local_0->unknown290.j + local_8->i * local_0->unknown290.i &&
					local_1->field_c > local_3->unknown28 && !local_0->unknown264 &&
					function_10f340(local_0->unknown018, 0x7000101, 0xa0005b9) && !local_2->field_16)
				{
					local_2->field_15 = 1;
					local_9.i = -1.0f * local_8->i;
					local_9.j = -1.0f * local_8->j;
				}
				else if (0.8f > local_3->unknown28)
				{
					local_9 = *local_8;
					local_2->field_15 = local_2->field_16 ? 2 : 0;
				}
				else
				{
					real local_10 = 0.0f;
					real local_11 = magnitude3d(&local_6->field_1c);
					if (local_11 > 0.0f)
						local_10 = (1.0f + (local_8->k * local_6->field_1c.k + local_8->j * local_6->field_1c.j + local_8->i * local_6->field_1c.i) / local_11) * 0.5f;
					real local_12 = (real)local_2->field_30 * g_510c54->rate * local_10;
					local_9.j = local_6->field_1c.j * local_12 + local_6->field_4.y - local_0->position.y;
					local_9.k = local_6->field_1c.k * local_12 + local_6->field_4.z - local_0->position.z;
					local_9.i = local_6->field_1c.i * local_12 + local_6->field_4.x - local_0->position.x;
					real local_13 = 0.0f;
					if (0.0f > local_8->k * local_9.k + local_8->j * local_9.j + local_8->i * local_9.i)
						local_9 = *local_8;
					else
					{
						local_13 = function_30bf0(&local_9);
						if (local_13 == 0.0f)
							local_9 = *local_8;
					}
					if (local_2->field_34 > local_13)
						local_2->field_15 = local_2->field_16 ? 2 : 0;
					else if (local_2->field_38 > 0.0f && !local_2->field_16)
					{
						vector3f local_14;
						local_12 = (real)local_2->field_32 * g_510c54->rate * local_10;
						local_14.i = local_6->field_1c.i * local_12 + local_6->field_4.x - local_0->position.x;
						local_14.j = local_6->field_1c.j * local_12 + local_6->field_4.y - local_0->position.y;
						local_14.k = local_6->field_1c.k * local_12 + local_6->field_4.z - local_0->position.z;
						if (!(local_2->field_38 > function_30bf0(&local_14)))
							goto local_15;
						local_2->field_15 = 3;
						local_9 = local_14;
					}
					else
						goto local_15;
				}
				if (!local_2->field_2e && (local_2->field_15 == 0 || local_2->field_15 == 2 || local_2->field_15 == 3))
				{
					point2f local_16 = {local_9.i, local_9.j};
					if (normalize2d(&local_16) > 0.0f && 0.866025388f > local_0->unknown290.j * local_16.y + local_0->unknown290.i * local_16.x)
						goto local_15;
				}
				s_unit_request local_17;
				memset(&local_17, 0, sizeof(local_17));
				local_17.type = 0x1a;
				local_17.type1a.unknown4 = local_2->field_15;
				*(point2f *)(local_17.arguments + 4) = *(point2f *)&local_9;
				local_17.type1a.unknown6 = true;
				point2f *local_18 = (point2f *)(local_17.arguments + 4);
				if (normalize2d(local_18) == 0.0f)
				{
					*local_18 = *(point2f *)&local_0->unknown290;
					if (normalize2d(local_18) == 0.0f)
						*local_18 = *g_468778;
				}
				if (function_e6900(local_0->unknown018, &local_17))
				{
					unit_submit_entry_request(0x12, NONE, local_0->unknown018, 0xd, NULL);
					local_2->field_12 = true;
				}
			}
		}
	local_15:
		if (local_2->field_12)
		{
			local_2->field_2d = false;
			function_1f4280(arg_0);
		}
		else
			function_1a9d80(arg_0, local_1->field_c, &local_2->field_2c, &local_2->field_2d);
		if (!local_2->field_12)
			function_1a9e00(arg_0, local_1->field_c, local_2->field_2d);
		if (!local_2->field_14 && !local_2->field_13 && !local_2->field_2d)
			result = true;
	}
	return result;
}

bool function_25d9b0(long arg_0);
bool function_fa1a0(real arg_0, real arg_1, point3f const *arg_2, point3f const *arg_3,
	real *arg_4, real const *arg_5, real const *arg_6, bool arg_7, vector3f *arg_8,
	real *arg_9, real *arg_10, real *arg_11, real *arg_12, real *arg_13);

struct s_1aab50
{
	byte field_0[0x10];
	real field_10;
	real field_14;
	real field_18;
	real field_1c;
	real field_20;
	real field_24;
	real field_28;
	real field_2c;
};

struct s_1aab51
{
	byte field_0[0x1c];
	point2f field_1c;
	real field_24;
	real field_28;
};

static __forceinline long real_to_long(real value);

// @retail 0x1aab50
bool __stdcall function_1aab50(long arg_0, s_slot *arg_1)
{
	s_1aa0d0 *local_0 = (s_1aa0d0 *)arg_1;
	bool local_2;
	if (!local_0->field_13 && !local_0->field_14 && !local_0->field_2d)
	{
		local_2 = true;
		s_actor_view *local_1 = actor_get(arg_0);
		s_1aab50 *local_3 = (s_1aab50 *)function_1e4f90(arg_0);
		s_prop_node_view *local_4 = NULL;
		if (local_3)
		{
			if (local_1->prop_index != NONE)
			{
				local_4 = prop_node_get(local_1->prop_index);
				real local_5 = 3.402823466e+38f;
				bool local_6 = true;
				if (!local_0->field_12 && !local_0->field_17 && !local_0->field_18)
				{
					if (local_1->unknown225 || !function_1e2030(arg_0))
						local_5 = local_3->field_10;
					if (local_4->unknown28 > local_5 && function_1e2030(arg_0))
					{
						local_0->field_14 = true;
						goto local_7;
					}
				}
				else
					local_6 = false;
				local_0->field_2c = true;
				if (local_6 && !((s_prop_state_64 *)function_25d690((s_prop_datum *)local_4))->unknown64 && local_3->field_1c > local_4->unknown28)
				{
					local_0->field_10 = 0x10;
					local_0->field_14 = true;
				}
			}
			else
				local_0->field_2c = false;
		}
	local_7:
		if (local_0->field_12)
			local_0->field_13 = local_1->unknown018 == NONE || !function_110ab0(local_1->unknown018);
		else if (!local_0->field_18)
		{
			s_1aa0d2 *local_8 = (s_1aa0d2 *)function_25d690((s_prop_datum *)local_4);
			s_prop_view_fields *local_9 = (s_prop_view_fields *)function_25d740((s_prop_node *)local_4);
			bool local_10 = false;
			vector3f local_11;
			if (0.8f > local_4->unknown28)
			{
				local_11 = local_9->unknown2c;
				local_10 = true;
			}
			else
			{
				real local_12 = (real)sqrt(local_8->field_1c.i * local_8->field_1c.i + local_8->field_1c.j * local_8->field_1c.j + local_8->field_1c.k * local_8->field_1c.k);
				s_ai_object *local_13 = ai_object_get(local_1->unknown018);
				real local_14 = 0.0f;
				if (local_12 > 0.0f)
					local_14 = (1.0f + (local_8->field_1c.k * local_9->unknown2c.k + local_8->field_1c.j * local_9->unknown2c.j + local_8->field_1c.i * local_9->unknown2c.i) / local_12) * 0.5f;
				real local_15 = (real)local_0->field_30;
				real local_16 = local_15 * g_510c54->rate * local_14;
				local_11.k = local_16 * local_8->field_1c.k + local_8->field_4.z - local_1->position.z;
				local_11.j = local_16 * local_8->field_1c.j + local_8->field_4.y - local_1->position.y;
				local_11.i = local_16 * local_8->field_1c.i + local_8->field_4.x - local_1->position.x;
				real local_17 = 0.0f;
				if (0.0f > local_9->unknown2c.k * local_11.k + local_9->unknown2c.j * local_11.j + local_9->unknown2c.i * local_11.i)
					local_11 = local_9->unknown2c;
				else
				{
					local_17 = function_30bf0(&local_11);
					if (local_17 == 0.0f)
						local_11 = local_9->unknown2c;
				}
				if (!local_0->field_17)
				{
					if (local_3->field_1c > local_17 && !function_25d9b0(local_1->prop_index) && !((s_prop_state_64 *)local_8)->unknown64)
					{
						local_0->field_14 = true;
						goto local_18;
					}
					if (local_3->field_20 > local_17)
					{
						real local_19 = local_3->field_28;
						real local_20;
						vector3f local_22;
						/* retail reuses the speed variable as the vertical speed output and stores that result below */
						if (function_fa1a0(local_19, 1.0f, &local_8->field_4, &local_1->position, &local_15, NULL, NULL, false, &local_22, NULL, NULL, NULL, &local_19, &local_20) ||
							(local_3->field_2c > local_15 && function_fa1a0(local_15 + 0.03f, 1.0f, &local_8->field_4, &local_1->position, NULL, NULL, NULL, false, &local_22, NULL, NULL, NULL, &local_19, &local_20)))
						{
							if (normalize2d((point2f *)&local_22) == 0.0f)
							{
								local_22 = local_1->unknown290;
								if (normalize2d((point2f *)&local_22) == 0.0f)
									local_22 = *g_4687a8;
							}
							s_1aab51 *local_24 = (s_1aab51 *)local_0;
							local_24->field_24 = local_20;
							local_0->field_18 = true;
							local_24->field_1c = *(point2f *)&local_22;
							local_24->field_28 = local_19;
						}
					}
				}
				else if (local_0->field_2e)
				{
					if (local_0->field_34 > local_17 || (0.0f > local_17 &&
						(local_8->field_1c.k - local_13->velocity.k) * local_11.k + (local_8->field_1c.j - local_13->velocity.j) * local_11.j + (local_8->field_1c.i - local_13->velocity.i) * local_11.i > 0.7f))
						local_10 = true;
				}
				else if (0.5f > local_17 - (local_13->velocity.k * local_11.k + local_13->velocity.j * local_11.j + local_11.i * local_13->velocity.i) * local_15 * g_510c54->rate)
					local_10 = true;
			}
		local_18:
			if (local_0->field_18 || (local_10 && !local_0->field_2e))
			{
				point2f local_25 = {local_11.i, local_11.j};
				if (normalize2d(&local_25) > 0.0f)
				{
					real local_26 = local_0->field_17 ? 0.0f : 0.95f;
					if (local_26 > local_1->unknown290.j * local_25.y + local_1->unknown290.i * local_25.x)
					{
						local_0->field_18 = false;
						goto local_27;
					}
				}
			}
			if (local_10)
			{
				s_unit_request local_28;
				memset(&local_28, 0, sizeof(local_28));
				local_28.type1a.unknown4 = 0;
				local_28.type = 0x1a;
				point2f *local_29 = (point2f *)(local_28.arguments + 4);
				*local_29 = *(point2f *)&local_11;
				local_28.type1a.unknown6 = true;
				if (normalize2d(local_29) == 0.0f)
				{
					*local_29 = *(point2f *)&local_1->unknown290;
					if (normalize2d(local_29) == 0.0f)
						*local_29 = *g_468778;
				}
				if (function_e6900(local_1->unknown018, &local_28))
				{
					unit_submit_entry_request(0x13, NONE, local_1->unknown018, 0xd, NULL);
					local_0->field_12 = true;
				}
			}
		}
	local_27:
		long local_30 = g_510c54->game_time;
		if (!local_0->field_12 && !local_0->field_18)
		{
			if (local_0->field_17)
			{
				if ((real)*(short *)((byte *)local_0 + 0x1a) * g_510c54->rate > 0.5f)
					local_0->field_14 = true;
			}
			else
			{
				real local_31 = (real)g_510c54->field_2_3 * local_3->field_14;
				long local_32 = real_to_long(local_31);
				if (local_30 >= local_0->field_c + local_32)
					local_0->field_14 = true;
			}
		}
		if (!local_0->field_12 && !local_0->field_17 && !local_0->field_18)
			function_1a9d80(arg_0, local_3->field_1c, &local_0->field_2c, &local_0->field_2d);
		else
			function_1f4280(arg_0);
		if (!local_0->field_17 && !local_0->field_18 && !local_0->field_12)
			function_1a9e00(arg_0, local_0->field_34, local_0->field_2d);
	}
	else
		local_2 = false;
	return local_2;
}

// score 45 (was 44): retail branches to 0x1aa0b6 where ours branches to another
// place, loads [esi+0x18] into EAX rather than ECX and reads the pool pointer
// at 0x4e0300 into EDX before it masks the index.
// @retail 0x1a9ec0
short __stdcall function_1a9ec0(long arg_0)
{
	s_actor_view *local_0 = actor_get(arg_0);
	s_actor_tag_entry *local_1 = (s_actor_tag_entry *)function_1e4f90(arg_0);
	long local_2 = 0;
	if (local_1 && local_0->prop_index != NONE && !function_25d9b0(local_0->prop_index) && !function_110ab0(local_0->unknown018))
	{
		s_ai_object *local_3 = ai_object_get(local_0->unknown018);
		if (!(local_3->flags19 & 1) || (!(local_3->flags19 & 2) && (local_1->flags & 1)))
		{
			s_prop_node_view *local_4 = prop_node_get(local_0->prop_index);
			s_prop_state_view *local_5 = (s_prop_state_view *)function_25d690((s_prop_datum *)local_4);
			s_prop_view_fields *local_6 = (s_prop_view_fields *)function_25d740((s_prop_node *)local_4);
			vector3f const *facing = &local_0->unknown290;
			if ((local_0->unknown229 || !((s_prop_state_64 *)local_5)->unknown64) && local_4->unknown27 >= 1)
			{
				if (local_0->unknown225)
					local_2 = 3;
				else
				{
					bool choose_four = !function_1e2030(arg_0) || (local_6 && local_1->unknown0c > local_4->unknown28 &&
						-0.4f > local_6->unknown2c.k * facing->k + local_6->unknown2c.j * facing->j + local_6->unknown2c.i * facing->i &&
						!local_0->unknown264 && function_10f340(local_0->unknown018, 0x7000101, 0xa0005b9));
					if (choose_four)
					{
						local_2 = 4;
					}
					else if (local_5->unknown3c == NONE && local_1->unknown04 >= local_4->unknown28 &&
						(local_0->times[3] == NONE || (real)(g_510c54->game_time - local_0->times[3]) * g_510c54->rate > function_1e9700(0x15) * local_1->unknown18))
					{
						real local_8 = function_1c9ee0(local_1->unknown08);
						if (local_8 > function_259a0(&g_4e7408->unknown0))
							local_2 = 3;
					}
				}
			}
		}
	}
	return (short)local_2;
}

bool unit_animation_get_timing(long arg_0, long arg_1, long arg_2, short *arg_3, real *arg_4, short *arg_5, real *arg_6);
long function_1e4a50(long arg_0);

struct s_1aa750
{
	byte field_0[4];
	real field_4;
};

struct s_1aa751
{
    long field_0;
    short field_4;
    byte field_6[3];
    char field_9;
    bool field_a;
    byte field_b[0x24 - 0xb];
    short field_24;
    short field_26;
    real field_28;
    real field_2c;
};

// @retail 0x1aa750
bool __stdcall function_1aa750(long arg_0, s_slot *arg_1, bool arg_2)
{
	bool local_11;
	s_actor_view *local_0 = actor_get(arg_0);
	s_1aa0d1 *local_1 = (s_1aa0d1 *)function_1e4f90(arg_0);
	s_1aa751 *local_3 = (s_1aa751 *)((byte *)arg_1 + 0xc);
	s_ai_object *local_2 = ai_object_get(local_0->unknown018);
	local_11 = false;
	if (!((local_2->flags19 & 1) && ((local_2->flags19 & 2) || !(*(byte *)local_1 & 1))))
	{
		short local_4 = 0;
		short local_5 = 0;
		real local_6 = 0.0f;
		real local_7 = 0.0f;
		memset(local_3, 0, 0x30);
		local_3->field_0 = g_510c54->game_time;
		local_3->field_4 = NONE;
		local_3->field_a = (local_2->flags19 & 1) != 0;
		local_3->field_26 = 0;
		local_3->field_2c = 0.0f;
		if (local_1)
		{
			local_3->field_28 = local_1->field_c;
			long unit_index = local_0->unknown018;
			long local_8 = arg_2 ? 0xa000040 : local_3->field_a ? 0xe00067d : 0x500000a;
			if (unit_animation_get_timing(unit_index, 0x7000101, local_8, &local_4, &local_6, &local_5, &local_7))
			{
				short local_16 = local_4;
				if (local_16 == NONE)
				{
					if (arg_2)
					{
						local_6 = 0.0f;
						local_16 = 0;
					}
					else
					{
						local_16 = local_5 / 2;
						local_6 = local_7 * 0.5f;
					}
				}
				local_3->field_24 = local_16;
			}
			else
				local_3->field_24 = 0;
			local_3->field_9 = 0;
			if (local_0->prop_index != NONE)
				function_1fb7e0(0x1f, arg_0, NULL, prop_node_get(local_0->prop_index)->object_index, NONE);
			if (!arg_2)
			{
				s_1aa750 *local_9 = (s_1aa750 *)function_1e4a50(local_0->unknown054);
				real local_10 = 0.0f;
				if (local_9)
					local_10 = local_9->field_4 + 0.2f;
				local_10 += local_6;
				local_3->field_28 = local_3->field_28 > local_10 ? local_3->field_28 : local_10;
				local_8 = local_3->field_a ? 0xe00067d : 0xc0006cd;
				real local_15 = 0.0f;
				short local_14;
				real local_13;
				short local_12 = 0;
				unit_index = local_0->unknown018;
				if (unit_animation_get_timing(unit_index, 0x7000101, local_8, &local_12, &local_13, &local_14, &local_15))
				{
					local_3->field_26 = local_12;
					local_3->field_2c = local_15;
				}
			}
			local_11 = true;
		}
	}
	return local_11;
}
