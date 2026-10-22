#include "unknown_11c920.h"
#include "globals.h"

// @flags /O2 /arch:SSE /Gr

struct s_audio_object_header
{
	byte unknown00[8];
	long *object;
};

struct s_audio_object_definition
{
	byte unknown000[0x1f0];
	short type;
};

struct s_audio_lookup_root
{
	byte unknown00[0xc8];
	long count;
	byte *entries;
};

struct s_audio_lookup_entry
{
	short value;
	short unknown02;
};

struct s_audio_lookup_table
{
	byte unknown00[0x24];
	long count;
	s_audio_lookup_entry *entries;
};

// @retail 0x20ba00
short function_20ba00(short index)
{
	short result = NONE;
	s_audio_lookup_root *root = (s_audio_lookup_root *)g_4e034c;
	if (root && root->count > 0)
	{
		long definition_index = *(long *)(root->entries + 0x64);
		if (definition_index != NONE)
		{
			s_audio_lookup_table *table = (s_audio_lookup_table *)g_4e3b44[definition_index & 0xffff].data;
			if (table && index >= 0 && index < table->count)
				result = table->entries[index].value;
		}
	}
	return result;
}

// @retail 0x20bb70
bool function_20bb70(long object_index)
{
	bool result = false;
	long definition_index = *((s_audio_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	s_audio_object_definition *definition = (s_audio_object_definition *)g_4e3b44[definition_index & 0xffff].data;
	if (definition->type == 6)
		result = true;
	return result;
}


#include "units.h"
long function_1caa10(long object_index);
long function_26bc60(long clump_index);
bool function_267550(long actor_index);
long function_1b8c80(long object_index);
short __stdcall function_c8b80(long object_index, s_object_seat *seats, short maximum_count);
long function_114480(long unit_index);

PRIVATE inline byte *audio_condition_object(long index)
{
    return (byte *)((s_audio_object_header *)g_4e0300->data)[index & 0xffff].object;
}

PRIVATE __forceinline bool audio_condition_parent(byte *unit)
{
    return *(short *)(unit + 0x1fc) != NONE &&
        (g_4e0300->data + (*(long *)(unit + 0x14) & 0xffff) * 12)[3] == 1;
}

// @retail 0x20bbb0
bool __stdcall function_20bbb0(short condition, long object_index, real *weight)
{
    bool result = true;
    if (object_index != NONE)
    {
        byte *object = audio_condition_object(object_index);
        byte *unit = NULL;
        byte *actor = NULL;
        if ((1 << object[0xaa]) & 3)
        {
            unit = object;
            long actor_index = *(long *)(unit + 0x12c);
            if (actor_index != NONE)
                actor = g_4f55f0->data + (actor_index & 0xffff) * 0x888;
        }
        result = false;
        switch (condition)
        {
        case 0:
            if (weight)
                *weight *= 0.8f;
            result = true;
            break;
        case 1:
            if (unit)
                result = *(long *)(unit + 0x13c) != NONE;
            break;
        case 2:
            result = actor != NULL;
            break;
        case 3:
            if (object[0xaa] == 0) result = !(bool)((object[0x10a] >> 2) & 1);
            break;
        case 4:
            if (object[0xaa] == 0 && (bool)((object[0x10a] >> 2) & 1)) result = true;
            break;
        case 5:
            result = object[0xaa] == 1 && !function_20bb70(object_index);
            break;
        case 6:
            result = object[0xaa] == 5;
            break;
        case 7:
            if (unit)
                result = *(long *)(unit + 0x13c) != NONE || *(long *)(unit + 0x12c) != NONE;
            break;
        case 8:
            if (unit && unit[0xaa] == 1)
                result = function_20bb70(function_1caa10(object_index));
            break;
        case 9:
            if (unit)
                result = *(short *)(unit + 0x1fc) != NONE;
            break;
        case 10:
            if (unit && audio_condition_parent(unit))
                result = function_20bb70(function_1caa10(*(long *)(unit + 0x14)));
            break;
        case 11:
            if (unit)
            {
                if (audio_condition_parent(unit))
                {
                    byte *parent = audio_condition_object(*(long *)(unit + 0x14));
                    result = *(long *)(parent + 0x248) == object_index;
                }
            }
            break;
        case 12:
            if (unit)
            {
                if (audio_condition_parent(unit))
                {
                    byte *parent = audio_condition_object(*(long *)(unit + 0x14));
                    result = *(long *)(parent + 0x24c) == object_index;
                }
            }
            break;
        case 13:
            if (unit)
            {
                if (audio_condition_parent(unit))
                {
                    byte *parent = audio_condition_object(*(long *)(unit + 0x14));
                    result = *(long *)(parent + 0x248) != object_index && *(long *)(parent + 0x24c) != object_index;
                }
            }
            break;
        case 14:
            if (actor)
            {
                long clump = *(long *)(actor + 0x7c);
                if (clump != NONE)
                    result = *(short *)(g_502420->data + (clump & 0xffff) * 0x50 + 0x26) == 1;
            }
            break;
        case 15:
            if (actor)
            {
                long clump = *(long *)(actor + 0x7c);
                if (clump != NONE)
                    result = *(short *)(g_502420->data + (clump & 0xffff) * 0x50 + 0x26) == 1 && (short)function_26bc60(clump) == 1;
            }
            break;
        case 16:
            if (actor)
            {
                long clump = *(long *)(actor + 0x7c);
                if (clump != NONE)
                    result = *(short *)(g_502420->data + (clump & 0xffff) * 0x50 + 0x26) == 1 && (short)function_26bc60(clump) == 2;
            }
            break;
        case 17:
            if (unit)
            {
                long player = *(long *)(unit + 0x13c);
                if (player != NONE)
                    result = g_4e8c24->data[(player & 0xffff) * 0x21c + 0x88] == 0;
            }
            break;
        case 18:
            if (unit)
            {
                long player = *(long *)(unit + 0x13c);
                if (player != NONE)
                    result = g_4e8c24->data[(player & 0xffff) * 0x21c + 0x88] == 1;
            }
            break;
        case 19:
            if (actor)
                result = *(short *)(actor + 0x24) == 6;
            break;
        case 20:
            if (actor)
                result = function_267550(*(long *)(unit + 0x12c));
            break;
        case 21:
            if (unit)
            {
                byte *parent;
                if (audio_condition_parent(unit) &&
                    (parent = audio_condition_object(*(long *)(unit + 0x14)),
                    *(long *)(parent + 0x24c) == object_index || *(long *)(parent + 0x248) == object_index))
                {
                    s_object_seat seats[64];
                    result = true;
                    short count = function_c8b80(function_1b8c80(*(long *)(unit + 0x14)), seats, 64);
                    for (long i = 0; i < count; i++)
                    {
                        long occupant = function_c8f60(seats[i].object_index, seats[i].seat_index);
                        if (occupant != NONE && occupant != object_index)
                        {
                            byte *vehicle = audio_condition_object(seats[i].object_index);
                            if (*(long *)(vehicle + 0x24c) == occupant || *(long *)(vehicle + 0x248) == occupant)
                            {
                                result = false;
                                break;
                            }
                        }
                    }
                }
            }
            break;
        case 22:
            if (unit)
            {
                long definition_index = function_114480(object_index);
                if (definition_index != NONE)
                    result = !(g_4e3b44[definition_index & 0xffff].bytes[8] & 1);
            }
            break;
        case 23:
            if (unit)
            {
                long definition_index = function_114480(object_index);
                if (definition_index != NONE)
                    result = (g_4e3b44[definition_index & 0xffff].bytes[8] & 1) != 0;
            }
            break;
        case 24:
            if (object[0xaa] == 5)
            {
                byte type = g_4e3b44[*(long *)object & 0xffff].bytes[0x128];
                if (type == 0x15 || type == 0x16)
                    result = true;
            }
            break;
        }
    }
    else if (condition != 0)
        result = false;
    return result;
}
