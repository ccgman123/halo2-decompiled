// @flags /O2 /GL- /GX
// Havok (third-party physics library): adding a box to the three-axis sweep
// (axis_sweep_2e1090.h): its corners are quantized to 16 bits, its endpoints
// inserted into each axis's sorted list, and every box it overlaps reported
// as a pair (or, for a box that stands for a grid cell, added to that cell's
// list). (Its own file, not src/unknown_2e2190.cpp beside it: it needs /GX
// for the stack array it unwinds.)
#include "sweep_search_2e1090.h"

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

// one bit per box, set for the boxes whose x interval holds a box's lower
// x: the box's lower x value is `low_value`, and its endpoints sit at
// `low_slot` and `high_slot` on the x axis
__forceinline void mark_holding_x_2e24b0(class_2e1090 *sweep, int count, int low_value, int low_slot, int high_slot, long *bits)
{
	clear_blocks_2e1090(bits, count >> 3);
	endpoint_2e0f80 *data = sweep->axes[0].data;
	endpoint_2e0f80 *endpoint = data + 1;
	if (sweep->cells_used)
	{
		// start from the grid cell's box: the boxes it holds, less those
		// that start inside it
		int const slot = low_value >> (16 - sweep->cell_shift);
		if (slot > 0)
		{
			cell_2e1090 *cell = &sweep->cells[slot - 1];
			flip_bit_2e1090(bits, cell->box);
			unsigned short *held = cell->boxes.data;
			for (int remaining = cell->boxes.size - 1; remaining >= 0; remaining--)
			{
				flip_bit_2e1090(bits, *held);
				held++;
			}
			element_2e1090 &cell_box = sweep->elements.data[cell->box];
			endpoint = sweep->axes[0].data + cell_box.slots[4] + 1;
			{
				endpoint_2e0f80 *end = sweep->axes[0].data + cell_box.slots[5];
				while (endpoint < end)
				{
					if ((endpoint->value & 1) == 0)
					{
						drop_bit_2e1090(bits, endpoint->id);
					}
					endpoint++;
				}
			}
			data = sweep->axes[0].data;
			endpoint = data + cell_box.slots[4] + 1;
		}
	}
	endpoint_2e0f80 *end = data + low_slot;
	while (endpoint < end)
	{
		flip_bit_2e1090(bits, endpoint->id);
		endpoint++;
	}
	// the box's own lower endpoint
	endpoint++;
	{
		end = sweep->axes[0].data + high_slot;
		while (endpoint < end)
		{
			if ((endpoint->value & 1) == 0)
			{
				flip_bit_2e1090(bits, endpoint->id);
			}
			endpoint++;
		}
	}
}

// of the marked boxes, each that overlaps `box` on y and z too: a plain box
// is reported as a pair with `owner`, and a grid cell's box gets `index` in
// its cell's list
__forceinline void report_added_2e24b0(class_2e1090 *sweep, long *owner, int index, long *bits,
	element_2e1090 &box, pairs_2e1090 *pairs)
{
	pair_2e1090 pair;
	pair.first = owner;
	element_2e1090 *group = sweep->elements.data;
	long *word = bits;
	long *end = bits + (sweep->elements.size >> 5) + 1;
	for (; word < end; group += 32, word++)
	{
		element_2e1090 *other = group;
		unsigned long found = *word;
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
				unsigned long const apart = apart_yz_2e1090(*other, box);
				if (!apart)
				{
					if ((other->owner & 1) == 0)
					{
						pair.second = other->owner;
						pairs->append(pair);
					}
					else
					{
						((cell_2e1090 *)((char *)sweep->cells + (other->owner & ~1)))->boxes.append((unsigned short)index);
					}
				}
			}
			other++;
			found >>= 1;
		}
	}
}

