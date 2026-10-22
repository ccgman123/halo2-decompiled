// @flags /O2 /GL-
// Havok (third-party physics library): a capsule against a triangle: up to
// two contacts, from the capsule's ends projected onto the triangle, from the
// closest points of its segment and the triangle's edges, or where the
// segment passes through the triangle. The queries are marked in the timing
// stream.
#include "pair_agents_2eab70.h"
#include "timer_mark_2ebc80.h"
#include "triangle_agent_2f42f0.h"
#include <math.h>
#include <xmmintrin.h>

static __declspec(align(16)) const float g_412420[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static __declspec(align(16)) const unsigned long g_4124a0[4] = {0x80000000, 0x80000000, 0x80000000, 0x80000000};

// corner indices: the corner before a corner at [i], and after it at [i + 2]
static const signed char g_4136b0[5] = {2, 0, 1, 2, 0};

// three axes and a position
struct frame_2f4f00
{
	__m128 axes[3];
	__m128 position;
};

// a point taken out of a frame
inline __m128 frame_point_2f4f00(frame_2f4f00 const *m, __m128 const *point)
{
	__m128 p = *point;
	__m128 z = _mm_add_ps(_mm_mul_ps(m->axes[2], _mm_shuffle_ps(p, p, 0xaa)), m->position);
	__m128 y = _mm_mul_ps(m->axes[1], _mm_shuffle_ps(p, p, 0x55));
	__m128 x = _mm_mul_ps(m->axes[0], _mm_shuffle_ps(p, p, 0x00));
	return _mm_add_ps(_mm_add_ps(x, y), z);
}

// where a body sits: two vectors, then its frame
struct placement_2f4f00
{
	__m128 field_0;
	__m128 field_10;
	frame_2f4f00 frame;
};

// a capsule or a triangle: its radius and its points (two ends or three corners)
struct shape_2f4f00
{
	void const *table;
	long field_4;
	long field_8;
	float radius;
	__m128 points[3];
};

// one side of a query: a shape, the key of the part concerned, where it sits,
// and the side it is a part of
struct body_2f4f00
{
	shape_2f4f00 const *shape;
	long key;
	placement_2f4f00 const *placement;
	body_2f4f00 const *parent;
};

struct input_2f4f00
{
	long field_0;
	long field_4;
	float tolerance;
};

// takes the pairs a query finds
class pair_collector_2f4f00
{
public:
	virtual void slot_0() {}
	virtual void add_pair(void const *a, void const *b) {}
};

// a contact: its point, with the separation in the fourth term, its normal,
// and the two sides
struct contact_2f4f00
{
	__m128 point;
	__m128 normal;
	void const *a;
	void const *b;
};

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

// the object kept for the pair: the triangle's weights from its 20th byte
struct triangle_agent_2f4f00
{
	char unknown0[0x14];
	weights_308c20 weights;
};

// the cross product of two vectors' first three terms
inline __m128 cross_2f4f00(__m128 const &a, __m128 const &b)
{
	__m128 b_yzx = _mm_shuffle_ps(b, b, 0xc9);
	__m128 a_zxy = _mm_shuffle_ps(a, a, 0xd2);
	__m128 second = _mm_mul_ps(a_zxy, b_yzx);
	__m128 b_zxy = _mm_shuffle_ps(b, b, 0xd2);
	__m128 a_yzx = _mm_shuffle_ps(a, a, 0xc9);
	return _mm_sub_ps(_mm_mul_ps(a_yzx, b_zxy), second);
}

// the sum of a vector's first three terms, in the first
inline __m128 sum_2f4f00(__m128 const &v)
{
	return _mm_add_ss(_mm_shuffle_ps(v, v, 0xaa), _mm_add_ss(_mm_shuffle_ps(v, v, 0x55), v));
}

// a vector of four copies of a float
inline __m128 splat_2f4f00(float value)
{
	__m128 const single = _mm_set_ss(value);
	return _mm_shuffle_ps(single, single, 0);
}

// a vector scaled to unit length
inline __m128 normalized_2f4f00(__m128 const &v)
{
	__m128 const sum = sum_2f4f00(_mm_mul_ps(v, v));
	__m128 const inverse = _mm_rsqrt_ss(sum);
	__m128 const correction = _mm_sub_ss(_mm_set_ss(3.0f), _mm_mul_ss(_mm_mul_ss(sum, inverse), inverse));
	__m128 const scale = _mm_mul_ss(_mm_mul_ss(_mm_set_ss(0.5f), inverse), correction);
	return _mm_mul_ps(_mm_shuffle_ps(scale, scale, 0), v);
}

// a vector of three terms and a zero
inline void set_vector_2f4f00(__m128 &v, float x, float y, float z)
{
	float *terms = (float *)&v;
	terms[3] = 0.0f;
	terms[0] = x;
	terms[1] = y;
	terms[2] = z;
}

// what a query works out: the triangle's planes, the contacts, and its corners
struct query_2f4f00
{
	matrix_308c20 planes;
	contact_2f4f00 contacts[3];
	__m128 corners[3];
};

// the up to two contacts of a capsule and a triangle, each with the
// separation in its point's fourth term, or that term left at the largest
// float when there is none
__forceinline void capsule_triangle_2f4f00(void const *a, void const *b, void const *input, weights_308c20 const *weights, query_2f4f00 *query)
{
	shape_2f4f00 const *capsule = ((body_2f4f00 const *)a)->shape;
	shape_2f4f00 const *triangle = ((body_2f4f00 const *)b)->shape;
	__m128 ends[2];
	{
		frame_2f4f00 const *frame = &((body_2f4f00 const *)a)->placement->frame;
		__m128 const *end = capsule->points;
		__m128 *placed = ends;
		for (int count = 2; count != 0; count--)
		{
			*placed = frame_point_2f4f00(frame, end);
			placed++;
			end++;
		}
	}
	{
		frame_2f4f00 const *frame = &((body_2f4f00 const *)b)->placement->frame;
		__m128 const *corner = triangle->points;
		__m128 *placed = query->corners;
		for (int count = 3; count != 0; count--)
		{
			*placed = frame_point_2f4f00(frame, corner);
			placed++;
			corner++;
		}
	}
	__m128 normal;
	function_308c20(query->corners, weights, &query->planes, &normal);
	__m128 const *corners = query->corners;
	matrix_308c20 const &planes = query->planes;
	contact_2f4f00 *contacts = query->contacts;

	// each end's distances outside the three edges, and above the triangle
	__m128 heights[2];
	heights[0] = _mm_sub_ps(ends[0], corners[0]);
	heights[1] = _mm_sub_ps(ends[1], corners[0]);
	{
		frame_2f4f00 const *frame = (frame_2f4f00 const *)&planes;
		__m128 *height = heights;
		for (int count = 2; count != 0; count--)
		{
			*height = frame_point_2f4f00(frame, height);
			height++;
		}
	}
	float const radius = capsule->radius + triangle->radius;
	float const reach = radius + ((input_2f4f00 const *)input)->tolerance;
	contacts[0].point.m128_f32[3] = 3.402823466e+38f;
	contacts[1].point.m128_f32[3] = 3.402823466e+38f;
	contacts[2].point.m128_f32[3] = 3.402823466e+38f;
	__m128 const reach4 = splat_2f4f00(reach);
	if (_mm_movemask_ps(_mm_cmplt_ps(reach4, heights[1])) & _mm_movemask_ps(_mm_cmplt_ps(reach4, heights[0])))
	{
		return;
	}
	if (heights[0].m128_f32[3] < -reach && heights[1].m128_f32[3] < -reach)
	{
		return;
	}
	int signs[2];
	signs[0] = _mm_movemask_ps(heights[0]);
	signs[1] = _mm_movemask_ps(heights[1]);
	if ((signs[0] ^ signs[1]) & 8)
	{
		// the segment passes through the triangle's plane: where, in the edges' terms
		float const along = heights[0].m128_f32[3] / (heights[0].m128_f32[3] - heights[1].m128_f32[3]);
		__m128 const along4 = splat_2f4f00(along);
		__m128 const crossing = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(*(__m128 const *)g_412420, along4), heights[0]), _mm_mul_ps(along4, heights[1]));
		if ((_mm_movemask_ps(crossing) & 7) == 7)
		{
			// inside the triangle: the deeper end, or the closest point of an
			// edge to the segment
			contact_2f4f00 *contact = &contacts[1];
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
							contact->point = _mm_add_ps(ends[end], _mm_mul_ps(splat_2f4f00(shift), normal));
						}
						else
						{
							contact->normal = _mm_xor_ps(normal, *(__m128 const *)g_4124a0);
							float const shift = -triangle->radius - heights[end].m128_f32[3];
							contact->point = _mm_add_ps(ends[end], _mm_mul_ps(splat_2f4f00(shift), normal));
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
						set_vector_2f4f00(plane, terms[edge], terms[edge + 4], terms[edge + 8]);
						if (change.m128_f32[3] < 0.0f)
						{
							scale = -change.m128_f32[3];
							slope = -slope;
						}
						__m128 const scale4 = splat_2f4f00(scale);
						__m128 const slope4 = splat_2f4f00(-slope);
						float const distance = (float)sqrt(distance_squared);
						plane = _mm_mul_ps(scale4, plane);
						__m128 const direction = normalized_2f4f00(_mm_add_ps(plane, _mm_mul_ps(slope4, normal)));
						contact->normal = direction;
						__m128 const at4 = splat_2f4f00(at);
						float const depth = -distance;
						contact->point = _mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_sub_ps(*(__m128 const *)g_412420, at4), ends[0]), _mm_mul_ps(at4, ends[1])), _mm_mul_ps(splat_2f4f00(triangle->radius - depth), direction));
						contact->point.m128_f32[3] = depth - radius;
					}
				}
			}
			return;
		}
	}

	// each end that lies over the triangle gives a contact, and each that does
	// not, the closest of the edges; when the segment passes outside one edge
	// on both ends, only the closest of the edges
	__m128 const direction = _mm_sub_ps(ends[1], ends[0]);
	bool outside = false;
	if (((signs[0] | signs[1]) & 7) != 7)
	{
		outside = true;
	}
	contact_2f4f00 *contact = contacts;
	for (int end = 0; end < 2;)
	{
		if (!outside && (signs[end] & 7) == 7)
		{
			if (signs[end] & 8)
			{
				float const shift = -triangle->radius - heights[end].m128_f32[3];
				contact->point = _mm_add_ps(ends[end], _mm_mul_ps(splat_2f4f00(shift), normal));
				contact->point.m128_f32[3] = -heights[end].m128_f32[3] - radius;
				contact->normal = _mm_xor_ps(normal, *(__m128 const *)g_4124a0);
			}
			else
			{
				float const shift = triangle->radius - heights[end].m128_f32[3];
				contact->point = _mm_add_ps(ends[end], _mm_mul_ps(splat_2f4f00(shift), normal));
				contact->point.m128_f32[3] = heights[end].m128_f32[3] - radius;
				contact->normal = normal;
			}
			end++;
			contact++;
			continue;
		}
		int bit = 1;
		for (int edge = 0; edge < 3; edge++, bit <<= 1)
		{
			if (outside)
			{
				if (signs[0] & (bit & signs[1]))
				{
					continue;
				}
			}
			else if (signs[end] & bit)
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
				if (!outside)
				{
					continue;
				}
				end = 1;
			}
			__m128 across;
			if (clamped == 0)
			{
				across = cross_2f4f00(direction, side);
			}
			else if (!(closest.distance_squared <= 1.4210854715202004e-14f))
			{
				across = closest.offset;
			}
			else
			{
				float const *terms = (float const *)&planes;
				set_vector_2f4f00(closest.offset, terms[edge], terms[edge + 4], terms[edge + 8]);
				across = cross_2f4f00(direction, side);
				float length_squared;
				_mm_store_ss(&length_squared, sum_2f4f00(_mm_mul_ps(across, across)));
				if (!(length_squared > 1.4210854715202004e-14f))
				{
					across = closest.offset;
				}
			}
			across = normalized_2f4f00(across);
			float dot;
			_mm_store_ss(&dot, sum_2f4f00(_mm_mul_ps(across, closest.offset)));
			float distance = dot;
			if (dot < 0.0f)
			{
				across = _mm_xor_ps(across, *(__m128 const *)g_4124a0);
				distance = -dot;
			}
			contact->point = _mm_add_ps(closest.first_point, _mm_mul_ps(splat_2f4f00(triangle->radius - distance), across));
			contact->point.m128_f32[3] = distance - radius;
			contact->normal = across;
		}
		if (outside)
		{
			break;
		}
		end++;
		contact++;
	}
}

