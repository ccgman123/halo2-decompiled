// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_18A2F0.CPP: object looping sound queries: the time left on a
   slot of g_4ed288, and the flags of the looping sound data (g_4ed28c) */

#include "unknown_11c920.h"
#include "globals.h"

extern "C" void _ReadWriteBarrier(void);

struct s_looping_sound
{
	short salt;
	byte value2;
	char type;
	union
	{
		word flags;
		struct
		{
			word bit0 : 1;
			word bit1 : 1;
			word bit2 : 1;
			word bit3 : 1;
			word bit4 : 1;
			word bit5 : 1;
			word bit6 : 1;
			word bit7 : 1;
			word bit8 : 1;
		} flag_bits;
	};
	short value6;
	real value8;
	long tag_index;
	long value10;
	union
	{
		long value14;
		struct
		{
			char byte14;
			char byte15;
			short short16;
		};
	};
};

struct s_looping_sound_definition
{
	byte unknown00[4];
	real duration;
};

static __forceinline long float_to_int_nearest(real value)
{
	long result;

	__asm
	{
		fld value
		fistp result
	}
	return result;
}

static inline s_looping_sound *looping_sound_get(long datum_index)
{
	return (s_looping_sound *)g_4ed28c->data + (datum_index & 0xffff);
}

static inline long looping_sound_slot_find(long datum_index)
{
	long i = 0;
	s_looping_sound_slot *slot = g_4ed288->slots;

	for (; i < 16; i++, slot++)
	{
		if (slot->datum_index == datum_index)
		{
			return i;
		}
	}
	return NONE;
}

real sound_permutation_reference_duration(long definition_index, s_sound_permutation_reference const *reference); /* unknown_20b3c0.cpp */

void function_126360(long sound_index); /* unknown_124f90.cpp */
void sound_choose_permutation(long definition_index, s_sound_permutation_reference *reference, bool *all_used); /* unknown_20b3c0.cpp */

/* takes the slot of g_4ed288 that plays a sound (stopping what it plays), or
   a free one, and chooses the permutation it plays next */
// @retail 0x189ee0
long function_189ee0(long tag_index)
{
	long result = NONE;
	long free_index = NONE;

	for (long i = 0; i < 16; i++)
	{
		s_looping_sound_slot *slot = &g_4ed288->slots[i];

		if (slot->datum_index == tag_index)
		{
			if (!slot->active)
			{
				function_126360(slot->source_index);
			}
			result = i;
		}
		else if (free_index == NONE)
		{
			if (slot->datum_index == NONE)
			{
				free_index = i;
			}
			else
			{
				real duration = sound_permutation_reference_duration(slot->datum_index, &slot->permutation);
				long ticks = float_to_int_nearest((real)g_510c54->field_2_3 * duration);

				if (slot->end_time + ticks <= g_510c54->game_time)
				{
					free_index = i;
				}
			}
		}
	}

	if (result == NONE)
	{
		if (free_index == NONE)
		{
			goto done;
		}
		result = free_index;
	}

	{
		s_looping_sound_slot *slot = &g_4ed288->slots[result];
		sound_choose_permutation(tag_index, &slot->permutation, NULL);
		slot->end_time = g_510c54->game_time;
		slot->datum_index = tag_index;
		slot->active = false;
	}
done:
	return result;
}

/* the time left on the slot playing a sound, in thirtieths of a second */
// @retail 0x18a240
long function_18a240(long tag_index)
{
	real result = 0.0f;

	if (tag_index != NONE)
	{
		long index = looping_sound_slot_find(tag_index);
		if (index != NONE)
		{
			s_looping_sound_slot *slot = &g_4ed288->slots[index];
			real duration = sound_permutation_reference_duration(tag_index, &slot->permutation);
			s_game_time_globals *game_time = g_510c54;
			long ticks = float_to_int_nearest((real)game_time->field_2_3 * duration);

			result = (real)(slot->end_time - game_time->game_time + ticks) * game_time->rate;
			result = result > 0.0f ? result : 0.0f;
		}
	}
	return float_to_int_nearest(result * 30.0f);
}

