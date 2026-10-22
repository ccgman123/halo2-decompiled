// @flags /O2 /Ob1 /Gr
#include "unknown_11c920.h"
#include <xtl.h>
#include "unknown_223b60.h"

__int64 timing_counter_peek(timing_counter *c);

static inline __int64 read_tsc(void)
{
	volatile __int64 t = 0;
	__asm rdtsc
}

// @retail 0x223af0
timing_counter *timing_counter_start(timing_counter *c)
{
	((volatile timing_counter *)c)->total = 0;
	c->start = read_tsc();
	c->stopped = false;
	return c;
}

// @retail 0x223b20
__int64 timing_counter_resume(timing_counter *c)
{
	__int64 total = timing_counter_peek(c);

	if (c->stopped)
	{
		((volatile timing_counter *)c)->stopped = false;
		c->start = read_tsc();
	}
	return total;
}

// score 4: identical body and length; our prologue stores the first delta
// zero before pushing esi, while retail pushes esi first and then stores both
// zero dwords. The permuter found no better variant.
// @retail 0x223b60
__int64 timing_counter_stop(timing_counter *c)
{
	if (!c->stopped)
	{
		__int64 delta = read_tsc() - c->start;
		if (delta < 0) delta = 0;
		c->total += delta;
		c->stopped = true;
	}
	return c->total;
}

// @retail 0x223bb0
double timing_ticks_to_seconds(__int64 ticks)
{
	const __int64 frequency = 733333333;
	__int64 q = ticks / frequency;
	__int64 r = ticks - q * frequency;
	return (double)r / (double)frequency + (double)q;
}

// @retail 0x223c20
__int64 timing_counter_peek(timing_counter *c)
{
	__int64 total = c->total;
	if (!c->stopped)
	{
		__int64 delta = read_tsc() - c->start;
		if (delta < 0) delta = 0;
		total += delta;
	}
	return total;
}

// score 28: the conversion is correct, but our compiler keeps the union local
// in registers (`movzx eax`; `shr ax`) while retail keeps it in memory
// (`xor ecx, ecx`; `mov cx, [eax]`; `shr word ptr [esp + 4], 5`) and puts the
// blue channel in dl. No source shape or annealed permuter run reproduced it.
// @retail 0x223e70
void function_223e70(const word *rgb, S3TC_COLOR *out)
{
	union
	{
		dword d;
		word w;
		byte bytes[4];
	} u;
	word w = *rgb;
	dword c = w;
	byte b = (byte)(c << 3);
	b |= b >> 5;
	u.d = c;
	u.w >>= 5;
	byte g = (byte)(u.bytes[0] << 2);
	g |= g >> 6;
	byte r = (byte)(u.d >> 6);
	r <<= 3;
	r |= r >> 5;
	u.bytes[0] = b;
	u.bytes[1] = g;
	u.bytes[2] = r;
	*(dword *)out = u.d;
}

PRIVATE inline s_fixup_element *fixup_entry_target(s_fixup_entry *entry, byte *base8)
{
	if (entry->target_index == 0xFFFF)
		return (s_fixup_element *)(base8 + entry->target_offset);
	return (s_fixup_element *)(*(byte **)(base8 + entry->target_offset + 4) + (entry->target_index << 5));
}

// @retail 0x223c70
void fixup_group_apply(s_fixup_group *group, byte *base)
{
	byte *base8 = base + 8;
	s_fixup_pointer *root = (s_fixup_pointer *)((byte *)group + group->pointer_offset);
	root->count = 1;
	root->pointer = base8;
	for (long i = 0; i < group->entry_count; i++)
	{
		s_fixup_entry *entry = &group->entries[i];
		if (entry->offset == NONE)
			continue;
		byte *address = base + group->data_offset + entry->offset + 8;
		switch (entry->type)
		{
		case 0:
		{
			s_fixup_pointer *target = (s_fixup_pointer *)(base8 + entry->target_offset);
			target->count = entry->value / entry->target_index;
			target->pointer = address;
			break;
		}
		case 1:
		{
			s_fixup_pointer *target = (s_fixup_pointer *)(base8 + entry->target_offset);
			target->count = entry->value;
			target->pointer = address;
			break;
		}
		default:
		{
			s_fixup_element *element = fixup_entry_target(entry, base8);
			short quotient = (short)((entry->value - element->base) / element->divisor);
			element->relative = (long)address - element->unknown4;
			element->quotient = quotient;
			s_fixup_trailer *trailer = &element->inline_trailer;
			element->trailer = trailer;
			trailer->count = 1;
			trailer->packed = (dword)address - element->unknown4;
			trailer->unknown8 = 0;
			trailer->packed &= 0xFFFFFFF;
			break;
		}
		}
	}
}

PRIVATE inline bool fixup_element_busy_223d80(s_fixup_element *element)
{
	D3DResource *resource = (D3DResource *)element->trailer;
	bool busy = false;
	if (resource)
		busy = resource->IsBusy() > 0;
	return busy;
}

// @retail 0x223d80
bool fixup_group_has_resource(s_fixup_group *group, byte *base)
{
	bool result = false;
	byte *base8 = base + 8;
	for (long i = 0; i < group->entry_count; i++)
	{
		s_fixup_entry *entry = &group->entries[i];
		if (entry->offset != NONE && entry->type == 2)
		{
			if (fixup_element_busy_223d80(fixup_entry_target(entry, base8)))
			{
				result = true;
				break;
			}
		}
	}
	return result;
}

// @retail 0x223e00
void fixup_group_release_resources(s_fixup_group *group, byte *base)
{
	byte *base8 = base + 8;
	for (long i = 0; i < group->entry_count; i++)
	{
		s_fixup_entry *entry = &group->entries[i];
		if (entry->offset != NONE && entry->type == 2)
		{
			D3DResource *resource = (D3DResource *)fixup_entry_target(entry, base8)->trailer;
			if (resource)
				resource->BlockUntilNotBusy();
		}
	}
}
