// @flags /O2 /GL-
// Havok (third-party physics library): cutting five contact points down to
// the four that span the most: the point farthest from the first, the point
// farthest from that, the point farthest from the line through those two, and
// the point farthest from that; the one left over is handed back to its owner
// and replaced by the last. And adding a contact point to the kept ones.
#include <xmmintrin.h>
#include "contact_points_302a10.h"

// the squared length of a vector's first three terms
inline float length_squared_302a10(__m128 v)
{
	__m128 squares = _mm_mul_ps(v, v);
	float result;
	_mm_store_ss(&result, _mm_add_ss(_mm_shuffle_ps(squares, squares, 0xaa), _mm_add_ss(_mm_shuffle_ps(squares, squares, 0x55), squares)));
	return result;
}

// the cross product of two vectors' first three terms
inline __m128 cross_302a10(__m128 const &a, __m128 const &b)
{
	return _mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(a, a, 0xc9), _mm_shuffle_ps(b, b, 0xd2)),
		_mm_mul_ps(_mm_shuffle_ps(a, a, 0xd2), _mm_shuffle_ps(b, b, 0xc9)));
}

// score 30 (was 102): the dropped point is copied field by field through a
// source and a destination of its own, and the five is set before the used
// flags; ours still keeps the second point's offset in ebx where retail keeps
// it in edx, and so holds the third point in a register instead of reloading
// it, which moves the cross product's registers
// @retail 0x302a10
int __cdecl function_302a10(point_302a10 **points, void *arg_1, void *arg_2, owner_302a10 *owner)
{
	int count = 5;
	bool used[5];
	for (int k = 0; k < 5; k++)
	{
		used[k] = false;
	}
	int best = 0;

	point_302a10 const *first = points[0];
	float largest = 0.0f;
	for (int i = 0; i < 5; i++)
	{
		if (!used[i] && i != best)
		{
			float distance = length_squared_302a10(_mm_sub_ps(first->position, points[i]->position));
			if (distance > largest)
			{
				largest = distance;
				best = i;
			}
		}
	}
	int second_index = best;
	used[best] = true;

	point_302a10 const *second = points[second_index];
	largest = 0.0f;
	for (int i = 0; i < 5; i++)
	{
		if (!used[i])
		{
			float distance = length_squared_302a10(_mm_sub_ps(second->position, points[i]->position));
			if (distance > largest)
			{
				largest = distance;
				best = i;
			}
		}
	}
	used[best] = true;

	point_302a10 const *third = points[best];
	__m128 edge = _mm_sub_ps(points[second_index]->position, third->position);
	largest = 0.0f;
	for (int i = 0; i < 5; i++)
	{
		if (!used[i])
		{
			float distance = length_squared_302a10(cross_302a10(_mm_sub_ps(third->position, points[i]->position), edge));
			if (distance > largest)
			{
				largest = distance;
				best = i;
			}
		}
	}
	used[best] = true;

	point_302a10 const *fourth = points[best];
	largest = 0.0f;
	for (int i = 0; i < 5; i++)
	{
		if (!used[i])
		{
			float distance = length_squared_302a10(_mm_sub_ps(fourth->position, points[i]->position));
			if (distance > largest)
			{
				largest = distance;
				best = i;
			}
		}
	}
	used[best] = true;

	for (int i = 0; i < count;)
	{
		if (!used[i])
		{
			owner->release_id(points[i]->id);
			point_302a10 const *source = points[count - 1];
			point_302a10 *dest = points[i];
			dest->position = source->position;
			dest->normal = source->normal;
			dest->field_20 = source->field_20;
			used[i] = used[count - 1];
			count--;
		}
		else
		{
			i++;
		}
	}
	return count;
}

// transforms a point by a 3x4 matrix of SSE columns (0x2da610)
struct c_transformed_point
{
	__m128 value;
	void transform(const void *matrix, const __m128 *point);
};

static __declspec(align(16)) const float g_ones_412420[4] = {1.0f, 1.0f, 1.0f, 1.0f};

// where a body sits: two vectors, then its transform
struct placement_302c70
{
	__m128 field_0;
	__m128 field_10;
	char transform[0x40];
};

struct body_302c70
{
	void const *shape;
	long key;
	placement_302c70 const *placement;
};

struct tolerance_302c70
{
	long field_0;
	float tolerance;
};

struct input_302c70
{
	char unknown0[0xc];
	tolerance_302c70 const *tolerances;
};

// a new contact point: the points in the two bodies' frames, the separation,
// the fraction between them, and the normal
struct candidate_302c70
{
	char unknown0[0x10];
	__m128 position;
	float distance;
	float fraction;
	char unknown28[8];
	__m128 other;
	__m128 normal;
};

// adds a contact point to a pair's kept points: one close to a kept point
// with much the same normal replaces it; otherwise it is added, the owner
// giving it an id, and five points are cut down to four
// @retail 0x302c70
void __cdecl function_302c70(void const *a, void const *b, void const *input, candidate_302c70 const *candidate, owner_302a10 *owner, point_302a10 *points, int *count)
{
	float const tolerance = ((input_302c70 const *)input)->tolerances->tolerance;
	float nearest = tolerance * tolerance;
	int found = -1;
	int i;
	for (i = 0; i < *count; i++)
	{
		float const distance = length_squared_302a10(_mm_sub_ps(points[i].position, candidate->position));
		if (distance < nearest)
		{
			if (length_squared_302a10(_mm_sub_ps(points[i].field_20, candidate->normal)) < 0.1f * 0.1f)
			{
				nearest = distance;
				found = i;
			}
		}
	}
	if (found >= 0)
	{
		point_302a10 *point = &points[found];
		float const id = point->normal.m128_f32[3];
		point->position = candidate->position;
		point->normal = candidate->other;
		point->field_20 = candidate->normal;
		point->field_20.m128_f32[3] = candidate->distance;
		point->position.m128_f32[3] = candidate->fraction;
		point->normal.m128_f32[3] = id;
		return;
	}
	point_302a10 spare;
	point_302a10 *slot = i < 4 ? &points[i] : &spare;
	slot->position = candidate->position;
	slot->normal = candidate->other;
	slot->field_20 = candidate->normal;
	slot->position.m128_f32[3] = candidate->fraction;
	slot->field_20.m128_f32[3] = candidate->distance;
	void const *frame_b = ((body_302c70 const *)b)->placement->transform;
	void const *frame_a = ((body_302c70 const *)a)->placement->transform;
	c_transformed_point on_a;
	on_a.transform(frame_a, &slot->position);
	c_transformed_point on_b;
	on_b.transform(frame_b, &slot->normal);
	float const fraction = candidate->fraction;
	__m128 const scale = _mm_set_ss(fraction);
	__m128 const splat = _mm_shuffle_ps(scale, scale, 0);
	contact_302a10 contact;
	contact.position = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(*(__m128 const *)g_ones_412420, splat), on_b.value), _mm_mul_ps(splat, on_a.value));
	contact.position.m128_f32[3] = slot->field_20.m128_f32[3];
	contact.normal = slot->field_20;
	unsigned short const id = owner->add_contact(a, b, input, &contact);
	if (id != 0xffff)
	{
		slot->id_and_flags = id | 0x3f000000;
		if (++*count > 4)
		{
			point_302a10 *list[5];
			list[0] = &points[0];
			list[1] = &points[1];
			list[2] = &points[2];
			list[3] = &points[3];
			list[4] = slot;
			*count = function_302a10(list, (void *)a, (void *)b, owner);
		}
	}
}
