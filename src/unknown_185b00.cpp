// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_185B00.CPP: the per player control state (g_4ed284, allocated by
   185ab0): flags, then four entries of 0x94 bytes, one per local player,
   holding the unit, its facing and the pitch bounds */

#include "unknown_11c920.h"
#include "globals.h"
#include <math.h>
#include <string.h>

#define k_maximum_local_players 4

struct yaw_pitch2f
{
	real yaw;
	real pitch;
};

/* 0x44 of an entry (0x24 bytes) */
struct s_player_control_aim
{
	long value44;
	long value48;
	long player_index;
	byte unknown50[0xc];
	short value5c;
	byte unknown5e[2];
	real value60;
	real value64;
};

struct s_player_control_target
{
	long a;
	long b;
	long c;
};

struct s_player_control_entry
{
	long unit_index;
	byte unknown04[8];
	short value0c;
	short value0e;
	yaw_pitch2f facing;
	byte unknown18[0x10];
	short value28;
	char value2a;
	char value2b;
	short value2c;
	short value2e;
	byte value30;
	byte unknown31[0x13];
	s_player_control_aim aim;
	byte unknown68[4];
	s_player_control_target target;
	byte unknown78[8];
	real minimum_pitch;
	real maximum_pitch;
	byte value88;
	byte value89;
	byte unknown8a[0xa];
};

struct s_player_control_globals
{
	bool initialized;
	byte unknown01[3];
	dword flags4;
	dword flags8;
	dword flagsc;
	dword flags10;
	s_player_control_entry entries[k_maximum_local_players];
};

struct s_unknown_185ab0;
extern s_unknown_185ab0 *g_4ed284;

struct s_player_datum
{
	short salt;
	byte unknown02[0x2a];
	long unit_index;
};

struct s_control_object
{
	byte unknown00[0x150];
	vector3f forward;
	byte unknown15c[0x214 - 0x15c];
	byte value214[4];
	byte unknown218[0x23d - 0x218];
	char value23d;
	byte unknown23e[3];
	char value241;
};

struct s_object_header
{
	byte unknown00[8];
	s_control_object *object;
};

static inline s_player_control_globals *player_control_globals(void)
{
	return (s_player_control_globals *)g_4ed284;
}

static inline byte *record_pool_lookup_checked(s_record_pool *data, long datum_index)
{
	byte *result = 0;

	if (datum_index != NONE)
	{
		long index = datum_index & 0xffff;

		if (index < data->high_water_index)
		{
			byte *datum = data->data + data->size * index;
			short salt = *(short *)datum;

			if (salt != 0 && salt == (datum_index >> 16))
			{
				result = datum;
			}
		}
	}

	return result;
}

static inline void function_x9a60c1(yaw_pitch2f *angles, vector3f const *vector)
{
	angles->yaw = (real)atan2(vector->j, vector->i);
	angles->pitch = (real)atan2(vector->k, sqrt(vector->i * vector->i + vector->j * vector->j));
}

static __forceinline void player_control_entry_set_facing(s_player_control_entry *entry, vector3f const *forward)
{
	function_x9a60c1(&entry->facing, forward);
	if (entry->facing.yaw < 0.0f)
	{
		entry->facing.yaw += 6.2831855f;
	}
}

// @retail 0x1874b0
void function_1874b0(long player_index, vector3f const *forward)
{
	player_control_entry_set_facing(&player_control_globals()->entries[player_index], forward);
}

static inline s_player_control_entry *function_x523cb6(long player_index)
{
	return &player_control_globals()->entries[player_index];
}

static inline void player_control_aim_reset(s_player_control_aim *aim)
{
	aim->value60 = 0.0f;
	aim->value64 = 0.0f;
	aim->value44 = NONE;
	aim->value48 = NONE;
	aim->player_index = NONE;
	aim->value5c = 0;
}

