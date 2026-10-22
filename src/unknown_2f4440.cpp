// @flags /O2 /GL-
// Havok (third-party physics library): a capsule against a triangle, up to
// three contacts: from the capsule's ends projected onto the triangle, from
// the closest points of its segment and the triangle's edges, or where the
// segment passes through the triangle; when the segment lies outside an edge
// at both ends, the edges are taken as segments of a capsule. Each contact
// keeps an id the owner hands out while it lies within the tolerance, and
// hands it back when it no longer does. Marked in the timing stream.
#include "pair_agents_2eab70.h"
#include "timer_mark_2ebc80.h"
#include "growable_array_2d91d0.h"
#include <math.h>
#include <xmmintrin.h>

static __declspec(align(16)) const float g_412420[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static __declspec(align(16)) const unsigned long g_4124a0[4] = {0x80000000, 0x80000000, 0x80000000, 0x80000000};

// corner indices: the corner before a corner at [i], and after it at [i + 2]
static const signed char g_4136b0[5] = {2, 0, 1, 2, 0};

// three axes and a position
struct frame_2f4440
{
	__m128 axes[3];
	__m128 position;
};

// a point taken out of a frame
inline __m128 frame_point_2f4440(frame_2f4440 const *m, __m128 const *point)
{
	__m128 p = *point;
	__m128 z = _mm_add_ps(_mm_mul_ps(m->axes[2], _mm_shuffle_ps(p, p, 0xaa)), m->position);
	__m128 y = _mm_mul_ps(m->axes[1], _mm_shuffle_ps(p, p, 0x55));
	__m128 x = _mm_mul_ps(m->axes[0], _mm_shuffle_ps(p, p, 0x00));
	return _mm_add_ps(_mm_add_ps(x, y), z);
}

// where a body sits: two vectors, then its frame
struct placement_2f4440
{
	__m128 field_0;
	__m128 field_10;
	frame_2f4440 frame;
};

// a capsule or a triangle: its radius and its points (two ends or three corners)
struct shape_2f4440
{
	void const *table;
	long field_4;
	long field_8;
	float radius;
	__m128 points[3];
};

// one side of a query: a shape, the key of the part concerned, and where it
// sits
struct body_2f4440
{
	shape_2f4440 const *shape;
	long key;
	placement_2f4440 const *placement;
};

struct input_2f4440
{
	long field_0;
	long field_4;
	float tolerance;
};

// a contact found: its point, with the separation in the fourth term, and
// its normal (0x2f28f0's contact)
struct contact_point_2f28f0
{
	__m128 point;
	__m128 normal;
	__m128 field_20;
};

struct body_2f28f0;

void __cdecl function_2f28f0(body_2f28f0 const *body, __m128 const *segment, float radius, contact_point_2f28f0 *points);

typedef void *(__fastcall *constructor_proc)(void *);
void __stdcall vector_constructor_iterator(void *array, unsigned size, int count, constructor_proc constructor);
void *__fastcall function_31bc70(void *arg_0);

// the closest point on the first segment, the offset from the closest point
// on the second, and the square of that offset's length (0x308590)
struct closest_points_308590
{
	__m128 first_point;
	__m128 offset;
	float distance_squared;
};

int __cdecl function_308590(__m128 const *first_start, __m128 const *first_direction, __m128 const *second_start, __m128 const *second_direction, closest_points_308590 *result);

// the weight of each edge, of the normal, and of the last row (0x308c20)
struct weights_308c20
{
	float edges[3];
	float normal;
	float last;
};

struct vector_308c20
{
	__m128 value;
};

// the triangle's edge planes and its plane, as a frame
struct matrix_308c20
{
	vector_308c20 rows[4];
};

void __cdecl function_308c20(__m128 const *corners, weights_308c20 const *weights, matrix_308c20 *out, __m128 *normal);

// a contact kept: its point, with the separation in the fourth term, its
// normal, and its id
struct contact_2f4440
{
	__m128 point;
	__m128 normal;
	unsigned short id;
};

ELEMENTS_2D91D0_ALIGNED(contacts_2f4440, contact_2f4440);

struct results_2f4440
{
	contacts_2f4440 *contacts;
};

// hands out the ids of contacts, and takes them back
class contact_owner_2f4440
{
public:
	virtual void slot_0() {}
	virtual unsigned short add_contact(void const *a, void const *b, void const *input, contact_point_2f28f0 *contact) { return 0; }
	virtual void release_id(unsigned short id) {}
};

// the object kept for the pair: the owner, the three contacts' ids, and the
// triangle's weights
struct agent_2f4440
{
	char unknown0[8];
	contact_owner_2f4440 *owner;
	unsigned short ids[3];
	weights_308c20 weights;
};

// the cross product of two vectors' first three terms
inline __m128 cross_2f4440(__m128 const &a, __m128 const &b)
{
	__m128 b_yzx = _mm_shuffle_ps(b, b, 0xc9);
	__m128 a_zxy = _mm_shuffle_ps(a, a, 0xd2);
	__m128 second = _mm_mul_ps(a_zxy, b_yzx);
	__m128 b_zxy = _mm_shuffle_ps(b, b, 0xd2);
	__m128 a_yzx = _mm_shuffle_ps(a, a, 0xc9);
	return _mm_sub_ps(_mm_mul_ps(a_yzx, b_zxy), second);
}

// the sum of a vector's first three terms, in the first
inline __m128 sum_2f4440(__m128 const &v)
{
	return _mm_add_ss(_mm_shuffle_ps(v, v, 0xaa), _mm_add_ss(_mm_shuffle_ps(v, v, 0x55), v));
}

// a vector of four copies of a float
inline __m128 splat_2f4440(float value)
{
	__m128 const single = _mm_set_ss(value);
	return _mm_shuffle_ps(single, single, 0);
}

// a vector scaled to unit length
inline __m128 normalized_2f4440(__m128 const &v)
{
	__m128 const sum = sum_2f4440(_mm_mul_ps(v, v));
	__m128 const inverse = _mm_rsqrt_ss(sum);
	__m128 const correction = _mm_sub_ss(_mm_set_ss(3.0f), _mm_mul_ss(_mm_mul_ss(sum, inverse), inverse));
	__m128 const scale = _mm_mul_ss(_mm_mul_ss(_mm_set_ss(0.5f), inverse), correction);
	return _mm_mul_ps(_mm_shuffle_ps(scale, scale, 0), v);
}

// what a query works out: the triangle's planes, the contacts, and its corners
struct query_2f4440
{
	matrix_308c20 planes;
	contact_point_2f28f0 contacts[3];
	__m128 corners[3];
};

// the up to three contacts of a capsule and a triangle, each with the
// separation in its point's fourth term, or that term left at the largest
// float (or the tolerance) when there is none
__forceinline void capsule_triangle_2f4440(void const *a, void const *b, void const *input, weights_308c20 const *weights, query_2f4440 *query)
{
	shape_2f4440 const *capsule = ((body_2f4440 const *)a)->shape;
	shape_2f4440 const *triangle = ((body_2f4440 const *)b)->shape;
	__m128 ends[2];
	{
		frame_2f4440 const *frame = &((body_2f4440 const *)a)->placement->frame;
		__m128 const *end = capsule->points;
		__m128 *placed = ends;
		for (int count = 2; count != 0; count--)
		{
			*placed = frame_point_2f4440(frame, end);
			placed++;
			end++;
		}
	}
	{
		frame_2f4440 const *frame = &((body_2f4440 const *)b)->placement->frame;
		__m128 const *corner = triangle->points;
		__m128 *placed = query->corners;
		for (int count = 3; count != 0; count--)
		{
			*placed = frame_point_2f4440(frame, corner);
			placed++;
			corner++;
		}
	}
	__m128 normal;
	function_308c20(query->corners, weights, &query->planes, &normal);
	__m128 const *corners = query->corners;
	matrix_308c20 const &planes = query->planes;
	contact_point_2f28f0 *contacts = query->contacts;

	int signs[2];
	{
		// each end's distances outside the three edges, and above the triangle
		__m128 heights[2];
		heights[0] = _mm_sub_ps(ends[0], corners[0]);
		heights[1] = _mm_sub_ps(ends[1], corners[0]);
		{
			frame_2f4440 const *frame = (frame_2f4440 const *)&planes;
			__m128 *height = heights;
			for (int count = 2; count != 0; count--)
			{
				*height = frame_point_2f4440(frame, height);
				height++;
			}
		}
		float const radius = capsule->radius + triangle->radius;
		float const reach = radius + ((input_2f4440 const *)input)->tolerance;
		contacts[0].point.m128_f32[3] = 3.402823466e+38f;
		contacts[1].point.m128_f32[3] = 3.402823466e+38f;
		contacts[2].point.m128_f32[3] = 3.402823466e+38f;
		__m128 const reach4 = splat_2f4440(reach);
		if (_mm_movemask_ps(_mm_cmplt_ps(reach4, heights[1])) & _mm_movemask_ps(_mm_cmplt_ps(reach4, heights[0])))
		{
			return;
		}
		if (heights[0].m128_f32[3] < -reach && heights[1].m128_f32[3] < -reach)
		{
			return;
		}
		signs[0] = _mm_movemask_ps(heights[0]);
		signs[1] = _mm_movemask_ps(heights[1]);
		if ((signs[0] ^ signs[1]) & 8)
		{
			// the segment passes through the triangle's plane: where, in the edges' terms
			float const along = heights[0].m128_f32[3] / (heights[0].m128_f32[3] - heights[1].m128_f32[3]);
			__m128 const along4 = splat_2f4440(along);
			__m128 const crossing = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(*(__m128 const *)g_412420, along4), heights[0]), _mm_mul_ps(along4, heights[1]));
			if ((_mm_movemask_ps(crossing) & 7) == 7)
			{
				// inside the triangle: the deeper end, or the closest point of an
				// edge to the segment
				contact_point_2f28f0 *contact = &contacts[1];
				contact->point.m128_f32[3] = -3.402823466e+38f;
				for (int end = 0; end < 2; end++)
				{
					int const sign = signs[end];
					if ((sign & 7) == 7)
					{
						float const separation = -(float)fabs(heights[end].m128_f32[3]) - radius;
						if (separation > contact->point.m128_f32[3])
						{
							if (sign & 8)
							{
								contact->normal = normal;
								float const shift = triangle->radius - heights[end].m128_f32[3];
								contact->point = _mm_add_ps(ends[end], _mm_mul_ps(splat_2f4440(shift), normal));
							}
							else
							{
								contact->normal = _mm_xor_ps(normal, *(__m128 const *)g_4124a0);
								float const shift = -triangle->radius - heights[end].m128_f32[3];
								contact->point = _mm_add_ps(ends[end], _mm_mul_ps(splat_2f4440(shift), normal));
							}
							contact->point.m128_f32[3] = separation;
						}
					}
				}
				__m128 change = _mm_sub_ps(heights[1], heights[0]);
				float const rise_squared = change.m128_f32[3] * change.m128_f32[3];
				for (int edge = 0; edge < 3; edge++)
				{
					float scale = change.m128_f32[3];
					float slope = change.m128_f32[edge];
					float const inverse = 1.0f / (slope * slope + rise_squared);
					float const offset = heights[0].m128_f32[3] * slope - change.m128_f32[3] * heights[0].m128_f32[edge];
					float const distance_squared = offset * offset * inverse;
					float const limit = contact->point.m128_f32[3] + radius;
					if (distance_squared < limit * limit)
					{
						float const at = -(slope * heights[0].m128_f32[edge] + change.m128_f32[3] * heights[0].m128_f32[3]) * inverse;
						if (at > -0.0001f && at < 1.0001f)
						{
							float const *terms = (float const *)&planes;
							__m128 plane;
							float *plane_terms = (float *)&plane;
							plane_terms[3] = 0.0f;
							plane_terms[0] = terms[edge];
							plane_terms[1] = terms[edge + 4];
							plane_terms[2] = terms[edge + 8];
							if (change.m128_f32[3] < 0.0f)
							{
								scale = -change.m128_f32[3];
								slope = -slope;
							}
							__m128 const scale4 = splat_2f4440(scale);
							__m128 const slope4 = splat_2f4440(-slope);
							float const distance = (float)sqrt(distance_squared);
							plane = _mm_mul_ps(scale4, plane);
							__m128 const direction = normalized_2f4440(_mm_add_ps(plane, _mm_mul_ps(slope4, normal)));
							contact->normal = direction;
							__m128 const at4 = splat_2f4440(at);
							float const depth = -distance;
							contact->point = _mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_sub_ps(*(__m128 const *)g_412420, at4), ends[0]), _mm_mul_ps(at4, ends[1])), _mm_mul_ps(splat_2f4440(triangle->radius - depth), direction));
							contact->point.m128_f32[3] = depth - radius;
						}
					}
				}
				return;
			}
		}

		__m128 const direction = _mm_sub_ps(ends[1], ends[0]);
		if (((signs[0] | signs[1]) & 7) != 7)
		{
			goto edges;
		}

		// each end that lies over the triangle gives a contact, and each that does
		// not, the closest of the edges it lies outside
		contact_point_2f28f0 *contact = contacts;
		for (int end = 0; end < 2; end++, contact++)
		{
			int const sign = signs[end];
			if ((sign & 7) == 7)
			{
				if (sign & 8)
				{
					float const shift = -triangle->radius - heights[end].m128_f32[3];
					contact->point = _mm_add_ps(ends[end], _mm_mul_ps(splat_2f4440(shift), normal));
					contact->point.m128_f32[3] = -heights[end].m128_f32[3] - radius;
					contact->normal = _mm_xor_ps(normal, *(__m128 const *)g_4124a0);
				}
				else
				{
					float const shift = triangle->radius - heights[end].m128_f32[3];
					contact->point = _mm_add_ps(ends[end], _mm_mul_ps(splat_2f4440(shift), normal));
					contact->point.m128_f32[3] = heights[end].m128_f32[3] - radius;
					contact->normal = normal;
				}
				continue;
			}
			int bit = 1;
			for (int edge = 0; edge < 3; edge++, bit <<= 1)
			{
				if (signs[end] & bit)
				{
					continue;
				}
				__m128 const *start = &corners[g_4136b0[edge + 2]];
				__m128 const side = _mm_sub_ps(corners[g_4136b0[edge]], *start);
				closest_points_308590 closest;
				int const clamped = function_308590(&ends[0], &direction, start, &side, &closest);
				float const limit = radius + contact->point.m128_f32[3];
				if (!(closest.distance_squared < limit * limit))
				{
					continue;
				}
				if (clamped & (1 << end))
				{
					continue;
				}
				__m128 across;
				if (clamped == 0)
				{
					across = cross_2f4440(direction, side);
				}
				else if (!(closest.distance_squared <= 1.4210854715202004e-14f))
				{
					across = closest.offset;
				}
				else
				{
					float const *terms = (float const *)&planes;
					float *offset_terms = (float *)&closest.offset;
					offset_terms[0] = terms[edge];
					offset_terms[1] = terms[edge + 4];
					offset_terms[2] = terms[edge + 8];
					across = cross_2f4440(direction, side);
					float length_squared;
					_mm_store_ss(&length_squared, sum_2f4440(_mm_mul_ps(across, across)));
					offset_terms[3] = 0.0f;
					if (length_squared <= 1.4210854715202004e-14f)
					{
						across = closest.offset;
					}
				}
				across = normalized_2f4440(across);
				float dot;
				_mm_store_ss(&dot, sum_2f4440(_mm_mul_ps(across, closest.offset)));
				float distance = dot;
				if (dot < 0.0f)
				{
					across = _mm_xor_ps(across, *(__m128 const *)g_4124a0);
					distance = -dot;
				}
				contact->point = _mm_add_ps(closest.first_point, _mm_mul_ps(splat_2f4440(triangle->radius - distance), across));
				contact->point.m128_f32[3] = distance - radius;
				contact->normal = across;
			}
		}
	}
	return;

	// outside an edge at both ends: each such edge taken as a capsule's
	// segment
