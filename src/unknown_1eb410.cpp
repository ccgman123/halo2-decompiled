// @flags /O2 /Gr /GL-
#include "unknown_1efac0.h"

struct s_shape_array16
{
	void *data;
	long count;
	long capacity;
	__forceinline ~s_shape_array16()
	{
		if (!(capacity & 0x80000000))
			g_480118->allocate((long)data, (capacity & 0x7fffffff) * 0x10, 0x12);
	}
};

struct s_shape_array48
{
	void *data;
	long count;
	long capacity;
	__forceinline ~s_shape_array48()
	{
		if (!(capacity & 0x80000000))
			g_480118->allocate((long)data, (capacity & 0x7fffffff) * 0x30, 0x12);
	}
};

struct c_shape_arrays : c_a
{
	byte field_8[0x30 - 8];
	s_shape_array48 first;
	byte field_3c[0xd4 - 0x3c];
	s_shape_array16 second;
	void operator delete(void *block)
	{
		g_480118->allocate((long)block, ((c_a *)block)->flags, 0x22);
	}
};

// @retail 0x1eb410 deleting c_shape_arrays

// @retail 0x1eb440 destructor c_shape_arrays

