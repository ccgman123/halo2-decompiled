// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_218AC0.CPP: sound tag queries: the data sizes and durations of
   permutations, pitch range choice and rate limits (part of retail's
   unknown_218ac0.cpp; src/unknown_218c60.cpp and src/unknown_219110.cpp
   hold more of it) */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
#include "unknown_218ac0.h"
#include "unknown_218850.h"
#include <math.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define PIN(value, lower, upper) ((lower) > (value) ? (lower) : ((value) > (upper) ? (upper) : (value)))

/* the sound formats: a definition's sample rate, encoding (mono, stereo,
   codec) and compression index these */
const long g_44a054[3] = { 22050, 44100, 32000 };	/* samples per second */
const real g_44a060[3] = { 1.0f, 0.5f, 1.0f };	/* per encoding */
const real g_44a06c[5] = { 0.5f, 1.77777779f, 0.5f, 0.5f, 1.0f };	/* samples per byte */
const long g_44a080[5] = { 2, 36, 2, 2, 1 };	/* bytes per block */
const long g_44a094[3] = { 1, 2, 1 };	/* channels */

/* a permutation (16 bytes) */
struct s_sound_permutation
{
	byte unknown00[8];
	long sample_count;
	short first_chunk;
	short chunk_count;
};

/* the sound globals' permutation and chunk tables */
struct s_sound_globals_permutations_view
{
	byte unknown00[4];
	struct
	{
		byte unknown00[0x18];
		short pitch_lower;
		short pitch_upper;
		byte unknown1c[0x38 - 0x1c];
	} *classes;
	byte unknown08[4];
	struct
	{
		byte unknown00[8];
		short pitch_lower;
		short pitch_upper;
		byte unknown0c[8];
	} *playback_parameters;
	byte unknown10[0x2c - 0x10];
	s_sound_permutation *permutations;
	byte unknown30[0x44 - 0x30];
	s_sound_chunk *chunks;
};

#define SOUND_GLOBALS_PERMUTATIONS ((s_sound_globals_permutations_view *)g_51ebd4)

/* a pitch in cents to a playback rate */
static inline real sound_pitch_to_rate(long pitch)
{
	return (real)exp(pitch * 0.000577622675f);
}

/* the size of the first chunks of a permutation */
// @retail 0x218ac0
long sound_permutation_chunks_size(long chunk_count, s_sound_permutation const *permutation)
{
	long size = 0;

	for (long chunk_index = 0; chunk_index < chunk_count; chunk_index++)
	{
		size += SOUND_CHUNK_SIZE(&SOUND_GLOBALS_PERMUTATIONS->chunks[permutation->first_chunk + chunk_index]);
	}
	return size;
}

/* the size of the whole blocks of sound data that play for a duration */
// @retail 0x218b00
long sound_format_duration_to_bytes(long encoding, long compression, long arg_da1d74, real duration)
{
	real block_count = 1.0f / g_44a060[encoding] / g_44a06c[compression] * (real)g_44a054[arg_da1d74] * duration /
		(real)g_44a094[encoding] / (real)g_44a080[compression];

	return g_44a094[encoding] * g_44a080[compression] * real_truncate(block_count);
}

/* the duration in milliseconds of a permutation's samples, played at the
   pitch of its class and playback parameters */
struct s_218b80
{
	byte field_0[0x18];
	short field_18;
	short field_1a;
	byte field_1c[0x38 - 0x1c];
};

struct s_218b81
{
	byte field_0[8];
	short field_8;
	short field_a;
	byte field_c[8];
};

// @retail 0x218b80
real sound_permutation_duration(s_sound_definition const *definition, s_sound_pitch_range const *arg_58ecd0, long sample_count)
{
	s_sound_globals_permutations_view *globals = SOUND_GLOBALS_PERMUTATIONS;
	s_218b80 const *local_1 = (s_218b80 const *)&globals->classes[definition->class_index];
	s_218b81 const *local_2 = (s_218b81 const *)&globals->playback_parameters[definition->playback_index];
	long natural_pitch = SOUND_GLOBALS_DEFINITIONS->pitch_bounds[arg_58ecd0->bounds_index].unknown00;
	short local_3 = local_1->field_18;
	long pitch = (short)(local_3 > local_1->field_1a ? local_1->field_1a : local_3) +
		(short)(local_2->field_8 > local_2->field_a ? local_2->field_a : local_2->field_8);
	real rate = sound_pitch_to_rate(natural_pitch);
	return rate * g_44a06c[(char)definition->format] * g_44a060[(char)definition->type] * sample_count * 1000.0f /
		(g_44a054[(char)definition->unknown03] * sound_pitch_to_rate(pitch));
}

