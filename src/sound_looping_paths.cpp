// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
#include <math.h>

struct s_portal_path_bsp;
struct s_14b240_owner;
struct s_bsp3d_disk;
void __stdcall function_18d730(s_portal_path_bsp *bsp, long cluster, point3f const *point,
	real range, long *indices, long *gains, long *portals, long *clusters, long *count);
real function_14b240(s_14b240_owner const *owner, s_bsp3d_disk const *disk, point3f const *point);
extern real g_44a0b4;
extern real g_4670fc;
extern long g_509420;
struct s_unknown_13bf00;
extern s_unknown_13bf00 *g_510c50;

struct s_loop_path_sound
{
	byte field_0[0x24];
	long tag;
	long field_28;
	long detail_tag;
	byte field_30[0x44 - 0x30];
	real range;
	dword flags;
	real inside;
	real portal;
	real outside;
	real fade_time;
	byte field_5c[8];
};

struct s_loop_path_bsp
{
	byte field_0[0x60];
	byte *portals;
	byte field_64[0xd8 - 0x64];
	s_loop_path_sound *sounds;
};

struct s_loop_path_choice
{
	real distance;
	real inside;
	real portal;
	real outside;
	long source;
	long tag;
	long cluster;
	real gain;
	long decibels;
	bool muted;
};

struct s_loop_path_target
{
	long tag;
	real gain;
	real step;
	long decibels;
	long priority;
	long cluster;
	long source;
	bool audible;
	bool muted;
};

struct s_loop_path_datum
{
	short salt;
	byte state;
	byte type;
	word flags;
	short field_6;
	real gain;
	long tag;
	long decibels;
	char source;
	char listener;
	short cluster;
};

struct s_loop_path_override
{
	byte field_0[0x3c];
	long tag;
	byte field_40[8];
	real suppression;
	real gain;
};

PRIVATE inline real loop_path_pin(real value, real lower, real upper)
{
	return lower > value ? lower : (value > upper ? upper : value);
}

__forceinline long loop_path_decibels(real value)
{
	if (value < g_44a0b4) return 0xc2800000;
	if (value > 0.0f) return 0;
	return *(long *)&value;
}

__forceinline real loop_path_linear(long value)
{
	long bits = loop_path_decibels(*(real *)&value);
	return (real)exp(*(real *)&bits * 0.05f * 2.3025851f);
}

__forceinline long loop_path_logarithmic(real value)
{
	real pinned = loop_path_pin(value, 0.0f, 1.0f);
	real decibels = g_44a0b4;
	if (value > 0.0f)
	{
		decibels = (real)(log10(pinned) * 20.0f);
		decibels = loop_path_pin(decibels, g_44a0b4, 0.0f);
	}
	return loop_path_decibels(decibels);
}

PRIVATE inline s_loop_path_datum *loop_path_get(s_record_pool *pool, long index)
{
	return &((s_loop_path_datum *)pool->data)[index & 0xffff];
}

__forceinline long loop_path_start(s_record_pool *pool, long tag, real gain)
{
	long index = NONE;
	if (tag != NONE)
	{
		index = record_pool_allocate(pool);
		if (index != NONE)
		{
			s_loop_path_datum *sound = loop_path_get(pool, index);
			sound->tag = tag;
			sound->state = 3;
			sound->flags = 0;
			sound->flags |= 0x100;
			sound->field_6 = NONE;
			sound->type = 0;
			sound->decibels = 0;
			sound->source = (char)0xff;
			sound->cluster = NONE;
		}
		if (index != NONE)
		{
			loop_path_get(pool, index)->flags |= 1;
			loop_path_get(pool, index)->gain = gain;
		}
	}
	return index;
}

