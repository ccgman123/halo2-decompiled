// @flags /O2 /GL- /GX
// Havok (third-party physics library): casting the first shape of a pair
// along a direction against the second by conservative advancement: the
// closest points found by the handler the dispatcher's table picks for the
// two shapes give how far the first can move before they can touch; it is
// moved that far and the closest points found again, until they are within
// the tolerance, the moves run out, or the shapes separate.
#include <xmmintrin.h>
#include "pair_agents_2eab70.h"

// six vectors copied one at a time (0x2e7d80): where a body sits
struct class_2e7d80
{
	__m128 rows[6];

	class_2e7d80(const class_2e7d80 &other);
};

// a shape: its type from its sixth slot
class shape_2fe830
{
public:
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void slot_2() {}
	virtual void slot_3() {}
	virtual void slot_4() {}
	virtual long type() const { return 0; }
};

// one side of a query: a shape, a key, where it sits, and its parent side
struct body_2fe830
{
	shape_2fe830 const *shape;
	long key;
	class_2e7d80 const *placement;
	body_2fe830 const *parent;
};

// a hit: its point, with the distance or fraction in the fourth lane, and
// its normal
struct hit_2fe830
{
	__m128 position;
	__m128 normal;
};

// a cast's result: the hit and the two sides
struct cast_hit_2fe830
{
	hit_2fe830 hit;
	body_2fe830 const *a;
	body_2fe830 const *b;
};

// a collector of cast results that stops at its early-out fraction
class cast_collector_2fe830
{
public:
	virtual void slot_0() {}
	virtual void add_hit(cast_hit_2fe830 const *hit) {}

	float early_out;
};

// a collector of hits that stops at its early-out distance
class hit_collector_2fe830
{
public:
	float early_out;

	virtual ~hit_collector_2fe830() {}
	virtual void add_hit(hit_2fe830 const *candidate) {}
};

// a collector that keeps the closest hit (0x301a00)
class closest_collector_2fe830 : public hit_collector_2fe830
{
public:
	bool has_hit;
	hit_2fe830 hit;

	closest_collector_2fe830()
	{
		has_hit = false;
		hit.position.m128_f32[3] = 3.402823466e+38f;
		early_out = 3.402823466e+38f;
	}
};

struct query_header_2fe830;
struct cast_table_2fe830;

typedef void (__cdecl *closest_handler_2fe830)(body_2fe830 const *a, body_2fe830 const *b, query_header_2fe830 const *header, closest_collector_2fe830 *collector);

// the dispatcher's table of handlers that find two shapes' closest points
struct cast_table_2fe830
{
	char unknown0[0x218c];
	closest_handler_2fe830 handlers[1][32];
};

// how close counts as touching, and how many moves to try
struct cast_config_2fe830
{
	char unknown0[0x18];
	float tolerance;
	long iterations;
};

struct query_header_2fe830
{
	cast_table_2fe830 const *table;
	long field_4;
	float tolerance;
	cast_config_2fe830 const *config;
};

// a cast: the closest-point query's header, the direction the first shape
// moves along, and two more terms
struct query_2fe830
{
	query_header_2fe830 header;
	__m128 direction;
	float field_20;
	float extra_tolerance;
};

// the dot product of two vectors' first three terms
inline float dot3_2fe830(__m128 const &a, __m128 const &b)
{
	__m128 const product = _mm_mul_ps(a, b);
	float result;
	_mm_store_ss(&result, _mm_add_ss(_mm_shuffle_ps(product, product, 0xaa), _mm_add_ss(_mm_shuffle_ps(product, product, 0x55), product)));
	return result;
}

