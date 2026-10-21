// @flags /O2 /GL-
// Havok (third-party physics library): a world refreshing every member's
// pairs: each member of each group, in both lists of groups, has its pairs
// ended and begun again and its group marked, then every added object has its
// overlaps brought up to date.
#include "growable_array_2d91d0.h"

class entity_316000;
class object_30dce0;

struct group_30f710;

// a member: the group it belongs to at +0x44
struct member_30f710
{
	char unknown0[0x44];
	group_30f710 *group;
};

ELEMENTS_2D91D0(members_30f710, member_30f710 *);

struct group_30f710
{
	char unknown0[0x28];
	bool changed;
	char unknown29[0x3c - 0x29];
	members_30f710 members;
};

ELEMENTS_2D91D0(groups_30f710, group_30f710 *);
ELEMENTS_2D91D0(objects_30f710, object_30dce0 *);

class class_30e140
{
public:
	void function_30e140(entity_316000 *entity);
};

struct member_30c8e0;

class owner_30c8e0
{
public:
	void function_30df80(member_30c8e0 *member);
};

// a flag passed by value
struct flag_30dce0
{
	bool value;

	flag_30dce0(bool value_)
	{
		value = value_;
	}
};

class world_30dce0
{
public:
	void function_30dce0(object_30dce0 *object, flag_30dce0 notify);
};

class class_30f710
{
public:
	char unknown0[8];
	groups_30f710 active_groups;
	groups_30f710 inactive_groups;
	char unknown20[0x120 - 0x20];
	objects_30f710 objects;

	void function_30f710();
};

// score 24 (was 24): the two group loops match; the last call differs because
// retail passes 0x30dce0 its flag as a one-byte object built in the argument
// slot, where 0x30dce0's declaration (src/unknown_30dce0.cpp) takes a bool.
// @retail 0x30f710
void class_30f710::function_30f710()
{
	int i;
	for (i = 0; i < active_groups.size; i++)
	{
		members_30f710 *members = &active_groups.data[i]->members;
		for (int j = 0; j < members->size; j++)
		{
			member_30f710 *member = members->data[j];
			((class_30e140 *)this)->function_30e140((entity_316000 *)member);
			((owner_30c8e0 *)this)->function_30df80((member_30c8e0 *)member);
			member->group->changed = true;
		}
	}
	for (i = 0; i < inactive_groups.size; i++)
	{
		members_30f710 *members = &inactive_groups.data[i]->members;
		for (int j = 0; j < members->size; j++)
		{
			member_30f710 *member = members->data[j];
			((class_30e140 *)this)->function_30e140((entity_316000 *)member);
			((owner_30c8e0 *)this)->function_30df80((member_30c8e0 *)member);
			member->group->changed = true;
		}
	}
	for (int k = 0; k < objects.size; k++)
	{
		((world_30dce0 *)this)->function_30dce0(objects.data[k], flag_30dce0(true));
	}
}
