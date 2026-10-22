// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_10AAC0.CPP: the interpolated values of g_5107f4
   (unknown_10a980.cpp): each of its 32 entries blends from one value to
   another over a time, for one object and name */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_10a980.h"


struct s_interpolated_value
{
	long name;
	real initial_value;
	real final_value;
	long start_time;
	long duration;
};

struct s_interpolated_values
{
	s_interpolated_value entries[32];
	long object_indices[32];
};

// @retail 0x10aac0
bool function_10aac0(long object_index, long name, real *value, long *index)
{
	bool result = false;

	*index = NONE;
	if (object_index != NONE)
	{
		s_interpolated_values *values = (s_interpolated_values *)g_5107f4;

		for (long i = 0; i < sizeof(values->object_indices) / sizeof(values->object_indices[0]); i++)
		{
			if (values->object_indices[i] == object_index && values->entries[i].name == name)
			{
				s_interpolated_value *entry = &values->entries[i];
				real initial_value = entry->initial_value;
				real final_value = entry->final_value;
				long duration = entry->duration;
				long elapsed = g_510c54->game_time - entry->start_time;

				if (elapsed < duration && duration)
					*value = (real)elapsed / (real)duration * (final_value - initial_value) + initial_value;
				else
					*value = final_value;
				*index = i;
				result = true;
				break;
			}
		}
	}
	return result;
}

struct s_object_10ab80
{
	byte unknown000[0xc1];
	byte unknown0c1 : 1;
	byte interpolating : 1;
	byte : 6;
};

struct s_object_header_10ab80
{
	byte unknown00[8];
	s_object_10ab80 *object;
};

static __forceinline long real_to_long(real value);
static inline long real_to_long(real value)
{
	long result;
	__asm
	{
		fld value
		fistp result
	}
	return result;
}

// score 19 (was 19): ours keeps the name argument in EDI and the g_5107f4 pointer in ESI;
// retail keeps the name in ESI and the pointer in EDI.
// @retail 0x10ab80
void function_10ab80(long object_index, long name, real value, real frames)
{
	real seconds = frames * (1.0f / 30.0f);

	if (object_index != NONE && name && name != NONE)
	{
		long index = NONE;
		real current_value;
		real initial_value = function_10aac0(object_index, name, &current_value, &index) ? current_value : value;
		s_interpolated_values *values = (s_interpolated_values *)g_5107f4;

		if (index == NONE)
		{
			long oldest_index = NONE;
			long oldest_time = 0x7fffffff;

			for (unsigned long i = 0; i < 32; i++)
			{
				if (values->object_indices[i] == NONE)
				{
					index = i;
					break;
				}
				if (values->entries[i].start_time < oldest_time)
				{
					oldest_time = values->entries[i].start_time;
					oldest_index = i;
				}
			}
			if (index == NONE)
				index = oldest_index;
		}

		s_object_10ab80 *object = ((s_object_header_10ab80 *)g_4e0300->data)[object_index & 0xffff].object;
		s_interpolated_value *entry = &values->entries[index];

		entry->name = name;
		values->object_indices[index] = object_index;
		entry->initial_value = initial_value;
		entry->final_value = value;
		entry->start_time = g_510c54->game_time;
		entry->duration = real_to_long((real)g_510c54->field_2_3 * (0.0f > seconds ? 0.0f : seconds));
		object->interpolating = true;
	}
}