// score 207 (was 207): the code matches retail but for the stack layout
// (retail closest collector at esp+0x30 and the cast result at esp+0x60, here
// the reverse) and esi and edi holding the first body and the input the other
// way round; renaming the result and declaring the input first do not change
// either.
// @retail 0x2fe830
void __cdecl function_2fe830(void const *a_, void const *b_, void const *input_, void *collector_, void *second_collector_)
{
	body_2fe830 const *a = (body_2fe830 const *)a_;
	body_2fe830 const *b = (body_2fe830 const *)b_;
	query_2fe830 const *input = (query_2fe830 const *)input_;
	cast_collector_2fe830 *collector = (cast_collector_2fe830 *)collector_;
	cast_collector_2fe830 *second_collector = (cast_collector_2fe830 *)second_collector_;
	cast_hit_2fe830 result;
	closest_collector_2fe830 closest;
	query_header_2fe830 header = input->header;
	header.tolerance += input->extra_tolerance;
	long const first_type = a->shape->type();
	long const second_type = b->shape->type();
	closest_handler_2fe830 const handler = input->header.table->handlers[first_type][second_type];
	handler(a, b, &header, &closest);
	if (!closest.has_hit)
	{
		return;
	}
	result.hit.position = closest.hit.position;
	result.hit.position.m128_f32[3] = closest.hit.position.m128_f32[3];
	result.hit.normal = closest.hit.normal;
	result.a = a;
	result.b = b;
	if (closest.hit.position.m128_f32[3] < input->header.tolerance && second_collector != 0)
	{
		second_collector->add_hit(&result);
	}
	float const approach = dot3_2fe830(closest.hit.normal, input->direction);
	float const end_distance = closest.hit.position.m128_f32[3] + approach;
	if (end_distance > 0.0f)
	{
		return;
	}
	if (approach + input->field_20 >= 0.0f)
	{
		return;
	}
	if (closest.hit.position.m128_f32[3] <= input->header.config->tolerance)
	{
		if (closest.hit.position.m128_f32[3] > 0.0f)
		{
			float const fraction = closest.hit.position.m128_f32[3] / (closest.hit.position.m128_f32[3] - end_distance);
			if (fraction > collector->early_out)
			{
				return;
			}
			result.hit.position.m128_f32[3] = fraction;
			collector->add_hit(&result);
		}
		else
		{
			result.hit.position.m128_f32[3] = 0.0f;
			collector->add_hit(&result);
		}
		return;
	}
	result.hit.position.m128_f32[3] = closest.hit.position.m128_f32[3] / (closest.hit.position.m128_f32[3] - end_distance);
	closest_collector_2fe830 moved_closest;
	class_2e7d80 moved(*a->placement);
	body_2fe830 moved_a;
	moved_a.shape = a->shape;
	moved_a.parent = a;
	moved_a.placement = &moved;
	moved_a.key = a->key;
	for (int i = input->header.config->iterations - 1; i >= 0; i--)
	{
		__m128 const single = _mm_set_ss(result.hit.position.m128_f32[3]);
		moved.rows[5] = _mm_add_ps(a->placement->rows[5], _mm_mul_ps(_mm_shuffle_ps(single, single, 0), input->direction));
		moved_closest.has_hit = false;
		moved_closest.hit.position.m128_f32[3] = 3.402823466e+38f;
		moved_closest.early_out = 3.402823466e+38f;
		handler(&moved_a, b, &header, &moved_closest);
		if (!moved_closest.has_hit)
		{
			return;
		}
		float const moved_approach = dot3_2fe830(moved_closest.hit.normal, input->direction);
		if (moved_approach >= 0.0f)
		{
			return;
		}
		float const speed = -moved_approach;
		float const moved_distance = moved_closest.hit.position.m128_f32[3];
		if (moved_distance + result.hit.position.m128_f32[3] * speed > speed)
		{
			return;
		}
		float const fraction = moved_distance / speed + result.hit.position.m128_f32[3];
		if (fraction > collector->early_out)
		{
			return;
		}
		result.hit.position = moved_closest.hit.position;
		result.hit.position.m128_f32[3] = fraction;
		result.hit.normal = moved_closest.hit.normal;
		if (moved_distance <= input->header.config->tolerance)
		{
			break;
		}
	}
	collector->add_hit(&result);
}

// an object that finds the closest points of a pair of shapes itself, in its
// third slot
class closest_query_2fec10
{
public:
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void get_closest(body_2fe830 const *a, body_2fe830 const *b, query_header_2fe830 const *header, closest_collector_2fe830 *collector) {}
};

