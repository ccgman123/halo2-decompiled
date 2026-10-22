// @flags /O2 /GL-
// Havok (third-party physics library): a sphere against a box: the contact
// between them, from the box's nearest point to the sphere's centre, or from
// the face nearest the centre when the centre is inside; and the pair
// reported when the centre is closer to the box than the sum of their radii.
// Each is bracketed by two marks in the timing stream.
#include "timer_mark_2ebc80.h"
#include <xmmintrin.h>

static __declspec(align(16)) const unsigned long g_sign_mask_4124a0[4] = {0x80000000, 0x80000000, 0x80000000, 0x80000000};
static __declspec(align(16)) const unsigned long g_magnitude_mask_455940[4] = {0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff};

// three axes and a position
struct c_query_transform
{
	__m128 axes[3];
	__m128 position;
};

// a point taken into a transform's frame (0x2da660)
struct c_query_point
{
	__m128 value;
	void inverse_transform(const c_query_transform *matrix, const __m128 *point);
};

// a point placed by a transform
struct placed_point_2f23c0
{
	__m128 value;

	void set_transformed(c_query_transform const *transform, __m128 const *point)
	{
		__m128 p = *point;
		__m128 z = _mm_add_ps(_mm_mul_ps(transform->axes[2], _mm_shuffle_ps(p, p, 0xaa)), transform->position);
		__m128 y = _mm_mul_ps(transform->axes[1], _mm_shuffle_ps(p, p, 0x55));
		__m128 x = _mm_mul_ps(transform->axes[0], _mm_shuffle_ps(p, p, 0x00));
		value = _mm_add_ps(_mm_add_ps(x, y), z);
	}
};

struct sphere_2f23c0
{
	char unknown0[0xc];
	float radius;
};

// a box: its radius and its half extents
struct box_2f23c0
{
	char unknown0[0xc];
	float radius;
	__m128 half_extents;
};

struct motion_2f23c0
{
	char unknown0[0x20];
	c_query_transform transform;
};

struct body_2f23c0
{
	void const *shape;
	long key;
	motion_2f23c0 const *motion;
};

class collector_2f23c0
{
public:
	virtual void slot_0() {}
	virtual void add_pair(body_2f23c0 const *a, body_2f23c0 const *b) {}
};

struct input_2f2130
{
	long field_0;
	long field_4;
	float tolerance;
};

// a contact: its point, with the separation in the fourth lane, its normal,
// and the two sides
struct contact_2f2130
{
	__m128 point;
	__m128 normal;
	body_2f23c0 const *a;
	body_2f23c0 const *b;
};

class contact_collector_2f2130
{
public:
	virtual void slot_0() {}
	virtual void add_contact(contact_2f2130 const *contact) {}
};

// a direction turned by a transform
__forceinline __m128 rotate_2f2130(c_query_transform const *transform, __m128 const &d)
{
	__m128 z = _mm_mul_ps(transform->axes[2], _mm_shuffle_ps(d, d, 0xaa));
	__m128 y = _mm_mul_ps(transform->axes[1], _mm_shuffle_ps(d, d, 0x55));
	__m128 x = _mm_mul_ps(transform->axes[0], _mm_shuffle_ps(d, d, 0x00));
	return _mm_add_ps(_mm_add_ps(x, y), z);
}

// one over the square root of the low term, refined by a Newton step, in
// all four terms
__forceinline __m128 inverse_sqrt_2f2130(__m128 const &value)
{
	__m128 const estimate = _mm_rsqrt_ss(value);
	__m128 const product = _mm_mul_ss(_mm_mul_ss(value, estimate), estimate);
	__m128 const refined = _mm_mul_ss(_mm_mul_ss(_mm_set_ss(0.5f), estimate), _mm_sub_ss(_mm_set_ss(3.0f), product));
	return _mm_shuffle_ps(refined, refined, 0);
}

// a vector of four copies of a float
inline __m128 splat_2f2130(float value)
{
	__m128 const single = _mm_set_ss(value);
	return _mm_shuffle_ps(single, single, 0);
}

