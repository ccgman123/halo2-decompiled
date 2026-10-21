// @flags /O2 /GL- /GX
// Havok (third-party physics library): bringing an object's overlaps up to
// date: the broad phase lists the pairs its bounding box touches; each other
// object the filter accepts is added to the object (and to the object's
// listener when it has one of kind 2), each one it rejects is removed the
// same way.
#include "stack_array_30d740.h"
#include "growable_array_2d91d0.h"

// a flag passed by value
struct flag_30dce0
{
	bool value;

	flag_30dce0(bool value_)
	{
		value = value_;
	}
};

struct bool_result_30dce0
{
	bool value;
};

class collidable_30dce0;

struct box_30dce0
{
	__m128 minimum;
	__m128 maximum;
};


class listener_30dce0
{
public:
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void slot_2() {}
	virtual void slot_3() {}
	virtual void slot_4() {}
	virtual void slot_5() {}
	virtual void bounding_box(box_30dce0 *box) {}
	virtual void add_overlap(collidable_30dce0 *other, flag_30dce0 flag) {}
	virtual void remove_overlap(collidable_30dce0 *other) {}
	virtual void overlaps_changed() {}
};

// the part the broad phase keeps
struct handle_30dce0
{
	char unknown0[8];
	long kind;
	char unknownc[4];
	listener_30dce0 *listener;
};

struct base_30dce0
{
	char unknown0[0x10];
};

class collidable_30dce0 : public base_30dce0, public handle_30dce0
{
};

// an object, listening to its own overlaps
class object_30dce0 : public listener_30dce0
{
public:
	char unknown4[8];
	collidable_30dce0 collidable;
};

class broad_phase_30dce0
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
	virtual void query(box_30dce0 const *box, pairs_30d870 *pairs) {}
};

class filter_30dce0
{
public:
	virtual bool_result_30dce0 accept(collidable_30dce0 const *first, collidable_30dce0 const *second) { bool_result_30dce0 result = {false}; return result; }
};

// an array of objects, owning no storage until it grows
struct collidables_30dce0
{
	ELEMENTS_2D91D0_MEMBERS(collidable_30dce0 *)

	collidables_30dce0() : data(0), size(0), capacity_and_flags(0x80000000) {}
	~collidables_30dce0()
	{
		if ((capacity_and_flags & 0x80000000) == 0)
		{
			g_memory_2d88a0->release_block(data, (capacity_and_flags & 0x7fffffff) * sizeof(collidable_30dce0 *), 0x12);
		}
	}
	void append(collidable_30dce0 *value)
	{
		if (size == (capacity_and_flags & 0x7fffffff))
		{
			function_2d91d0(this, sizeof(collidable_30dce0 *));
		}
		data[size] = value;
		size++;
	}
};

struct filtering_30dce0
{
	char unknown0[8];
	filter_30dce0 filter;
};

class world_30dce0
{
public:
	char unknown0[0xc4];
	broad_phase_30dce0 *broad_phase;
	char unknownc8[0xd0 - 0xc8];
	filtering_30dce0 *filter_owner;
	char unknownd4[0x234 - 0xd4];
	long pair_capacity;

	void function_30dce0(object_30dce0 *object, flag_30dce0 notify);
};

// score 161 (was 200): the flag is now a plain byte written into a local, which
// matches retail's size, but retail keeps this in ebx and the object in esi
// while ours keeps this in edi and the object in ebx, and retail builds the
// flag in the argument slot where ours copies it there
// @retail 0x30dce0
void world_30dce0::function_30dce0(object_30dce0 *object, flag_30dce0 notify)
{
	pairs_30d870 pairs(pair_capacity);
	box_30dce0 box;
	object->bounding_box(&box);
	broad_phase->query(&box, &pairs);
	collidables_30dce0 added;
	collidables_30dce0 removed;
	collidable_30dce0 *own = &object->collidable;
	for (int i = 0; i < pairs.size; i++)
	{
		if ((handle_30dce0 *)pairs.data[i].second != own)
		{
			if (filter_owner->filter.accept(own, static_cast<collidable_30dce0 *>((handle_30dce0 *)pairs.data[i].second)).value)
			{
				added.append(static_cast<collidable_30dce0 *>((handle_30dce0 *)pairs.data[i].second));
			}
			else
			{
				removed.append(static_cast<collidable_30dce0 *>((handle_30dce0 *)pairs.data[i].second));
			}
		}
	}
	if (removed.size != 0)
	{
		for (int i = 0; i < removed.size; i++)
		{
			object->remove_overlap(removed.data[i]);
			if (removed.data[i]->kind == 2)
			{
				removed.data[i]->listener->remove_overlap(own);
			}
		}
	}
	if (notify.value)
	{
		object->overlaps_changed();
	}
	flag_30dce0 flag(true);
	if (added.size != 0)
	{
		for (int i = 0; i < added.size; i++)
		{
			object->add_overlap(added.data[i], flag);
			if (added.data[i]->kind == 2)
			{
				added.data[i]->listener->add_overlap(own, flag);
			}
		}
	}
}