// score 190 (was 190): stack layout: retail puts closest at esp+0x30 and
// result at 0x60, ours the reverse (declaration order, scopes and names do
// not move them); retail also resets moved_closest before it moves the body
// in the loop, which here scores 197.
// the same cast, the closest points found by the object itself; a move that
// finds no closest points passes on the last hit
// @retail 0x2fec10
void class_2fec10::function_2fec10(void const *a_, void const *b_, void const *input_, void *collector_, void *second_collector_)
{
	closest_query_2fec10 *query = (closest_query_2fec10 *)this;
	body_2fe830 const *a = (body_2fe830 const *)a_;
	body_2fe830 const *b = (body_2fe830 const *)b_;
	query_2fe830 const *input = (query_2fe830 const *)input_;
	cast_collector_2fe830 *collector = (cast_collector_2fe830 *)collector_;
	cast_collector_2fe830 *second_collector = (cast_collector_2fe830 *)second_collector_;
	cast_hit_2fe830 result;
	closest_collector_2fe830 closest;
	query_header_2fe830 header = input->header;
	header.tolerance += input->extra_tolerance;
	query->get_closest(a, b, &header, &closest);
	if (!closest.has_hit)
	{
		return;
	}
	result.hit.position = closest.hit.position;
	result.hit.position.m128_f32[3] = closest.hit.position.m128_f32[3];
	result.hit.normal = closest.hit.normal;
	result.a = a;
	result.b = b;
	if (closest.hit.position.m128_f32[3] < input->header.tolerance && second_collector != 0)
	{
		second_collector->add_hit(&result);
	}
	float const approach = dot3_2fe830(closest.hit.normal, input->direction);
	float const end_distance = closest.hit.position.m128_f32[3] + approach;
	if (end_distance > 0.0f)
	{
		return;
	}
	if (approach + input->field_20 >= 0.0f)
	{
		return;
	}
	if (closest.hit.position.m128_f32[3] <= input->header.config->tolerance)
	{
		if (closest.hit.position.m128_f32[3] > 0.0f)
		{
			float const fraction = closest.hit.position.m128_f32[3] / (closest.hit.position.m128_f32[3] - end_distance);
			if (fraction > collector->early_out)
			{
				return;
			}
			result.hit.position.m128_f32[3] = fraction;
			collector->add_hit(&result);
		}
		else
		{
			result.hit.position.m128_f32[3] = 0.0f;
			collector->add_hit(&result);
		}
		return;
	}
	result.hit.position.m128_f32[3] = closest.hit.position.m128_f32[3] / (closest.hit.position.m128_f32[3] - end_distance);
	closest_collector_2fe830 moved_closest;
	class_2e7d80 moved(*a->placement);
	body_2fe830 moved_a;
	moved_a.shape = a->shape;
	moved_a.parent = a;
	moved_a.placement = &moved;
	moved_a.key = a->key;
	for (int i = input->header.config->iterations - 1; i >= 0; i--)
	{
		__m128 const single = _mm_set_ss(result.hit.position.m128_f32[3]);
		moved.rows[5] = _mm_add_ps(a->placement->rows[5], _mm_mul_ps(_mm_shuffle_ps(single, single, 0), input->direction));
		moved_closest.has_hit = false;
		moved_closest.hit.position.m128_f32[3] = 3.402823466e+38f;
		moved_closest.early_out = 3.402823466e+38f;
		query->get_closest(&moved_a, b, &header, &moved_closest);
		float const moved_approach = dot3_2fe830(moved_closest.hit.normal, input->direction);
		if (moved_approach >= 0.0f)
		{
			return;
		}
		float const speed = -moved_approach;
		float const moved_distance = moved_closest.hit.position.m128_f32[3];
		if (moved_distance + result.hit.position.m128_f32[3] * speed > speed)
		{
			return;
		}
		float const fraction = moved_distance / speed + result.hit.position.m128_f32[3];
		if (fraction > collector->early_out)
		{
			return;
		}
		result.hit.position = moved_closest.hit.position;
		result.hit.position.m128_f32[3] = fraction;
		result.hit.normal = moved_closest.hit.normal;
		if (moved_distance <= input->header.config->tolerance)
		{
			break;
		}
	}
	collector->add_hit(&result);
}
