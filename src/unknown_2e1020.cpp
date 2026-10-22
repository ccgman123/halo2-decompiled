// @flags /O2 /GL-
// Havok (third-party physics library): a three-axis sweep of boxes
// (axis_sweep_2e1090.h): the boxes of all of them into an array, the box of
// one picked by an index the caller points to, and the boxes a box overlaps.
#include "axis_sweep_2e1090.h"

// the boxes of all the elements, in order
// @retail 0x2e1020
void class_2e1090::function_2e1020(bounds_array_2e1020 *out)
{
	if ((out->capacity_and_flags & 0x7fffffff) < elements.size)
	{
		function_2d9160(out, elements.size, sizeof(bounds_2e0020));
	}
	out->size = elements.size;
	for (int i = 0; i < elements.size; i++)
	{
		function_2e0020(&elements.data[i], &out->data[i]);
	}
}

// @retail 0x2e1090
void class_2e1090::function_2e1090(long const *index, bounds_2e0020 *bounds)
{
	function_2e0020(&elements.data[*index], bounds);
}

// where a range's endpoints would sit on an axis: the first endpoint not
// below its lower end, and the last one below its upper end
__forceinline void find_range_2e10b0(endpoints_2e0f80 *axis, unsigned long low, unsigned long high,
	unsigned short *first_slot, unsigned short *last_slot)
{
	endpoint_2e0f80 *data = axis->data;
	entry_2dfec0 *last = (entry_2dfec0 *)&data[axis->size - 2];
	*first_slot = (unsigned short)((endpoint_2e0f80 *)((sorted_2dfec0 *)axis)->first_not_below(
		(entry_2dfec0 *)(data + 1), last, (unsigned short)low) - data);
	*last_slot = (unsigned short)((endpoint_2e0f80 *)((sorted_2dfec0 *)axis)->first_not_below(
		(entry_2dfec0 *)(data + 1), last, (unsigned short)high) - 1 - data);
}

// a box found overlapping on x: reported when it overlaps on y and z too and
// is not a grid cell's
__forceinline void report_2e10b0(element_2e1090 const *box, unsigned long const *range, pairs_2e1090 *pairs)
{
	unsigned long const *slots = (unsigned long const *)box->slots;
	if (((range[1] - slots[0]) | (slots[1] - range[0])) & 0x80008000)
	{
		return;
	}
	long const owner = box->owner;
	if (owner & 1)
	{
		return;
	}
	if (pairs->size == (pairs->capacity_and_flags & 0x7fffffff))
	{
		function_2d91d0(pairs, sizeof(pair_2e1090));
	}
	pair_2e1090 *added = &pairs->data[pairs->size];
	added->first = 0;
	added->second = owner;
	pairs->size++;
}

// longs in a block from the allocator's stack area
struct stack_longs_2e10b0
{
	long *data;
	long capacity;

	stack_longs_2e10b0(long count)
	{
		capacity = count;
		data = (long *)g_memory_2d88a0->allocate_stack((capacity * sizeof(long) + 15) & ~15);
	}
	~stack_longs_2e10b0()
	{
		g_memory_2d88a0->release_stack(data, (capacity * sizeof(long) + 15) & ~15);
	}
};

