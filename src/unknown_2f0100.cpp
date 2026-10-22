// @flags /O2 /GL-
// Havok (third-party physics library): a sphere against a capsule: the
// contact between them from the point of the capsule's segment nearest the
// sphere's centre, kept under an id its owner hands out once, or passed on;
// and the pair reported when the centre is closer to the segment than the sum
// of their radii. Each is bracketed by two marks in the timing stream.
#include "pair_collector_2f09b0.h"
#include "timer_mark_2ebc80.h"
#include "growable_array_2d91d0.h"
#include <xmmintrin.h>
#include <math.h>

// the point of a segment nearest a point (0x308830)
long __cdecl function_308830(__m128 const &point, __m128 const &start, __m128 const &end, __m128 &out);

struct sphere_2f09b0
{
	char unknown0[0xc];
	float radius;
};

// a capsule: its radius and the two ends of its segment
struct capsule_2f09b0
{
	char unknown0[0xc];
	float radius;
	__m128 ends[2];
};

// three axes and a position
struct transform_2f09b0
{
	__m128 axes[3];
	__m128 position;
};

struct motion_2f09b0
{
	char unknown0[0x20];
	transform_2f09b0 transform;
};

struct body_2f09b0
{
	void const *shape;
	long key;
	motion_2f09b0 const *motion;
};

// a point placed by a transform
struct point_2f09b0
{
	__m128 value;

	void set_transformed(transform_2f09b0 const *transform, __m128 const *point)
	{
		__m128 p = *point;
		__m128 z = _mm_add_ps(_mm_mul_ps(transform->axes[2], _mm_shuffle_ps(p, p, 0xaa)), transform->position);
		__m128 y = _mm_mul_ps(transform->axes[1], _mm_shuffle_ps(p, p, 0x55));
		__m128 x = _mm_mul_ps(transform->axes[0], _mm_shuffle_ps(p, p, 0x00));
		value = _mm_add_ps(_mm_add_ps(x, y), z);
	}
};

struct input_2f0450
{
	long field_0;
	long field_4;
	float tolerance;
};

// a contact: its point, with the separation in the fourth lane, its normal,
// and the two sides
struct contact_2f0450
{
	__m128 point;
	__m128 normal;
	body_2f09b0 const *a;
	body_2f09b0 const *b;
};

class contact_collector_2f0450
{
public:
	virtual void slot_0() {}
	virtual void add_contact(contact_2f0450 const *contact) {}
};

// one over the square root of the low term, refined by a Newton step, in
// all four terms
__forceinline __m128 inverse_sqrt_2f0450(__m128 const &value)
{
	__m128 const estimate = _mm_rsqrt_ss(value);
	__m128 const product = _mm_mul_ss(_mm_mul_ss(value, estimate), estimate);
	__m128 const refined = _mm_mul_ss(_mm_mul_ss(_mm_set_ss(0.5f), estimate), _mm_sub_ss(_mm_set_ss(3.0f), product));
	return _mm_shuffle_ps(refined, refined, 0);
}

// a vector of four copies of a float
inline __m128 splat_2f0450(float value)
{
	__m128 const single = _mm_set_ss(value);
	return _mm_shuffle_ps(single, single, 0);
}

// a contact kept by its owner: its point, with the distance in the fourth
// lane, its normal, and its id
// a vector, which can be normalized in place
struct vector_2f0100
{
	__m128 value;

	__forceinline void scale_to_unit_length()
	{
		__m128 const squared = _mm_mul_ps(value, value);
		__m128 const length_squared = _mm_add_ss(_mm_shuffle_ps(squared, squared, 0xaa), _mm_add_ss(_mm_shuffle_ps(squared, squared, 0x55), squared));
		value = _mm_mul_ps(inverse_sqrt_2f0450(length_squared), value);
	}
};

struct kept_contact_2f0100
{
	union
	{
		__m128 position;
		float position_and_distance[4];
	};
	vector_2f0100 normal;
	unsigned short id;
};

ELEMENTS_2D91D0_ALIGNED(kept_contacts_2f0100, kept_contact_2f0100);

struct results_2f0100
{
	kept_contacts_2f0100 *contacts;
};

class contact_owner_2f0100
{
public:
	virtual void slot_0() {}
	virtual unsigned short add_contact(void const *a, void const *b, void const *input, kept_contact_2f0100 *contact) { return 0; }
	virtual void release_id(unsigned short id) {}
};

class class_2f0100
{
public:
	char unknown0[8];
	contact_owner_2f0100 *owner;
	unsigned short id;

