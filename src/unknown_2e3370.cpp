// @flags /O2 /GL- /GX
// Havok (third-party physics library): a three-axis sweep of boxes
// (axis_sweep_2e1090.h): the endpoints of every box a box overlaps, in three
// sorted lists, one per axis.
#include "axis_sweep_2e1090.h"
#include "reference_counted_2d88a0.h"
#include <new>

// an array of longs whose storage comes from the allocator's stack area
// (src/unknown_2e18e0.cpp)
struct array_longs_2e1ab0
{
	long *data;
	long size;
	unsigned long capacity_and_flags;

	array_longs_2e1ab0() : data(0), size(0), capacity_and_flags(0x80000000) {}
	~array_longs_2e1ab0()
	{
		if ((capacity_and_flags & 0x80000000) == 0)
		{
			g_memory_2d88a0->release_block(data, (capacity_and_flags & 0x7fffffff) * sizeof(long), 0x12);
		}
	}
};

struct longs_2e1ab0 : array_longs_2e1ab0
{
	long *stack_block;
	long stack_capacity;

	longs_2e1ab0(long capacity);
	~longs_2e1ab0();
};

// a block from the top of the allocator's stack area, or from its slot 9 when
// the area is too small
inline void *stack_allocate_2e3370(long size)
{
	memory_2d88a0 *memory = g_memory_2d88a0;
	long free = memory->stack_free;
	if (size > free)
	{
		return memory->allocate_stack_overflow(size);
	}
	char *block = memory->stack_cursor;
	memory->stack_cursor = block + size;
	memory->stack_free = free - size;
	return block;
}

// sorts entries from low to high (both included) by key (src/unknown_2e0120.cpp)
struct entry_2e0250;
void __cdecl function_2e0250(entry_2e0250 *entries, int low, int high, long context);

// a list of endpoints over storage it does not own
struct endpoint_list_2e3370
{
	endpoint_2e0f80 *data;
	long size;
	unsigned long capacity_and_flags;

	endpoint_list_2e3370(endpoint_2e0f80 *storage, long capacity) : data(storage), size(0), capacity_and_flags(capacity | 0x80000000) {}

	void append(endpoint_2e0f80 endpoint)
	{
		data[size] = endpoint;
		size++;
	}
};

// the three lists, and the storage that follows them
struct overlap_endpoints_2e3370
{
	endpoint_list_2e3370 axes[3];
	endpoint_2e0f80 storage[1];
};

// where a range's endpoints would sit on an axis: the first endpoint not
// below its lower end, and the last one below its upper end
__forceinline void find_range_2e3370(endpoints_2e0f80 *axis, unsigned long low, unsigned long high,
	unsigned short *first_slot, unsigned short *last_slot)
{
	endpoint_2e0f80 *data = axis->data;
	entry_2dfec0 *last = (entry_2dfec0 *)&data[axis->size - 2];
	*first_slot = (unsigned short)((endpoint_2e0f80 *)((sorted_2dfec0 *)axis)->first_not_below(
		(entry_2dfec0 *)(data + 1), last, (unsigned short)low) - data);
	*last_slot = (unsigned short)((endpoint_2e0f80 *)((sorted_2dfec0 *)axis)->first_not_below(
		(entry_2dfec0 *)(data + 1), last, (unsigned short)high) - 1 - data);
}

// a box found overlapping on x: kept when it overlaps on y and z too and is
// not a grid cell's
__forceinline void keep_2e3370(element_2e1090 *box, unsigned long const *range, longs_2e1ab0 *found)
{
	unsigned long const *slots = (unsigned long const *)box->slots;
	unsigned long const outside = ((range[1] - slots[0]) | (slots[1] - range[0])) & 0x80008000;
	if (outside)
	{
		return;
	}
	if (box->owner & 1)
	{
		return;
	}
	found->data[found->size] = (long)box;
	found->size++;
}

