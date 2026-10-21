// @flags /O2 /GL- /GX
// Havok (third-party physics library): removing an entity from the world, or
// queueing it for removal while the world is locked; and first moving the
// constraints and actions that tie it only to fixed entities from its island
// to the world.
#include "growable_array_2d91d0.h"
#include "stack_array_30d740.h"

class entity_316000;
class constraint_owner_30f960;

struct info_30f960
{
	long field_0;
	long field_4;
	long field_8;
	long field_c;
};

// a constraint between two entities
class constraint_30f960
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
	virtual long type() { return 0; }
	virtual void slot_9() {}
	virtual void get_info(info_30f960 *info) {}

	char unknown4[8];
	entity_316000 *first;
	entity_316000 *second;
	constraint_owner_30f960 *owner;
};

ELEMENTS_2D91D0(constraints_30f960, constraint_30f960 *);

// what keeps constraints: an island, or the world
class constraint_owner_30f960
{
public:
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void add_constraint(constraint_30f960 *constraint, info_30f960 const *info) {}
	virtual void remove_constraint(constraint_30f960 *constraint, info_30f960 const *info) {}

	char unknown4[0x58 - 4];
	constraints_30f960 constraints;
};

// something acting on entities, which can list them
class action_30f960
{
public:
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void slot_2() {}
	virtual void slot_3() {}
	virtual void slot_4() {}
	virtual void slot_5() {}
	virtual void entities(array_longs_30d740 *out) {}

	char unknown4[0x10 - 4];
	constraint_owner_30f960 *owner;
};

class entity_316000
{
public:
	char unknown0[0x40];
	bool fixed;
	constraint_owner_30f960 *island;
	char unknown48[0x94 - 0x48];
	action_30f960 **actions;
	long action_count;
};

struct member_316d60;
struct entry_3169c0;

class class_316c50
{
public:
	void function_316d60(member_316d60 *member);
};

class list_owner_3169c0
{
public:
	void function_3169c0(entry_3169c0 *entry);
};

// whether the entity was removed now, given back by value
struct bool_30f8d0
{
	bool value;

	bool_30f8d0(bool value_)
	{
		value = value_;
	}
};

ELEMENTS_2D91D0(entities_30f8d0, entity_316000 *);

class class_30f030
{
public:
	void function_30f6b0();
};

class class_30d320
{
public:
	void function_30d320();
};

class world_30e2c0
{
public:
	void function_30e2c0(entity_316000 *entity);
};

class class_30e140
{
public:
	char unknown0[0x24];
	int pending_count;
	char unknown28[0x2c - 0x28];
	constraint_owner_30f960 *constraint_owner;
	char unknown30[0xf0 - 0x30];
	entities_30f8d0 removals;
	char unknownfc[0x12c - 0xfc];
	char locked;

	void function_30e140(entity_316000 *entity);
	bool_30f8d0 function_30f8d0(entity_316000 *entity);
	void function_30f960(entity_316000 *entity);
};

// @retail 0x30f8d0
bool_30f8d0 class_30e140::function_30f8d0(entity_316000 *entity)
{
	if (locked == 1)
	{
		removals.append(entity);
		((referenced_2d88a0 *)entity)->reference_count++;
		return bool_30f8d0(false);
	}
	if (pending_count > 0)
	{
		((class_30f030 *)this)->function_30f6b0();
	}
	function_30e140(entity);
	((class_30d320 *)entity)->function_30d320();
	((world_30e2c0 *)this)->function_30e2c0(entity);
	return bool_30f8d0(true);
}

// the entity's island gives up to the world each constraint whose other
// entity is fixed, and each action all of whose other entities are fixed;
// then the entity is removed
// score 23 (was 169): the constraint is read where it is used, and the one
// that is moved is read before the list shrinks, so the loop uses the
// registers retail uses; the last call still loads this after its arguments
// instead of before them
// @retail 0x30f960
void class_30e140::function_30f960(entity_316000 *entity)
{
	constraint_owner_30f960 *island = entity->island;
	for (int i = 0; i < island->constraints.size; i++)
	{
		if (island->constraints.data[i]->type() != 11)
		{
		if ((island->constraints.data[i]->first == entity && island->constraints.data[i]->second->fixed) ||
			(island->constraints.data[i]->second == entity && island->constraints.data[i]->first->fixed))
		{
			constraints_30f960 *constraints = &island->constraints;
			constraint_30f960 *moved = island->constraints.data[i];
			constraints->size--;
			for (int j = i; j < constraints->size; j++)
			{
				constraints->data[j] = constraints->data[j + 1];
			}
			info_30f960 info;
			moved->get_info(&info);
			island->remove_constraint(moved, &info);
			constraint_owner->constraints.append(moved);
			constraint_owner->add_constraint(moved, &info);
			moved->owner = constraint_owner;
			i--;
		}
		}
	}
	longs_30d740 others(0x10);
	for (int k = 0; k < entity->action_count; k++)
	{
		entity->actions[k]->entities(&others);
		for (int m = 0; m < others.size; m++)
		{
			entity_316000 const *other = (entity_316000 const *)others.data[m];
			if (!other->fixed && other != entity)
			{
				goto next;
			}
		}
		((class_316c50 *)constraint_owner)->function_316d60((member_316d60 *)entity->actions[k]);
		((list_owner_3169c0 *)island)->function_3169c0((entry_3169c0 *)entity->actions[k]);
		entity->actions[k]->owner = constraint_owner;
next:;
	}
	function_30f8d0(entity);
}
