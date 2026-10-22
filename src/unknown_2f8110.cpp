// @flags /O2 /GL-
// Havok (third-party physics library): the contacts of a shape of several
// spheres with a triangle, worked out without an agent: the triangle's
// corners and the spheres' centres taken into the world, the terms that solve
// for a point's weights in the triangle's plane worked out, and each sphere
// close enough to the triangle passed to the collector as a contact. The
// query is marked in the timing stream.
#include "timer_mark_2ebc80.h"
#include "point_triangle_308d90.h"

void __cdecl function_308910(__m128 const *corners, float *out);

// three axes and a position (0x2da4d0)
struct c_query_transform
{
	__m128 axes[3];
	__m128 position;
};

// a point taken out of a transform's frame (0x2da610, inlined here)
inline __m128 transform_point_2f8110(c_query_transform const *m, __m128 const *point)
{
	__m128 p = *point;
	__m128 z = _mm_add_ps(_mm_mul_ps(m->axes[2], _mm_shuffle_ps(p, p, 0xaa)), m->position);
	__m128 y = _mm_mul_ps(m->axes[1], _mm_shuffle_ps(p, p, 0x55));
	__m128 x = _mm_mul_ps(m->axes[0], _mm_shuffle_ps(p, p, 0x00));
	return _mm_add_ps(_mm_add_ps(x, y), z);
}

// where a body sits: two vectors, then its transform
struct placement_2f8110
{
	__m128 field_0;
	__m128 field_10;
	c_query_transform transform;
};

// a triangle with a radius
struct triangle_2f8110
{
	void const *table;
	long field_4;
	long field_8;
	float radius;
	__m128 corners[3];
};

// spheres, each a centre with its radius in the fourth lane
struct spheres_2f8110
{
	void const *table;
	long field_4;
	long field_8;
	long count;
	__m128 spheres[8];
};

// one side of a query: a shape, the key of the part concerned, where it sits,
// and the side it is a part of
struct body_2f8110
{
	void const *shape;
	long key;
	placement_2f8110 const *placement;
	body_2f8110 const *parent;
};

struct input_2f8110
{
	long field_0;
	long field_4;
	float tolerance;
};

// a contact: its point, with the separation in the fourth lane, its normal,
// and the two sides
struct contact_2f8110
{
	__m128 point;
	__m128 normal;
	void const *a;
	void const *b;
};

// takes the contacts a query finds
class collector_2f8110
{
public:
	virtual void slot_0() {}
	virtual void add_contact(contact_2f8110 const *contact) {}
};

// score 77 (was 81): the contact's two sides are set before the call that
// works out the terms, as retail does; ours still keeps the triangle's shape
// in esi where retail keeps it in ecx, and keeps the sphere count in a stack
// slot where retail reads it from the shape
// @retail 0x2f8110
void __cdecl function_2f8110(void const *a, void const *b, void const *input, void *collector)
{
	timer_add_2ebc80("TtMultiSphereTriangle");
	body_2f8110 const *side_a = (body_2f8110 const *)a;
	body_2f8110 const *side_b = (body_2f8110 const *)b;
	triangle_2f8110 const *triangle = (triangle_2f8110 const *)side_b->shape;
	spheres_2f8110 const *spheres = (spheres_2f8110 const *)side_a->shape;
	long const sphere_count = spheres->count;
	__m128 corners[3];
	{
		c_query_transform const *frame = &side_b->placement->transform;
		__m128 const *corner = triangle->corners;
		__m128 *placed = corners;
		for (int count = 3; count != 0; count--)
		{
			*placed = transform_point_2f8110(frame, corner);
			placed++;
			corner++;
		}
	}
	__m128 centres[8];
	{
		c_query_transform const *frame = &side_a->placement->transform;
		__m128 const *sphere = spheres->spheres;
		__m128 *placed = centres;
		for (int i = sphere_count - 1; i >= 0; i--)
		{
			*placed = transform_point_2f8110(frame, sphere);
			placed++;
			sphere++;
		}
	}
	float terms[4];
	contact_2f8110 contact;
	contact.a = a;
	contact.b = b;
	function_308910(triangle->corners, terms);
	__m128 const *centre = centres;
	__m128 const *sphere = spheres->spheres;
	for (int count = sphere_count; count > 0; count--)
	{
		float const radii = sphere->m128_f32[3] + triangle->radius;
		closest_308d90 closest;
		function_308d90(centre, corners, terms, &closest);
		if (radii + ((input_2f8110 const *)input)->tolerance > closest.distance)
		{
			__m128 const single = _mm_set_ss(triangle->radius - closest.distance);
			__m128 const scale = _mm_shuffle_ps(single, single, 0);
			contact.point = _mm_add_ps(*centre, _mm_mul_ps(scale, closest.normal));
			contact.point.m128_f32[3] = closest.distance - radii;
			contact.normal = closest.normal;
			((collector_2f8110 *)collector)->add_contact(&contact);
		}
		centre++;
		sphere++;
	}
	timer_add_2ebc80("Et");
}
