// @flags /O2 /GL-
// Havok (third-party physics library): the contact between a sphere and a
// triangle, bracketed by two marks in the timing stream: the triangle's
// corners are taken out of its frame, and the point of the triangle nearest
// the sphere's centre gives the contact when it lies within their radii and
// the tolerance. The contact keeps an id the owner hands out once; a miss
// hands the id back.
#include <xmmintrin.h>
#include "growable_array_2d91d0.h"
#include "pair_agents_2eab70.h"
#include "point_triangle_308d90.h"
#include "timer_mark_2ebc80.h"

// a shape with a radius: a sphere, or a triangle whose corners follow
struct rounded_2f1080
{
	char unknown0[0xc];
	float radius;
	__m128 corners[3];
};

struct transform_2f1080
{
	__m128 axes[3];
	__m128 position;
};

// where a body sits: two vectors, then its transform
struct placement_2f1080
{
	__m128 field_0;
	__m128 field_10;
	transform_2f1080 transform;
};

// one side of a query: a shape, a key, where it sits, and its parent side
struct body_2f1080
{
	rounded_2f1080 const *shape;
	long key;
	placement_2f1080 const *placement;
	body_2f1080 const *parent;
};

struct input_2f1080
{
	char unknown0[8];
	float tolerance;
};

// a contact: its point, with the separation in the fourth lane, its normal,
// and its id
struct contact_2f1080
{
	union
	{
		__m128 position;
		float position_and_distance[4];
	};
	__m128 normal;
	unsigned short id;
};

ELEMENTS_2D91D0_ALIGNED(contacts_2f1080, contact_2f1080);

struct results_2f1080
{
	contacts_2f1080 *contacts;
};

class contact_owner_2f1080
{
public:
	virtual void slot_0() {}
	virtual unsigned short add_contact(void const *a, void const *b, void const *input, contact_2f1080 *contact) { return 0; }
	virtual void release_id(unsigned short id) {}
};

// a point placed by a transform
struct placed_point_2f1080
{
	__m128 value;

	void set_transformed(transform_2f1080 const *transform, __m128 const *point)
	{
		__m128 p = *point;
		__m128 z = _mm_add_ps(_mm_mul_ps(transform->axes[2], _mm_shuffle_ps(p, p, 0xaa)), transform->position);
		__m128 y = _mm_mul_ps(transform->axes[1], _mm_shuffle_ps(p, p, 0x55));
		__m128 x = _mm_mul_ps(transform->axes[0], _mm_shuffle_ps(p, p, 0x00));
		value = _mm_add_ps(_mm_add_ps(x, y), z);
	}
};

// score 27 (was 39): retail keeps the point-triangle status in the stack slot
// 0x28 and the normal in xmm1; ours drops the unused status, reloads the
// normal into xmm0, and its x87 reload of the distance comes late
// @retail 0x2f1080
void class_2f1080::function_2f1080(void const *a_, void const *b_, void const *input_, void *collector)
{
	timer_add_2ebc80("TtSphereTriangle");
	body_2f1080 const *a = (body_2f1080 const *)a_;
	body_2f1080 const *b = (body_2f1080 const *)b_;
	input_2f1080 const *input = (input_2f1080 const *)input_;
	results_2f1080 *results = (results_2f1080 *)collector;
	contact_2f1080 *contact = &results->contacts->next_slot();
	rounded_2f1080 const *sphere = a->shape;
	rounded_2f1080 const *triangle = b->shape;
	__m128 const *centre = &a->placement->transform.position;
	transform_2f1080 const *frame = &b->placement->transform;
	placed_point_2f1080 corners[3];
	__m128 const *vertex = triangle->corners;
	placed_point_2f1080 *corner = corners;
	for (int count = 3; count != 0; count--)
	{
		corner->set_transformed(frame, vertex);
		corner++;
		vertex++;
	}
	closest_308d90 closest;
	int status = function_308d90(centre, &corners[0].value, terms, &closest);
	float const radii = sphere->radius + triangle->radius;
	if (radii + input->tolerance > closest.distance)
	{
		__m128 const single = _mm_set_ss(triangle->radius - closest.distance);
		__m128 const scale = _mm_shuffle_ps(single, single, 0);
		contact->position = _mm_add_ps(*centre, _mm_mul_ps(scale, closest.normal));
		contact->normal = closest.normal;
		contact->position_and_distance[3] = closest.distance - radii;
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
