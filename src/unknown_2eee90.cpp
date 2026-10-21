// @flags /O2 /GL-
// Havok (third-party physics library): registering the handlers for pairs of
// shape types in the handler table, and the convex pair's contact query.
#include "agent_table_2e5d50.h"
#include "convex_pair_2ee2e0.h"
#include "timer_mark_2ebc80.h"

void *__cdecl function_2ee390(long arg_0, long arg_1, long arg_2, long value);
void __cdecl function_2ee500(void const *a, void const *b, void const *input, void *collector);
void __cdecl function_2ee8b0(void const *a, void const *b, void const *input, void *collector);
void __cdecl function_2fe830(void const *a, void const *b, void const *input, void *collector, void *second_collector);

// registers handlers for pairs of shape types, each pair both ways round
// where the pair differs
// @retail 0x2eee90
void __cdecl function_2eee90(s_1c3a40 *table)
{
	handlers_2e5d50 handlers;
	handlers.handler[0] = (handler_2e5d50)function_2ee390;
	handlers.handler[1] = (handler_2e5d50)function_2ee500;
	handlers.handler[2] = (handler_2e5d50)function_2ee8b0;
	handlers.handler[3] = (handler_2e5d50)function_2fe830;
	table->function_2e5d50(&handlers, 1, 1);
}

// score 33 (was 191): only the stack slots differ: retail has the kept
// contact, the transform (shared with the contact passed on) and the closest
// points low to high, here the reverse order for the contact and the closest
// points; names, a pointer and declaration order do not move them.
// the contact of the pair's two convex shapes, passed on when within the
// tolerance; skipped while the shapes' motion since the last query cannot
// have used up the distance they were apart, and a miss may keep the
// separating direction for next time
// @retail 0x2eeed0
void convex_pair_2ee2e0::function_2eeed0(body_2ee3f0 const *a, body_2ee3f0 const *b, void const *input_, void *collector)
{
	timer_add_2ebc80("LtGsk");
	kept_contact_2ee610 kept;
	if (query_kept_2ee610(this, a, b, input_, &kept).value)
	{
		contact_2ee8b0 contact;
		contact.a = a;
		contact.b = b;
		contact.point = transform_2ee8b0(&b->placement->transform, kept.local_point);
		contact.point.m128_f32[3] = kept.found.distance;
		contact.normal = kept.normal.value;
		((contact_collector_2ee8b0 *)collector)->add_contact(&contact);
	}
	timer_add_2ebc80("lt");
}
