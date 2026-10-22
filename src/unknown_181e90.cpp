// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_181E90.CPP: whether the ratio of two lengths lies outside the
   allowed range */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "globals.h"
#include <xmmintrin.h>

/* the smallest allowed ratio (set at run time) */
real g_54e870;

// @retail 0x181e90
bool function_181e90(real numerator, real denominator)
{
	if (!(numerator < 0.001f) && !(denominator < 0.001f))
	{
		real ratio = numerator / denominator;
		real pinned = ratio < g_54e870 ? g_54e870 : (ratio > 16.0f ? 16.0f : ratio);
		if (pinned != ratio)
		{
			return true;
		}
	}
	return false;
}

struct s_scale_definition;
struct s_scale_owner
{
	byte unknown00[0x3c];
	s_scale_definition *definition;
	real get_inverse_scale() const;
};

struct s_181ee0
{
	byte field_0[0x2c];
	real field_2c;
};

PRIVATE __forceinline real function_181ee1(s_scale_owner const *arg_0)
{
	real local_0 = ((s_181ee0 const *)arg_0->definition)->field_2c;
	real local_1;
	if (local_0 == 0.0f)
		local_1 = 0.0f;
	else
		local_1 = 1.0f / local_0;
	return local_1;
}

// @retail 0x181ee0
void __cdecl function_181ee0(s_scale_owner const *a, s_scale_owner const *b, real *scale_a, real *scale_b)
{
	real inverse_a = function_181ee1(a);
	real inverse_b = function_181ee1(b);

	*scale_a = 1.0f;
	*scale_b = 1.0f;
	if (!(inverse_a < 0.001f) && !(inverse_b < 0.001f))
	{
		real ratio = inverse_a / inverse_b;
		if (ratio > 0.001f)
		{
			if (ratio < g_54e870)
			{
				*scale_a = ratio / g_54e870;
				*scale_b = 1.0f;
			}
			else if (ratio > 16.0f)
			{
				*scale_a = 16.0f / ratio;
				*scale_b = 1.0f;
			}
		}
	}
}
/* a Havok transform: three rotation columns and a translation */
struct s_havok_transform
{
	__m128 rotation[3];
	__m128 translation;
};

static __forceinline void havok_vector4_set(__m128 *vector, real x, real y, real z, real w)
{
	*(real volatile *)&vector->m128_f32[0] = x;
	*(real volatile *)&vector->m128_f32[1] = y;
	*(real volatile *)&vector->m128_f32[2] = z;
	*(real volatile *)&vector->m128_f32[3] = w;
}

// @retail 0x181f80
void function_181f80(s_havok_transform *transform, transform4x3f const *matrix)
{
	havok_vector4_set(&transform->rotation[0], matrix->forward.i, matrix->forward.j, matrix->forward.k, 0.0f);
	havok_vector4_set(&transform->rotation[1], matrix->left.i, matrix->left.j, matrix->left.k, 0.0f);
	havok_vector4_set(&transform->rotation[2], matrix->up.i, matrix->up.j, matrix->up.k, 0.0f);
	transform->translation = _mm_set_ps(0.0f, matrix->position.z, matrix->position.y, matrix->position.x);
}
struct s_motion_entry
{
	byte field_0[0xa];
	short basis_index;
	byte field_c[4];
	long tag_index;
};

struct s_motion_basis
{
	vector3f first;
	vector3f second;
};

struct s_motion_globals_view
{
	byte field_0[0x10];
	s_motion_entry *entries;
	byte field_14[0xb4 - 0x14];
	long basis_count;
	s_motion_basis *bases;
};

bool function_355e0(long tag, long index, real *first, real *second);

// @retail 0x181db0
bool function_181db0(long index, vector3f *result)
{
	s_motion_entry *entry = &((s_motion_globals_view *)g_4e0348)->entries[index];
	s_motion_globals_view *globals = (s_motion_globals_view *)g_4e0348;
	bool valid = false;
	if (entry->basis_index != NONE && globals->basis_count > entry->basis_index)
	{
		s_motion_basis *basis = &globals->bases[entry->basis_index];
		real first, second;
		valid = function_355e0(entry->tag_index, 0, &first, &second);
		if (!valid)
			valid = function_355e0(entry->tag_index, 1, &first, &second);
		if (valid)
		{
			first = 0.0f - first;
			second = 0.0f - second;
			vector3f *out = *(vector3f *const volatile *)&result;
			out->i = (real)(basis->second.i * second) + (real)(basis->first.i * first);
			out->j = (real)(basis->second.j * second) + (real)(basis->first.j * first);
			out->k = (real)(basis->second.k * second) + (real)(basis->first.k * first);
		}
	}
	return valid;
}