static inline void player_control_target_reset(s_player_control_target *target)
{
	memset(target, 0, 8);
	target->b = NONE;
}

static inline void player_control_target_new(s_player_control_target *target)
{
	memset(target, 0, sizeof(*target));
	player_control_target_reset(target);
	target->c = NONE;
}

// @retail 0x1872d0
void player_control_set_unit(long player_index, long unit_index)
{
	if (player_index != NONE)
	{
		s_player_control_entry *entry = function_x523cb6(player_index);

		if (!player_control_globals()->initialized || entry->unit_index != unit_index)
		{
			memset(entry, 0, sizeof(*entry));
			entry->maximum_pitch = 1.4922565f;
			entry->value89 = 0;
			entry->value88 = 0;
			entry->value30 = 0;
			entry->minimum_pitch = -1.4922565f;
			entry->value0c = 0;
			entry->value0e = 0;
			entry->unit_index = unit_index;
			entry->value28 = NONE;
			entry->value2a = NONE;
			entry->value2b = NONE;
			entry->value2c = NONE;
			entry->value2e = NONE;
			player_control_aim_reset(&entry->aim);
			player_control_target_new(&entry->target);

			if (unit_index != NONE)
			{
				s_control_object *object = ((s_object_header *)g_4e0300->data)[unit_index & 0xffff].object;

				player_control_entry_set_facing(entry, &object->forward);
				*(long *)&entry->value28 = *(long *)object->value214;
				entry->value2c = object->value23d;
				entry->value2e = object->value241;
			}
		}
	}
}

/* a unit's seat in its tag (0xb0 bytes) */
struct s_player_control_seat
{
	dword : 2;
	dword flag2 : 1;
	dword : 1;
	dword flag4 : 1;
	dword : 27;
	byte unknown04[0x60 - 4];
	byte camera[0xb0 - 0x60];
};

/* a unit's tag */
struct s_player_control_unit_definition
{
	byte unknown00[0xd4];
	byte camera[0x1c8 - 0xd4];
	long seat_count;
	s_player_control_seat *seats;
};

struct s_player_control_unit
{
	long definition_index;
	byte unknown04[0x14 - 4];
	long parent_index;
	byte unknown18[0x1fc - 0x18];
	short seat_index;
};

/* the unit a player controls, the seat it sits in, the camera definition
   it looks through and the point it looks from */
struct s_player_control_camera
{
	long unit_index;
	short seat_index;
	byte unknown06[2];
	void *camera;
	point3f position;
};

#define PLAYER_CONTROL_UNIT(index) ((s_player_control_unit *)((s_object_header *)g_4e0300->data)[(index) & 0xffff].object)
#define PLAYER_CONTROL_UNIT_DEFINITION(index) ((s_player_control_unit_definition *)g_4e3b44[(index) & 0xffff].data)

void function_cafc0(long unit_index, point3f *position);
point3f *function_b9ef0(long object_index, point3f *result);

// @retail 0x1871e0
void player_control_get_camera(long player_index, s_player_control_camera *camera)
{
	camera->camera = NULL;
	camera->unit_index = function_x523cb6(player_index)->unit_index;
	camera->seat_index = NONE;

	if (camera->unit_index != NONE)
	{
		s_player_control_unit *unit = PLAYER_CONTROL_UNIT(camera->unit_index);

		function_cafc0(camera->unit_index, &camera->position);
		if (unit->seat_index != NONE)
		{
			s_player_control_unit *parent = PLAYER_CONTROL_UNIT(unit->parent_index);
			s_player_control_seat *seat = &PLAYER_CONTROL_UNIT_DEFINITION(parent->definition_index)->seats[unit->seat_index];

			camera->unit_index = unit->parent_index;
			camera->camera = seat->camera;
			camera->seat_index = unit->seat_index;
			if (TEST_FIELD_BIT(seat->flag4) && TEST_FIELD_BIT(seat->flag2))
			{
				function_b9ef0(camera->unit_index, &camera->position);
			}
		}
		else
		{
			camera->camera = PLAYER_CONTROL_UNIT_DEFINITION(unit->definition_index)->camera;
		}
	}
}

