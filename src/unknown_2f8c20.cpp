// @flags /O2 /GL- /GX
// Havok (third-party physics library): a query of a shape made of rays
// against a convex shape: each ray taken into the convex shape's frame and
// cast against it, and each hit passed on in world space; and queries between
// the two sides of a pair passed on to the function that runs them, with the
// sides changed.
#include "pair_agents_2eab70.h"
#include "side_swap_collectors_2eb1d0.h"
#include "swapped_query_2ed2a0.h"
#include "timer_mark_2ebc80.h"

void __cdecl function_2fe830(void const *a, void const *b, void const *input, void *collector, void *second_collector);

static __declspec(align(16)) const float g_ones_412420[4] = {1.0f, 1.0f, 1.0f, 1.0f};

// three axes and a position (0x2da4d0)
struct c_query_transform
{
	__m128 axes[3];
	__m128 position;
	void inverse_product(const c_query_transform *a, const c_query_transform *b);

	// a point placed by the transform
	__m128 apply(__m128 const &p) const
	{
		__m128 z = _mm_add_ps(_mm_mul_ps(axes[2], _mm_shuffle_ps(p, p, 0xaa)), position);
		__m128 y = _mm_mul_ps(axes[1], _mm_shuffle_ps(p, p, 0x55));
		__m128 x = _mm_mul_ps(axes[0], _mm_shuffle_ps(p, p, 0x00));
		return _mm_add_ps(_mm_add_ps(x, y), z);
	}

	// a direction turned by the transform
	__m128 rotate(__m128 const &d) const
	{
		__m128 z = _mm_mul_ps(axes[2], _mm_shuffle_ps(d, d, 0xaa));
		__m128 y = _mm_mul_ps(axes[1], _mm_shuffle_ps(d, d, 0x55));
		__m128 x = _mm_mul_ps(axes[0], _mm_shuffle_ps(d, d, 0x00));
		return _mm_add_ps(_mm_add_ps(x, y), z);
	}
};

// where a body sits: two vectors, then its transform
struct placement_2f8c20
{
	__m128 field_0;
	__m128 field_10;
	c_query_transform transform;
};

// a ray: its two ends, its length in the first end's fourth lane
struct ray_2f8c20
{
	__m128 from;
	__m128 to;
};

// a shape made of rays
struct rays_2f8c20
{
	char unknown0[0x10];
	ray_2f8c20 const *rays;
	long ray_count;
};

// a ray as a convex shape takes it
struct ray_input_2f8c20
{
	__m128 from;
	__m128 to;
	long field_20;
	long field_24;
};

// what a ray cast found: the normal, and how far along the ray
struct ray_output_2f8c20
{
	__m128 normal;
	char unknown10[0x14 - 0x10];
	float fraction;
};

// whether a ray hit, given back through a hidden pointer
class answer_2f8c20
{
public:
	answer_2f8c20() {}

	char value;
	char unknown1[3];
};

class convex_2f8c20
{
public:
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void slot_2() {}
	virtual void slot_3() {}
	virtual void slot_4() {}
	virtual void slot_5() {}
	virtual void slot_6() {}
	virtual void slot_7() {}
	virtual answer_2f8c20 cast_ray(ray_input_2f8c20 const *ray, ray_output_2f8c20 *output) const { return answer_2f8c20(); }
};

struct body_2f8c20
{
	void const *shape;
	long key;
	placement_2f8c20 const *placement;
	body_2f8c20 const *parent;
};

// a hit in world space: its point, with its distance in the fourth lane, its
// normal, and the two sides
struct hit_2f8c20
{
	__m128 point;
	__m128 normal;
	body_2f8c20 const *a;
	body_2f8c20 const *b;
};

class hit_collector_2f8c20
{
public:
	virtual void slot_0() {}
	virtual void add_hit(hit_2f8c20 const *hit) {}
};

// @retail 0x2f8c20
void __cdecl function_2f8c20(void const *a_, void const *b_, void const *input, void *collector_)
{
	body_2f8c20 const *a = (body_2f8c20 const *)a_;
	body_2f8c20 const *b = (body_2f8c20 const *)b_;
	hit_collector_2f8c20 *collector = (hit_collector_2f8c20 *)collector_;
	timer_add_2ebc80("TtmultiRay-cvx");
	rays_2f8c20 const *shape = (rays_2f8c20 const *)a->shape;
	convex_2f8c20 const *convex = (convex_2f8c20 const *)b->shape;
	c_query_transform a_in_b;
	a_in_b.inverse_product(&b->placement->transform, &a->placement->transform);
	long const ray_count = shape->ray_count;
	ray_2f8c20 const *ray = shape->rays;
	hit_2f8c20 hit;
	hit.a = a;
	hit.b = b;
	ray_input_2f8c20 input_ray;
	input_ray.field_24 = 0;
	for (long i = ray_count; i > 0; i--)
	{
		input_ray.from = a_in_b.apply(ray->from);
		input_ray.to = a_in_b.apply(ray->to);
		ray_output_2f8c20 output;
		output.fraction = 1.0f;
		answer_2f8c20 const result = convex->cast_ray(&input_ray, &output);
		if (result.value)
		{
			float const fraction = output.fraction;
			__m128 const along = _mm_set_ps1(fraction);
			__m128 const point = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(*(__m128 const *)g_ones_412420, along), input_ray.from), _mm_mul_ps(along, input_ray.to));
			hit.point = b->placement->transform.apply(point);
			hit.point.m128_f32[3] = (fraction - 1.0f) * ray->from.m128_f32[3];
			hit.normal = b->placement->transform.rotate(output.normal);
			collector->add_hit(&hit);
		}
		ray++;
	}
	timer_add_2ebc80("Et");
}

// the query with the two sides the other way round, its hits passed on as seen from the other side
// @retail 0x2f8e90
void __cdecl function_2f8e90(void const *a, void const *b, void const *input, void *collector)
{
	flipped_collector_2ed110 flipped(collector);
	function_2f8c20(b, a, input, &flipped);
}

// the query with the two sides the other way round: its direction turned
// around, and each collector wrapped in one that turns its results back
// @retail 0x2f8ef0
void __cdecl function_2f8ef0(void const *a, void const *b, void const *input, void *collector, void *second_collector)
{
	query_2ed2a0 const *query = (query_2ed2a0 const *)input;
	query_2ed2a0 swapped;
	swap_query_2ed2a0(&swapped, query);
	swapped_collector_2ed2a0 first(query->direction, collector);
	if (second_collector)
	{
		swapped_collector_2ed2a0 second(query->direction, second_collector);
		function_2fe830(b, a, &swapped, &first, &second);
	}
	else
	{
		function_2fe830(b, a, &swapped, &first, 0);
	}
}
