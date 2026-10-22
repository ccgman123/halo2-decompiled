// @flags /O2 /GL- /GX
// Havok (third-party physics library): a query against a container's parts
// that welds their contacts: each part's convex contact is gathered, the
// contacts are taken closest first, and a contact's surface hides the
// contacts behind it; each part's kept points are then brought up to date.
#include "pair_agents_2eab70.h"
#include "reference_counted_2d88a0.h"
#include "timer_mark_2ebc80.h"
#include <xmmintrin.h>

// a mark in the timing stream, when there is room for it
__forceinline void timer_add_2fa2f0(char const *label)
{
	timer_mark_2ebc80 *mark = (timer_mark_2ebc80 *)g_480154;
	bool const room = (char *)mark < g_480158;
	if (room)
	{
		mark->label = label;
		unsigned long time;
		__asm
		{
			rdtsc
			mov time, eax
		}
		mark->time = time;
		g_480154 = (char *)(mark + 1);
	}
}

// three axes and a position
struct c_query_transform
{
	__m128 axes[3];
	__m128 position;
};

// where a body sits: two vectors, then its transform
struct placement_2fa2f0
{
	__m128 field_0;
	__m128 field_10;
	c_query_transform transform;
};

// a direction turned back by a transform's axes (src/unknown_2da610.cpp)
struct vector_2da6e0
{
	__m128 value;
	void inverse_rotate(const c_query_transform *matrix, const __m128 *direction);
};

// a shape: its kind, and for a container, its parts; a shape that wraps
// another holds it in its fourth word
class shape_2fa2f0
{
public:
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void slot_2() {}
	virtual void slot_3() {}
	virtual void slot_4() {}
	virtual long kind() const { return 0; }
	virtual void slot_6() {}
	virtual void slot_7() {}
	virtual void slot_8() {}
	virtual void slot_9() {}
	virtual void slot_10() {}
	virtual void slot_11() {}
	virtual void slot_12() {}
	// a container's part, built in the buffer given when it must be
	virtual shape_2fa2f0 const *part(long key, char *buffer) const { return 0; }

	char unknown4[8];
	shape_2fa2f0 const *child;
};

// one side of a query: a shape, a key, where it sits, and its parent side
struct body_2fa2f0
{
	shape_2fa2f0 const *shape;
	long key;
	placement_2fa2f0 const *placement;
	body_2fa2f0 const *parent;
};

// whether a dispatcher's table has a convex handler for a pair of kinds
// (src/unknown_2daec0.cpp)
struct result_2f98a0
{
	char value;

	result_2f98a0(bool value_) : value((char)value_) {}
};

struct dispatcher_2f98a0;
typedef result_2f98a0 (__cdecl *pair_test_2daf00)(dispatcher_2f98a0 const *dispatcher, long first, long second);
extern pair_test_2daf00 g_461f0c;

struct limits_2f97e0;

struct input_2fa2f0
{
	dispatcher_2f98a0 const *dispatcher;
	long field_4;
	float tolerance;
	limits_2f97e0 const *limits;
};

// a vector copied as a whole
struct vector_2fa0d0
{
	__m128 value;

	vector_2fa0d0 &operator=(vector_2fa0d0 const &other)
	{
		value = other.value;
		return *this;
	}
};

// a part's contact as its agent finds it
struct contact_2fa2f0
{
	vector_2fa0d0 field_0;
	vector_2fa0d0 field_10;
	float field_20;
	float field_24;
	vector_2fa0d0 field_30;
	vector_2fa0d0 field_40;
};

// a ray: a point and a direction (src/unknown_2fa0d0.cpp)
struct ray_2f97e0
{
	__m128 start;
	__m128 direction;
};

struct surface_2f97e0
{
	__m128 point;
	__m128 direction;
};

// a part's contact as the welding keeps it: the contact, its surface in the
// container's frame, the part's agent, shape and key, whether it is a
// surface that hides others, and whether it was found
struct entry_2fa0d0
{
	vector_2fa0d0 field_0;
	vector_2fa0d0 field_10;
	float field_20;
	float field_24;
	vector_2fa0d0 field_30;
	vector_2fa0d0 field_40;
	surface_2f97e0 surface;
	long field_70;
	long field_74;
	long field_78;
	char state;
	char field_7d;
};

struct entries_2fa0d0
{
	entry_2fa0d0 *data;
};

class ray_test_2f97e0
{
public:
	int function_2fa0d0(ray_2f97e0 const *ray, entries_2fa0d0 *entries, int first, int end, limits_2f97e0 const *limits);
};

// appends a blank entry (src/unknown_2f9e80.cpp)
struct block_2f9b40;
class class_2f9e80
{
public:
	entry_2fa0d0 *data;
	long size;
	unsigned long capacity_and_flags;