// takes the contacts a query finds
class contact_collector_2f4f00
{
public:
	virtual void slot_0() {}
	virtual void add_contact(contact_2f4f00 const *contact) {}
};

// the closer of the two contacts passed to the collector, when it is within
// the tolerance
__forceinline void report_contact_2f4f00(contact_2f4f00 const *contacts, contact_2f4f00 *out, void const *input, void *collector)
{
	float const tolerance = ((input_2f4f00 const *)input)->tolerance;
	if (contacts[0].point.m128_f32[3] < contacts[1].point.m128_f32[3])
	{
		if (!(contacts[0].point.m128_f32[3] < tolerance))
		{
			return;
		}
		out->point = contacts[0].point;
		out->normal = contacts[0].normal;
	}
	else
	{
		if (!(contacts[1].point.m128_f32[3] < tolerance))
		{
			return;
		}
		out->point = contacts[1].point;
		out->normal = contacts[1].normal;
	}
	((contact_collector_2f4f00 *)collector)->add_contact(out);
}

// the pair passed to the collector when either contact overlaps
__forceinline void report_pair_2f4f00(contact_2f4f00 const *contacts, void const *a, void const *b, void *collector)
{
	if (contacts[0].point.m128_f32[3] < 0.0f || contacts[1].point.m128_f32[3] < 0.0f)
	{
		((pair_collector_2f4f00 *)collector)->add_pair(a, b);
	}
}

