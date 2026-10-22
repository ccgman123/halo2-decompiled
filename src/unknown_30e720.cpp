// @flags /O2 /GL- /GX
// Havok (third-party physics library): splitting a world's islands. An island
// with no bodies left is removed; one marked for checking has its bodies
// sorted into the groups their joins connect, and every group after the
// first moves to a new island with its bodies, the actions and joins that
// reach it and the linked pairs that touch it.
#include "growable_array_2d91d0.h"
#include "stack_array_30d740.h"
#include "joined_groups_3720a0.h"

// an array of 4-byte values whose storage comes from the allocator's stack
// area (its constructor and destructor are in unknown_2e18e0.cpp)
struct array_longs_2e1ab0
{
	long *data;
	long size;
	unsigned long capacity_and_flags;

	array_longs_2e1ab0() : data(0), size(0), capacity_and_flags(0x80000000) {}
	~array_longs_2e1ab0()
	{
		if ((capacity_and_flags & 0x80000000) == 0)
		{
			g_memory_2d88a0->release_block(data, (capacity_and_flags & 0x7fffffff) * sizeof(long), 0x12);
		}
	}
};

struct longs_2e1ab0 : array_longs_2e1ab0
{
	long *stack_block;
	long stack_capacity;

	longs_2e1ab0(long capacity);
	~longs_2e1ab0();
};

// an island as it is made (the constructor is in unknown_3171a0.cpp)
class object_3171a0
{
public:
	REFERENCED_NEW_2D88A0(0x2d)

	char unknown0[0xb0];

	object_3171a0(long value);
};

struct id_pairs_317d90;

class class_317d90
{
public:
	void function_317d90(id_pairs_317d90 *pairs) const;
};

class island_30e720;

// what a body holds that says how much it adds to its island
class shape_30e720
{
public:
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void slot_2() {}
	virtual void slot_3() {}
	virtual void slot_4() {}
	virtual long slot_5() { return 0; }
};

struct body_30e720
{
	char unknown0[0x3c];
	shape_30e720 *shape;
	char unknown40[4];
	island_30e720 *island;
	char unknown48[0x58 - 0x48];
	long index;
};

ELEMENTS_2D91D0(bodies_30e720, body_30e720 *);

// a list of bodies with room for sixteen inside it
struct listed_bodies_30e720
{
	body_30e720 **data;
	long size;
	unsigned long capacity_and_flags;

	listed_bodies_30e720() : data(0), size(0), capacity_and_flags(0x80000000) {}
	~listed_bodies_30e720()
	{
		if ((capacity_and_flags & 0x80000000) == 0)
		{
			g_memory_2d88a0->release_block(data, (capacity_and_flags & 0x7fffffff) * sizeof(body_30e720 *), 0x12);
		}
	}
};

struct inline_bodies_30e720 : listed_bodies_30e720
{
	body_30e720 *storage[16];

	inline_bodies_30e720()
	{
		data = storage;
		size = 0;
		capacity_and_flags = 16 | 0x80000000;
	}
};

// an action on bodies: it lists them, and is kept by the island of the first
// one still in an island
class action_30e720
{
public:
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void slot_2() {}
	virtual void slot_3() {}
	virtual void slot_4() {}
	virtual void slot_5() {}
	virtual void list_bodies(listed_bodies_30e720 *bodies) {}

	char unknown4[0xc];
	island_30e720 *island;
};

ELEMENTS_2D91D0(actions_30e720, action_30e720 *);

// what a join hands its islands when it moves
struct join_info_30e720
{
	long values[4];
};

// a join between two bodies
class join_30e720
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
	virtual void slot_8() {}
	virtual void slot_9() {}
	virtual void fill_move_info(join_info_30e720 *info) {}

	char unknown4[8];
	body_30e720 *first;
	body_30e720 *second;
	island_30e720 *island;
};

ELEMENTS_2D91D0(joins_30e720, join_30e720 *);

// a linked pair's side: a part 0x10 into the object that holds the body
struct head_30e720
{
	char unknown0[0x10];
};

struct side_30e720
{
	char unknown0[0x10];
};

struct linked_30e720 : head_30e720, side_30e720
{
	body_30e720 *body;
};

struct link_30e720
{
	side_30e720 *first;
	side_30e720 *second;
	long field_8;
	long field_c;
};

ELEMENTS_2D91D0_ALIGNED(links_30e720, link_30e720);

class island_30e720
{
public:
	virtual ~island_30e720() {}
	virtual void slot_1() {}
	virtual void join_added(join_30e720 *join, join_info_30e720 *info) {}
	virtual void join_removed(join_30e720 *join, join_info_30e720 *info) {}

	char unknown4[0x10];
	long weight;
	char unknown18[0x24 - 0x18];
	long index;
	bool needs_split;
	char unknown29[0x30 - 0x29];
	char field_30;
	char unknown31[0x3c - 0x31];
	bodies_30e720 bodies;
	char unknown48[4];
	actions_30e720 actions;
	joins_30e720 joins;
	char unknown64[4];
	links_30e720 links;
	char unknown74[0xae - 0x74];
	char flag_ae;

	// whether the island was taken out of its world
	bool taken_out_of_world() const
	{
		bool const taken = index == -1;
		return taken;
	}
};

