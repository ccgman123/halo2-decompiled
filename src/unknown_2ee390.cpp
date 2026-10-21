// @flags /O2 /GL- /GX
// Havok (third-party physics library): creating the object kept for a pair of
// convex shapes (constructor 0x2ee2e0), and the queries that pass a pair on
// when the search between its shapes finds them touching, or pass on the
// contact it finds.
#include "convex_pair_2ee2e0.h"
#include "timer_mark_2ebc80.h"

// @retail 0x2ee390
void *__cdecl function_2ee390(long arg_0, long arg_1, long arg_2, long value)
{
	return new convex_pair_2ee2e0((description_2ed170 const *)arg_0, (description_2ed170 const *)arg_1, value);
}

// the search run from the pair's kept state; the pair passed on when it
// fails, or when the shapes' radii cover the distance it finds
// @retail 0x2ee3f0
void convex_pair_2ee2e0::function_2ee3f0(body_2ee3f0 const *a, body_2ee3f0 const *b, void const *input, collector_2ee3f0 *collector)
{
	timer_add_2ebc80("TtGsk");
	convex_2fefd0 const *first = a->shape;
	convex_2fefd0 const *second = b->shape;
	c_query_transform transform;
	transform.inverse_product(&a->placement->transform, &b->placement->transform);
	class_3048a0 search;
	search.field_c = true;
	closest_3048a0 closest;
	int status = search.function_3048a0(first, second, &transform, &state, &closest);
	timer_add_2ebc80("Et");
	if (status == 0)
	{
		if (closest.point.m128_f32[3] - first->radius - second->radius < 0.0f)
		{
			collector->slot_1(a, b);
		}
	}
	else
	{
		collector->slot_1(a, b);
	}
}

// the same with a search state started for the pair alone
// @retail 0x2ee500
void __cdecl function_2ee500(void const *a_, void const *b_, void const *input, void *collector_)
{
	body_2ee3f0 const *a = (body_2ee3f0 const *)a_;
	body_2ee3f0 const *b = (body_2ee3f0 const *)b_;
	collector_2ee3f0 *collector = (collector_2ee3f0 *)collector_;
	timer_add_2ebc80("TtGsk");
	convex_2fefd0 const *first = a->shape;
	convex_2fefd0 const *second = b->shape;
	c_query_transform transform;
	transform.inverse_product(&a->placement->transform, &b->placement->transform);
	pair_state_2fefd0 state;
	if (second->type() == 5)
	{
		state.function_2ff0c0(first, (triangle_2ff0c0 const *)second, &transform);
	}
	else
	{
		state.start(first, second, &transform);
	}
	class_3048a0 search;
	search.field_c = true;
	closest_3048a0 closest;
	int status = search.function_3048a0(first, second, &transform, &state, &closest);
	timer_add_2ebc80("Et");
	if (status != 0 || closest.point.m128_f32[3] - first->radius - second->radius < 0.0f)
	{
		collector->slot_1(a, b);
	}
}

// @retail 0x2ee610
found_2ee610 convex_pair_2ee2e0::function_2ee610(body_2ee3f0 const *a, body_2ee3f0 const *b, void const *input, kept_contact_2ee610 *out)
{
	return query_kept_2ee610(this, a, b, input, out);
}

