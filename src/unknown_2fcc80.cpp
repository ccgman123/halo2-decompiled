// @flags /O2 /GL-
// Havok (third-party physics library): a ray cast against a triangle: the
// ray must cross the triangle's plane (or start on it) before the nearest hit
// so far, at a point inside all three edges (with a small tolerance); the
// hit's normal faces the ray's start and comes back normalized.
#include <xmmintrin.h>

static __declspec(align(16)) const float g_412420[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static __declspec(align(16)) const unsigned long g_4124a0[4] = {0x80000000, 0x80000000, 0x80000000, 0x80000000};

struct ray_input_2fcc80
{
	__m128 from;
	__m128 to;
};

struct hit_2fcc80
{
	bool value;
};

// the dot product of two vectors' first three terms
__forceinline float dot3_2fcc80(__m128 const &a, __m128 const &b)
{
	__m128 const product = _mm_mul_ps(a, b);
	float result;
	_mm_store_ss(&result, _mm_add_ss(_mm_shuffle_ps(product, product, 0xaa), _mm_add_ss(_mm_shuffle_ps(product, product, 0x55), product)));
	return result;
}

__forceinline __m128 cross_2fcc80(__m128 const &a, __m128 const &b)
{
	__m128 const b_yzx = _mm_shuffle_ps(b, b, 0xc9);
	__m128 const b_zxy = _mm_shuffle_ps(b, b, 0xd2);
	__m128 const a_zxy = _mm_shuffle_ps(a, a, 0xd2);
	__m128 const a_yzx = _mm_shuffle_ps(a, a, 0xc9);
	__m128 const second = _mm_mul_ps(a_zxy, b_yzx);
	return _mm_sub_ps(_mm_mul_ps(a_yzx, b_zxy), second);
}

// one over the square root of the first term, refined by a Newton step, in
// all four terms
__forceinline __m128 inverse_sqrt_2fcc80(__m128 const &value)
{
	__m128 const estimate = _mm_rsqrt_ss(value);
	__m128 const product = _mm_mul_ss(_mm_mul_ss(value, estimate), estimate);
	__m128 const refined = _mm_mul_ss(_mm_mul_ss(_mm_set_ss(0.5f), estimate), _mm_sub_ss(_mm_set_ss(3.0f), product));
	return _mm_shuffle_ps(refined, refined, 0);
}

// a vector, which can be normalized in place
struct vector_2fcc80
{
	__m128 value;

	__forceinline void scale_to_unit_length()
	{
		__m128 const squares = _mm_mul_ps(value, value);
		__m128 const sum = _mm_add_ss(_mm_shuffle_ps(squares, squares, 0xaa), _mm_add_ss(_mm_shuffle_ps(squares, squares, 0x55), squares));
		value = _mm_mul_ps(inverse_sqrt_2fcc80(sum), value);
	}
};

struct ray_output_2fcc80
{
	vector_2fcc80 normal;
	long key;
	float fraction;
};

class triangle_2fcc80
{
public:
	char unknown0[0x10];
	__m128 corners[3];

	hit_2fcc80 function_2fcc80(ray_input_2fcc80 const *input, ray_output_2fcc80 *output) const;
};

// @retail 0x2fcc80
hit_2fcc80 triangle_2fcc80::function_2fcc80(ray_input_2fcc80 const *input, ray_output_2fcc80 *output) const
{
	hit_2fcc80 hit;
	__m128 normal = _mm_sub_ps(corners[1], corners[0]);
	normal = cross_2fcc80(normal, _mm_sub_ps(corners[2], corners[0]));
	float const offset = dot3_2fcc80(corners[0], normal);
	float const from_side = dot3_2fcc80(normal, input->from) - offset;
	float const to_side = dot3_2fcc80(normal, input->to) - offset;
	if (from_side * to_side >= 0.0f && (from_side != 0.0f || to_side == 0.0f))
	{
		hit.value = false;
		return hit;
	}
	{
		float const fraction = from_side / (from_side - to_side);
		if (fraction < output->fraction)
		{
			float const length_squared = dot3_2fcc80(normal, normal);
			float const tolerance = length_squared * -0.0001f;
			__m128 const f = _mm_set1_ps(fraction);
			__m128 const point = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(*(__m128 const *)g_412420, f), input->from), _mm_mul_ps(f, input->to));
			__m128 const a = _mm_sub_ps(corners[0], point);
			__m128 const b = _mm_sub_ps(corners[1], point);
			if (dot3_2fcc80(cross_2fcc80(a, b), normal) >= tolerance)
			{
				__m128 const c = _mm_sub_ps(corners[2], point);
				if (dot3_2fcc80(cross_2fcc80(b, c), normal) >= tolerance
					&& dot3_2fcc80(cross_2fcc80(c, a), normal) >= tolerance)
				{
					output->fraction = fraction;
					if (!(from_side > 0.0f))
					{
						output->normal.value = _mm_xor_ps(normal, *(__m128 const *)g_4124a0);
					}
					else
					{
						output->normal.value = normal;
					}
					output->normal.scale_to_unit_length();
					output->key = -1;
					hit.value = true;
					return hit;
				}
			}
		}
	}
	hit.value = false;
	return hit;
}