ELEMENTS_2D91D0(islands_30e720, island_30e720 *);

class c_311100
{
public:
	char unknown0[8];
	islands_30e720 islands;

	void function_30e720();
};

// score 465 (was 506): the same steps in the same order as retail. The
// island test now goes through a bool local, so it is set with sete and
// tested, as in retail (grouping_3720a0::single_group, in a shared header,
// would need the same to match retail's sete there; a local copy of it scores
// 495). Still: the registers differ throughout (retail keeps the world in edi,
// the island index in ebx and zero in ebp; it reloads the world at the top of
// the groups loop, entering past the reload, where ours reloads it on the back
// edge), and the frame is realigned to 8 bytes because the grouping is 8 bytes
// here; retail's grouping takes 12 bytes (0x34-0x40) and no realignment (a
// local wrapper with a third word drops the realignment but scores 518-525).
// Retail converts a linked pair's side to its holder with a plain `sub 0x10`
// left unfolded; static_cast null-checks it here, and char arithmetic folds
// the offset away (both tried: 509, or 485 with the wrapper). The grouping's
// destructor is the empty function 0x175f40, called here without retail's
// `lea ecx`.
// @retail 0x30e720
void c_311100::function_30e720()
{
	for (int i = islands.size - 1; i >= 0; i--)
	{
		island_30e720 *island = islands.data[i];
		if (island->bodies.size == 0)
		{
			islands.data[i] = islands.data[islands.size - 1];
			islands.data[i]->index = i;
			islands.size--;
			delete island;
		}
		else if (island->needs_split)
		{
			pairs_30d870 pairs(island->actions.size * 4 + island->links.size + island->joins.size);
			((class_317d90 *)island)->function_317d90((id_pairs_317d90 *)&pairs);
			longs_2e1ab0 parents(island->bodies.size);
			longs_2e1ab0 scratch(pairs.size);
			grouping_3720a0 grouping((array_2d91d0 *)&parents, (array_2d91d0 *)&scratch);
			grouping.function_372200((array_2d91d0 const *)&pairs, island->bodies.size);
			if (!grouping.single_group())
			{
				groups_372300 groups;
				grouping.function_372390(&groups);
				for (int g = 1; g < groups.size; g++)
				{
					island_30e720 *created = (island_30e720 *)new object_3171a0((long)this);
					islands.append(created);
					created->index = islands.size - 1;
					created->field_30 = island->field_30;
					if ((created->bodies.capacity_and_flags & 0x7fffffff) < groups.data[g].size)
					{
						function_2d9160(&created->bodies, groups.data[g].size, sizeof(body_30e720 *));
					}
					created->bodies.resize(groups.data[g].size);
					if (island->flag_ae)
					{
						created->flag_ae = 1;
					}
					for (int j = 0; j < groups.data[g].size; j++)
					{
						body_30e720 *body = island->bodies.data[groups.data[g].data[j]];
						body->island = created;
						body->index = j;
						long weight = body->shape->slot_5();
						island->weight -= weight;
						created->weight += weight;
						created->bodies.data[j] = body;
					}
				}
				for (int k = 0; k < island->bodies.size;)
				{
					body_30e720 *body = island->bodies.data[k];
					if (body->island != island)
					{
						island->bodies.size--;
						island->bodies.data[k] = island->bodies.data[island->bodies.size];
					}
					else
					{
						body->index = k;
						k++;
					}
				}
				for (int a = island->actions.size - 1; a >= 0; a--)
				{
					inline_bodies_30e720 listed;
					island->actions.data[a]->list_bodies(&listed);
					island_30e720 *owner = 0;
					for (int n = 0; n < listed.size; n++)
					{
						owner = listed.data[n]->island;
						if (!owner->taken_out_of_world())
						{
							break;
						}
					}
					island->actions.data[a]->island = owner;
					island_30e720 *kept_by = island->actions.data[a]->island;
					if (kept_by != island)
					{
						kept_by->actions.append(island->actions.data[a]);
						island->actions.size--;
						island->actions.data[a] = island->actions.data[island->actions.size];
					}
				}
				for (int c = island->joins.size - 1; c >= 0; c--)
				{
					join_30e720 *join = island->joins.data[c];
					island_30e720 *owner = join->first->island;
					if (owner->taken_out_of_world())
					{
						owner = join->second->island;
					}
					if (owner != island)
					{
						join = island->joins.data[c];
						island->joins.size--;
						island->joins.data[c] = island->joins.data[island->joins.size];
						owner->joins.append(join);
						join->island = owner;
						join_info_30e720 info;
						join->fill_move_info(&info);
						owner->join_added(join, &info);
						island->join_removed(join, &info);
					}
				}
				long kept = 0;
				for (int l = 0; l < island->links.size; l++)
				{
					link_30e720 *link = &island->links.data[l];
					island_30e720 *owner = static_cast<linked_30e720 &>(*link->first).body->island;
					if (owner->taken_out_of_world())
					{
						owner = static_cast<linked_30e720 &>(*link->second).body->island;
					}
					if (owner != island)
					{
						owner->links.append_copy(*link);
					}
					else
					{
						island->links.data[kept] = *link;
						kept++;
					}
				}
				island->links.resize(kept);
			}
			island->needs_split = false;
		}
	}
}
