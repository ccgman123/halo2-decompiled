// @flags /O2 /GL- /GX
// Havok (third-party physics library): a convex shape made from a list of
// points and a list of planes: the points kept four to a block, each block
// holding the four x, then the four y, then the four z (the last block padded
// with copies of the last point), with the box around them as a centre and
// half extents.
#include "shape_2de750.h"
#include "growable_array_2d91d0.h"

void __cdecl function_2fe730(const void *points, long count, long stride, void *output);

struct bounds_2fdaa0
{
	__m128 lower;
	__m128 upper;
};

struct vector_2fdaa0
{
	__m128 value;

	void set_add(__m128 const &a, __m128 const &b)
	{
		value = _mm_add_ps(a, b);
	}

	void set_sub(__m128 const &a, __m128 const &b)
	{
		value = _mm_sub_ps(a, b);
	}

	void scale(float s)
	{
		__m128 const single = _mm_set_ss(s);
		value = _mm_mul_ps(_mm_shuffle_ps(single, single, 0), value);
	}
};

// four points: their x, their y and their z
struct block_2fdaa0
{
	__m128 x;
	__m128 y;
	__m128 z;
};

// the blocks, with room for three in the object itself
struct blocks_2fdaa0
{
	ELEMENTS_2D91D0_MEMBERS(block_2fdaa0)
	block_2fdaa0 storage[3];

	blocks_2fdaa0() : data(storage), size(0), capacity_and_flags(3 | 0x80000000) {}
	~blocks_2fdaa0()
	{
		if ((capacity_and_flags & 0x80000000) == 0)
		{
			g_memory_2d88a0->release_block(data, (capacity_and_flags & 0x7fffffff) * sizeof(block_2fdaa0), 0x12);
		}
	}

	// point i's coordinates in its block (the index taken as unsigned)
	void set_point(unsigned int i, float const *point)
	{
		data[i >> 2].x.m128_f32[i & 3] = point[0];
		data[i >> 2].y.m128_f32[i & 3] = point[1];
		data[i >> 2].z.m128_f32[i & 3] = point[2];
	}
};

// planes given by their address and count
struct plane_list_2fdcf0
{
	__m128 const *planes;
	long count;
};

struct planes_2fdcf0
{
	__m128 *data;
	long size;
	unsigned long capacity_and_flags;

	planes_2fdcf0() : data(0), size(0), capacity_and_flags(0x80000000) {}
	~planes_2fdcf0()
	{
		if ((capacity_and_flags & 0x80000000) == 0)
		{
			g_memory_2d88a0->release_block(data, (capacity_and_flags & 0x7fffffff) * sizeof(__m128), 0x12);
		}
	}

	// a copy of the planes, in storage of exactly that many when there is
	// not room
	__forceinline void assign(plane_list_2fdcf0 const &source)
	{
		long const &count = source.count;
		long capacity = capacity_and_flags & 0x7fffffff;
		if (capacity < count)
		{
			if ((capacity_and_flags & 0x80000000) == 0)
			{
				g_memory_2d88a0->release_block(data, capacity * sizeof(__m128), 0x12);
			}
			long const plane_count = count;
			data = (__m128 *)g_memory_2d88a0->allocate_block(plane_count * sizeof(__m128), 0x12);
			capacity_and_flags = count;
		}
		__m128 *const out = data;
		size = count;
		long const n = count;
		__m128 const *const in = source.planes;
		for (int i = 0; i < n; i++)
		{
			out[i] = in[i];
		}
	}
};

// what the shape is made from
struct info_2fdcf0
{
	long user_value;
	float radius;
	float const *points;
	long point_count;
	long field_10;
	plane_list_2fdcf0 planes;
};

class class_2fdcf0 : public radius_shape_2de750
{
public:
	REFERENCED_NEW_2D88A0(0x22)

	vector_2fdaa0 half_extents;
	vector_2fdaa0 centre;
	blocks_2fdaa0 blocks;
	long point_count;
	planes_2fdcf0 planes;

	class_2fdcf0(void const *source);
	void function_2fdaa0(float const *points, long stride, long count);
};

// the points, `stride` bytes apart, kept in blocks of four, and the box
// around them
// @retail 0x2fdaa0
void class_2fdcf0::function_2fdaa0(float const *points, long stride, long count)
{
	long const padded = (count + 3) & ~3;
	blocks_2fdaa0 *const list = &blocks;
	point_count = count;
	list->resize(padded / 4);
	char const *point = (char const *)points;
	int i;
	for (i = 0; i < count; i++)
	{
		list->set_point(i, (float const *)point);
		point += stride;
	}
	point -= stride;
	for (; i < padded; i++)
	{
		float const *last = (float const *)point;
		list->data[i >> 2].x.m128_f32[i & 3] = last[0];
		list->data[i >> 2].y.m128_f32[i & 3] = last[1];
		list->data[i >> 2].z.m128_f32[i & 3] = last[2];
	}
	bounds_2fdaa0 bounds;
	function_2fe730(points, count, stride, &bounds);
	centre.set_add(bounds.lower, bounds.upper);
	centre.scale(0.5f);
	half_extents.set_sub(bounds.upper, bounds.lower);
	half_extents.scale(0.5f);
}

// @retail 0x2fdcf0
class_2fdcf0::class_2fdcf0(void const *source) :
	radius_shape_2de750(*(radius_info_2de750 const *)source)
{
	info_2fdcf0 const *info = (info_2fdcf0 const *)source;
	planes.assign(info->planes);
	function_2fdaa0(info->points, 0x10, info->point_count);
}