// score 570 (was 541): retail's frame is 0x10 smaller (it keeps the outside
// flag at 0x10, sharing the inside case's slope slot; ours gives it its own
// byte), and retail keeps the second body in ebx and the capsule in edi, ours
// the other way round;
// the edge planes' terms are copied with integer moves where retail uses
// the FPU, and the code for the edges is placed before the report instead of
// after the return. Round 47 changed the shared end loop to step the end and
// the contact in each arm (retail has the increments in both arms and the
// loop test at the top), which took 31-33 off 0x2f5960, 0x2f63d0 and
// 0x2f6db0 but added 32 to 0x2f4f00; a block scope for the end loop, or a
// goto into it, scored worse over the four. The edge contact's dot product
// is read into its own local, so the sign flip stays on the FPU as in retail.
// The closer contact of the capsule and the triangle passed to the collector,
// with the triangle's weights kept in the object
// @retail 0x2f4f00
void class_2f4f00::function_2f4f00(void const *a, void const *b, void const *input, void *collector)
{
	timer_add_2ebc80("TtCapsuleTriangle");
	contact_2f4f00 out;
	out.a = a;
	out.b = b;
	query_2f4f00 query;
	capsule_triangle_2f4f00(a, b, input, &((triangle_agent_2f4f00 const *)this)->weights, &query);
	report_contact_2f4f00(query.contacts, &out, input, collector);
	timer_add_2ebc80("Et");
}