/* a unit's tag, as the field of view reads it */
struct s_player_control_fov_definition
{
	byte unknown00[0xcc];
	real field_of_view;
};

extern real g_54e854;
real function_c8880(long unit_index, short field_240); /* unknown_0c8880.cpp */

/* the field of view of a player's unit at its zoom level */
struct s_187131
{
	real field_0;
	real field_4;
};

// @retail 0x187130
real function_187130(long player_index)
{
	s_player_control_entry *entry = function_x523cb6(player_index);
	s_187131 local_0;
	local_0.field_4 = g_54e854;

	if (entry->unit_index != NONE)
	{
		short field_240 = entry->value2e;
		s_player_control_unit *unit = PLAYER_CONTROL_UNIT(entry->unit_index);
		local_0.field_0 = ((s_player_control_fov_definition *)g_4e3b44[unit->definition_index & 0xffff].data)->field_of_view;
		real magnification;

		local_0.field_4 = local_0.field_0;
		magnification = function_c8880(entry->unit_index, field_240);
		if (magnification != 1.0f)
		{
			real zoomed = local_0.field_0 / magnification;

			if (zoomed > 0.031415928f && 3.1101768f > zoomed)
			{
				local_0.field_4 = zoomed;
			}
		}
	}
	return local_0.field_4;
}

// @retail 0x185b00
void function_185b00(void)
{
	s_player_control_globals *globals = player_control_globals();

	globals->flags8 = 0;
	globals->flags4 = 0;
	globals->flagsc = 0;
	globals->flags10 = 0;
	for (long i = 0; i < k_maximum_local_players; i++)
	{
		player_control_set_unit(i, NONE);
	}
	globals->initialized = true;
}

// @retail 0x185b30
real function_185b30(short count, real const *values, real t)
{
	bool negative = t < 0.0f;
	long last = count - 1;
	real position = (real)(fabs(t) * last);

	if (0.0f > position)
	{
		position = 0.0f;
	}
	else if (position > (real)count - 1.0f)
	{
		position = (real)count - 1.0f;
	}

	short index = (short)position;
	index = (short)(index < 0 ? 0 : (index > last ? last : index));

	short next = (short)(index + 1 <= last ? index + 1 : last);
	real result = (values[next] - values[index]) * (position - index) + values[index];

	if (negative)
	{
		result = 0.0f - result;
	}
	return result;
}

// @retail 0x187420
real function_187420(long player_index)
{
	s_player_control_globals *globals = player_control_globals();

	return globals->entries[player_index].aim.value60 > globals->entries[player_index].aim.value64 ? globals->entries[player_index].aim.value60 : globals->entries[player_index].aim.value64;
}

// @retail 0x187450
long function_187450(long player_index)
{
	s_player_control_entry *entry = &player_control_globals()->entries[player_index];
	__assume(entry != NULL);
	long result = NONE;
	long datum_index = entry->aim.player_index;

	if (datum_index != NONE)
	{
		s_player_datum *player = (s_player_datum *)record_pool_lookup_checked(g_4e8c24, datum_index);
		if (player && player->unit_index != NONE)
		{
			result = datum_index;
		}
	}
	return result;
}

// @retail 0x187a40
short function_187a40(long player_index)
{
	short result = NONE;

	if (player_index != NONE)
	{
		result = player_control_globals()->entries[player_index].value2e;
	}
	return result;
}

// @retail 0x187a60
void function_187a60(long datum_index)
{
	for (long i = 0; i < k_maximum_local_players; i++)
	{
		s_player_control_entry *entry = &player_control_globals()->entries[i];

		if (entry->aim.value44 == datum_index)
		{
			entry->aim.value60 = 0.0f;
			entry->aim.value64 = 0.0f;
			entry->aim.value44 = NONE;
			entry->aim.value48 = NONE;
			entry->aim.player_index = NONE;
			entry->aim.value5c = 0;
		}
	}
}

