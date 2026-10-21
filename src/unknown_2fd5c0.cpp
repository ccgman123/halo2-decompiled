// @flags /O2 /GL-
// Havok (third-party physics library): a convex shape bounded by planes and
// spanned by vertices stored four to a block: a ray clipped against its
// planes, and its vertices copied out one to a vector with the shape's
// radius as fourth term.
#include <xmmintrin.h>

// the one-byte answer of a test, returned through a hidden pointer
class answer_2fd5c0
{
public:
	answer_2fd5c0() {}
	answer_2fd5c0(bool value_) : value((char)value_) {}

	char value;
};

// a segment from one point to another
struct ray_2fd5c0
{
	__m128 from;
	__m128 to;
};

// what a cast fills in: the normal, the key and the fraction along the ray
struct hit_2fd5c0
{
	__m128 normal;
	long key;
	float fraction;
};

// four vertices: their x, y and z terms
struct vertex_block_2fd730
{
	float x[4];
	float y[4];
	float z[4];
};

class convex_2fd5c0
{
public:
	char unknown0[0xc];
	float radius;
	char unknown10[0x20];
	vertex_block_2fd730 *vertex_blocks;
	char unknown34[0x9c];
	long vertex_count;
	__m128 *planes;
	long planes_used;

	answer_2fd5c0 function_2fd5c0(ray_2fd5c0 const *ray, hit_2fd5c0 *hit) const;
	__m128 *function_2fd730(__m128 *out) const;
};

// a point's signed distance from a plane (normal and offset)
inline float plane_distance_2fd5c0(__m128 const &plane, __m128 const &offset, __m128 const &point)
{
	__m128 const product = _mm_mul_ps(plane, point);
	float result;
	_mm_store_ss(&result, _mm_add_ps(_mm_add_ss(_mm_shuffle_ps(product, product, 0x55), product), _mm_add_ss(_mm_shuffle_ps(product, product, 0xaa), offset)));
	return result;
}

// the ray clipped against every plane: where it enters the last plane it
// crosses inward, unless it leaves first or starts behind none of them
// @retail 0x2fd5c0
answer_2fd5c0 convex_2fd5c0::function_2fd5c0(ray_2fd5c0 const *ray, hit_2fd5c0 *hit) const
{
	float enter = -1.0f;
	float leave = hit->fraction;
	__m128 normal;
	for (int index = planes_used - 1; index >= 0; index--)
	{
		__m128 const plane = planes[index];
		__m128 const offset = _mm_shuffle_ps(plane, plane, 0xff);
		float const from_distance = plane_distance_2fd5c0(plane, offset, ray->from);
		float const to_distance = plane_distance_2fd5c0(plane, offset, ray->to);
		if (from_distance >= 0.0f)
		{
			if (to_distance >= 0.0f)
			{
				return false;
			}
			float const fraction = from_distance / (from_distance - to_distance);
			if (enter <= fraction)
			{
				enter = fraction;
				normal = plane;
			}
		}
		else
		{
			if (!(to_distance >= 0.0f))
			{
				continue;
			}
			float const fraction = from_distance / (from_distance - to_distance);
			if (!(leave < fraction))
			{
				leave = fraction;
			}
		}
		if (leave < enter)
		{
			return false;
		}
	}
	if (enter < 0.0f)
	{
		return false;
	}
	hit->normal = normal;
	hit->fraction = enter;
	hit->key = -1;
	return true;
}

// a vector set from four floats
inline void set_2fd730(__m128 &vector, float x, float y, float z, float w)
{
	float *terms = (float *)&vector;
	terms[0] = x;
	terms[1] = y;
	terms[2] = z;
	terms[3] = w;
}

// a vector a copy is stored in
struct vertex_2fd730
{
	__m128 value;

	void copy(__m128 const &vector)
	{
		value = vector;
	}
};

// @retail 0x2fd730
__m128 *convex_2fd5c0::function_2fd730(__m128 *out) const
{
	vertex_block_2fd730 const *block = vertex_blocks;
	vertex_2fd730 *next = (vertex_2fd730 *)out;
	__m128 vertex;
	int index = vertex_count - 1;
	for (; index >= 3; index -= 4)
	{
		set_2fd730(vertex, block->x[0], block->y[0], block->z[0], radius);
		(next++)->copy(vertex);
		set_2fd730(vertex, block->x[1], block->y[1], block->z[1], radius);
		(next++)->copy(vertex);
		set_2fd730(vertex, block->x[2], block->y[2], block->z[2], radius);
		(next++)->copy(vertex);
		set_2fd730(vertex, block->x[3], block->y[3], block->z[3], radius);
		(next++)->copy(vertex);
		block++;
	}
	if (index >= 0)
	{
		set_2fd730(vertex, block->x[0], block->y[0], block->z[0], radius);
		(next++)->copy(vertex);
	}
	if (index >= 1)
	{
		set_2fd730(vertex, block->x[1], block->y[1], block->z[1], radius);
		(next++)->copy(vertex);
	}
	if (index >= 2)
	{
		set_2fd730(vertex, block->x[2], block->y[2], block->z[2], radius);
		next->copy(vertex);
	}
	return out;
}