// @retail 0x18a2f0
long function_18a2f0(long datum_index, long seconds)
{
	long result = 0;

	if (datum_index != NONE)
	{
		long index = looping_sound_slot_find(datum_index);
		if (index != NONE)
		{
			s_game_time_globals *game_time = g_510c54;
			real scaled = (real)seconds * (1.0f / 30.0f);
			long ticks = float_to_int_nearest(scaled * (real)game_time->field_2_3);
			long remaining = g_4ed288->slots[index].end_time - game_time->game_time + ticks;
			result = remaining > 0 ? remaining : 0;
		}
	}
	return result;
}

void function_126360(long sound_index); /* unknown_124f90.cpp */

/* stops the sound a slot plays for a looping sound and frees the slot */
// @retail 0x18a380
void function_18a380(long datum_index)
{
	if (datum_index != NONE)
	{
		long index = looping_sound_slot_find(datum_index);
		if (index != NONE)
		{
			s_looping_sound_slot *slot = &g_4ed288->slots[index];
			function_126360(slot->source_index);
			slot->source_index = NONE;
			slot->end_time = NONE;
			slot->datum_index = NONE;
		}
	}
}

// @retail 0x18a5e0
word *function_18a5e0(long datum_index)
{
	word *flags = &looping_sound_get(datum_index)->flags;

	*flags |= 2;
	return flags;
}

// @retail 0x18a720
void function_18a720(long datum_index, bool set)
{
	word *flags = &looping_sound_get(datum_index)->flags;

	if (set)
	{
		*flags |= 0x10;
	}
	else
	{
		*flags &= ~0x10;
	}
}

// @retail 0x18ae50
long __stdcall function_18ae50(long a, long b, real const *values)
{
	if (values[a] > values[b])
	{
		return 1;
	}
	return 0;
}

#pragma intrinsic(_ReadWriteBarrier)
// @retail 0x188f90
long looping_sound_new_attached(long tag_index, long object_index, short marker_index, long value)
{
	long datum_index = NONE;

	if (tag_index != NONE)
	{
		datum_index = record_pool_allocate(g_4ed28c);
		if (datum_index != NONE)
		{
			s_looping_sound *sound = looping_sound_get(datum_index);

			sound->tag_index = tag_index;
			_ReadWriteBarrier();
			sound->value2 = 3;
			sound->flags = 0;
			_ReadWriteBarrier();
			sound->flag_bits.bit8 = true;
			if (object_index != NONE)
			{
				*(char volatile *)&sound->type = 1;
				*(long volatile *)&sound->value14 = value;
				sound->value10 = object_index;
				sound->value6 = marker_index;
			}
			else
			{
				sound->value6 = NONE;
				sound->type = 0;
				sound->value10 = 0;
				sound->byte14 = NONE;
				sound->short16 = NONE;
			}
		}
	}
	return datum_index;
}
#pragma function(_ReadWriteBarrier)

#pragma auto_inline(off)
// @retail 0x18a5a0
long function_18a5a0(long tag_index, long object_index, real value)
{
	long datum_index = NONE;

	if (tag_index != NONE)
	{
		datum_index = looping_sound_new_attached(tag_index, object_index, NONE, 0);
		if (datum_index != NONE)
		{
			s_looping_sound *sound = looping_sound_get(datum_index);
			sound->flag_bits.bit0 = true;
			sound->value8 = value;
		}
	}
	return datum_index;
}

#pragma auto_inline(on)