// @retail 0x18ae80
void __stdcall function_18ae80(s_portal_path_bsp *bsp_data, long listener, point3f const *point,
	long cluster, long override_tag, real elapsed)
{
	s_loop_path_bsp *bsp = (s_loop_path_bsp *)bsp_data;
	s_loop_path_target targets[3];
	long target_count = 0;
	long primary = NONE;
	if (cluster != NONE)
	{
		long indices[64], gains[64], portals[64], clusters[64];
		long count = 0;
		function_18d730(bsp_data, cluster, point, 20.0f, indices, gains, portals, clusters, &count);
		s_loop_path_choice choices[2];
		long choice_count = 0;
		for (long i = 0; i < count && choice_count != 2; i++)
		{
			long index = indices[i];
			if (index == NONE) continue;
			s_loop_path_sound *sound = &bsp->sounds[index];
			if (sound->tag == NONE) continue;
			bool duplicate = false;
			for (long j = 0; j < choice_count && !duplicate; j++)
				duplicate = choices[j].tag == sound->tag;
			if (duplicate) continue;
			choices[choice_count].source = index;
			choices[choice_count].tag = sound->tag;
			choices[choice_count].cluster = clusters[i];
			choices[choice_count].decibels = gains[i];
			if (sound->flags & 1)
			{
				choices[choice_count].inside = sound->inside;
				choices[choice_count].portal = sound->portal;
				choices[choice_count].outside = sound->outside;
			}
			else choices[choice_count].inside = choices[choice_count].portal = choices[choice_count].outside = 1.0f;
			if (sound->flags & 8)
			{
				choices[choice_count].inside *= g_4670fc;
				choices[choice_count].portal *= g_4670fc;
				choices[choice_count].outside *= g_4670fc;
				choices[choice_count].muted = g_509420 == 0;
			}
			else choices[choice_count].muted = false;
			choices[choice_count].distance = portals[i] != NONE ? function_14b240((s_14b240_owner *)bsp,
				(s_bsp3d_disk *)(bsp->portals + portals[i] * 0x24), point) : 0.0f;
			choice_count++;
		}
		bool inside = cluster == clusters[0];
		if (count > 1 && choice_count && inside)
		{
			s_loop_path_sound *sound = &bsp->sounds[choices[0].source];
			for (long i = 1; i < count; i++)
			{
				long index = indices[i];
				if (index != NONE && bsp->sounds[index].tag == choices[0].tag)
				{
					choices[0].distance = function_14b240((s_14b240_owner *)bsp,
						(s_bsp3d_disk *)(bsp->portals + portals[i] * 0x24), point);
					if (sound->flags & 4) choices[0].outside = bsp->sounds[index].flags & 1 ? bsp->sounds[index].inside : 1.0f;
					if (sound->flags & 2) choices[0].portal = bsp->sounds[index].flags & 1 ? bsp->sounds[index].inside : 1.0f;
					break;
				}
			}
		}
		s_loop_path_sound *definitions = *(s_loop_path_sound **)((byte *)g_4e0348 + 0xd8);
		for (long i = 0; i < choice_count; i++)
		{
			s_loop_path_choice *choice = &choices[i];
			real range = bsp->sounds[choice->source].range;
			real fraction = fabs(-range) < 0.0001f ? (choice->distance < 0.0f ? 0.0f : 1.0f) : loop_path_pin(choice->distance / range, 0.0f, 1.0f);
			real blend = loop_path_pin(fraction * fraction * 3.0f - fraction * fraction * fraction * 2.0f, 0.0f, 1.0f);
			choice->gain = ((inside ? choice->inside : choice->outside) - choice->portal) * blend + choice->portal;
			s_loop_path_target *target = &targets[target_count++];
			target->tag = choice->tag;
			target->gain = choice->gain;
			target->decibels = choice->decibels;
			target->audible = i == 0 && (!g_510c50 || !((byte *)g_510c50)[5] || g_4ed288->flag240);
			target->priority = i;
			target->cluster = choice->cluster;
			target->source = choice->source;
			real fade_time = definitions[choice->source].fade_time;
			target->step = elapsed / (fabs(fade_time) < 0.0001f ? 2.0f : fade_time);
			target->muted = choice->muted;
		}
		if (inside && choice_count) primary = choices[0].source;
	}
	if (override_tag != NONE)
	{
		s_loop_path_override *definition = (s_loop_path_override *)g_4e3b44[override_tag & 0xffff].data;
		for (long i = 0; i < target_count; i++)
		{
			long gain = loop_path_logarithmic(1.0f - definition->suppression);
			*(real *)&targets[i].decibels += *(real *)&gain;
		}
		if (definition->tag != NONE)
		{
			s_loop_path_target *target = &targets[target_count++];
			target->tag = definition->tag;
			target->gain = 1.0f;
			target->decibels = loop_path_logarithmic(definition->gain);
			target->audible = true;
			target->priority = 2;
		}
	}
	s_record_pool *pool = g_4ed28c;
	long existing[64];
	long existing_count = 0;
	dword used[2] = { 0, 0 };
	for (long i = data_next_absolute_index_inlined(pool, 0); i != NONE; i = data_next_absolute_index_inlined(pool, i + 1))
	{
		s_loop_path_datum *sound = (s_loop_path_datum *)(pool->data + pool->size * i);
		if (sound->type == 0 && sound->source != NONE)
		{
			sound->flags &= ~0x200;
			existing[existing_count++] = (sound->salt << 16) | i;
		}
	}
	for (long i = 0; i < target_count; i++)
	{
		s_loop_path_target *target = &targets[i];
		long index = NONE;
		for (long j = 0; j < existing_count; j++)
		{
			if (existing[j] != NONE && loop_path_get(pool, existing[j])->tag == target->tag)
			{
				index = existing[j];
				used[j >> 5] |= 1 << (j & 31);
				break;
			}
		}
		if (index == NONE)
		{
			index = loop_path_start(pool, target->tag, target->gain);
			if (index != NONE) loop_path_get(pool, index)->decibels = 0xc2800000;
		}
		if (index != NONE)
		{
			s_loop_path_datum *sound = loop_path_get(pool, index);
			real current = loop_path_linear(sound->decibels);
			real delta = loop_path_linear(target->decibels) - current;
			real gain = current + loop_path_pin(delta, -target->step, target->step);
			sound->decibels = loop_path_logarithmic(gain);
			sound->flags |= 0x200;
			if (target->audible) sound->flags &= ~0x80;
			else sound->flags |= 0x80;
			if (target->muted) sound->flags |= 0x10;
			else sound->flags &= ~0x10;
			sound->listener = (char)g_4686c4;
			sound->cluster = (short)target->cluster;
			sound->gain += loop_path_pin(target->gain - sound->gain, -target->step, target->step);
			sound->source = (char)target->source;
		}
	}
	s_loop_path_sound *definitions = *(s_loop_path_sound **)((byte *)g_4e0348 + 0xd8);
	for (long i = data_next_absolute_index_inlined(pool, 0); i != NONE; i = data_next_absolute_index_inlined(pool, i + 1))
	{
		s_loop_path_datum *sound = (s_loop_path_datum *)(pool->data + pool->size * i);
		long index = (sound->salt << 16) | i;
		if (sound->type == 0 && sound->source != NONE && !(bool)(((dword)sound->flags >> 9) & 1))
		{
			real time = definitions[sound->source].fade_time;
			real step = elapsed / (fabs(time) < 0.0001f ? 2.0f : time);
			real gain = loop_path_linear(sound->decibels);
			gain += loop_path_pin(0.0f - gain, -step, step);
			sound->gain += loop_path_pin(0.0f - sound->gain, -step, step);
			if (fabs(gain) < 0.0001f) loop_path_get(pool, index)->flags |= 2;
			else sound->decibels = loop_path_logarithmic(gain);
		}
	}
	long detail = primary == NONE ? NONE : bsp->sounds[primary].detail_tag;
	long index = g_4ed288->indices[listener * 2];
	if (index != NONE && loop_path_get(pool, index)->tag != detail)
	{
		loop_path_get(pool, index)->flags |= 2;
		g_4ed288->indices[listener * 2] = NONE;
		index = NONE;
	}
	if (index == NONE && detail != NONE)
		g_4ed288->indices[listener * 2] = loop_path_start(pool, detail, 1.0f);
}