// the contact of a sphere with a box, passed on when they are within the
// tolerance or the centre is inside the box
__forceinline void sphere_box_contact_2f1ea0(void const *a_, void const *b_, void const *input_, void *collector_)
{
	body_2f23c0 const *a = (body_2f23c0 const *)a_;
	body_2f23c0 const *b = (body_2f23c0 const *)b_;
	input_2f2130 const *input = (input_2f2130 const *)input_;
	timer_add_2ebc80("TtSphereBox");
	__m128 magnitude;
	contact_2f2130 contact;
	contact.a = a;
	contact.b = b;
	__m128 const *centre = &a->motion->transform.position;
	c_query_point local;
	local.inverse_transform(&b->motion->transform, centre);
	box_2f23c0 const *box = (box_2f23c0 const *)b->shape;
	sphere_2f23c0 const *sphere = (sphere_2f23c0 const *)a->shape;
	magnitude = _mm_and_ps(local.value, *(__m128 const *)g_magnitude_mask_455940);
	__m128 const outside = _mm_sub_ps(_mm_min_ps(magnitude, box->half_extents), magnitude);
	float separation;
	if (_mm_movemask_ps(outside) & 7)
	{
		float const radius = box->radius + sphere->radius;
		__m128 const squares = _mm_mul_ps(outside, outside);
		__m128 const length_squared = _mm_add_ss(_mm_shuffle_ps(squares, squares, 0xaa), _mm_add_ss(_mm_shuffle_ps(squares, squares, 0x55), squares));
		__m128 const inverse = inverse_sqrt_2f2130(length_squared);
		float length;
		_mm_store_ss(&length, _mm_mul_ss(length_squared, inverse));
		separation = length - radius;
		if (separation > input->tolerance)
		{
			goto done;
		}
		__m128 const sign = _mm_and_ps(local.value, *(__m128 const *)g_sign_mask_4124a0);
		__m128 const direction = _mm_xor_ps(_mm_xor_ps(_mm_mul_ps(inverse, outside), sign), *(__m128 const *)g_sign_mask_4124a0);
		contact.normal = rotate_2f2130(&b->motion->transform, direction);
	}
	else
	{
		float const *depths_of = (float const *)&magnitude;
		float const dx = depths_of[0] - box->half_extents.m128_f32[0];
		float const dy = depths_of[1] - box->half_extents.m128_f32[1];
		float const dz = depths_of[2] - box->half_extents.m128_f32[2];
		c_query_transform const *transform = &b->motion->transform;
		int axis;
		float depth;
		__m128 normal;
		if (dx > dy)
		{
			if (dx > dz)
			{
				normal = transform->axes[0];
				depth = dx;
				axis = 0;
			}
			else
			{
				normal = transform->axes[2];
				depth = dz;
				axis = 2;
			}
		}
		else if (dy > dz)
		{
			normal = transform->axes[1];
			depth = dy;
			axis = 1;
		}
		else
		{
			normal = transform->axes[2];
			depth = dz;
			axis = 2;
		}
		contact.normal = normal;
		if (local.value.m128_f32[axis] < 0.0f)
		{
			contact.normal = _mm_xor_ps(normal, *(__m128 const *)g_sign_mask_4124a0);
		}
		separation = depth - box->radius - sphere->radius;
	}
	contact.point = _mm_add_ps(*centre, _mm_mul_ps(splat_2f2130(-separation - sphere->radius), contact.normal));
	contact.point.m128_f32[3] = separation;
	((contact_collector_2f2130 *)collector_)->add_contact(&contact);
done:
	timer_add_2ebc80("Et");
}

class class_2f1ea0
{
public:
	void function_2f1ea0(void const *a, void const *b, void const *input, void *collector);
};

// score 70 (was 70): the contact's point and normal sit at esp+0x20 in retail and
// esp+0x30 in ours, so the shape's pointer takes edi here and ebx in retail.
// @retail 0x2f1ea0
void class_2f1ea0::function_2f1ea0(void const *a, void const *b, void const *input, void *collector)
{
	sphere_box_contact_2f1ea0(a, b, input, collector);
}

// the same as a function
// score 70 (was 70): as 0x2f1ea0, the contact's point and normal sit at esp+0x20
// in retail and esp+0x30 in ours.
// @retail 0x2f2130
void __cdecl function_2f2130(void const *a, void const *b, void const *input, void *collector)
{
	sphere_box_contact_2f1ea0(a, b, input, collector);
}

// @retail 0x2f23c0
void __cdecl function_2f23c0(body_2f23c0 const *a, body_2f23c0 const *b, void const *input, collector_2f23c0 *collector)
{
	timer_add_2ebc80("TtSphereBox");
	sphere_2f23c0 const *sphere = (sphere_2f23c0 const *)a->shape;
	box_2f23c0 const *box = (box_2f23c0 const *)b->shape;
	__m128 const *centre = &a->motion->transform.position;
	c_query_point local;
	local.inverse_transform(&b->motion->transform, centre);
	__m128 const sign = _mm_and_ps(local.value, *(__m128 const *)g_sign_mask_4124a0);
	__m128 const magnitude = _mm_and_ps(local.value, *(__m128 const *)g_magnitude_mask_455940);
	placed_point_2f23c0 nearest;
	nearest.value = _mm_xor_ps(_mm_min_ps(magnitude, box->half_extents), sign);
	placed_point_2f23c0 world;
	world.set_transformed(&b->motion->transform, &nearest.value);
	__m128 const offset = _mm_sub_ps(world.value, *centre);
	__m128 const squares = _mm_mul_ps(offset, offset);
	float distance_squared;
	_mm_store_ss(&distance_squared, _mm_add_ss(_mm_shuffle_ps(squares, squares, 0xaa), _mm_add_ss(_mm_shuffle_ps(squares, squares, 0x55), squares)));
	float const radius = box->radius + sphere->radius;
	if (distance_squared < radius * radius)
	{
		collector->add_pair(a, b);
	}
	timer_add_2ebc80("Et");
}
