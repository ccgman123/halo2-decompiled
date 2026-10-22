// @flags /O2 /GL-
// Havok (third-party physics library): a box placed by a transform: the
// absolute difference of two of its axes' rows scaled by each other's half
// extent, and one of its corners from a feature key.
#include "box_corners_413610.h"
#include <xmmintrin.h>

static __declspec(align(16)) const unsigned long absolute_mask_305b30[4] = {0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff};

__declspec(align(16)) const float g_corner_signs_413610[8][4] = {
	{1.0f, 1.0f, 1.0f, 0.0f},
	{-1.0f, 1.0f, 1.0f, 0.0f},
	{1.0f, -1.0f, 1.0f, 0.0f},
	{-1.0f, -1.0f, 1.0f, 0.0f},
	{1.0f, 1.0f, -1.0f, 0.0f},
	{-1.0f, 1.0f, -1.0f, 0.0f},
	{1.0f, -1.0f, -1.0f, 0.0f},
	{-1.0f, -1.0f, -1.0f, 0.0f},
};

// a vector of four copies of a float
inline __m128 splat_305b30(float value)
{
	__m128 const single = _mm_set_ss(value);
	return _mm_shuffle_ps(single, single, 0);
}

struct vector_305b30
{
	__m128 value;

	void set(float x, float y, float z)
	{
		float *components = (float *)&value;
		components[0] = x;
		components[1] = y;
		components[2] = z;
		components[3] = 0.0f;
	}

	void set_product(__m128 const &a, __m128 const &b)
	{
		value = _mm_mul_ps(a, b);
	}
};

class box_305b30
{
public:
	char unknown0[0x70];
	__m128 axes[3];
	__m128 position;
	float half_extents[4];

	void function_305b30(__m128 *out, long i, long j) const;
};

// |half_extents[j] * row i - half_extents[i] * row j| of the axes
// @retail 0x305b30
void box_305b30::function_305b30(__m128 *out, long i, long j) const
{
	vector_305b30 row;
	row.set(axes[0].m128_f32[i], axes[1].m128_f32[i], axes[2].m128_f32[i]);
	__m128 const scaled_i = _mm_mul_ps(splat_305b30(half_extents[j]), row.value);
	row.set(axes[0].m128_f32[j], axes[1].m128_f32[j], axes[2].m128_f32[j]);
	__m128 const scaled_j = _mm_mul_ps(splat_305b30(half_extents[i]), row.value);
	*out = _mm_sub_ps(scaled_i, scaled_j);
	*out = _mm_and_ps(_mm_sub_ps(scaled_i, scaled_j), *(__m128 const *)absolute_mask_305b30);
}

// a box's corner key: an id, then a word whose low byte's high nibble picks
// the corner and whose high byte's low nibble is kept in the vertex
struct corner_key_305bf0
{
	unsigned short id;
	union
	{
		unsigned short word;
		struct
		{
			unsigned char corner;
			unsigned char flags;
		};
	};
};

struct corner_305bf0
{
	__m128 world;
	vector_305b30 local;
	char unknown20[0x10];
	float tagged_one;
	long field_34;
	unsigned short id;
};

class box_corner_305bf0
{
public:
	char unknown0[0x30];
	__m128 half_extents;
	char unknown40[0x30];
	__m128 axes[3];
	__m128 position;

	void function_305bf0(corner_305bf0 *out, corner_key_305bf0 const *key) const;

	// a point placed by the transform
	void place(__m128 *out, __m128 const *point) const
	{
		__m128 const p = _mm_load_ps((float const *)point);
		*out = _mm_add_ps(_mm_add_ps(
			_mm_mul_ps(axes[0], _mm_shuffle_ps(p, p, 0x00)),
			_mm_mul_ps(axes[1], _mm_shuffle_ps(p, p, 0x55))),
			_mm_add_ps(_mm_mul_ps(axes[2], _mm_shuffle_ps(p, p, 0xaa)), position));
	}
};

// out: the corner the key picks, in the box's frame and placed by its
// transform; 1.0 with the key's high nibble in its top four bits; the id
// @retail 0x305bf0
void box_corner_305bf0::function_305bf0(corner_305bf0 *out, corner_key_305bf0 const *key) const
{
	out->id = key->id;
	float tagged = 1.0f;
	*(long *)&tagged |= (key->word >> 8) << 28;
	out->tagged_one = tagged;
	out->local.set_product(half_extents, *(__m128 const *)((char const *)g_corner_signs_413610 + (key->corner & 0xf0)));
	place(&out->world, &out->local.value);
}