/* the actions a player's controls ask for in a tick */
struct s_player_action_flags1c
{
	dword bit0 : 1;
	dword bit1 : 1;
	dword bit2 : 1;
	dword bit3 : 1;
	dword bit4 : 1;
	dword bit5 : 1;
	dword unknown : 26;
};

struct s_player_action_flags20
{
	word bit0 : 1;
	word bit1 : 1;
	word bit2 : 1;
	word bit3 : 1;
	word unknown : 12;
};

struct s_player_action
{
	real throttle_i;
	real throttle_j;
	real trigger;
	byte unknown0c[4];
	real pitch;
	real yaw;
	dword flags18;
	union
	{
		dword flags1c;
		s_player_action_flags1c bits1c;
	};
	union
	{
		word flags20;
		s_player_action_flags20 bits20;
	};
	byte unknown22[2];
	real zoom;
};

struct s_player_action_triggers
{
	byte unknown00[8];
	byte left;
	byte right;
};

#define FLAG(bit) (1 << (bit))
#define SET_FLAG(flags, bit, value) ((value) ? ((flags) |= FLAG(bit)) : ((flags) &= ~FLAG(bit)))

// @retail 0x187b30
void player_control_update_action_flags(s_player_action *action, s_player_action_triggers const *triggers)
{
	if (action->flags20 & FLAG(2))
	{
		g_4ed284->flags4 |= FLAG(0);
	}
	if (action->flags18 & FLAG(1))
	{
		g_4ed284->flags4 |= FLAG(1);
	}
	if (action->flags18 & FLAG(26))
	{
		g_4ed284->flags4 |= FLAG(5);
	}
	if (action->flags18 & FLAG(2))
	{
		g_4ed284->flags4 |= FLAG(20);
	}
	if (action->flags18 & FLAG(5))
	{
		g_4ed284->flags4 |= FLAG(6);
	}
	if (action->flags1c & FLAG(4))
	{
		g_4ed284->flags4 |= FLAG(2);
	}
	if (action->flags1c & FLAG(5))
	{
		g_4ed284->flags4 |= FLAG(3);
	}
	if (action->flags1c & FLAG(2))
	{
		g_4ed284->flags4 |= FLAG(9);
	}
	if (action->flags1c & FLAG(0))
	{
		g_4ed284->flags4 |= FLAG(7);
	}
	if (action->flags1c & FLAG(1))
	{
		g_4ed284->flags4 |= FLAG(8);
	}
	if (action->trigger > 0.0f)
	{
		g_4ed284->flags4 |= FLAG(4);
	}
	if (triggers->left > 0)
	{
		g_4ed284->flags4 |= FLAG(18);
	}
	if (triggers->right > 0)
	{
		g_4ed284->flags4 |= FLAG(19);
	}
	if (action->yaw > 0.0f)
	{
		g_4ed284->flags4 |= FLAG(10);
	}
	else if (action->yaw < 0.0f)
	{
		g_4ed284->flags4 |= FLAG(11);
	}
	if (action->pitch > 0.0f)
	{
		g_4ed284->flags4 |= FLAG(12);
	}
	else if (action->pitch < 0.0f)
	{
		g_4ed284->flags4 |= FLAG(13);
	}
	if (action->throttle_i > 0.0f)
	{
		g_4ed284->flags4 |= FLAG(14);
	}
	else if (action->throttle_i < 0.0f)
	{
		g_4ed284->flags4 |= FLAG(15);
	}
	if (action->throttle_j > 0.0f)
	{
		g_4ed284->flags4 |= FLAG(16);
	}
	else if (action->throttle_j < 0.0f)
	{
		g_4ed284->flags4 |= FLAG(17);
	}
	if (action->zoom > 0.0f)
	{
		g_4ed284->flags4 |= FLAG(23);
	}
	else if (action->zoom < 0.0f)
	{
		g_4ed284->flags4 |= FLAG(24);
	}

	s_unknown_185ab0 *globals = g_4ed284;
	if (TEST_FIELD_BIT(globals->bits8.bit0))
	{
		action->flags20 = (action->flags20 & ~(FLAG(1) | FLAG(2))) | FLAG(3);
	}
	else if (TEST_FIELD_BIT(g_4ed284->bitsc.bit0))
	{
		SET_FLAG(globals->flagsc, 0, TEST_FIELD_BIT(action->bits20.bit2));
		action->flags20 = (action->flags20 & ~(FLAG(1) | FLAG(2))) | FLAG(3);
	}

	if (TEST_FIELD_BIT(globals->bits8.bit2))
	{
		action->flags1c &= ~FLAG(4);
	}
	else if (TEST_FIELD_BIT(g_4ed284->bitsc.bit2))
	{
		SET_FLAG(globals->flagsc, 2, TEST_FIELD_BIT(action->bits1c.bit4));
		action->flags1c &= ~FLAG(4);
	}

	if (TEST_FIELD_BIT(globals->bits8.bit3))
	{
		action->flags1c &= ~FLAG(5);
	}
	else if (TEST_FIELD_BIT(g_4ed284->bitsc.bit3))
	{
		SET_FLAG(globals->flagsc, 3, TEST_FIELD_BIT(action->bits1c.bit5));
		action->flags1c &= ~FLAG(5);
	}

	s_185ab0_flags bits = globals->bits4;
	if (TEST_FIELD_BIT(bits.bit21))
	{
		action->yaw = (real)fabs(action->yaw);
	}
	else if (TEST_FIELD_BIT(bits.bit22))
	{
		action->yaw = -(real)fabs(action->yaw);
	}

	if (TEST_FIELD_BIT(globals->bitsc.bit12))
	{
		action->pitch = action->pitch > 0.0f ? 0.0f : action->pitch;
	}
	if (TEST_FIELD_BIT(globals->bitsc.bit13))
	{
		action->pitch = action->pitch > 0.0f ? action->pitch : 0.0f;
	}
	if (TEST_FIELD_BIT(globals->bitsc.bit10))
	{
		action->yaw = action->yaw > 0.0f ? 0.0f : action->yaw;
	}
	if (TEST_FIELD_BIT(globals->bitsc.bit11))
	{
		action->yaw = action->yaw > 0.0f ? action->yaw : 0.0f;
	}
}