// score 454 (was 469): the frame is laid out differently, the registers
// differ in places;
// the edge planes' terms are copied with integer moves where retail uses
// the FPU, and the code for the edges is placed before the report instead of
// after the return. Round 47 changed the shared end loop to step the end and
// the contact in each arm (retail has the increments in both arms and the
// loop test at the top), which took 31-33 off 0x2f5960, 0x2f63d0 and
// 0x2f6db0 but added 32 to 0x2f4f00; a block scope for the end loop, or a
// goto into it, scored worse over the four. The edge contact's dot product
// is read into its own local, so the sign flip stays on the FPU as in retail.
// The closer contact of the capsule and the triangle passed to the collector,
// with the triangle's weights worked out here
// @retail 0x2f5960
void __cdecl function_2f5960(void const *a, void const *b, void const *input, void *collector)
{
	timer_add_2ebc80("TtCapsuleTriangle");
	triangle_measures_308a50 measures;
	function_308a50(((body_2f4f00 const *)b)->shape->points, &measures);
	contact_2f4f00 out;
	out.a = a;
	out.b = b;
	query_2f4f00 query;
	capsule_triangle_2f4f00(a, b, input, (weights_308c20 const *)&measures, &query);
	report_contact_2f4f00(query.contacts, &out, input, collector);
	timer_add_2ebc80("Et");
}

