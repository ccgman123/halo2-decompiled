// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_218E50.CPP: a sample of a sound permutation's promotion data.
   Decompiled by lane F for 0x18c720. */

#include "unknown_11c920.h"
#include "globals.h"
#include "sound_promotions.h"

/* the sound's permutation set with the given index */
static inline s_sound_permutation_set *sound_permutation_set_get(s_sound_promotion_tag const *sound, short set_index)
{
	return &((s_sound_globals_promotion_view *)g_51ebd4)->sets[sound->permutation_base + set_index];
}

// @retail 0x218e50
// score 40 (was 40): the argument is read into BP where retail reads it into EBP, so the values are
// kept in the 16 bit registers instead of the 32 bit ones.
real function_218e50(long tag_index, short set_index, short permutation_index, short sample_index)
{
	s_sound_promotion_tag *sound = (s_sound_promotion_tag *)g_4e3b44[tag_index & 0xffff].bytes;
	s_sound_permutation_set *set = sound_permutation_set_get(sound, set_index);
	s_sound_globals_promotion_view *globals = (s_sound_globals_promotion_view *)g_51ebd4;
	long valid_index;

	if (permutation_index < 0)
	{
		valid_index = 0;
	}
	else
	{
		long last_index = set->permutation_count - 1;
		valid_index = permutation_index <= last_index ? permutation_index : last_index;
	}

	if (valid_index == permutation_index)
	{
		s_sound_promotion_data *data = globals->promotions[sound->promotion_index].data;
		s_sound_promotion_entry *entry = &data->entries[globals->permutations[set->first_permutation + permutation_index].entry_index];
		long count = entry->sample_count;

		if (count > 0)
		{
			if (sample_index < 0)
				count = 0;
			else
			{
				--count;
				if (sample_index <= count)
					count = sample_index;
			}
			return data->samples[(short)count + entry->sample_offset] * (1.0f / 255.0f);
		}
	}
	return 0.0f;
}
