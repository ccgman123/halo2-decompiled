// @flags /O2 /GL-
// Havok (third-party physics library): a shape of several rays against a
// convex shape that keeps its contacts: as 0x2f89b0, each ray is carried
// into the convex shape's frame and cast against it, and each hit becomes a
// contact point; a ray that hits keeps the point the contact manager gave it
// the first time, and a ray that misses gives its point back.
#include "timer_mark_2ebc80.h"
#include "pair_agents_2eab70.h"
#include <xmmintrin.h>

static __declspec(align(16)) const float g_412420[4] = {1.0f, 1.0f, 1.0f, 1.0f};

struct c_query_transform
{
	__m128 axes[3];
	__m128 position;

	void inverse_product(const c_query_transform *a, const c_query_transform *b);
};

struct c_transformed_point
{
	__m128 value;
	void transform(const void *matrix, const __m128 *point);
};

struct vector_2da6e0
{
	__m128 value;
	void rotate(const c_query_transform *matrix, const __m128 *direction);
};

// a ray: its start, with its length in the fourth lane, and its end
struct ray_2f90a0
{
	__m128 start;
	__m128 end;
};

struct rays_shape_2f90a0
{
	char unknown0[0x10];
	ray_2f90a0 const *rays;
	int ray_count;
};

struct ray_input_2f90a0
{
	__m128 from;
	__m128 to;
	long field_20;
	long field_24;
};

struct ray_output_2f90a0
{
	__m128 normal;
	float field_10;
	float fraction;
	char unknown18[8];
};

struct hit_2f90a0
{
	bool value;
	char unknown1[3];
};

class convex_2f90a0
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
	virtual hit_2f90a0 cast_ray(ray_input_2f90a0 const *input, ray_output_2f90a0 *output) const { hit_2f90a0 hit = {false}; return hit; }
};

struct motion_2f90a0
{
	char unknown0[0x20];
	c_query_transform transform;
};

struct body_2f90a0
{
	void const *shape;
	long key;
	motion_2f90a0 const *motion;
};

// a contact point: its position, with its distance in the fourth lane, its
// normal, and the id the contact manager gave it
struct contact_point_2f90a0
{
	union
	{
		c_transformed_point position;
		float position_and_distance[4];
	};
	vector_2da6e0 normal;
	unsigned short id;
	char unknown22[0x30 - 0x22];
};

struct contact_points_2f90a0
{
	contact_point_2f90a0 *points;
	int count;
	int capacity;
};

void __cdecl function_2d91d0(void *array, long element_size);

// the contact manager: it gives a new contact point an id (0xffff when it
// takes no more), and takes an id back
class contact_manager_2f90a0
{
public:
	virtual void slot_0() {}
	virtual unsigned short add_contact(void const *a, void const *b, void const *input, contact_point_2f90a0 *point) { return 0; }
	virtual void release_id(unsigned short id) {}
};

// a point placed by a transform
inline __m128 transformed_2f90a0(c_query_transform const *transform, __m128 const &p)
{
	__m128 z = _mm_add_ps(_mm_mul_ps(transform->axes[2], _mm_shuffle_ps(p, p, 0xaa)), transform->position);
	__m128 y = _mm_mul_ps(transform->axes[1], _mm_shuffle_ps(p, p, 0x55));
	__m128 x = _mm_mul_ps(transform->axes[0], _mm_shuffle_ps(p, p, 0x00));
	return _mm_add_ps(_mm_add_ps(x, y), z);
}

// score 13 (was 70): with &entries[i] held in a local for the add-contact
// branch, only that branch's tail differs: retail takes &entries[i] (lea)
// before the 0xffff test and marks it valid through it, then reloads the id
// into eax/cx where ours uses ecx/dx; the else branch differs the same way.
// a contact point of the pair for one ray
// @retail 0x2f90a0
void class_2f90a0::function_2f90a0(void const *a_, void const *b_, void const *input, void *collector)
{
	timer_add_2ebc80("TtmultiRay-cvx");
	body_2f90a0 const *a = (body_2f90a0 const *)a_;
	body_2f90a0 const *b = (body_2f90a0 const *)b_;
	rays_shape_2f90a0 const *rays = (rays_shape_2f90a0 const *)a->shape;
	convex_2f90a0 const *convex = (convex_2f90a0 const *)b->shape;
	ray_input_2f90a0 ray;
	c_query_transform transform;
	transform.inverse_product(&b->motion->transform, &a->motion->transform);
	int const count = rays->ray_count;
	ray.field_24 = 0;
	ray_2f90a0 const *each = rays->rays;
	for (int i = 0; i < count; i++)
	{
		ray.from = transformed_2f90a0(&transform, each->start);
		ray.to = transformed_2f90a0(&transform, each->end);
		ray_output_2f90a0 output;
		output.fraction = 1.0f;
		hit_2f90a0 hit = convex->cast_ray(&ray, &output);
		if (!hit.value)
		{
			if (entries[i].valid)
			{
				manager->release_id(entries[i].id);
				entries[i].valid = false;
			}
		}
		else
		{
			contact_points_2f90a0 *points = *(contact_points_2f90a0 **)collector;
			if (points->count == (points->capacity & 0x7fffffff))
			{
				function_2d91d0(points, sizeof(contact_point_2f90a0));
			}
			contact_point_2f90a0 *point = &points->points[points->count++];
			__m128 f = _mm_set1_ps(output.fraction);
			__m128 at = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(*(__m128 const *)g_412420, f), ray.from), _mm_mul_ps(f, ray.to));
			point->position.transform(&b->motion->transform, &at);
			point->normal.rotate(&b->motion->transform, &output.normal);
			point->position_and_distance[3] = (output.fraction - 1.0f) * ((float const *)&each->start)[3];
			contact_entry_2f90a0 *entry = &entries[i];
			if (!entries[i].valid)
			{
				entry->id = manager->add_contact(a, b, input, point);
				if (entries[i].id != 0xffff)
				{
					entries[i].valid = true;
					point->id = entries[i].id;
				}
				else
				{
					(*(contact_points_2f90a0 **)collector)->count--;
				}
			}
			point->id = entries[i].id;
		}
		each++;
	}
	timer_add_2ebc80("Et");
}