#include "local_cameras.h"
#include "sound_sources.h"
#include <float.h>
#include <string.h>

struct s_sound_portal_bsp;
struct s_sound_listener_rotation;
struct s_sound_portal_angles
{
	real lower;
	real upper;
	real horizontal_scale;
	real vertical_scale;
};
void function_18ab70(s_sound_portal_bsp const *bsp, long portal_index, s_sound_listener_rotation const *listener,
	bool reverse, point3f const *point, real radius, s_sound_portal_angles *angles);
void __stdcall function_18dc90(s_portal_path_bsp *bsp, long cluster_index, point3f const *point,
	real maximum_range, long *indices, long *gains, long *portals, long *clusters, long *count);

struct s_sound_environment
{
	real transition_time;
	real fade;
	long index;
	s_sound_portal_angles angles;
};
void sound_environments_update(s_sound_environment const *requests);
long function_189fe0(s_sound_request const *request, long tag_index);

struct s_listener_environment_definition
{
	byte field_0[0x24];
	long tag;
	real radius;
	real transition_time;
	byte field_30[0x18];
};
struct s_listener_environment_bsp
{
	byte field_0[0xe0];
	s_listener_environment_definition *environments;
};

PRIVATE inline long sound_first_listener()
{
	long result = NONE;
	for (long i = 0; i < 4; ++i)
	{
		if (g_4e8c20->entries[i] != NONE)
		{
			result = i;
			break;
		}
	}
	return result;
}

