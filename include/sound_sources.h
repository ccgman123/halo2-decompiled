/* SOUND_SOURCES.H: the views shared by the sound source callbacks
   (unknown_18c250.cpp, unknown_18c810.cpp) */
#ifndef SOUND_SOURCES_H
#define SOUND_SOURCES_H

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
#include "object_queries.h"

struct s_object;

extern s_record_pool *g_4e637c;

struct s_looping_sound_source
{
	byte unknown00[0xc];
	long value0c;
	byte unknown10[0xbc - 0x10];
};

s_object *function_badc0(long object_index, dword type_mask);

struct s_tag_instance_view
{
	dword group_tag;
	byte unknown04[4];
	byte *data;
	byte unknown0c[4];
};

struct s_sound_tag
{
	byte unknown00[6];
	short class_index;
};

/* the spatialization of a sound class */
struct s_sound_class_spatialization
{
	union
	{
		byte flags;
		struct
		{
			dword bit0 : 1;
			dword bit1 : 1;
			dword bit2 : 1;
		} bits;
	};
	real angle;
	real value8;
	real valuec;
};

struct s_sound_class_view
{
	byte unknown00[0x28];
	s_sound_class_spatialization spatialization;
	byte unknown38[0x38 - 0x38];
};

struct s_sound_globals_view
{
	byte unknown00[4];
	s_sound_class_view *classes;
};

struct s_sound_source_view
{
	byte unknown00;
	byte flags;
	byte unknown02[0x1a];
	long value1c;
	long value20;
};

struct s_sound_spatialization_view
{
	dword flags;
	real value4;
	real value8;
	long valuec;
	long value10;
};

static inline s_sound_class_spatialization *sound_class_get_spatialization(s_sound_class_view *sound_class)
{
	return (sound_class->spatialization.flags & 7) ? &sound_class->spatialization : NULL;
}

/* a sound marker: a node and a position and direction relative to it
   (unknown_189010.cpp builds them as s_sound_source_description) */
struct s_sound_marker
{
	byte node_index;
	byte flag0 : 1;
	byte flag1 : 1;
	byte unknown01 : 6;
	byte unknown02[2];
	point3f position;
	vector3f forward;
	long value1c;
	long value20;
	long value24;
};

/* where in the world a sound plays */
struct s_sound_position
{
	point3f position;
	dword compressed_forward;
	vector3f velocity;
	s_location location;
};

/* where a sound plays from: what the update callbacks fill in */
struct s_type_99c531
{
	union
	{
		struct
		{
			word flag0 : 1;
			word flag1 : 1;
			word flag2 : 1;
			word flag3 : 1;
			word flag4 : 1;
			word flag5 : 1;
			word flag6 : 1;
			word flag7 : 1;
			word flag8 : 1;
			word flag9 : 1;
			word flag10 : 1;
			word flag11 : 1;
			word unknown0c : 4;
		};
		struct
		{
			word flags;
			byte unknown02;
			char audible : 4;
			char requested_audible : 4;
		};
	};
	real scale;
	dword unknown08;
	s_sound_position spatial;
	byte unknown30[0x44 - 0x30];
};

struct s_sound_source_state
{
	byte unknown00[0x24];
	long value24;
};

/* the sound source types: what a playing sound asks of its source */
struct s_sound_source_callbacks
{
	bool (__stdcall *update)(long object_index, long tag_index, s_sound_marker const *marker, s_type_99c531 *location);
	void (__stdcall *proc1)(long object_index, long tag_index, long a, long b);
	void (__stdcall *proc2)(long object_index, long unused, long tag_index, long set_index, long permutation, long scale);
	bool (__stdcall *spatialize)(long object_index, long tag_index, s_sound_source_view const *source, s_sound_spatialization_view *spatialization);
	void (__stdcall *stop)(long object_index, long source_index, long reason);
	void (__stdcall *detach)(long object_index, long source_index);
	bool (__stdcall *compare)(void const *a, void const *b);
	bool (__stdcall *same_source)(long a, s_sound_source_state const *state_a, long b, s_sound_source_state const *state_b);
};


/* what playing a sound asks for (0x58 bytes) */
struct s_sound_request
{
	s_type_99c531 location;
	long platform_playback;
	long object_index;
	s_sound_source_callbacks const *source;
	s_sound_marker const *marker;
	char const *variant;
};

bool __stdcall function_18c250(long object_index, long tag_index, s_sound_marker const *marker, s_type_99c531 *location);
bool __stdcall function_18c3b0(long object_index, long tag_index, s_sound_marker const *marker, s_type_99c531 *location);
void __stdcall function_18c630(long object_index, long tag_index, long a, long b);
void __stdcall function_18c6a0(long object_index, long unused, long tag_index, long set_index, long permutation, long scale);

#endif
