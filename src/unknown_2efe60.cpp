// @flags /O2 /GL-
// Havok (third-party physics library): the contact between two spheres,
// bracketed by two marks in the timing stream: when their centres are closer
// than the sum of their radii and a tolerance, a contact point halfway
// between their surfaces along the line of centres, its normal and distance,
// kept under an id the owner hands out once; otherwise the id is handed back.
#include "timer_mark_2ebc80.h"
#include "growable_array_2d91d0.h"
#include <xmmintrin.h>
#include <math.h>

// the direction used when the centres coincide
static __declspec(align(16)) const float g_461ea0[4] = {1.0f, 0.0f, 0.0f, 0.0f};

struct sphere_2efe60
{
	char unknown0[0xc];
	float radius;
};

struct motion_2efe60
{
	char unknown0[0x50];
	__m128 position;
};

struct body_2efe60
{
	sphere_2efe60 const *shape;
	long key;
	motion_2efe60 const *motion;
};

struct input_2efe60
{
	char unknown0[8];
	float tolerance;
};

// a contact: its point, with the distance in the fourth lane, its normal, and
// its id
struct contact_2efe60
{
	union
	{
		__m128 position;
		float position_and_distance[4];
	};
	__m128 normal;
	unsigned short id;
};

ELEMENTS_2D91D0_ALIGNED(contacts_2efe60, contact_2efe60);

struct results_2efe60
{
	contacts_2efe60 *contacts;
};

class owner_2efe60
{
public:
	virtual void slot_0() {}
	virtual unsigned short add_contact(body_2efe60 const *a, body_2efe60 const *b, input_2efe60 const *input, contact_2efe60 *contact) { return 0; }
	virtual void release_id(unsigned short id) {}
};

class sphere_contact_2efe60
{
public:
	virtual void slot_0() {}

	long field_4;
	owner_2efe60 *owner;
	unsigned short id;

	void function_2efe60(body_2efe60 const *a, body_2efe60 const *b, input_2efe60 const *input, results_2efe60 *results);
};

// @retail 0x2efe60
// score 23 (was 23): retail loads the second body's position, then its shape,
// then the first body's shape, and keeps the offset in xmm1; ours loads them
// in the other order and keeps them in other registers. Seventy source
// shapes (the loads in every order, the radii split and summed in other
// ways, the values cached in locals) and the permuter's 280 tries all scored
// 23 or worse.
void sphere_contact_2efe60::function_2efe60(body_2efe60 const *a, body_2efe60 const *b, input_2efe60 const *input, results_2efe60 *results)
{
	timer_add_2ebc80("TtSphereSphere");
	__m128 const *b_position = &b->motion->position;
	sphere_2efe60 const *b_shape = b->shape;
	sphere_2efe60 const *a_shape = a->shape;
	__m128 offset = _mm_sub_ps(a->motion->position, *b_position);
	float radius = input->tolerance + b_shape->radius + a_shape->radius;
	float const radius_squared = radius * radius;
	__m128 squares = _mm_mul_ps(offset, offset);
	float distance_squared;
	_mm_store_ss(&distance_squared, _mm_add_ss(_mm_shuffle_ps(squares, squares, 0xaa), _mm_add_ss(_mm_shuffle_ps(squares, squares, 0x55), squares)));
	if (distance_squared < radius_squared)
	{
		contacts_2efe60 *contacts = results->contacts;
		contact_2efe60 *contact = &contacts->next_slot();
		float distance;
		if (distance_squared > 0.0f)
		{
			float inverse = 1.0f / (float)sqrt(distance_squared);
			distance = inverse * distance_squared;
			__m128 scale = _mm_set_ss(inverse);
			contact->normal = _mm_mul_ps(_mm_shuffle_ps(scale, scale, 0), offset);
			distance = distance - (b_shape->radius + a_shape->radius);
		}
		else
		{
			contact->normal = *(__m128 const *)g_461ea0;
			distance = -(b_shape->radius + a_shape->radius);
		}
		__m128 along = _mm_set_ss(0.5f * distance + b_shape->radius);
		contact->position = _mm_add_ps(*b_position, _mm_mul_ps(_mm_shuffle_ps(along, along, 0), contact->normal));
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
	else
	{
		unsigned short held = id;
		if (held != 0xffff)
		{
			owner->release_id(held);
			id = 0xffff;
		}
	}
	timer_add_2ebc80("Et");
}
