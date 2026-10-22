// @flags /O2 /GL- /GX
// Havok (third-party physics library): adding a box to the three-axis sweep
// (axis_sweep_2e1090.h): its corners are quantized to 16 bits, its endpoints
// inserted into each axis's sorted list, and every box it overlaps reported
// as a pair (or, for a box that stands for a grid cell, added to that cell's
// list). (Its own file, not src/unknown_2e2190.cpp beside it: it needs /GX
// for the stack array it unwinds.)
#include "axis_sweep_2e1090.h"

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

// moves the boxes' endpoint indices past the new ones (src/unknown_2dfcc0.cpp)
struct quantized_box_2dfcc0;

class quantized_boxes_2dfcc0
{
public:
	void function_2dfcc0(quantized_box_2dfcc0 *boxes, long count, unsigned short const *limits);
	void function_2dfdc0(quantized_box_2dfcc0 *boxes, long count, unsigned short const *limits);
};

// takes two entries out of an axis's endpoints (src/unknown_2dffa0.cpp)
class array_2dffa0
{
public:
	long *data;
	int size;

	void remove_two(int first, int second);
};

// where `value` sits in the cell's list of boxes, or -1
inline long find_box_2e29b0(cell_2e1090 const *cell, unsigned short value)
{
	for (long position = 0; position < cell->boxes.size; position++)
	{
		if (cell->boxes.data[position] == value)
		{
			return position;
		}
	}
	return -1;
}

// score 385 (was 395): the corners are now stored high then low per axis, and
// the bounds are read in the expressions (no locals). Still differs: register
// choice from the start: retail loads both bounds first into xmm4/xmm5 and the
// 0x4125c0 bias after the scaling, ours the bias first; later the grid-cell
// walk keeps the endpoint in ebx and xors the bit in place where ours spills
// the word's address, and the box is 12 bytes shorter. Splitting the scaled
// terms, a bias local and swapped operands did not help.
// @retail 0x2e24b0
void class_2e1090::function_2e24b0(long *owner, bounds_2e0020 const *bounds, pairs_2e1090 *pairs)
{
	__declspec(align(16)) struct
	{
		unsigned long low[4];
		unsigned long high[4];
	} corners;
	{
		union
		{
			__m128 vector;
			unsigned long bits[4];
		} quantized;
		unsigned short low_words[3];
		__m128 const ceiling = *(__m128 const *)g_4125d0;
		__m128 const floor = *(__m128 const *)g_4125e0;
		quantized.vector = _mm_add_ps(_mm_max_ps(_mm_min_ps(
			_mm_mul_ps(_mm_add_ps(bounds->lower, lower_offset), scale), ceiling), floor), *(__m128 const *)g_4125c0);
		low_words[0] = (unsigned short)(quantized.bits[0] >> 7);
		low_words[1] = (unsigned short)(quantized.bits[1] >> 7);
		low_words[2] = (unsigned short)(quantized.bits[2] >> 7);
		quantized.vector = _mm_add_ps(_mm_max_ps(_mm_min_ps(
			_mm_mul_ps(_mm_add_ps(bounds->upper, upper_offset), scale), ceiling), floor), *(__m128 const *)g_4125c0);
		corners.high[0] = (unsigned short)(quantized.bits[0] >> 7) | 1;
		corners.low[0] = low_words[0] & 0xfffe;
		corners.high[1] = (unsigned short)(quantized.bits[1] >> 7) | 1;
		corners.low[1] = low_words[1] & 0xfffe;
		corners.high[2] = (unsigned short)(quantized.bits[2] >> 7) | 1;
		corners.low[2] = low_words[2] & 0xfffe;
	}
	long const index = elements.size;
	element_2e1090 *box = &elements.next_slot();
	element_2e1090 *box_data = elements.data;
	unsigned short *x_low_slot = &box->slots[4];
	unsigned short *x_high_slot = &box->slots[5];
	axes[0].function_2e0f80((long)box_data, index, (unsigned short)corners.low[0], (unsigned short)corners.high[0], x_low_slot, x_high_slot);
	axes[1].function_2e0f80((long)box_data, index, (unsigned short)corners.low[1], (unsigned short)corners.high[1], &box->slots[0], &box->slots[2]);
	axes[2].function_2e0f80((long)box_data, index, (unsigned short)corners.low[2], (unsigned short)corners.high[2], &box->slots[1], &box->slots[3]);
	((quantized_boxes_2dfcc0 *)this)->function_2dfcc0((quantized_box_2dfcc0 *)box_data, index, box->slots);
	box->owner = (long)owner;
	*owner = index;

	// one bit per box: set for the boxes whose x interval holds the new
	// box's lower x
	long const count = elements.size;
	longs_2e1ab0 overlaps((count >> 5) + 4);
	long x_high = *x_high_slot;
	long x_low = *x_low_slot;
	long *bits = overlaps.data;
	float *block = (float *)bits;
	__m128 const zero = _mm_setzero_ps();
	for (long blocks = count >> 7; blocks >= 0; blocks--)
	{
		_mm_store_ps(block, zero);
		block += 4;
	}
	endpoint_2e0f80 *endpoints = axes[0].data;
	endpoint_2e0f80 *endpoint = endpoints + 1;
	if (cells_used)
	{
		// start from the grid cell's box: the boxes it holds, less those
		// that start inside it
		long const grid_slot = (long)corners.low[0] >> (16 - cell_shift);
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
			for (endpoint_2e0f80 *inside = &axes[0].data[cell_box->slots[4] + 1]; inside < &axes[0].data[cell_box->slots[5]]; inside++)
			{
				if ((inside->value & 1) == 0)
				{
					clear_bit_2e1090(bits, inside->id);
				}
			}
			endpoints = axes[0].data;
			endpoint = &endpoints[cell_box->slots[4] + 1];
		}
	}
	for (endpoint_2e0f80 *end = &endpoints[x_low]; endpoint < end; endpoint++)
	{
		toggle_bit_2e1090(bits, endpoint->id);
	}
	endpoint++;
	for (endpoint_2e0f80 *end = &axes[0].data[x_high]; endpoint < end; endpoint++)
	{
		if ((endpoint->value & 1) == 0)
		{
			toggle_bit_2e1090(bits, endpoint->id);
		}
	}

	// of those, the ones whose y and z intervals overlap too
	element_2e1090 *group = elements.data;
	unsigned long *words_end = (unsigned long *)bits + (elements.size >> 5) + 1;
	for (unsigned long *word = (unsigned long *)bits; word < words_end; word++, group += 32)
	{
		unsigned long found = *word;
		element_2e1090 *other = group;
		while (found)
		{
			if ((found & 0xff) == 0)
			{
				other += 8;
				found >>= 8;
				continue;
			}
			if (found & 1)
			{
				unsigned long const *box_pairs = (unsigned long const *)box->slots;
				unsigned long const *other_pairs = (unsigned long const *)other->slots;
				unsigned long const apart = ((box_pairs[1] - other_pairs[0]) | (other_pairs[1] - box_pairs[0])) & 0x80008000;
				if (!apart)
				{
					long const other_owner = other->owner;
					if ((other_owner & 1) == 0)
					{
						pair_2e1090 pair;
						pair.first = owner;
						pair.second = other_owner;
						pairs->append(pair);
					}
					else
					{
						((cell_2e1090 *)((char *)cells + (other_owner & ~1)))->boxes.append((unsigned short)index);
					}
				}
			}
			other++;
			found >>= 1;
		}
	}
}

