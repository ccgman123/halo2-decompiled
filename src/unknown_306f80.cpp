// @flags /O2 /GL-
// Havok (third-party physics library): the closest points of an edge of one
// box and an edge of another (each picked by its axis and the signs of a
// corner): found only where both edges' closest points lie strictly inside
// the edges, the edges are not parallel, and the boxes are within the
// tolerance; the normal points away from the first box.
#include "box_corners_413610.h"
#include <math.h>
#include <xmmintrin.h>

static __declspec(align(16)) const unsigned long g_4124a0[4] = {0x80000000, 0x80000000, 0x80000000, 0x80000000};

// the smallest step from one, kept in the data section
float g_461f10 = 1.1920929e-07f;

struct bool_3058b0
{
	bool value;

	bool_3058b0(bool value_)
	{
		value = value_;
	}
};

// the closest points of two features, their normal and distance, and the
// features' keys: the low nibble an axis, the high nibble a corner
struct feature_305f60
{
	__m128 first_point;
	__m128 second_point;
	__m128 normal;
	char unknown30[0x34 - 0x30];
	float distance;
	unsigned short kind;
	unsigned short field_3a;
};

// a vector of four copies of a float
inline __m128 splat_306f80(float value)
{
	__m128 const single = _mm_set_ss(value);
	return _mm_shuffle_ps(single, single, 0);
}

// the dot product of two vectors' first three terms
__forceinline float dot3_306f80(__m128 const &a, __m128 const &b)
{
	__m128 const product = _mm_mul_ps(a, b);
	float result;
	_mm_store_ss(&result, _mm_add_ss(_mm_shuffle_ps(product, product, 0xaa), _mm_add_ss(_mm_shuffle_ps(product, product, 0x55), product)));
	return result;
}

__forceinline __m128 cross_306f80(__m128 const &a, __m128 const &b)
{
	return _mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(a, a, 0xc9), _mm_shuffle_ps(b, b, 0xd2)),
		_mm_mul_ps(_mm_shuffle_ps(a, a, 0xd2), _mm_shuffle_ps(b, b, 0xc9)));
}

// one over the square root of the first term, refined by a Newton step, in
// all four terms
__forceinline __m128 inverse_sqrt_306f80(__m128 const &value)
{
	__m128 const estimate = _mm_rsqrt_ss(value);
	__m128 const product = _mm_mul_ss(_mm_mul_ss(value, estimate), estimate);
	__m128 const refined = _mm_mul_ss(_mm_mul_ss(_mm_set_ss(0.5f), estimate), _mm_sub_ss(_mm_set_ss(3.0f), product));
	return _mm_shuffle_ps(refined, refined, 0);
}

// a pair of boxes: their half extents, the tolerances, and the second box's
// placement in the first's frame
class pair_305f60
{
public:
	char unknown0[0x20];
	__m128 first_half_extents;
	__m128 second_half_extents;
	char unknown40[0x60 - 0x40];
	float tolerance;
	float minimum_length;
	char unknown68[0x70 - 0x68];
	__m128 axes[3];
	__m128 position;

	bool_3058b0 function_306f80(feature_305f60 *feature);
};

// score 159 (was 164): the x87 products now match (second_squared times a
// bracketed product, axes[1] first in its term). Still differs: retail keeps
// between in a 16-byte slot of its own (a 0x58 frame, here 0x48, where it
// shares the slot of the edge read by index), and holds the second box's
// corner signs and product in xmm1 where ours uses xmm0, so the xmm registers
// differ from there on; const, a union, declaring it (or first_direction) at
// the top, computing it earlier and swapped operands do not fix either.
// @retail 0x306f80
bool_3058b0 pair_305f60::function_306f80(feature_305f60 *feature)
{
	int const first_axis = feature->kind & 0xf;
	__m128 const first_signs = *(__m128 const *)((feature->kind & 0xf0) + (char const *)g_corner_signs_413610);
	unsigned short const second_kind = feature->field_3a;
	int const second_axis = 0xf & second_kind;
	__m128 const second_signs = *(__m128 const *)((char const *)g_corner_signs_413610 + (second_kind & 0xf0));
	__m128 first_corner = _mm_mul_ps(first_signs, first_half_extents);
	__m128 const second_local = _mm_mul_ps(second_half_extents, second_signs);
	__m128 second_edge = _mm_mul_ps(splat_306f80(second_half_extents.m128_f32[second_axis] + second_half_extents.m128_f32[second_axis]), axes[second_axis]);
	float const first_edge = first_half_extents.m128_f32[first_axis] * -2.0f;
	float const along = first_edge * second_edge.m128_f32[first_axis];
	__m128 const second_corner = _mm_add_ps(_mm_add_ps(
		_mm_mul_ps(axes[0], _mm_shuffle_ps(second_local, second_local, 0x00)),
		_mm_mul_ps(axes[1], _mm_shuffle_ps(second_local, second_local, 0x55))),
		_mm_add_ps(_mm_mul_ps(axes[2], _mm_shuffle_ps(second_local, second_local, 0xaa)), position));
	float const second_squared = second_half_extents.m128_f32[second_axis] * second_half_extents.m128_f32[second_axis] * 4.0f;
	float const determinant = (float)fabs(second_squared * (first_edge * first_edge) - along * along);
	__m128 between = _mm_sub_ps(second_corner, first_corner);
	float const second_along = dot3_306f80(second_edge, between);
	float t = second_squared * (first_edge * between.m128_f32[first_axis]) - second_along * along;
	if (!(g_461f10 * determinant >= t))
	{
		float const most = 1.0f - g_461f10;
		if (!(most * determinant <= t))
		{
			t = t / determinant;
			float const s = t * along - second_along;
			if (!(s <= second_squared * g_461f10))
			{
				if (!(s >= second_squared * most))
				{
					first_corner.m128_f32[first_axis] = t * first_edge + first_corner.m128_f32[first_axis];
					__m128 first_direction = _mm_setzero_ps();
					first_direction.m128_f32[first_axis] = first_edge * 0.5f;
					__m128 const normal = cross_306f80(first_direction, second_edge);
					__m128 const squares = _mm_mul_ps(normal, normal);
					__m128 const length_squared = _mm_add_ss(_mm_add_ss(_mm_shuffle_ps(squares, squares, 0x55), squares), _mm_shuffle_ps(squares, squares, 0xaa));
					__m128 const inverse = inverse_sqrt_306f80(length_squared);
					float length;
					_mm_store_ss(&length, _mm_mul_ss(inverse, length_squared));
					if (!(length < minimum_length))
					{
						__m128 unit = _mm_mul_ps(inverse, normal);
						if (dot3_306f80(first_signs, unit) < 0.0f)
						{
							unit = _mm_xor_ps(unit, *(__m128 const *)g_4124a0);
						}
						float const distance = dot3_306f80(unit, between);
						if (!(tolerance < distance))
						{
							feature->distance = distance;
							feature->first_point = first_corner;
							feature->normal = _mm_xor_ps(unit, *(__m128 const *)g_4124a0);
							return bool_3058b0(true);
						}
					}
				}
			}
		}
	}
	return bool_3058b0(false);
}