// the contact of two convex shapes, with a search state started for the
// pair alone: the closest points the search finds, taken onto the shapes'
// surfaces, passed on when they are within the tolerance
// @retail 0x2ee8b0
void __cdecl function_2ee8b0(void const *a_, void const *b_, void const *input, void *collector)
{
	timer_add_2ebc80("LtGsk");
	body_2ee3f0 const *a = (body_2ee3f0 const *)a_;
	body_2ee3f0 const *b = (body_2ee3f0 const *)b_;
	convex_2fefd0 const *first = a->shape;
	convex_2fefd0 const *second = b->shape;
	kept_contact_2ee610 kept;
	c_query_transform transform;
	transform.inverse_product(&a->placement->transform, &b->placement->transform);
	pair_state_2fefd0 state;
	if (second->type() == 5)
	{
		state.function_2ff0c0(first, (triangle_2ff0c0 const *)second, &transform);
	}
	else
	{
		state.start(first, second, &transform);
	}
	first = a->shape;
	second = b->shape;
	class_3048a0 search;
	search.field_c = false;
	{
		closest_3048a0 closest;
		if (search.function_3048a0(first, second, &transform, &state, &closest) == 0)
		{
			((source_3032c0 const *)&search)->function_3032c0(&state, (plane_3032c0 *)&closest, &kept.found);
			kept.normal.rotate(&a->placement->transform, &kept.found.normal);
			float const distance = kept.found.distance - first->radius - second->radius;
			kept.found.distance = distance;
			if (distance < ((input_2ee8b0 const *)input)->tolerance)
			{
				kept.found.point = _mm_add_ps(kept.found.point, _mm_mul_ps(splat_2ee8b0(-first->radius), kept.found.normal));
				__m128 const surface = _mm_add_ps(kept.found.point, _mm_mul_ps(splat_2ee8b0(-distance), kept.found.normal));
				local_point_2ee8b0 local;
				local.inverse_transform(&transform, surface);
				kept.local_point = local.value;
				contact_2ee8b0 contact;
				contact.a = a;
				contact.b = b;
				contact.point = transform_2ee8b0(&b->placement->transform, kept.local_point);
				contact.point.m128_f32[3] = distance;
				contact.normal = kept.normal.value;
				((contact_collector_2ee8b0 *)collector)->add_contact(&contact);
			}
		}
	}
	timer_add_2ebc80("lt");
}

struct candidate_302c70;
struct body_302eb0;
struct input_302eb0;
struct contact_302eb0;
struct world_contacts_302eb0;
class id_owner_302eb0;
struct s_3029d0_entry;
class class_3029d0_owner;

void __cdecl function_302c70(void const *a, void const *b, void const *input, candidate_302c70 const *candidate, owner_302a10 *owner, point_302a10 *points, int *count);
void __cdecl function_302eb0(body_302eb0 const *a, body_302eb0 const *b, input_302eb0 const *input, float minimum,
	contact_302eb0 *contacts, int *count, world_contacts_302eb0 **out_list, id_owner_302eb0 *ids);
void __cdecl function_3029d0(s_3029d0_entry *entries, int *count, class_3029d0_owner *owner);

// score 14 (was 202): only the stack slots differ: retail keeps the kept
// contact at esp+0x20 and the search's closest points at esp+0x70, here the
// reverse; renaming either, a block scope around the search, and a pointer to
// the contact do not move them.
// the pair's kept contact points brought up to date: skipped, the points
// given up, while the shapes' motion since the last query cannot have used
// up the distance they were apart; else the search's contact, when within the
// tolerance, is added to the kept points and the kept points are passed on,
// and otherwise the points are given up and the separating direction may be
// kept for next time
// @retail 0x2eeb40
void convex_pair_2ee2e0::function_2eeb40(body_2ee3f0 const *a, body_2ee3f0 const *b, void const *input_, void *out)
{
	timer_add_2ebc80("LtGsk");
	kept_contact_2ee610 kept;
	if (query_kept_2ee610(this, a, b, input_, &kept).value)
	{
		function_302c70(a, b, input_, (candidate_302c70 const *)&kept, (owner_302a10 *)field_8, points, (int *)&field_18);
		function_302eb0((body_302eb0 const *)a, (body_302eb0 const *)b, (input_302eb0 const *)input_, kept.found.distance - 0.001f,
			(contact_302eb0 *)points, (int *)&field_18, (world_contacts_302eb0 **)out, (id_owner_302eb0 *)field_8);
	}
	else
	{
		function_3029d0((s_3029d0_entry *)points, (int *)&field_18, (class_3029d0_owner *)field_8);
	}
	timer_add_2ebc80("lt");
}