// of the marked boxes, each that overlaps `box` on y and z too: a plain box
// is reported as a pair with `owner`, and a grid cell's box loses `index`
// from its cell's list
__forceinline void report_removed_2e29b0(class_2e1090 *sweep, long *owner, unsigned long index, long *bits,
	element_2e1090 &box, pairs_2e1090 *pairs)
{
	pair_2e1090 pair;
	pair.first = owner;
	element_2e1090 *group = sweep->elements.data;
	long *word = bits;
	long *end = bits + (sweep->elements.size >> 5) + 1;
	for (; word < end; group += 32, word++)
	{
		element_2e1090 *other = group;
		unsigned long found = *word;
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
				unsigned long const apart = apart_yz_2e1090(box, *other);
				if (!apart)
				{
					if ((other->owner & 1) == 0)
					{
						pair.second = other->owner;
						pairs->append(pair);
					}
					else
					{
						cell_2e1090 *cell = (cell_2e1090 *)((char *)sweep->cells + (other->owner & ~1));
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

// score 106 (was 371): the quantizing (two-level helper, its steps as separate
// statements), the appends and the bit setter now match retail's code. Still:
// in the grid cell's walk ours reloads the cell pointer into edx where retail
// uses ecx; and in the pair scan retail keeps the found bits in ebx and the
// box in ecx, computing the box's upper minus the other's lower first, where
// ours has the bits in ecx, the box in eax and the other difference first
// (either argument order of the y/z test and either order of its terms
// compile alike), which shifts the scan's registers and its stack slots.
// @retail 0x2e24b0
void class_2e1090::function_2e24b0(long *owner, bounds_2e0020 const *bounds, pairs_2e1090 *pairs)
{
	quantized_box_2e1090 box;
	quantize_box_2e1090(this, *bounds, box);
	int const index = elements.size;
	element_2e1090 &element = elements.next_slot();
	element_2e1090 *data = elements.data;
	axes[0].function_2e0f80((long)data, index, (unsigned short)box.low[0], (unsigned short)box.high[0], &element.slots[4], &element.slots[5]);
	axes[1].function_2e0f80((long)data, index, (unsigned short)box.low[1], (unsigned short)box.high[1], &element.slots[0], &element.slots[2]);
	axes[2].function_2e0f80((long)data, index, (unsigned short)box.low[2], (unsigned short)box.high[2], &element.slots[1], &element.slots[3]);
	((quantized_boxes_2dfcc0 *)this)->function_2dfcc0((quantized_box_2dfcc0 *)data, index, element.slots);
	element.owner = (long)owner;
	*owner = index;
	{
		int const count = elements.size;
		longs_2e1ab0 overlaps((count >> 5) + 4);
		long *bits = overlaps.data;
		mark_holding_x_2e24b0(this, count, box.low[0], element.slots[4], element.slots[5], bits);
		report_added_2e24b0(this, owner, index, bits, element, pairs);
	}
}

// score 373 (was 504): the steps follow retail, with the bit setter and the
// removal scan as inline helpers taking their inputs as arguments. Still:
// retail keeps `this` in esi and the box in edi where ours has them the other
// way round (reading the owner's index first, a box pointer, a signed count
// compile alike), so most of the body differs in those two registers, and
// ours is 24 bytes short of retail.
// takes a box out of the sweep: every box it overlaps loses its pair (or,
// for a grid cell's box, the cell loses it), its endpoints leave each axis,
// and the last box moves into its place
// @retail 0x2e29b0
void class_2e1090::function_2e29b0(long *owner, pairs_2e1090 *pairs)
{
	unsigned long const count = elements.size;
	unsigned long const index = *owner;
	element_2e1090 &box = elements.data[index];
	{
		int const inner_count = elements.size;
		longs_2e1ab0 overlaps((inner_count >> 5) + 4);
		long *bits = overlaps.data;
		mark_holding_x_2e24b0(this, inner_count, axes[0].data[box.slots[4]].value, box.slots[4], box.slots[5], bits);
		report_removed_2e29b0(this, owner, index, bits, box, pairs);
	}

	element_2e1090 *data = elements.data;
	((array_2dffa0 *)&axes[0])->remove_two(box.slots[4], box.slots[5]);
	((array_2dffa0 *)&axes[1])->remove_two(box.slots[0], box.slots[2]);
	((array_2dffa0 *)&axes[2])->remove_two(box.slots[1], box.slots[3]);
	((quantized_boxes_2dfcc0 *)this)->function_2dfdc0((quantized_box_2dfcc0 *)data, count, box.slots);

	// the last box moves into the freed place
	if (index < count - 1)
	{
		box = elements.data[count - 1];
		axes[0].data[box.slots[4]].id = (unsigned short)index;
		axes[0].data[box.slots[5]].id = (unsigned short)index;
		if ((box.owner & 1) == 0)
		{
			axes[1].data[box.slots[0]].id = (unsigned short)index;
			axes[1].data[box.slots[2]].id = (unsigned short)index;
			axes[2].data[box.slots[1]].id = (unsigned short)index;
			axes[2].data[box.slots[3]].id = (unsigned short)index;
			*(long *)box.owner = index;
		}
		else
		{
			((cell_2e1090 *)((char *)cells + (box.owner & ~1)))->box = (unsigned short)index;
		}
		if (cells_used && (box.owner & 1) == 0)
		{
			// the grid cells holding the moved box hold it under its new index
			unsigned short const x_low_slot = box.slots[4];
			unsigned short const shift = (unsigned short)(16 - cell_shift);
			int first = axes[0].data[x_low_slot].value >> shift;
			if (first > 0 && elements.data[cells[first - 1].box].slots[5] > x_low_slot)
			{
				first--;
			}
			int const last = (axes[0].data[box.slots[5]].value >> shift) - 1;
			for (int m = first; m <= last; m++)
			{
				cell_2e1090 &cell = cells[m];
				long const position = find_box_2e29b0(&cell, (unsigned short)(count - 1));
				cell.boxes.data[position] = (unsigned short)index;
			}
		}
	}
	elements.resize(count - 1);
}
