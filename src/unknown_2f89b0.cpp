// @flags /O2 /GL-
// Havok (third-party physics library): a shape of several rays against a
// convex shape, bracketed by two marks in the timing stream: each ray is
// carried into the convex shape's frame and cast against it; each result is
// reported as a contact: the result point and normal in world space, and the
// distance back along the ray.
#include "timer_mark_2ebc80.h"
#include <xmmintrin.h>

static __declspec(align(16)) const float g_412420[4] = {1.0f, 1.0f, 1.0f, 1.0f};

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

// a ray: its start, with its length in the fourth lane, and its end
struct ray_2f89b0
{
	__m128 start;
	__m128 end;
};

struct rays_shape_2f89b0
{
	char unknown0[0x10];
	ray_2f89b0 const *rays;
	int ray_count;
};

struct ray_input_2f89b0
{
	__m128 from;
	__m128 to;
	long field_20;
	long field_24;
};

struct ray_output_2f89b0
{
	__m128 normal;
	float field_10;
	float fraction;
	char unknown18[8];
};

// whether the ray result, given back through a hidden pointer
class hit_2f89b0
{
public:
	hit_2f89b0() {}

	char value;
	char unknown1[3];
};

class convex_2f89b0
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
	virtual hit_2f89b0 cast_ray(ray_input_2f89b0 const *input, ray_output_2f89b0 *ray_output) const { hit_2f89b0 result; result.value = false; return result; }
};

struct motion_2f89b0
{
	char unknown0[0x20];
	c_query_transform transform;
};

struct body_2f89b0
{
	void const *shape;
	long key;
	motion_2f89b0 const *motion;
};

struct contact_2f89b0
{
	__m128 position;
	__m128 normal;
	body_2f89b0 const *first;
	body_2f89b0 const *second;
};

class collector_2f89b0
{
public:
	virtual void slot_0() {}
	virtual void add_contact(contact_2f89b0 const *contact) {}
};

class rays_convex_2f89b0
{
public:
	void function_2f89b0(body_2f89b0 const *a, body_2f89b0 const *b, void const *input, collector_2f89b0 *collector);
};

// each ray of the shape, carried into the convex shape's frame and cast
// against it, every hit reported as a contact between the two bodies
// @retail 0x2f89b0
void rays_convex_2f89b0::function_2f89b0(body_2f89b0 const *a, body_2f89b0 const *b, void const *input, collector_2f89b0 *collector)
{
	timer_add_2ebc80("TtmultiRay-cvx");
	rays_shape_2f89b0 const *rays = (rays_shape_2f89b0 const *)a->shape;
	convex_2f89b0 const *convex = (convex_2f89b0 const *)b->shape;
	c_query_transform b_in_a;
	b_in_a.inverse_product(&b->motion->transform, &a->motion->transform);
	int const count = rays->ray_count;
	ray_2f89b0 const *each = rays->rays;
	contact_2f89b0 touch;
	touch.first = a;
	touch.second = b;
	ray_input_2f89b0 ray;
	ray.field_24 = 0;
	for (int i = count; i > 0; i--)
	{
		ray.from = b_in_a.apply(each->start);
		ray.to = b_in_a.apply(each->end);
		ray_output_2f89b0 output;
		output.fraction = 1.0f;
		hit_2f89b0 hit = convex->cast_ray(&ray, &output);
		if (hit.value)
		{
			float const fraction = output.fraction;
			__m128 const along = _mm_set1_ps(fraction);
			__m128 const point = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(*(__m128 const *)g_412420, along), ray.from), _mm_mul_ps(along, ray.to));
			touch.position = b->motion->transform.apply(point);
			touch.position.m128_f32[3] = (fraction - 1.0f) * each->start.m128_f32[3];
			touch.normal = b->motion->transform.rotate(output.normal);
			collector->add_contact(&touch);
		}
		each++;
	}
	timer_add_2ebc80("Et");
}