// score 575 (was 591): retail's order of steps. The y/z overlap test
// keeps its masked result in a local, which retail stores to the stack (into
// the y/z slots' home). Still: the element count is kept in edi and the bits
// in ebx where retail has them the other way round, so each bit's word gets
// its address from lea instead of retail's [edi+eax*4] (re-reading the count
// or folding the size computation does not swap them); retail's frame puts
// the y/z slots at 0x30 and other spills 8-12 bytes higher than ours; in the
// gather loop retail keeps the y/z slot words in ebx/esi and the found bits in
// memory (ours keeps the bits in edx and reloads one slot word; passing the
// words by value compiles alike) and biases the box pointer by 0x24 (ours
// 0x1c).
// the endpoints of every box the bounds overlap (not the grid cells'), one
// list per axis, each sorted between a sentinel at each end
// @retail 0x2e3370
void class_2e1090::function_2e3370(bounds_2e0020 const *bounds, overlap_endpoints_2e3370 *lists)
{
	longs_2e1ab0 found(elements.size);
	long const count = elements.size;
	long const words = (count >> 5) + 4;
	long const bits_size = (words * 4 + 15) & ~15;
	long *bits = (long *)stack_allocate_2e3370(bits_size);
	{
		__m128 *block = (__m128 *)bits;
		__m128 const zero = _mm_setzero_ps();
		for (long blocks = count >> 7; blocks >= 0; blocks--)
		{
			_mm_store_ps((float *)block++, zero);
		}
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
	find_range_2e3370(&axes[1], low[1], high[1], &range[0], &range[2]);
	find_range_2e3370(&axes[2], low[2], high[2], &range[1], &range[3]);

	element_2e1090 *group = elements.data;
	long *words_end = bits + (elements.size >> 5) + 1;
	for (long *word = bits; word < words_end; word++, group += 32)
	{
		unsigned long found_bits = *word;
		if (found_bits)
		{
			element_2e1090 *box = group;
			do
			{
				if (found_bits & 0xf)
				{
					if (found_bits & 1)
					{
						keep_2e3370(&box[0], (unsigned long const *)range, &found);
					}
					if (found_bits & 2)
					{
						keep_2e3370(&box[1], (unsigned long const *)range, &found);
					}
					if (found_bits & 4)
					{
						keep_2e3370(&box[2], (unsigned long const *)range, &found);
					}
					if (found_bits & 8)
					{
						keep_2e3370(&box[3], (unsigned long const *)range, &found);
					}
				}
				found_bits >>= 4;
				box += 4;
			} while (found_bits);
		}
	}
	g_memory_2d88a0->release_stack(bits, bits_size);

	// two endpoints for each box, and a sentinel at each end
	long const capacity = found.size * 2 + 2;
	new (&lists->axes[0]) endpoint_list_2e3370(lists->storage, capacity);
	new (&lists->axes[1]) endpoint_list_2e3370(lists->storage + capacity, capacity);
	new (&lists->axes[2]) endpoint_list_2e3370(lists->storage + capacity * 2, capacity);
	endpoint_2e0f80 sentinel;
	sentinel.value = 0;
	sentinel.id = 0;
	lists->axes[0].append(sentinel);
	lists->axes[1].append(sentinel);
	lists->axes[2].append(sentinel);
	element_2e1090 **kept = (element_2e1090 **)found.data;
	for (long remaining = found.size - 1; remaining >= 0; remaining--)
	{
		lists->axes[0].append(axes[0].data[(*kept)->slots[4]]);
		lists->axes[0].append(axes[0].data[(*kept)->slots[5]]);
		lists->axes[1].append(axes[1].data[(*kept)->slots[0]]);
		lists->axes[1].append(axes[1].data[(*kept)->slots[2]]);
		lists->axes[2].append(axes[2].data[(*kept)->slots[1]]);
		lists->axes[2].append(axes[2].data[(*kept)->slots[3]]);
		kept++;
	}
	endpoint_list_2e3370 *list = lists->axes;
	for (int axis = 3; axis != 0; axis--)
	{
		long const last = list->size - 1;
		if (last > 1)
		{
			function_2e0250((entry_2e0250 *)(list->data + 1), 0, last - 1, 0);
		}
		list++;
	}
	sentinel.value = 0xfffc;
	sentinel.id = 0;
	lists->axes[0].append(sentinel);
	lists->axes[1].append(sentinel);
	lists->axes[2].append(sentinel);
}
