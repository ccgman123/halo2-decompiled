#ifndef SWEEP_BATCHES_2E50D0_H
#define SWEEP_BATCHES_2E50D0_H
// Havok (third-party physics library): the three-axis sweep
// (axis_sweep_2e1090.h) adding and taking out many boxes at once, and the
// pass that reports the pairs those boxes start or stop
// (src/unknown_2e50d0.cpp, src/stubs/lane_k1.cpp).
#include "axis_sweep_2e1090.h"

// a list of owners, each pointing at the word that holds its box's index
struct owners_2e50d0
{
	long **data;
	long size;
	long capacity_and_flags;
};

// whether the pairs the boxes take part in start (1) or stop (0), passed by
// value
struct pair_change_2e2f70
{
	char starting;

	pair_change_2e2f70(char value) : starting(value) {}
};

class sweep_batches_2e50d0 : public class_2e1090
{
public:
	// each box's lower x endpoint (its index and value) against the sweep:
	// the pairs they make, reported to `pairs`
	void function_2e2f70(endpoint_2e0f80 *endpoints, long count, pairs_2e1090 *pairs, pair_change_2e2f70 change);
	void function_2e50d0(owners_2e50d0 const *owners, bounds_array_2e1020 const *bounds, pairs_2e1090 *pairs);
	void function_2e5590(owners_2e50d0 const *owners, pairs_2e1090 *pairs);
};

#endif