#pragma intrinsic(_ReadWriteBarrier)
// @retail 0x18a600
long function_18a600(long tag_index, real value)
{
	long datum_index = NONE;

	if (tag_index != NONE)
	{
		datum_index = record_pool_allocate(g_4ed28c);
		if (datum_index != NONE)
		{
			s_looping_sound_definition *definition = (s_looping_sound_definition *)g_4e3b44[tag_index & 0xffff].bytes;
			s_looping_sound *sound = looping_sound_get(datum_index);

			sound->type = 4;
			sound->value2 = 3;
			_ReadWriteBarrier();
			sound->flags = 0;
			_ReadWriteBarrier();
			sound->flag_bits.bit8 = true;
			sound->value8 = value;
			sound->tag_index = tag_index;
			sound->value6 = NONE;
			_ReadWriteBarrier();
			sound->flag_bits.bit0 = true;
			if (definition->duration > 0.0f)
			{
				s_game_time_globals *local_0 = g_510c54;
				long local_1 = *(long const volatile *)&local_0->game_time;
				sound->value10 = local_1 + float_to_int_nearest((real)g_510c54->field_2_3 * definition->duration);
			}
			else
			{
				sound->value10 = NONE;
			}
		}
	}
	return datum_index;
}
#pragma function(_ReadWriteBarrier)

#pragma intrinsic(_ReadWriteBarrier)
// @retail 0x18a6c0
long function_18a6c0(long tag_index, long value)
{
	long datum_index = NONE;

	if (tag_index != NONE)
	{
		datum_index = record_pool_allocate(g_4ed28c);
		if (datum_index != NONE)
		{
			s_looping_sound *sound = looping_sound_get(datum_index);

			sound->type = 2;
			sound->value2 = 3;
			sound->value10 = value;
			sound->value8 = 0.0f;
			sound->flags = 0;
			_ReadWriteBarrier();
			sound->flag_bits.bit8 = true;
			sound->tag_index = tag_index;
			sound->value6 = NONE;
			_ReadWriteBarrier();
			sound->flag_bits.bit0 = true;
		}
	}
	return datum_index;
}
#pragma function(_ReadWriteBarrier)

#pragma intrinsic(_ReadWriteBarrier)
// @retail 0x18a750
long function_18a750(long tag_index, long value)
{
	long datum_index = NONE;

	if (tag_index != NONE)
	{
		datum_index = record_pool_allocate(g_4ed28c);
		if (datum_index != NONE)
		{
			s_looping_sound *sound = looping_sound_get(datum_index);

			sound->type = 3;
			sound->value2 = 3;
			sound->value10 = value;
			sound->value8 = 0.0f;
			_ReadWriteBarrier();
			sound->flags = 0;
			_ReadWriteBarrier();
			sound->flag_bits.bit8 = true;
			sound->tag_index = tag_index;
			sound->value6 = NONE;
			_ReadWriteBarrier();
			sound->flag_bits.bit0 = true;
		}
	}
	return datum_index;
}
#pragma function(_ReadWriteBarrier)
long __stdcall function_18d1c0(long tag_index);

/* a copy of function_18d2e0 (unknown_18d290.cpp, built /Ob1), which retail
   inlines here */
static inline void looping_sounds_stop_tag(long tag_index, long mode)
{
	if (tag_index != NONE)
	{
		long index = function_18d1c0(tag_index);
		if (index != NONE)
		{
			s_looping_sound *sound = looping_sound_get(index);
			sound->flag_bits.bit5 = false;
			looping_sound_get(index)->flag_bits.bit1 = true;

			long other_index = function_18d1c0(tag_index);
			if (other_index != NONE)
			{
				s_looping_sound *other = looping_sound_get(other_index);
				if (other->tag_index == tag_index)
				{
					other->flag_bits.bit5 = false;
				}
			}

			switch (mode)
			{
			case 1:
				sound->flag_bits.bit2 = true;
				break;
			case 2:
				sound->flag_bits.bit3 = true;
				break;
			}
		}
	}
}

// @retail 0x18a4d0
void function_18a4d0(long tag_index)
{
	looping_sounds_stop_tag(tag_index, 0);
}