#include "object_markers.h"

extern real g_547634;
extern real g_547638;
real function_11ce20(vector3f const *a, vector3f const *b);

struct s_control_pitch_camera
{
	byte field_0[8];
	real target;
	real minimum;
	real maximum;
};

struct s_control_turn_seat
{
	byte field_0[8];
	long field_8;
	byte field_c[0x88 - 0xc];
	real minimum;
	real maximum;
	byte field_90[0xb0 - 0x90];
};

struct s_control_pitch_unit
{
	byte field_0[0x7c];
	vector3f field_7c;
	vector3f field_88;
};

PRIVATE inline real control_turn_difference(real a, real b)
{
	real difference = a - b;
	if (difference >= g_547638) difference -= g_547634;
	if (-g_547638 >= difference) difference += g_547634;
	return difference;
}

PRIVATE inline real control_pitch_pin(real value, real minimum, real maximum)
{
	return minimum > value ? minimum : (value > maximum ? maximum : value);
}

// @retail 0x187510
void __stdcall function_187510(long player_index, real yaw_delta, real pitch_delta)
{
	s_player_control_entry *entry = function_x523cb6(player_index);
	real minimum = -1.4922565f;
	real maximum = 1.4922565f;
	byte *settings = *(byte **)((byte *)g_4e034c + 0xf4);
	s_player_control_camera camera;
	player_control_get_camera(player_index, &camera);
	entry->facing.yaw += yaw_delta;
	if (camera.seat_index != NONE)
	{
		s_player_control_unit *unit = PLAYER_CONTROL_UNIT(camera.unit_index);
		s_control_turn_seat *seat = &((s_control_turn_seat *)PLAYER_CONTROL_UNIT_DEFINITION(unit->definition_index)->seats)[camera.seat_index];
		if (seat->minimum != 0.0f || seat->maximum != 0.0f)
		{
			s_object_marker marker;
			function_b8d30(camera.unit_index, seat->field_8, &marker, 1, false);
			real yaw = (real)atan2(marker.matrix.forward.j, marker.matrix.forward.i);
			real upper = seat->maximum + yaw;
			real lower = seat->minimum + yaw;
			real range = control_turn_difference(upper, lower);
			real to_upper = control_turn_difference(upper, entry->facing.yaw);
			real from_lower = control_turn_difference(entry->facing.yaw, lower);
			real positive_range = range + 6.2831855f;
			if (!(range < 0.0f)) positive_range = range;
			if (!(to_upper >= 0.0f && positive_range > to_upper) &&
				!(from_lower >= 0.0f && positive_range > from_lower))
			{
				if (fabs(to_upper) > fabs(from_lower))
					entry->facing.yaw = lower;
				else
					entry->facing.yaw = upper;
			}
		}
	}
	while (entry->facing.yaw < 0.0f) entry->facing.yaw += 6.2831855f;
	while (entry->facing.yaw > 6.2831855f) entry->facing.yaw -= 6.2831855f;
	if (camera.camera)
	{
		s_control_pitch_camera *definition = (s_control_pitch_camera *)camera.camera;
		s_control_pitch_unit *unit = (s_control_pitch_unit *)PLAYER_CONTROL_UNIT(camera.unit_index);
		real target = definition->target;
		if (definition->maximum != 0.0f || definition->minimum != 0.0f)
		{
			minimum = definition->minimum;
			maximum = definition->maximum;
			if (camera.seat_index != NONE && unit->field_7c.k > 0.2f)
			{
				vector3f direction;
				direction.i = (real)(cos(entry->facing.yaw) * cos(0.0));
				direction.j = (real)(sin(entry->facing.yaw) * cos(0.0));
				direction.k = (real)sin(0.0);
				real angle = 1.5707964f - function_11ce20(&unit->field_7c, &direction);
				minimum -= angle;
				maximum -= angle;
				target -= angle;
			}
			minimum = control_pitch_pin(minimum, -1.4922565f, 1.4922565f);
			maximum = control_pitch_pin(maximum, -1.4922565f, 1.4922565f);
		}
		if (target != 0.0f || entry->value30)
		{
			real amount = (real)fabs(entry->facing.pitch - target) * 0.63661975f;
			real delta = target - entry->facing.pitch;
			real movement = (real)sqrt(unit->field_88.i * unit->field_88.i + unit->field_88.j * unit->field_88.j + unit->field_88.k * unit->field_88.k) * g_510c54->rate;
			real rate = target != 0.0f ? movement * amount * 0.08f : *(real *)(settings + 0x5c) * movement * amount;
			entry->facing.pitch += control_pitch_pin(delta, -rate, rate);
		}
	}
	entry->minimum_pitch += control_pitch_pin(minimum - entry->minimum_pitch, -0.012271847f, 0.012271847f);
	entry->maximum_pitch += control_pitch_pin(maximum - entry->maximum_pitch, -0.012271847f, 0.012271847f);
	entry->facing.pitch += pitch_delta;
	entry->facing.pitch = control_pitch_pin(entry->facing.pitch, entry->minimum_pitch, entry->maximum_pitch);
}