edges:
	{
		float const tolerance = ((input_2f4440 const *)input)->tolerance;
		contacts[0].point.m128_f32[3] = tolerance;
		contacts[1].point.m128_f32[3] = tolerance;
		contacts[2].point.m128_f32[3] = tolerance;
		__m128 segment[2];
		vector_constructor_iterator(segment, sizeof(__m128), 2, function_31bc70);
		int bit = 1;
		for (int edge = 0; edge < 3; edge++, bit <<= 1)
		{
			if ((signs[0] | signs[1]) & bit)
			{
				continue;
			}
			segment[0] = corners[g_4136b0[edge + 2]];
			segment[1] = corners[g_4136b0[edge]];
			function_2f28f0((body_2f28f0 const *)a, segment, triangle->radius, contacts);
		}
		return;
	}
}

// score 727 (was 731): the frame is 0x20 larger and laid out differently
// (retail: the edges' segment at 0x40, the contacts at 0x130; ours 0x50 and
// 0x150), our build keeps the second body in ebx where retail keeps the
// capsule there and the triangle in edi (and reloads the body), retail keeps
// the input in esi, and the end loop's counters live in different registers;
// the code is otherwise in retail's order. The edge contact's dot product is
// read into its own local (retail's sign flip on the FPU), and the near-zero
// tests are written with <= for retail's jp; retail copies the edge plane's
// terms with fld/fstp indexed by the edge, ours with integer moves through a
// stepped pointer (a float-returning accessor changed nothing).
// The contacts of the capsule and the triangle that lie within the tolerance
// added to the results, each with its id from the owner
// @retail 0x2f4440
void class_2f4440::function_2f4440(void const *a, void const *b, void const *input, void *collector)
{
	timer_add_2ebc80("TtCapsuleTri");
	agent_2f4440 *agent = (agent_2f4440 *)this;
	query_2f4440 query;
	capsule_triangle_2f4440(a, b, input, &agent->weights, &query);
	contact_point_2f28f0 *point = query.contacts;
	unsigned short *id = agent->ids;
	for (int count = 3; count != 0; count--)
	{
		if (point->point.m128_f32[3] < ((input_2f4440 const *)input)->tolerance)
		{
			if (*id == 0xffff)
			{
				*id = agent->owner->add_contact(a, b, input, point);
				if (*id == 0xffff)
				{
					goto next;
				}
			}
			{
				contact_2f4440 &kept = ((results_2f4440 *)collector)->contacts->next_slot();
				kept.point = point->point;
				kept.normal = point->normal;
				kept.id = *id;
			}
		}
		else if (*id != 0xffff)
		{
			agent->owner->release_id(*id);
			*id = 0xffff;
		}
	next:
		id++;
		point++;
	}
	timer_add_2ebc80("Et");
}