// @retail 0x18a530
void function_18a530(long tag_index)
{
	looping_sounds_stop_tag(tag_index, 2);
}

struct s_sound_duration_view
{
	byte unknown00[0x10];
	long duration;
};

// @retail 0x18a1c0
long function_18a1c0(long datum_index)
{
	s_sound_duration_view *sound = (s_sound_duration_view *)g_4e3b44[datum_index & 0xffff].bytes;
	real duration = (real)sound->duration * 0.001f;
	long seconds = float_to_int_nearest(duration * 30.0f);
	long ticks = function_18a2f0(datum_index, seconds);

	return float_to_int_nearest((real)ticks * g_510c54->rate * 30.0f);
}

struct s_unknown_13bf00;
extern s_unknown_13bf00 *g_510c50;

struct s_510c50_looping_view
{
	byte unknown00[5];
	bool enabled;
};

void function_18d3d0(void);
bool function_18d360(long tag_index);
void function_18d290(long looping_sound_index, long tag_index);

struct s_looping_sound_tag_flags
{
	byte flags;
};

// @retail 0x18a430
void __stdcall function_18a430(long tag_index, long object_index, real scale)
{
	// Retail passes all three arguments on the stack.
	long const *tag_index_pointer = &tag_index;
	long const *object_reference = &object_index;
	real const *scale_reference = &scale;
	if (*tag_index_pointer != NONE)
	{
		s_looping_sound_tag_flags *sound = (s_looping_sound_tag_flags *)g_4e3b44[tag_index & 0xffff].bytes;
		bool detached;
		long datum_index;

		function_18a4d0(tag_index);
		if (sound->flags & 4)
		{
			function_18d3d0();
		}
		detached = function_18d360(tag_index);
		if (detached)
		{
			datum_index = function_18a600(*tag_index_pointer, *scale_reference);
		}
		else
		{
			datum_index = function_18a5a0(*tag_index_pointer, *object_reference, *scale_reference);
		}
		if (datum_index != NONE)
		{
			function_18d290(datum_index, tag_index);
			if (detached && g_510c50 && ((s_510c50_looping_view *)g_510c50)->enabled)
			{
				looping_sound_get(datum_index)->flags |= 0x40;
			}
		}
	}
}

#include "sound_sources.h"
#include "object_queries.h"

bool __stdcall function_bab40(long object_index, long name, real *value);
void function_18cbc0(long looping_sound_index, s_type_99c531 *location);
bool function_18d670(point3f const *point, real radius);
real function_18d6e0();
bool function_21a250(long definition_index, dword sound_flags, long identifier, long controller_definition_index,
	s_type_99c531 *source, long state, dword flags, real fade_duration);
void function_219c30(long definition_index, s_type_99c531 const *source);
point3f *function_b9dd0(long object_index, point3f *result);
void function_b9fc0(long object_index, vector3f *forward, vector3f *up);
dword vector3d_compress(vector3f const *vector);
long __stdcall function_18d1c0(long value);

struct s_loop_update_definition
{
	dword flags;
	byte field_4[0xc];
	real radius;
};
struct s_loop_update_object_header
{
	byte field_0[3];
	byte type;
	byte field_4[8];
};

PRIVATE inline long loop_update_owner(s_looping_sound const *sound)
{
	return sound->type == 1 ? sound->value10 : NONE;
}

