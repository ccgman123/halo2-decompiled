#ifndef CONVEX_PAIR_2EE2E0_H
#define CONVEX_PAIR_2EE2E0_H
// Havok (third-party physics library): the object kept for a pair of convex
// shapes, and the sides of a query it is given.
#include "pair_agent_2ed170.h"
#include "convex_search_3048a0.h"
#include "contact_points_302a10.h"
#include "timer_mark_2ebc80.h"

struct c_query_transform
{
	__m128 axes[3];
	__m128 position;

	void inverse_product(const c_query_transform *a, const c_query_transform *b);
};

class convex_2fefd0
{
public:
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void slot_2() {}
	virtual void slot_3() {}
	virtual void slot_4() {}
	virtual long type() const { return 0; }

	long field_4;
	long field_8;
	float radius;
};

struct triangle_2ff0c0;

struct pair_state_2fefd0
{
	unsigned short first_id;
	unsigned short second_id;
	unsigned short field_4;
	unsigned short field_6;
	bool field_8;
	unsigned char field_9;
	bool first_flag;
	unsigned char second_flag;

	void start(convex_2fefd0 const *first, convex_2fefd0 const *second, c_query_transform const *transform);
	void function_2ff0c0(convex_2fefd0 const *first, triangle_2ff0c0 const *triangle, c_query_transform const *transform);
};

// where a body sits: two vectors, then its transform
struct placement_2ee3f0
{
	__m128 field_0;
	__m128 field_10;
	c_query_transform transform;
};

// one side of a query: a shape, a key, where it sits, and its parent side
struct body_2ee3f0
{
	convex_2fefd0 const *shape;
	long key;
	placement_2ee3f0 const *placement;
	body_2ee3f0 const *parent;
};

// told of each pair found touching
class collector_2ee3f0
{
public:
	virtual void slot_0() {}
	virtual void slot_1(body_2ee3f0 const *a, body_2ee3f0 const *b) {}
};

struct contact_3032c0;

// the result of a query of the pair kept for later: the contact as the search
// gives it, then its point in the second shape's frame and its normal in the
// world
struct kept_contact_2ee610;

// whether a query found a contact
struct found_2ee610
{
	bool value;

	found_2ee610(bool value_)
	{
		value = value_;
	}
};

// the object as its constructor's file declares it, with its full size: its
// search state, and the last separating direction with a distance in its
// fourth lane that the shapes' motion must use up before they can touch; and
// the contact points it keeps (their count in field_18)
class convex_pair_2ee2e0 : public sized_2ed170
{
public:
	pair_state_2fefd0 state;
	long field_18;
	char unknown1c[0x20 - 0x1c];
	__m128 separation;
	point_302a10 points[4];

	convex_pair_2ee2e0(description_2ed170 const *first, description_2ed170 const *second, long arg_3);
	void function_2ee3f0(body_2ee3f0 const *a, body_2ee3f0 const *b, void const *input, collector_2ee3f0 *collector);
	found_2ee610 function_2ee610(body_2ee3f0 const *a, body_2ee3f0 const *b, void const *input, kept_contact_2ee610 *out);
	void function_2eeed0(body_2ee3f0 const *a, body_2ee3f0 const *b, void const *input, void *collector);
	void function_2eeb40(body_2ee3f0 const *a, body_2ee3f0 const *b, void const *input, void *out);
};

// a direction turned by a transform (0x2da6e0)
struct vector_2da6e0
{
	__m128 value;
	void rotate(const c_query_transform *matrix, const __m128 *direction);
};

// the closest points as the search finds them: a plane through the second
// shape's point, and the contact made from it (0x3032c0)
struct plane_3032c0;

struct contact_3032c0
{
	__m128 normal;
	__m128 point;
	float distance;
	float weight;
};

class source_3032c0
{
public:
	void function_3032c0(void *unused, plane_3032c0 *plane, contact_3032c0 *out) const;
};

// a contact passed to the collector: its point, with the separation in the
// fourth lane, its normal, and the two sides
struct contact_2ee8b0
{
	__m128 point;
	__m128 normal;
	body_2ee3f0 const *a;
	body_2ee3f0 const *b;
};

class contact_collector_2ee8b0
{
public:
	virtual void slot_0() {}
	virtual void add_contact(contact_2ee8b0 const *contact) {}
};

struct input_2ee8b0
{
	long field_0;
	long field_4;
	float tolerance;
};

// a vector of four copies of a float
inline __m128 splat_2ee8b0(float value)
{
	__m128 const single = _mm_set_ss(value);
	return _mm_shuffle_ps(single, single, 0);
}

// a point taken into a transform's frame (0x2da660, inlined here)
struct local_point_2ee8b0
{
	__m128 value;