// score 504 (was 504): retail's order of steps, 56 bytes short. Retail
// is short of registers where ours is not: it keeps `this` in esi, the box in
// edi, the lower x slot (scaled by 4) in ebx and the bits in eax, and spills
// the element count, the clearing loop's counter and the overlap walk's box
// pointer to the stack, reloading them at each use; ours keeps `this` in edi,
// the box in esi and the counters in registers, so retail's extra spills and
// reloads (about a dozen instructions) are missing throughout. Reordering the
// early locals or reading the count again did not change the code.
// takes a box out of the sweep: every box it overlaps loses its pair (or,
// for a grid cell's box, the cell loses it), its endpoints leave each axis,
// and the last box moves into its place
// @retail 0x2e29b0
void class_2e1090::function_2e29b0(long *owner, pairs_2e1090 *pairs)
{
	long const count = elements.size;
	unsigned long const index = *owner;
	element_2e1090 *box = &elements.data[index];
	{
		// one bit per box: set for the boxes whose x interval holds the
		// box's lower x
		longs_2e1ab0 overlaps((count >> 5) + 4);
		long const x_high = box->slots[5];
		long const x_low = box->slots[4];
		long const x_low_value = axes[0].data[x_low].value;
		long *bits = overlaps.data;
		float *block = (float *)bits;
		__m128 const zero = _mm_setzero_ps();
		for (long blocks = count >> 7; blocks >= 0; blocks--)
		{
			_mm_store_ps(block, zero);
			block += 4;
		}
		endpoint_2e0f80 *endpoint = axes[0].data + 1;
		if (cells_used)
		{
			// start from the grid cell's box: the boxes it holds, less those
			// that start inside it
			long const grid_slot = x_low_value >> (16 - cell_shift);
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
				for (endpoint_2e0f80 *inside = &axes[0].data[cell_box->slots[4] + 1]; inside < &axes[0].data[cell_box->slots[5]]; inside++)
				{
					if ((inside->value & 1) == 0)
					{
						clear_bit_2e1090(bits, inside->id);
					}
				}
				endpoint = &axes[0].data[cell_box->slots[4] + 1];
			}
		}
		for (endpoint_2e0f80 *end = &axes[0].data[x_low]; endpoint < end; endpoint++)
		{
			toggle_bit_2e1090(bits, endpoint->id);
		}
		endpoint++;
		for (endpoint_2e0f80 *end = &axes[0].data[x_high]; endpoint < end; endpoint++)
		{
			if ((endpoint->value & 1) == 0)
			{
				toggle_bit_2e1090(bits, endpoint->id);
			}
		}

		// of those, the ones whose y and z intervals overlap too
		element_2e1090 *group = elements.data;
		unsigned long *words_end = (unsigned long *)bits + (elements.size >> 5) + 1;
		for (unsigned long *word = (unsigned long *)bits; word < words_end; word++, group += 32)
		{
			unsigned long found = *word;
			element_2e1090 *other = group;
			while (found)
			{
				if ((found & 0xff) == 0)
				{
					other += 8;
					found >>= 8;
					continue;
				}
				if (found & 1)
				{
					unsigned long const *box_pairs = (unsigned long const *)box->slots;
					unsigned long const *other_pairs = (unsigned long const *)other->slots;
					unsigned long const apart = ((box_pairs[1] - other_pairs[0]) | (other_pairs[1] - box_pairs[0])) & 0x80008000;
					if (!apart)
					{
						long const other_owner = other->owner;
						if ((other_owner & 1) == 0)
						{
							pair_2e1090 pair;
							pair.first = owner;
							pair.second = other_owner;
							pairs->append(pair);
						}
						else
						{
							cell_2e1090 *cell = (cell_2e1090 *)((char *)cells + (other_owner & ~1));
							long const position = find_box_2e29b0(cell, (unsigned short)index);
							cell->boxes.size--;
							cell->boxes.data[position] = cell->boxes.data[cell->boxes.size];
						}
					}
				}
				other++;
				found >>= 1;
			}
		}
	}

	element_2e1090 *boxes = elements.data;
	((array_2dffa0 *)&axes[0])->remove_two(box->slots[4], box->slots[5]);
	((array_2dffa0 *)&axes[1])->remove_two(box->slots[0], box->slots[2]);
	((array_2dffa0 *)&axes[2])->remove_two(box->slots[1], box->slots[3]);
	((quantized_boxes_2dfcc0 *)this)->function_2dfdc0((quantized_box_2dfcc0 *)boxes, count, box->slots);

	// the last box moves into the freed place
	if (index < (unsigned long)(count - 1))
	{
		*box = elements.data[count - 1];
		axes[0].data[box->slots[4]].id = (unsigned short)index;
		axes[0].data[box->slots[5]].id = (unsigned short)index;
		long const moved_owner = box->owner;
		if ((moved_owner & 1) == 0)
		{
			axes[1].data[box->slots[0]].id = (unsigned short)index;
			axes[1].data[box->slots[2]].id = (unsigned short)index;
			axes[2].data[box->slots[1]].id = (unsigned short)index;
			axes[2].data[box->slots[3]].id = (unsigned short)index;
			*(long *)box->owner = index;
		}
		else
		{
			((cell_2e1090 *)((char *)cells + (moved_owner & ~1)))->box = (unsigned short)index;
		}
		if (cells_used && (box->owner & 1) == 0)
		{
			// the grid cells holding the moved box hold it under its new index
			endpoint_2e0f80 *x_axis = axes[0].data;
			unsigned short const x_low_slot = box->slots[4];
			unsigned short const shift = (unsigned short)(16 - cell_shift);
			long first_cell = x_axis[x_low_slot].value >> shift;
			if (first_cell > 0 && elements.data[cells[first_cell - 1].box].slots[5] > x_low_slot)
			{
				first_cell--;
			}
			long const last_cell = (x_axis[box->slots[5]].value >> shift) - 1;
			for (long cell_slot = first_cell; cell_slot <= last_cell; cell_slot++)
			{
				cell_2e1090 *cell = &cells[cell_slot];
				cell->boxes.data[find_box_2e29b0(cell, (unsigned short)(count - 1))] = (unsigned short)index;
			}
		}
	}
	elements.resize(count - 1);
}