// @retail 0x18a7b0
void __stdcall function_18a7b0(long datum_index)
{
	(void)&datum_index;
	s_looping_sound *sound = looping_sound_get(datum_index);
	byte *local_0 = (byte *)sound;
	s_loop_update_definition *definition = (s_loop_update_definition *)g_4e3b44[sound->tag_index & 0xffff].bytes;
	bool active;
	real scale = sound->value8;
	bool previous = TEST_FIELD_BIT(sound->flag_bits.bit8);
	if (!TEST_FIELD_BIT(sound->flag_bits.bit0))
		active = function_bab40(loop_update_owner(sound), sound->value14, &scale);
	else active = !TEST_FIELD_BIT(sound->flag_bits.bit1);
	long identifier = datum_index;
	s_looping_sound *current = looping_sound_get(datum_index);
	if (current->type == 0)
	{
		long const &local_1 = (long)current->byte14;
		if (local_1 != NONE)
			identifier = current->tag_index | 0x8000;
	}
	long controller = NONE;
	if (((definition->flags & 0x10) && sound->type == 1) || (definition->flags & 0x20))
	{
		long owner = loop_update_owner(sound);
		controller = owner != NONE ? owner : datum_index | 0x2000;
	}
	if (!active && (sound->value2 == 3 || !previous))
	{
		if (sound->value2 != 3) sound->value2 = 3;
	}
	else
	{
		dword play_flags;
		if (definition->flags & 8) play_flags = 0;
		else play_flags = 1;
		if (((1 << sound->type) & 0xe) && !(definition->flags & 0x40)) play_flags |= 2;
		else play_flags &= ~2;
		s_type_99c531 location;
		location.flags = 0;
		function_18cbc0(datum_index, &location);
		real radius = TEST_FIELD_BIT(location.flag3) ? *(real *)(location.unknown30 + 8) : definition->radius;
		dword flags;
		if (sound->flags & 0x10) flags = 1;
		else flags = 0;
		if (TEST_FIELD_BIT(sound->flag_bits.bit7)) flags |= 4;
		else flags &= ~4;
		if (sound->type == 4) flags |= 0x10;
		else flags &= ~0x10;
		if ((play_flags & 2) && !TEST_FIELD_BIT(location.flag1) && !function_18d670(&location.spatial.position, radius))
			local_0[5] &= 0xfe;
		else
		{
			if (active)
			{
				bool resume = g_510c50 && ((s_510c50_looping_view *)g_510c50)->enabled && sound->type == 1 &&
					((1 << ((s_loop_update_object_header *)g_4e0300->data)[sound->value10 & 0xffff].type) & 2);
				long state;
				if (resume || ((1 << sound->value2) & 3))
				{
					state = 1;
					sound->value2 = 1;
				}
				else
				{
					state = 0;
					sound->value2 = 0;
				}
				if (function_21a250(sound->tag_index, play_flags, identifier, controller, &location, state, flags, 0.0f))
				{
					sound->flags |= 2;
					sound->value2 = 3;
				}
				else if (controller != NONE)
				{
					s_type_99c531 fallback = location;
					if (sound->type == 1)
					{
						long owner = sound->value10;
						vector3f forward;
						function_b9dd0(owner, &fallback.spatial.position);
						function_b9fc0(owner, &forward, NULL);
						location.spatial.compressed_forward = vector3d_compress(&forward);
						function_ba1d0(loop_update_owner(sound), &fallback.spatial.velocity, NULL);
					}
					function_219c30(controller, &fallback);
				}
			}
			else
			{
				real fade = TEST_FIELD_BIT(sound->flag_bits.bit2) ? function_18d6e0() :
					(TEST_FIELD_BIT(sound->flag_bits.bit3) ? 0.01f : 0.0f);
				if (!previous || function_21a250(sound->tag_index, play_flags, identifier, controller, &location, 2, flags, fade))
					sound->value2 = 3;
				else sound->value2 = 2;
			}
			sound->flag_bits.bit8 = true;
		}
	}
	if (sound->value2 == 3 && (sound->flags & 0xe) && TEST_FIELD_BIT(sound->flag_bits.bit0))
	{
		if (function_18d1c0(sound->tag_index) == datum_index)
		{
			long tag = sound->tag_index;
			long index = function_18d1c0(tag);
			if (index != NONE)
			{
				s_looping_sound *other = looping_sound_get(index);
				if (other->tag_index == tag) other->flag_bits.bit5 = false;
			}
		}
		record_pool_release(g_4ed28c, datum_index);
	}
}