	__forceinline void inverse_transform(c_query_transform const *matrix, __m128 const &point)
	{
		__m128 d = _mm_sub_ps(point, matrix->position);
		__m128 c0 = matrix->axes[0];
		__m128 c1 = matrix->axes[1];
		__m128 c2 = matrix->axes[2];
		__m128 t0 = _mm_shuffle_ps(c0, c1, 0x44);
		__m128 t2 = _mm_shuffle_ps(c0, c1, 0xee);
		__m128 t1 = _mm_shuffle_ps(c2, c2, 0x44);
		__m128 t3 = _mm_shuffle_ps(c2, c2, 0xee);
		__m128 row_0 = _mm_shuffle_ps(t0, t1, 0x88);
		__m128 row_1 = _mm_shuffle_ps(t0, t1, 0xdd);
		__m128 r2 = _mm_shuffle_ps(t2, t3, 0x88);
		value = _mm_add_ps(_mm_add_ps(
			_mm_mul_ps(row_0, _mm_shuffle_ps(d, d, 0x00)),
			_mm_mul_ps(row_1, _mm_shuffle_ps(d, d, 0x55))),
			_mm_mul_ps(r2, _mm_shuffle_ps(d, d, 0xaa)));
	}
};

// a point placed by a transform (0x2da610, inlined here)
__forceinline __m128 transform_2ee8b0(c_query_transform const *m, __m128 const &p)
{
	__m128 z = _mm_add_ps(_mm_mul_ps(m->axes[2], _mm_shuffle_ps(p, p, 0xaa)), m->position);
	__m128 y = _mm_mul_ps(m->axes[1], _mm_shuffle_ps(p, p, 0x55));
	__m128 x = _mm_mul_ps(m->axes[0], _mm_shuffle_ps(p, p, 0x00));
	return _mm_add_ps(_mm_add_ps(x, y), z);
}

// a vector moved along a direction by a distance
inline void add_scaled_2ee610(__m128 *value, float distance, __m128 const *direction)
{
	*value = _mm_add_ps(*value, _mm_mul_ps(splat_2ee8b0(distance), *direction));
}

struct kept_contact_2ee610
{
	contact_3032c0 found;
	__m128 local_point;
	vector_2da6e0 normal;
};

struct motion_input_2ee610
{
	long field_0;
	long field_4;
	float tolerance;
	bool const *keep_separation;
};

// the closest points of the pair's two convex shapes for later use, skipped
// while the shapes' motion since the last query cannot have used up the
// distance they were apart: the search's contact, its normal in the world,
// and its point taken onto the second shape's surface in that shape's frame;
// a miss may keep the separating direction for next time (0x2ee610, and
// inlined in the pair's other queries)
__forceinline found_2ee610 query_kept_2ee610(convex_pair_2ee2e0 *pair, body_2ee3f0 const *a, body_2ee3f0 const *b, void const *input_, kept_contact_2ee610 *out)
{
	motion_input_2ee610 const *input = (motion_input_2ee610 const *)input_;
	convex_2fefd0 const *first = a->shape;
	convex_2fefd0 const *second = b->shape;
	if (pair->separation.m128_f32[3] > 0.0f)
	{
		timer_add_2ebc80("StTim");
		__m128 const moved = _mm_mul_ps(_mm_sub_ps(_mm_sub_ps(b->placement->field_10, b->placement->field_0), _mm_sub_ps(a->placement->field_10, a->placement->field_0)), pair->separation);
		float along;
		_mm_store_ss(&along, _mm_add_ss(_mm_shuffle_ps(moved, moved, 0xaa), _mm_add_ss(_mm_shuffle_ps(moved, moved, 0x55), moved)));
		pair->separation.m128_f32[3] -= b->placement->field_0.m128_f32[3] + a->placement->field_0.m128_f32[3] + along;
		if (pair->separation.m128_f32[3] > 0.0f)
		{
			return found_2ee610(false);
		}
	}
	timer_add_2ebc80("StGsk");
	c_query_transform transform;
	transform.inverse_product(&a->placement->transform, &b->placement->transform);
	class_3048a0 search;
	search.field_c = false;
	closest_3048a0 closest;
	if (search.function_3048a0(first, second, &transform, &pair->state, &closest) != 0)
	{
		return found_2ee610(false);
	}
	((source_3032c0 const *)&search)->function_3032c0(&pair->state, (plane_3032c0 *)&closest, &out->found);
	vector_2da6e0 *normal = &out->normal;
	normal->rotate(&a->placement->transform, &out->found.normal);
	float const distance = out->found.distance - first->radius - second->radius;
	out->found.distance = distance;
	if (distance < input->tolerance)
	{
		add_scaled_2ee610(&out->found.point, -first->radius, &out->found.normal);
		__m128 const surface = _mm_add_ps(out->found.point, _mm_mul_ps(splat_2ee8b0(-distance), out->found.normal));
		local_point_2ee8b0 local;
		local.inverse_transform(&transform, surface);
		out->local_point = local.value;
		return found_2ee610(true);
	}
	if (pair->field_18 == 0 && *input->keep_separation)
	{
		pair->separation = normal->value;
		pair->separation.m128_f32[3] = distance - input->tolerance * 1.1f;
	}
	return found_2ee610(false);
}

#endif