// score 389 (was 418): the frame is laid out differently, the registers
// differ in places (retail: inside case, report, ret, then the edges' code
// jumping back; ending the query inline with a goto to the report and placing
// the edges' code after it scored 522: the corner loops rotate);
// the edge planes' terms are copied with integer moves where retail uses
// the FPU, and the code for the edges is placed before the report instead of
// after the return. Round 47 changed the shared end loop to step the end and
// the contact in each arm (retail has the increments in both arms and the
// loop test at the top), which took 31-33 off 0x2f5960, 0x2f63d0 and
// 0x2f6db0 but added 32 to 0x2f4f00; a block scope for the end loop, or a
// goto into it, scored worse over the four. The edge contact's dot product
// is read into its own local, so the sign flip stays on the FPU as in retail.
// The pair passed to the collector when the capsule and the triangle overlap,
// with the triangle's weights kept in the object
// @retail 0x2f63d0
void class_2f63d0::function_2f63d0(void const *a, void const *b, void const *input, void *collector)
{
	timer_add_2ebc80("TtCapsuleTriangle");
	query_2f4f00 query;
	capsule_triangle_2f4f00(a, b, input, &((triangle_agent_2f4f00 const *)this)->weights, &query);
	report_pair_2f4f00(query.contacts, a, b, collector);
	timer_add_2ebc80("Et");
}

// score 434 (was 470): the frame is laid out differently, the registers
// differ in places;
// the edge planes' terms are copied with integer moves where retail uses
// the FPU, and the code for the edges is placed before the report instead of
// after the return. Round 47 changed the shared end loop to step the end and
// the contact in each arm (retail has the increments in both arms and the
// loop test at the top), which took 31-33 off 0x2f5960, 0x2f63d0 and
// 0x2f6db0 but added 32 to 0x2f4f00; a block scope for the end loop, or a
// goto into it, scored worse over the four. The edge contact's dot product
// is read into its own local, so the sign flip stays on the FPU as in retail.
// The pair passed to the collector when the capsule and the triangle overlap,
// with the triangle's weights worked out here
// @retail 0x2f6db0
void __cdecl function_2f6db0(void const *a, void const *b, void const *input, void *collector)
{
	timer_add_2ebc80("TtCapsuleTriangle");
	triangle_measures_308a50 measures;
	function_308a50(((body_2f4f00 const *)b)->shape->points, &measures);
	query_2f4f00 query;
	capsule_triangle_2f4f00(a, b, input, (weights_308c20 const *)&measures, &query);
	report_pair_2f4f00(query.contacts, a, b, collector);
	timer_add_2ebc80("Et");
}