/* the duration in milliseconds of a permutation played at a pitch */
// @retail 0x218ca0
real sound_definition_permutation_duration(long definition_index, long pitch_range_index, long permutation_index, real pitch)
{
	s_sound_definition *definition = sound_definition_get(definition_index);
	s_sound_pitch_range *selected_range = &SOUND_GLOBALS_DEFINITIONS->pitch_ranges[definition->pitch_range_base + pitch_range_index];

	return sound_permutation_duration(definition, selected_range,
		SOUND_GLOBALS_PERMUTATIONS->permutations[selected_range->first_permutation + permutation_index].sample_count) *
		(real)exp((pitch - SOUND_GLOBALS_DEFINITIONS->pitch_bounds[selected_range->bounds_index].unknown00) * 0.000577622675f);
}

/* the pitch range of a rate limit's stage, or 0 */
// @retail 0x218f10
short sound_definition_rate_limit_pitch_range(long stage_index, s_sound_definition const *definition)
{
	short result = 0;
	s_sound_rate_limit *rate_limit = sound_rate_limit_get(definition->rate_limit_index);

	if (rate_limit)
	{
		s_sound_rate_limit_stage *stages = rate_limit->stages;
		short *pitch_range_index = &stages[stage_index].pitch_range_index;

		if (*pitch_range_index != NONE)
		{
			result = *pitch_range_index;
		}
	}
	return result;
}

struct s_looping_playback_definition;

struct s_short_bounds
{
	short lower;
	short upper;
};

/* a pitch range's bounds in cents (10 bytes) */
struct s_sound_pitch_bounds_view
{
	short natural_pitch;
	s_short_bounds natural;
	s_short_bounds playback;
};

static inline s_sound_pitch_range *sound_definition_pitch_range_get(s_sound_definition const *definition, long pitch_range_index)
{
	return &SOUND_GLOBALS_DEFINITIONS->pitch_ranges[definition->pitch_range_base + pitch_range_index];
}

static inline s_sound_pitch_bounds_view *sound_pitch_bounds_get(s_sound_pitch_range const *arg_58ecd0)
{
	return (s_sound_pitch_bounds_view *)&SOUND_GLOBALS_DEFINITIONS->pitch_bounds[arg_58ecd0->bounds_index];
}

static inline bool sound_bounds_contain(s_short_bounds const *bounds, real value)
{
	return PIN(value, (real)bounds->lower, (real)bounds->upper) == value;
}

/* the pitch range a pitch plays in: the previous one while the pitch stays
   in its playback bounds, else the first whose bounds hold the pitch, else
   the nearest */
// @retail 0x218f50
short function_218f50(s_looping_playback_definition *playback_definition, short previous, real pitch)
{
	long local_0 = *(long const *)&previous;
	s_sound_definition *definition = (s_sound_definition *)playback_definition;
	long count = definition->pitch_range_count;
	long best_index = NONE;

	if ((short)local_0 != NONE && (short)local_0 < count)
	{
		s_sound_pitch_range *arg_58ecd0 = sound_definition_pitch_range_get(definition, (short)local_0);

		if (sound_bounds_contain(&sound_pitch_bounds_get(arg_58ecd0)->playback, pitch) && arg_58ecd0->permutation_count > 0)
		{
			return (short)local_0;
		}
	}

	real best_distance = 3.40282347e+38f;
	for (long index = 0; (short)index < count; index++)
	{
		s_sound_pitch_range *arg_58ecd0 = sound_definition_pitch_range_get(definition, (short)index);

		if (arg_58ecd0->permutation_count > 0)
		{
			s_sound_pitch_bounds_view *bounds = sound_pitch_bounds_get(arg_58ecd0);
			real distance;

			if (sound_bounds_contain(&bounds->playback, pitch))
			{
				return index;
			}

			if (pitch > (real)bounds->natural.upper)
				distance = pitch - (real)bounds->natural.upper;
			else
				distance = (real)bounds->natural.lower - pitch;

			if (distance < best_distance)
			{
				best_index = index;
				best_distance = distance;
			}
		}
	}
	return (short)best_index;
}
