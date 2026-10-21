// @flags /O2 /GL-
// Havok (third-party physics library): the contact between a sphere and a
// box, bracketed by two marks in the timing stream: the sphere's centre is
// carried into the box's frame; outside the box, the contact is along the
// line from the nearest point of the box, when it is within the tolerance;
// inside, it is along the axis whose face is nearest. The contact keeps an
// id the owner hands out once; a miss hands the id back.
#include "timer_mark_2ebc80.h"
#include "growable_array_2d91d0.h"
#include "pair_agents_2eab70.h"
#include <xmmintrin.h>

static __declspec(align(16)) const unsigned long g_455940[4] = {0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff};
static __declspec(align(16)) const unsigned long g_4124a0[4] = {0x80000000, 0x80000000, 0x80000000, 0x80000000};
// the same sign bits, kept apart for turning a direction round

struct c_query_transform
{
	__m128 axes[3];
	__m128 position;
};

struct c_query_point
{
	__m128 value;
	void inverse_transform(const c_query_transform *matrix, const __m128 *point);
};

struct sphere_2f1b80
{
	char unknown0[0xc];
	float radius;
};

struct box_2f1b80
{
	char unknown0[0xc];
	float radius;
	__m128 half_extents;
};

struct motion_2f1b80
{
	char unknown0[0x20];
	c_query_transform transform;
};

struct body_2f1b80
{
	void const *shape;
	long key;
	motion_2f1b80 const *motion;
};

struct input_2f1b80
{
	char unknown0[8];
	float tolerance;
};

// a contact: its point, with the distance in the fourth lane, its normal, and
// its id
struct contact_2f1b80
{
	union
	{
		__m128 position;
		float position_and_distance[4];
	};
	__m128 normal;
	unsigned short id;
};

ELEMENTS_2D91D0_ALIGNED(contacts_2f1b80, contact_2f1b80);

struct results_2f1b80
{
	contacts_2f1b80 *contacts;
};

class contact_owner_2f1b80
{
public:
	virtual void slot_0() {}
	virtual unsigned short add_contact(void const *a, void const *b, void const *input, contact_2f1b80 *contact) { return 0; }
	virtual void release_id(unsigned short id) {}
};

// a direction turned by a transform
inline __m128 rotated_2f1b80(c_query_transform const *transform, __m128 const &v)
{
	__m128 z = _mm_mul_ps(transform->axes[2], _mm_shuffle_ps(v, v, 0xaa));
	__m128 y = _mm_mul_ps(transform->axes[1], _mm_shuffle_ps(v, v, 0x55));
	__m128 x = _mm_mul_ps(transform->axes[0], _mm_shuffle_ps(v, v, 0x00));
	return _mm_add_ps(_mm_add_ps(x, y), z);
}