	void function_2f0100(void const *a, void const *b, void const *input, void *collector);
};

// score 6 (was 11): the two scaled indices of the perpendicular axis are held in the
// opposite registers: the first index is in eax here and ecx in retail.
// the contact of a sphere with a capsule, kept while they are within the
// tolerance; when the centre lies on the segment, the normal is one at right
// angles to it
// @retail 0x2f0100
void class_2f0100::function_2f0100(void const *a_, void const *b_, void const *input_, void *collector)
{
	timer_add_2ebc80("TtSphereCapsule");
	body_2f09b0 const *a = (body_2f09b0 const *)a_;
	body_2f09b0 const *b = (body_2f09b0 const *)b_;
	input_2f0450 const *input = (input_2f0450 const *)input_;
	results_2f0100 *results = (results_2f0100 *)collector;
	kept_contact_2f0100 *contact = &results->contacts->next_slot();
	sphere_2f09b0 const *sphere = (sphere_2f09b0 const *)a->shape;
	capsule_2f09b0 const *capsule = (capsule_2f09b0 const *)b->shape;
	__m128 const *centre = &a->motion->transform.position;
	point_2f09b0 ends[2];
	__m128 const *vertex = capsule->ends;
	transform_2f09b0 const *transform = &b->motion->transform;
	point_2f09b0 *end = ends;
	for (int count = 2; count != 0; count--)
	{
		end->set_transformed(transform, vertex);
		end++;
		vertex++;
	}
	__m128 nearest;
	function_308830(*centre, ends[0].value, ends[1].value, nearest);
	float const radius = sphere->radius + capsule->radius;
	__m128 const offset = _mm_sub_ps(*centre, nearest);
	float const limit = radius + input->tolerance;
	__m128 const squares = _mm_mul_ps(offset, offset);
	float distance_squared;
	_mm_store_ss(&distance_squared, _mm_add_ss(_mm_shuffle_ps(squares, squares, 0xaa), _mm_add_ss(_mm_shuffle_ps(squares, squares, 0x55), squares)));
	if (!(distance_squared >= limit * limit))
	{
		float distance;
		if (distance_squared > 0.0f)
		{
			contact->normal.value = offset;
			distance = (float)sqrt(distance_squared);
		}
		else
		{
			distance = 0.0f;
			__m128 axis;
			axis = _mm_sub_ps(ends[1].value, ends[0].value);
			float const *component = (float const *)&axis;
			int smallest = 0;
			int first = 1;
			int second = 2;
			float least = fabsf(component[0]);
			float const y = fabsf(component[1]);
			float const z = fabsf(component[2]);
			if (y < least)
			{
				first = 0;
				least = y;
				smallest = 1;
			}
			if (z < least)
			{
				second = smallest;
				smallest = 2;
			}
			float *normal = (float *)&contact->normal;
			normal[smallest] = 0.0f;
			float const negated = -component[first];
			normal[first] = component[second];
			normal[second] = negated;
		}
		contact->normal.scale_to_unit_length();
		contact->position = _mm_add_ps(*centre, _mm_mul_ps(splat_2f0450(capsule->radius - distance), contact->normal.value));
		contact->position_and_distance[3] = distance - radius;
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
	else
	{
		results->contacts->size--;
		unsigned short held = id;
		if (held != 0xffff)
		{
			owner->release_id(held);
			id = 0xffff;
		}
	}
	timer_add_2ebc80("Et");
}
// the contact of a sphere with a capsule, passed on when they are within the
// tolerance; when the centre lies on the segment, the normal is one at right
// angles to it
__forceinline void sphere_capsule_contact_2f0450(void const *a_, void const *b_, void const *input_, void *collector_)
{
	body_2f09b0 const *a = (body_2f09b0 const *)a_;
	body_2f09b0 const *b = (body_2f09b0 const *)b_;
	input_2f0450 const *input = (input_2f0450 const *)input_;
	timer_add_2ebc80("TtSphereCapsule");
	sphere_2f09b0 const *sphere = (sphere_2f09b0 const *)a->shape;
	capsule_2f09b0 const *capsule = (capsule_2f09b0 const *)b->shape;
	__m128 const *centre = &a->motion->transform.position;
	contact_2f0450 contact;
	contact.a = a;
	contact.b = b;
	point_2f09b0 ends[2];
	__m128 const *vertex = capsule->ends;
	transform_2f09b0 const *transform = &b->motion->transform;
	point_2f09b0 *end = ends;
	for (int count = 2; count != 0; count--)
	{
		end->set_transformed(transform, vertex);
		end++;
		vertex++;
	}
	__m128 nearest;
	function_308830(*centre, ends[0].value, ends[1].value, nearest);
	float const radius = sphere->radius + capsule->radius;
	__m128 const offset = _mm_sub_ps(*centre, nearest);
	float const limit = radius + input->tolerance;
	__m128 const squares = _mm_mul_ps(offset, offset);
	float distance_squared;
	_mm_store_ss(&distance_squared, _mm_add_ss(_mm_shuffle_ps(squares, squares, 0xaa), _mm_add_ss(_mm_shuffle_ps(squares, squares, 0x55), squares)));
	if (!(distance_squared >= limit * limit))
	{
		float distance;
		__m128 direction;
		if (distance_squared > 0.0f)
		{
			distance = (float)sqrt(distance_squared);
			direction = offset;
		}
		else
		{
			distance = 0.0f;
			__m128 axis;
			axis = _mm_sub_ps(ends[1].value, ends[0].value);
			float const *component = (float const *)&axis;
			int smallest = 0;
			int first = 1;
			int second = 2;
			float least = fabsf(component[0]);
			float const y = fabsf(component[1]);
			float const z = fabsf(component[2]);
			if (y < least)
			{
				first = 0;
				least = y;
				smallest = 1;
			}
			if (z < least)
			{
				second = smallest;
				smallest = 2;
			}
			float const negated = -component[first];
			contact.normal.m128_f32[smallest] = 0.0f;
			contact.normal.m128_f32[first] = component[second];
			contact.normal.m128_f32[second] = negated;
			direction = contact.normal;
		}
		__m128 const squared = _mm_mul_ps(direction, direction);
		__m128 const length_squared = _mm_add_ss(_mm_shuffle_ps(squared, squared, 0xaa), _mm_add_ss(_mm_shuffle_ps(squared, squared, 0x55), squared));
		contact.normal = _mm_mul_ps(inverse_sqrt_2f0450(length_squared), direction);
		contact.point = _mm_add_ps(*centre, _mm_mul_ps(splat_2f0450(capsule->radius - distance), contact.normal));
		contact.point.m128_f32[3] = distance - radius;
		((contact_collector_2f0450 *)collector_)->add_contact(&contact);
	}
	timer_add_2ebc80("Et");
}

class class_2f0450
{
public:
	void function_2f0450(void const *a, void const *b, void const *input, void *collector);
};

// score 2 (was 7): picking the perpendicular axis, retail loads the moved component
// before it stores the zero into the normal.
// @retail 0x2f0450
void class_2f0450::function_2f0450(void const *a, void const *b, void const *input, void *collector)
{
	sphere_capsule_contact_2f0450(a, b, input, collector);
}

// score 2 (was 7): as 0x2f0450.
// the same as a function
// @retail 0x2f0700
void __cdecl function_2f0700(void const *a, void const *b, void const *input, void *collector)
{
	sphere_capsule_contact_2f0450(a, b, input, collector);
}
// @retail 0x2f09b0
void __cdecl function_2f09b0(body_2f09b0 const *a, body_2f09b0 const *b, void const *input, collector_2f09b0 *collector)
{
	timer_add_2ebc80("TtSphereCapsule");
	sphere_2f09b0 const *sphere = (sphere_2f09b0 const *)a->shape;
	capsule_2f09b0 const *capsule = (capsule_2f09b0 const *)b->shape;
	__m128 const *centre = &a->motion->transform.position;
	point_2f09b0 ends[2];
	__m128 const *vertex = capsule->ends;
	transform_2f09b0 const *transform = &b->motion->transform;
	point_2f09b0 *end = ends;
	for (int count = 2; count != 0; count--)
	{
		end->set_transformed(transform, vertex);
		end++;
		vertex++;
	}
	__m128 nearest;
	function_308830(*centre, ends[0].value, ends[1].value, nearest);
	float const radius = capsule->radius + sphere->radius;
	__m128 const offset = _mm_sub_ps(nearest, *centre);
	__m128 const squares = _mm_mul_ps(offset, offset);
	float distance_squared;
	_mm_store_ss(&distance_squared, _mm_add_ss(_mm_shuffle_ps(squares, squares, 0xaa), _mm_add_ss(_mm_shuffle_ps(squares, squares, 0x55), squares)));
	if (distance_squared < radius * radius)
	{
		collector->add_pair(a, b);
	}
	timer_add_2ebc80("Et");
}
