// @flags /O2 /GL-
// Havok (third-party physics library): keeping a manifold's contacts up to
// date: each contact's two points are taken into world space by their
// bodies' transforms, and the contact is kept, as a world point between them
// with its distance, while it is near enough and has not slid too far;
// otherwise its id is given back and the last contact takes its place.
#include <xmmintrin.h>
#include "growable_array_2d91d0.h"

static __declspec(align(16)) const float g_ones_412420[4] = {1.0f, 1.0f, 1.0f, 1.0f};

struct transform_302eb0
{
	__m128 axes[3];
	__m128 position;

	// a point placed by the transform
	__m128 apply(__m128 const &p) const
	{
		__m128 z = _mm_add_ps(_mm_mul_ps(axes[2], _mm_shuffle_ps(p, p, 0xaa)), position);
		__m128 y = _mm_mul_ps(axes[1], _mm_shuffle_ps(p, p, 0x55));
		__m128 x = _mm_mul_ps(axes[0], _mm_shuffle_ps(p, p, 0x00));
		return _mm_add_ps(_mm_add_ps(x, y), z);
	}
};

// where a body sits: two vectors, then its transform
struct placement_302eb0
{
	__m128 field_0;
	__m128 field_10;
	transform_302eb0 transform;
};

struct body_302eb0
{
	void const *shape;
	long key;
	placement_302eb0 const *placement;
};

struct tolerance_302eb0
{
	long field_0;
	long field_4;
	float slide;
};

struct input_302eb0
{
	long field_0;
	long field_4;
	float distance;
	tolerance_302eb0 const *tolerance;
};

// a point whose fourth lane holds a weight, or an id in its low half
union point_302eb0
{
	__m128 point;
	struct
	{
		float xyz[3];
		union
		{
			float weight;
			unsigned short id;
		};
	} lanes;
};

// a contact as kept: a point on each body in its own frame, and the normal
struct contact_302eb0
{
	point_302eb0 first;
	point_302eb0 second;
	__m128 normal;
};

// a contact in world space, its distance in the point's fourth lane
struct world_contact_302eb0
{
	point_302eb0 point;
	__m128 normal;
	unsigned short id;
};

ELEMENTS_2D91D0_ALIGNED(world_contacts_302eb0, world_contact_302eb0);

class id_owner_302eb0
{
public:
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void release_id(unsigned short id) {}
};

// the sum of a vector's first three terms
inline float sum_302eb0(__m128 const &v)
{
	float sum;
	_mm_store_ss(&sum, _mm_add_ss(_mm_shuffle_ps(v, v, 0xaa), _mm_add_ss(_mm_shuffle_ps(v, v, 0x55), v)));
	return sum;
}

// a vector of four copies of a float
inline __m128 splat_302eb0(float value)
{
	__m128 const single = _mm_set_ss(value);
	return _mm_shuffle_ps(single, single, 0);
}

// @retail 0x302eb0
void __cdecl function_302eb0(body_302eb0 const *a, body_302eb0 const *b, input_302eb0 const *input, float minimum,
	contact_302eb0 *contacts, int *count, world_contacts_302eb0 **out_list, id_owner_302eb0 *ids)
{
	float const slide = input->tolerance->slide;
	float const slide_squared = slide * slide;
	int i = 0;
	world_contacts_302eb0 *out = *out_list;
	long const first = out->size;
	out->resize(*count + first);
	world_contact_302eb0 *written = out->data + first;
	contact_302eb0 *contact = contacts;
	for (; i < *count;)
	{
		transform_302eb0 const &a_transform = a->placement->transform;
		transform_302eb0 const &b_transform = b->placement->transform;
		__m128 const on_first = a_transform.apply(contact->first.point);
		__m128 const on_second = b_transform.apply(contact->second.point);
		__m128 const normal = contact->normal;
		float const distance = sum_302eb0(_mm_mul_ps(_mm_sub_ps(on_first, on_second), normal));
		__m128 const along = _mm_add_ps(on_second, _mm_mul_ps(splat_302eb0(distance), normal));
		__m128 const offset = _mm_sub_ps(on_first, along);
		float const slid = sum_302eb0(_mm_mul_ps(offset, offset));
		if (distance < input->distance && distance > minimum && slid < slide_squared)
		{
			__m128 const weight = _mm_set_ps1(contact->first.lanes.weight);
			written->point.point = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(*(__m128 const *)g_ones_412420, weight), on_second), _mm_mul_ps(weight, on_first));
			written->normal = contact->normal;
			written->point.lanes.weight = distance;
			written->id = contact->second.lanes.id;
			i++;
			contact++;
			written++;
		}
		else
		{
			ids->release_id(contact->second.lanes.id);
			(*count)--;
			contact_302eb0 const &last = contacts[*count];
			contact->first.point = last.first.point;
			contact->second.point = last.second.point;
			contact->normal = last.normal;
			(*out_list)->size--;
		}
	}
}