// one over the square root of the first term, refined by a Newton step, in
// all four terms
__forceinline __m128 inverse_sqrt_2f1b80(__m128 const &value)
{
	__m128 const estimate = _mm_rsqrt_ss(value);
	__m128 const product = _mm_mul_ss(_mm_mul_ss(value, estimate), estimate);
	__m128 const refined = _mm_mul_ss(_mm_mul_ss(_mm_set_ss(0.5f), estimate), _mm_sub_ss(_mm_set_ss(3.0f), product));
	return _mm_shuffle_ps(refined, refined, 0);
}
// score 174 (was 192): retail loads the sign mask at 0x4124a0 twice, into two
// registers, where ours loads it once (a second copy of the mask elsewhere gets
// 143, but retail has no such copy, so it can't match); retail also places the
// miss block after the contact's tail
// (here right after the outside branch), loads b after a's motion (here
// hoisted, so ebx and edi trade places), and keeps the box-frame centre at
// esp+0x30 and its magnitude at esp+0x20 (here the reverse; declaration order
// and names do not move them).
// @retail 0x2f1b80
void class_2f1b80::function_2f1b80(void const *a_, void const *b_, void const *input_, void *collector)
{
	timer_add_2ebc80("TtSphereBox");
	body_2f1b80 const *a = (body_2f1b80 const *)a_;
	body_2f1b80 const *b = (body_2f1b80 const *)b_;
	input_2f1b80 const *input = (input_2f1b80 const *)input_;
	results_2f1b80 *results = (results_2f1b80 *)collector;
	contact_2f1b80 *contact = &results->contacts->next_slot();
	__m128 const *centre = &a->motion->transform.position;
	c_query_point local;
	union
	{
		__m128 value;
		float f[4];
	} extent;
	local.inverse_transform(&b->motion->transform, centre);
	box_2f1b80 const *box = (box_2f1b80 const *)b->shape;
	void const *untyped_a = a->shape;
	sphere_2f1b80 const *sphere = (sphere_2f1b80 const *)untyped_a;
	extent.value = _mm_and_ps(local.value, *(__m128 const *)g_455940);
	__m128 const outside = _mm_sub_ps(_mm_min_ps(extent.value, box->half_extents), extent.value);
	float distance;
	if (_mm_movemask_ps(outside) & 7)
	{
		float box_radius = box->radius;
		float const radii = box_radius + sphere->radius;
		__m128 const squares = _mm_mul_ps(outside, outside);
		__m128 const sum = _mm_add_ss(_mm_shuffle_ps(squares, squares, 0xaa), _mm_add_ss(_mm_shuffle_ps(squares, squares, 0x55), squares));
		__m128 const scale = inverse_sqrt_2f1b80(sum);
		float length;
		_mm_store_ss(&length, _mm_mul_ss(sum, scale));
		distance = length - radii;
		if (!(distance > input->tolerance))
		{
			contact->normal = rotated_2f1b80(&b->motion->transform, _mm_xor_ps(_mm_xor_ps(_mm_mul_ps(scale, outside), _mm_and_ps(local.value, *(__m128 const *)g_4124a0)), *(__m128 const *)g_4124a0));
		}
		else
		{
			goto miss;
		}
	}
	else
	{
		motion_2f1b80 const *motion = b->motion;
		float const dx = extent.f[0] - box->half_extents.m128_f32[0];
		float const dy = extent.f[1] - box->half_extents.m128_f32[1];
		float const dz = extent.f[2] - box->half_extents.m128_f32[2];
		int axis;
		float depth;
		if (dx > dy)
		{
			if (dx > dz)
			{
				contact->normal = motion->transform.axes[0];
				depth = dx;
				axis = 0;
			}
			else
			{
				contact->normal = motion->transform.axes[2];
				depth = dz;
				axis = 2;
			}
		}
		else if (dz < dy)
		{
			contact->normal = motion->transform.axes[1];
			depth = dy;
			axis = 1;
		}
		else
		{
			contact->normal = motion->transform.axes[2];
			depth = dz;
			axis = 2;
		}
		if (local.value.m128_f32[axis] < 0.0f)
		{
			contact->normal = _mm_xor_ps(contact->normal, *(__m128 const *)g_4124a0);
		}
		distance = depth - box->radius - sphere->radius;
	}
	{
		__m128 const normal = contact->normal;
		__m128 const along = _mm_set_ss(-distance - sphere->radius);
		contact->position = _mm_add_ps(*centre, _mm_mul_ps(_mm_shuffle_ps(along, along, 0), normal));
		contact->position_and_distance[3] = distance;
		if (id == 0xffff)
		{
			id = owner->add_contact(a, b, input, contact);
		}
		unsigned short held = id;
		if (held != 0xffff)
		{
			contact->id = held;
		}
		else
		{
			results->contacts->size--;
		}
	}
	goto done;
miss:
	{
		results->contacts->size--;
		unsigned short held = id;
		if (held != 0xffff)
		{
			owner->release_id(held);
			id = 0xffff;
		}
	}
done:
	timer_add_2ebc80("Et");
}