PRIVATE __forceinline void sound_environment_play_cue(long tag)
{
	s_sound_request request;
	request.location.unknown02 = 0;
	request.location.flags = 0;
	request.location.audible = 0;
	request.location.requested_audible = 0;
	request.location.scale = 1.0f;
	request.location.unknown08 = 0;
	request.object_index = NONE;
	request.platform_playback = NONE;
	request.variant = NULL;
	request.marker = NULL;
	request.source = NULL;
	function_189fe0(&request, tag);
}

// @retail 0x18bb80
void __stdcall function_18bb80(real elapsed)
{
	long tags[4] = { NONE, NONE, NONE, NONE };
	s_portal_path_bsp *bsp = (s_portal_path_bsp *)g_4e0348;
	for (long listener = 0; listener < 4; ++listener)
	{
		s_local_camera *camera = local_camera_get(listener);
		if (camera && g_4e8c20->entries[listener] != NONE)
		{
			if (listener == sound_first_listener() && camera->active && camera->index != NONE)
			{
				long cluster = camera->index;
				point3f position = camera->position;
				function_18ae80(bsp, listener, &position, cluster, NONE, elapsed);
				long count;
				long indices[2], portals[2], clusters[2];
				if (cluster != NONE)
				{
					function_18dc90(bsp, cluster, &position, FLT_MAX, indices, NULL, portals, clusters, &count);
					if (count <= 1)
					{
						for (long j = count; j < 2; ++j)
						{
							clusters[j] = NONE;
							portals[j] = NONE;
							indices[j] = NONE;
						}
						count = 2;
					}
					if (portals[0] == NONE) portals[0] = portals[1];
				}
				s_sound_environment requests[2];
				long i = 0;
				do { requests[i].index = NONE; ++i; } while (i < 2);
				for (i = 0; i < count; ++i)
				{
					long index = indices[i];
					long tag = index != NONE ? ((s_listener_environment_bsp *)bsp)->environments[index].tag : NONE;
					real transition = 2.0f;
					if (index != NONE)
					{
						s_listener_environment_definition *definition = &((s_listener_environment_bsp *)g_4e0348)->environments[index];
						if (!(fabs(definition->transition_time) < 0.0001f)) transition = definition->transition_time;
					}
					requests[i].transition_time = transition;
					requests[i].fade = 1.0f;
					requests[i].index = tag;
					s_sound_portal_angles angles;
					if (index != NONE)
						function_18ab70((s_sound_portal_bsp *)bsp, portals[i], (s_sound_listener_rotation *)camera,
							cluster == clusters[i], &position, ((s_listener_environment_bsp *)bsp)->environments[index].radius, &angles);
					else
					{
						angles.lower = 0.0f;
						angles.upper = 6.2831855f;
						angles.horizontal_scale = tag != NONE ? 1.0f : 0.0f;
						angles.vertical_scale = 0.0f;
					}
					requests[i].angles = angles;
				}
				sound_environments_update(requests);
			}
			long tag = tags[listener];
			long *current = &g_4ed288->indices[listener * 2 + 1];
			if (*current != tag)
			{
				if (*current != NONE)
				{
					long cue = *(long *)(g_4e3b44[*current & 0xffff].bytes + 0x5c);
					if (cue != NONE) sound_environment_play_cue(cue);
				}
				if (tag != NONE)
				{
					long cue = *(long *)(g_4e3b44[tag & 0xffff].bytes + 0x54);
					if (cue != NONE) sound_environment_play_cue(cue);
				}
				*current = tag;
			}
		}
	}
}
