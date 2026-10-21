// @flags /O2 /GL-
// Havok (third-party physics library): the contacts of a shape of several
// spheres with a triangle, kept by the pair's object: the triangle's corners
// and the spheres' centres taken into the world, and each sphere close enough
// to the triangle given a contact with the id its owner hands out once; a
// sphere that moves away hands its id back. The query is marked in the timing
// stream.
#include "timer_mark_2ebc80.h"
#include "growable_array_2d91d0.h"
#include "point_triangle_308d90.h"
#include "pair_agents_2eab70.h"

// three axes and a position (0x2da4d0)
struct c_query_transform
{
	__m128 axes[3];
	__m128 position;
};

// a point taken out of a transform's frame (0x2da610, inlined here)
inline __m128 transform_point_2f7c40(c_query_transform const *m, __m128 const *point)
{
	__m128 p = *point;
	__m128 z = _mm_add_ps(_mm_mul_ps(m->axes[2], _mm_shuffle_ps(p, p, 0xaa)), m->position);
	__m128 y = _mm_mul_ps(m->axes[1], _mm_shuffle_ps(p, p, 0x55));
	__m128 x = _mm_mul_ps(m->axes[0], _mm_shuffle_ps(p, p, 0x00));
	return _mm_add_ps(_mm_add_ps(x, y), z);
}

// where a body sits: two vectors, then its transform
struct placement_2f7c40
{
	__m128 field_0;
	__m128 field_10;
	c_query_transform transform;
};

// a triangle with a radius
struct triangle_2f7c40
{
	void const *table;
	long field_4;
	long field_8;
	float radius;
	__m128 corners[3];
};

// spheres, each a centre with its radius in the fourth lane
struct spheres_2f7c40
{
	void const *table;
	long field_4;
	long field_8;
	long count;
	__m128 spheres[8];
};

// one side of a query: a shape, the key of the part concerned, where it sits,
// and the side it is a part of
struct body_2f7c40
{
	void const *shape;
	long key;
	placement_2f7c40 const *placement;
	body_2f7c40 const *parent;
};

struct input_2f7c40
{
	long field_0;
	long field_4;
	float tolerance;
};

// a contact: its point, with the separation in the fourth lane, its normal,
// and its id
struct contact_2f7c40
{
	union
	{
		__m128 position;
		float position_and_distance[4];
	};
	__m128 normal;
	unsigned short id;
};

ELEMENTS_2D91D0_ALIGNED(contacts_2f7c40, contact_2f7c40);

struct results_2f7c40
{
	contacts_2f7c40 *contacts;
};

class contact_owner_2f7c40
{
public:
	virtual void slot_0() {}
	virtual unsigned short add_contact(void const *a, void const *b, void const *input, contact_2f7c40 *contact) { return 0; }
	virtual void release_id(unsigned short id) {}
};

// @retail 0x2f7c40
void class_2f7c40::function_2f7c40(void const *a, void const *b, void const *input, void *collector)
{
	timer_add_2ebc80("TtMultiSphereTri");
	body_2f7c40 const *side_a = (body_2f7c40 const *)a;
	body_2f7c40 const *side_b = (body_2f7c40 const *)b;
	spheres_2f7c40 const *spheres = (spheres_2f7c40 const *)side_a->shape;
	triangle_2f7c40 const *triangle = (triangle_2f7c40 const *)side_b->shape;
	__m128 corners[3];
	{
		c_query_transform const *frame = &side_b->placement->transform;
		__m128 const *corner = triangle->corners;
		__m128 *placed = corners;
		for (int count = 3; count != 0; count--)
		{
			*placed = transform_point_2f7c40(frame, corner);
			placed++;
			corner++;
		}
	}
	long const sphere_count = spheres->count;
	__m128 const *first_sphere = spheres->spheres;
	__m128 centres[8];
	{
		c_query_transform const *frame = &side_a->placement->transform;
		__m128 const *sphere = first_sphere;
		__m128 *placed = centres;
		for (int i = sphere_count - 1; i >= 0; i--)
		{
			*placed = transform_point_2f7c40(frame, sphere);
			placed++;
			sphere++;
		}
	}
	results_2f7c40 *results = (results_2f7c40 *)collector;
	__m128 const *centre = centres;
	results->contacts->ensure_capacity(results->contacts->size + sphere_count);
	__m128 const *sphere = first_sphere;
	for (int i = sphere_count - 1; i >= 0; i--)
	{
		float const radius = sphere->m128_f32[3];
		float const radii = radius + triangle->radius;
		closest_308d90 closest;
		function_308d90(centre, corners, terms, &closest);
		if (radii + ((input_2f7c40 const *)input)->tolerance > closest.distance)
		{
			contacts_2f7c40 *contacts = results->contacts;
			int const index = contacts->size;
			contacts->size = index + 1;
			contact_2f7c40 *contact = &contacts->data[index];
			__m128 const single = _mm_set_ss(-radius);
			__m128 const scale = _mm_shuffle_ps(single, single, 0);
			contact->position = _mm_add_ps(*centre, _mm_mul_ps(scale, closest.normal));
			contact->normal = closest.normal;
			contact->position_and_distance[3] = closest.distance - radii;
			unsigned short held = ids[i];
			if (held == 0xffff)
			{
				held = owner->add_contact(a, b, input, contact);
				ids[i] = held;
			}
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
			unsigned short const held = ids[i];
			if (held != 0xffff)
			{
				owner->release_id(held);
				ids[i] = 0xffff;
			}
		}
		centre++;
		sphere++;
	}
	timer_add_2ebc80("Et");
}
