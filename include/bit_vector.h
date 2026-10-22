/* testing and setting one bit of a bit vector held as dwords, which several files expand inline
   (src/unknown_272b70.cpp and src/unknown_2c0d00.cpp still carry their own copies) */

#ifndef BIT_VECTOR_H
#define BIT_VECTOR_H

#include "unknown_11c920.h"

/* whether the bit is set */
static inline bool bit_vector_test(dword const *vector, long bit)
{
	return (vector[bit >> 5] & (1 << (bit & 31))) != 0;
}

/* sets or clears the bit */
static inline void bit_vector_set(dword *vector, long bit, bool value)
{
	dword *word = &vector[bit >> 5];
	if (value)
		*word |= 1 << (bit & 31);
	else
		*word &= ~(1 << (bit & 31));
}

#endif