	block_2f9b40 *function_2f9e80();
};

// the entries, held in place while there are four or fewer
struct welded_2fa2f0 : class_2f9e80
{
	entry_2fa0d0 storage[4];

	welded_2fa2f0()
	{
		data = storage;
		size = 0;
		capacity_and_flags = 0x80000004;
	}
	~welded_2fa2f0()
	{
		if ((capacity_and_flags & 0x80000000) == 0)
		{
			g_memory_2d88a0->release_block(data, (capacity_and_flags & 0x7fffffff) * sizeof(entry_2fa0d0), 0x12);
		}
	}
};

// part buffers of 256 bytes from the allocator's stack area
// (src/unknown_2f9c10.cpp)
struct element_2f9c10
{
	char bytes[0x100];
};

struct array_2f9c10
{
	element_2f9c10 *data;
	long size;
	unsigned long capacity_and_flags;

	array_2f9c10() : data(0), size(0), capacity_and_flags(0x80000000) {}
	~array_2f9c10()
	{
		if ((capacity_and_flags & 0x80000000) == 0)
		{
			g_memory_2d88a0->release_block(data, (capacity_and_flags & 0x7fffffff) * sizeof(element_2f9c10), 0x12);
		}
	}
};

struct stack_array_2f9c10 : array_2f9c10
{
	element_2f9c10 *stack_block;
	long stack_capacity;

	stack_array_2f9c10(long capacity);
	~stack_array_2f9c10();
};

// a kept point of a pair: its points in the two bodies' frames and the normal
// (src/unknown_302eb0.cpp)
struct kept_point_2fa2f0
{
	__m128 first;
	__m128 second;
	__m128 normal;
};

// whether a part's agent found a contact
struct found_2fa2f0
{
	char value;

	found_2fa2f0(bool value_) : value((char)value_) {}
};

// the agent kept for a part: its kind, and the points it keeps
class part_agent_2fa2f0
{
public:
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void slot_2() {}
	virtual void slot_3() {}
	virtual void slot_4(void const *a, void const *b, void const *input, void *collector) {}
	virtual void slot_5() {}
	virtual void slot_6() {}
	virtual void slot_7() {}
	virtual found_2fa2f0 slot_8(void const *a, void const *b, void const *input, contact_2fa2f0 *contact) { return false; }

	char unknown4[0x11];
	char kind;
	char unknown16[2];
	int count;
	char unknown1c[0x14];
	kept_point_2fa2f0 points[1];
};

struct agent_entry_2fa2f0
{
	long key;
	long field_4;
	part_agent_2fa2f0 *agent;
};

class owner_302a10;

class agents_2fa2f0
{
public:
	char unknown0[8];
	owner_302a10 *owner;
	agent_entry_2fa2f0 *entries;
	long entry_count;
};

// the agent that stands for none (src/unknown_301a40.cpp)
struct object_4805a4;
object_4805a4 *__cdecl function_301a40();

struct candidate_302c70;
struct point_302a10;
struct body_302eb0;
struct input_302eb0;
struct contact_302eb0;
struct world_contacts_302eb0;
class id_owner_302eb0;

void __cdecl function_302c70(void const *a, void const *b, void const *input, candidate_302c70 const *candidate, owner_302a10 *owner, point_302a10 *points, int *count);
void __cdecl function_302eb0(body_302eb0 const *a, body_302eb0 const *b, input_302eb0 const *input, float minimum,
	contact_302eb0 *contacts, int *count, world_contacts_302eb0 **out_list, id_owner_302eb0 *ids);

inline void swap_2fa2f0(entry_2fa0d0 &first, entry_2fa0d0 &second)
{
	entry_2fa0d0 const held = first;
	first = second;
	second = held;
}