// score 383 (was 521): one byte longer than retail. The pair is stored in
// place after the room check (append by value builds it first and spills the
// owner), and the element count is read again for the clearing loop. Still:
// retail reads the count once into ebx ahead of the allocator and keeps the
// stack area's free size in edx (ours re-reads it through a pointer); retail
// keeps the bits in edi and indexes them ([edi+eax*4]) where ours keeps them in
// ebx and forms each word's address with lea (local helpers that read and
// write the word separately compile alike); the low corner words sit at 0x20
// in retail, apart from the y/z slots, where ours shares 0x28 with them (no
// declaration order moves it); the grid cell's box is read before its address
// is formed.
// every box the bounds overlap, as pairs with no first owner
// @retail 0x2e10b0
void class_2e1090::function_2e10b0(bounds_2e0020 const *bounds, pairs_2e1090 *pairs)
{
	stack_longs_2e10b0 stack_bits((elements.size >> 5) + 4);
	long *bits = stack_bits.data;
	__m128 *block = (__m128 *)bits;
	__m128 const zero = _mm_setzero_ps();
	for (long blocks = elements.size >> 7; blocks >= 0; blocks--)
	{
		_mm_store_ps((float *)block++, zero);
	}

	unsigned long low[4];
	unsigned long high[4];
	unsigned short range[4];
	{
		union
		{
			__m128 vector;
			unsigned long bits[4];
		} quantized;
		unsigned short low_words[3];
		__m128 const lower = bounds->lower;
		__m128 const ceiling = *(__m128 const *)g_4125d0;
		__m128 const floor = *(__m128 const *)g_4125e0;
		__m128 const bias = *(__m128 const *)g_4125c0;
		quantized.vector = _mm_add_ps(_mm_max_ps(_mm_min_ps(
			_mm_mul_ps(_mm_add_ps(lower, lower_offset), scale), ceiling), floor), bias);
		low_words[0] = (unsigned short)(quantized.bits[0] >> 7);
		low_words[1] = (unsigned short)(quantized.bits[1] >> 7);
		low_words[2] = (unsigned short)(quantized.bits[2] >> 7);
		low[0] = low_words[0] & 0xfffe;
		low[1] = low_words[1] & 0xfffe;
		low[2] = low_words[2] & 0xfffe;
		quantized.vector = _mm_add_ps(_mm_max_ps(_mm_min_ps(
			_mm_mul_ps(_mm_add_ps(bounds->upper, upper_offset), scale), ceiling), floor), bias);
		high[0] = (unsigned short)(quantized.bits[0] >> 7) | 1;
		high[1] = (unsigned short)(quantized.bits[1] >> 7) | 1;
		high[2] = (unsigned short)(quantized.bits[2] >> 7) | 1;
	}

	// one bit per box: set for the boxes whose x interval overlaps
	endpoint_2e0f80 *endpoint = axes[0].data + 1;
	if (cells_used)
	{
		// start from the grid cell's box: the boxes it holds, less those
		// that start inside it
		long const grid_slot = (long)low[0] >> (16 - cell_shift);
		if (grid_slot > 0)
		{
			cell_2e1090 *cell = &cells[grid_slot - 1];
			toggle_bit_2e1090(bits, cell->box);
			unsigned short *held = cell->boxes.data;
			for (long remaining = cell->boxes.size - 1; remaining >= 0; remaining--)
			{
				toggle_bit_2e1090(bits, *held);
				held++;
			}
			element_2e1090 *cell_box = &elements.data[cell->box];
			endpoint_2e0f80 *inside = &axes[0].data[cell_box->slots[4] + 1];
			endpoint_2e0f80 *inside_end = &axes[0].data[cell_box->slots[5]];
			for (; inside < inside_end; inside++)
			{
				if ((inside->value & 1) == 0)
				{
					clear_bit_2e1090(bits, inside->id);
				}
			}
			endpoint = &axes[0].data[cell_box->slots[4] + 1];
		}
	}
	for (; endpoint->value < low[0]; endpoint++)
	{
		toggle_bit_2e1090(bits, endpoint->id);
	}
	for (; endpoint->value < high[0]; endpoint++)
	{
		if ((endpoint->value & 1) == 0)
		{
			toggle_bit_2e1090(bits, endpoint->id);
		}
	}

	// where the bounds would sit on y and z
	find_range_2e10b0(&axes[1], low[1], high[1], &range[0], &range[2]);
	find_range_2e10b0(&axes[2], low[2], high[2], &range[1], &range[3]);

	element_2e1090 *group = elements.data;
	long *words_end = bits + (elements.size >> 5) + 1;
	for (long *word = bits; word < words_end; word++, group += 32)
	{
		unsigned long found = *word;
		if (found)
		{
			element_2e1090 *box = group;
			do
			{
				if (found & 0xf)
				{
					if (found & 1)
					{
						report_2e10b0(&box[0], (unsigned long const *)range, pairs);
					}
					if (found & 2)
					{
						report_2e10b0(&box[1], (unsigned long const *)range, pairs);
					}
					if (found & 4)
					{
						report_2e10b0(&box[2], (unsigned long const *)range, pairs);
					}
					if (found & 8)
					{
						report_2e10b0(&box[3], (unsigned long const *)range, pairs);
					}
				}
				found >>= 4;
				box += 4;
			} while (found);
		}
	}
}
