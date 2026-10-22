// @flags /O2 /GL-
// Havok (third-party physics library): a three-axis sweep of boxes
// (axis_sweep_2e1090.h): the boxes of all of them into an array, the box of
// one picked by an index the caller points to, and the boxes a box overlaps.
#include "sweep_search_2e1090.h"

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

// score 2 (was 373): the search is the shared inline of sweep_search_2e1090.h
// (also 0x2e3370's), and the whole body matches but for one pair of loads:
// in the y range search retail loads the bounds' low y key before the axis's
// size, ours the size first (the z search matches). A separate place-range
// helper, the key in a local, the size read through the axis pointer, and a
// reference to the range box all compile alike.
// every box the bounds overlap, as pairs with no first owner
// @retail 0x2e10b0
void class_2e1090::function_2e10b0(bounds_2e0020 const *bounds, pairs_2e1090 *pairs)
{
	long *first_owner = 0;
	search_boxes_2e1090(this, *bounds, first_owner, pairs, 0, SEARCH_REPORT_PAIRS_2E1090);
}