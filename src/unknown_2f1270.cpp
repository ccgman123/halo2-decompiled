// @flags /O2 /GL-
// Havok (third-party physics library): queries of a sphere against a
// triangle in the world: the triangle's corners taken out of its frame, and
// the point of the triangle's surface nearest the sphere passed on when it
// lies within their radii and the tolerance.
#include "pair_agents_2eab70.h"
#include "point_triangle_308d90.h"
#include "timer_mark_2ebc80.h"

void __cdecl function_308910(__m128 const *corners, float *out);

// a shape with a radius: a sphere, or a triangle whose corners follow
struct rounded_2f1270
{
	char unknown0[0xc];
	float radius;
	__m128 corners[3];
};

struct transform_2f1270
{
	__m128 axes[3];
	__m128 position;
};

// where a body sits: two vectors, then its transform
struct placement_2f1270
{
	__m128 field_0;
	__m128 field_10;
	transform_2f1270 transform;
};

// one side of a query: a shape, a key, where it sits, and its parent side
struct body_2f1270
{
	rounded_2f1270 const *shape;
	long key;
	placement_2f1270 const *placement;
	body_2f1270 const *parent;
};

struct input_2f1270
{
	void *dispatcher;
	void *filter;
	float tolerance;
};

// a point where the two meet: the point, with how far apart they are in its
// fourth term, the direction from the triangle, and the two sides
struct contact_2f1270
{
	__m128 position;
	__m128 normal;
	body_2f1270 const *a;
	body_2f1270 const *b;
};

// told of each contact found
class collector_2f1270
{
public:
	virtual void slot_0() {}
	virtual void slot_1(contact_2f1270 const *contact) {}
};

// a point taken out of a frame
inline void transform_2f1270_point(__m128 &out, transform_2f1270 const *frame, __m128 const &p)
{
	out = _mm_add_ps(_mm_add_ps(
		_mm_mul_ps(frame->axes[0], _mm_shuffle_ps(p, p, 0x00)),
		_mm_mul_ps(frame->axes[1], _mm_shuffle_ps(p, p, 0x55))),
		_mm_add_ps(_mm_mul_ps(frame->axes[2], _mm_shuffle_ps(p, p, 0xaa)), frame->position));
}

// a vector of four copies of a float
inline __m128 splat_2f1270(float value)
{
	__m128 const single = _mm_set_ss(value);
	return _mm_shuffle_ps(single, single, 0);
}

// score 24 (was 26): retail keeps the result of the query in a stack slot
// that ours drops, and the two sides and the triangle come out of the
// registers in another order.
// @retail 0x2f1270
void class_2f1270::function_2f1270(void const *a_, void const *b_, void const *input_, void *collector)
{
	timer_add_2ebc80("TtSphereTriangle");
	body_2f1270 const *a = (body_2f1270 const *)a_;
	body_2f1270 const *b = (body_2f1270 const *)b_;
	contact_2f1270 contact;
	contact.a = a;
	contact.b = b;
	rounded_2f1270 const *sphere = a->shape;
	rounded_2f1270 const *triangle = b->shape;
	__m128 const *centre = &a->placement->transform.position;
	transform_2f1270 const *frame = &b->placement->transform;
	__m128 corners[3];
	__m128 *out = corners;
	__m128 const *in = triangle->corners;
	for (int i = 0; i < 3; i++)
	{
		transform_2f1270_point(out[i], frame, in[i]);
	}
	{
		closest_308d90 closest;
		int status = function_308d90(centre, corners, terms, &closest);
		float const radii = sphere->radius + b->shape->radius;
		if (radii + ((input_2f1270 const *)input_)->tolerance > closest.distance)
		{
			contact.position = _mm_add_ps(*centre, _mm_mul_ps(splat_2f1270(b->shape->radius - closest.distance), closest.normal));
			contact.position.m128_f32[3] = closest.distance - radii;
			contact.normal = closest.normal;
			((collector_2f1270 *)collector)->slot_1(&contact);
		}
	}
	timer_add_2ebc80("Et");
}

// score 47: retail's frame is 0xa4 bytes and ours 0x94, so the query's
// result, the contact and the closest point sit at other offsets, and
// retail keeps the result of the query in a stack slot that ours drops.
// @retail 0x2f13e0
void __cdecl function_2f13e0(void const *a_, void const *b_, void const *input_, void *collector)
{
	timer_add_2ebc80("TtSphereTriangle");
	body_2f1270 const *a = (body_2f1270 const *)a_;
	body_2f1270 const *b = (body_2f1270 const *)b_;
	float terms[4];
	function_308910(b->shape->corners, terms);
	contact_2f1270 contact;
	contact.a = a;
	contact.b = b;
	rounded_2f1270 const *sphere = a->shape;
	rounded_2f1270 const *triangle = b->shape;
	__m128 const *centre = &a->placement->transform.position;
	transform_2f1270 const *frame = &b->placement->transform;
	__m128 corners[3];
	__m128 *out = corners;
	__m128 const *in = triangle->corners;
	for (int i = 0; i < 3; i++)
	{
		transform_2f1270_point(out[i], frame, in[i]);
	}
	{
		closest_308d90 closest;
		int status = function_308d90(centre, corners, terms, &closest);
		float const radii = sphere->radius + b->shape->radius;
		if (radii + ((input_2f1270 const *)input_)->tolerance > closest.distance)
		{
			contact.position = _mm_add_ps(*centre, _mm_mul_ps(splat_2f1270(b->shape->radius - closest.distance), closest.normal));
			contact.position.m128_f32[3] = closest.distance - radii;
			contact.normal = closest.normal;
			((collector_2f1270 *)collector)->slot_1(&contact);
		}
	}
	timer_add_2ebc80("Et");
}