// score 494 (was 494): retail keeps this in esi and the second body in edi
// (here the other way round), so the timer compares its limit from memory;
// in the parts' loop retail reads the input again for the agent's call,
// keeps the agent in ebx and the found flag in another stack slot.
// @retail 0x2fa2f0
void class_2fa2f0::function_2fa2f0(void const *a_, void const *b_, void const *input_, void *collector)
{
	timer_add_2fa2f0("TtConvexWelder");
	body_2fa2f0 const *a = (body_2fa2f0 const *)a_;
	body_2fa2f0 const *b = (body_2fa2f0 const *)b_;
	input_2fa2f0 const *input = (input_2fa2f0 const *)input_;
	agents_2fa2f0 *agents = (agents_2fa2f0 *)this;
	body_2fa2f0 container_side;
	container_side.placement = b->placement;
	container_side.parent = b;
	container_side.shape = b->shape->child;
	container_side.key = b->key;
	((class_2e96a0 *)this)->function_2e96a0(a, &container_side, input, function_2e90f0);
	welded_2fa2f0 welded;
	stack_array_2f9c10 buffers(agents->entry_count);
	buffers.size = agents->entry_count;
	shape_2fa2f0 const *container = container_side.shape;

	// each part's contact through its agent, or the part passed on
	{
	body_2fa2f0 part;
	part.parent = b;
	part.placement = b->placement;
	for (long i = 0; i < agents->entry_count; i++)
	{
		agent_entry_2fa2f0 const *entry = &agents->entries[i];
		if ((object_4805a4 *)entry->agent == function_301a40())
		{
			continue;
		}
		part.shape = container->child->part(entry->key, buffers.data[i].bytes);
		part.key = entry->key;
		if (g_461f0c(input->dispatcher, a->shape->kind(), part.shape->kind()).value)
		{
			part_agent_2fa2f0 *agent = entry->agent;
			contact_2fa2f0 contact;
			found_2fa2f0 const found = agent->slot_8(a, &part, input, &contact);
			welded.function_2f9e80();
			welded.data[welded.size - 1].field_70 = (long)agent;
			welded.data[welded.size - 1].field_74 = (long)part.shape;
			welded.data[welded.size - 1].field_78 = entry->key;
			welded.data[welded.size - 1].field_7d = found.value;
			if (found.value)
			{
				entry_2fa0d0 *last = &welded.data[welded.size - 1];
				last->field_0 = contact.field_0;
				last->field_10 = contact.field_10;
				last->field_20 = contact.field_20;
				last->field_24 = contact.field_24;
				last->field_30 = contact.field_30;
				last->field_40 = contact.field_40;
				vector_2da6e0 direction;
				direction.inverse_rotate(&part.placement->transform, &contact.field_40.value);
				surface_2f97e0 *surface = &welded.data[welded.size - 1].surface;
				surface->point = contact.field_30.value;
				surface->direction = direction.value;
				surface->point.m128_f32[3] = contact.field_20;
				char const kind = agent->kind;
				welded.data[welded.size - 1].state = kind == 1 || kind == 2;
			}
		}
		else
		{
			entry->agent->slot_4(a, &part, input, collector);
		}
	}
	}

	// closest first; the contacts behind a closer one's surface are dropped
	// to the end
	long count = welded.size;
	for (long i = 0; i < count; i++)
	{
		entry_2fa0d0 *first = &welded.data[i];
		float closest_distance = first->field_20;
		long closest = i;
		for (long j = i; j < count; j++)
		{
			if (welded.data[j].field_20 < closest_distance)
			{
				closest_distance = welded.data[j].field_20;
				closest = j;
			}
		}
		swap_2fa2f0(welded.data[closest], *first);
		if (welded.data[i].field_7d)
		{
			count = ((ray_test_2f97e0 *)this)->function_2fa0d0((ray_2f97e0 const *)&welded.data[i].surface,
				(entries_2fa0d0 *)&welded, i + 1, count, input->limits);
		}
	}

	// and those behind each part's kept points
	long const total = welded.size;
	for (long k = 0; k < total; k++)
	{
		part_agent_2fa2f0 *agent = (part_agent_2fa2f0 *)welded.data[k].field_70;
		kept_point_2fa2f0 const *point = agent->points;
		for (int remaining = agent->count; remaining > 0; remaining--)
		{
			ray_2f97e0 ray;
			ray.start = point->second;
			ray.start.m128_f32[3] = point->normal.m128_f32[3];
			vector_2da6e0 direction;
			direction.inverse_rotate(&b->placement->transform, &point->normal);
			ray.direction = direction.value;
			count = ((ray_test_2f97e0 *)this)->function_2fa0d0(&ray, (entries_2fa0d0 *)&welded, 0, count, input->limits);
			point++;
		}
	}

	// the contacts kept added to each part's points, and its points brought
	// up to date
	for (long k = 0; k < total; k++)
	{
		body_2fa2f0 part;
		part.parent = b;
		part.placement = b->placement;
		entry_2fa0d0 const *entry = &welded.data[k];
		part.shape = (shape_2fa2f0 const *)entry->field_74;
		part.key = entry->field_78;
		part_agent_2fa2f0 *agent = (part_agent_2fa2f0 *)entry->field_70;
		if (k < count && entry->field_7d)
		{
			function_302c70(a, &part, input, (candidate_302c70 const *)entry, agents->owner,
				(point_302a10 *)agent->points, &agent->count);
		}
		function_302eb0((body_302eb0 const *)a, (body_302eb0 const *)&part, (input_302eb0 const *)input,
			welded.data[k].field_20 - 0.001f, (contact_302eb0 *)agent->points, &agent->count,
			(world_contacts_302eb0 **)collector, (id_owner_302eb0 *)agents->owner);
	}
	timer_add_2fa2f0("Et");
}
