// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_257630.CPP: joint slot handler 0x7f (handler at 0x47faa8) */

#include "unknown_11c920.h"
#include "slot_handler.h"
#include "unknown_26e370.h"
#include "unknown_2551c0.h"

/* the slot state of handler 0x7f */
struct s_slot_7f_state
{
	s_slot_header header;
	bool following;
	byte unknown0d[3];
	long joint_index;
	byte unknown14[0x40 - 0x14];
};

/* the actor's point at +0x22c */
struct s_actor_7f_view
{
	byte unknown000[0x22c];
	point3f unknown22c;
};

/* the clumps of g_502420 (0x50 bytes), with the time at +0x4c */
struct s_clump_7f_view
{
	byte unknown00[0x4c];
	long time;
};

/* the joint's flag at +0x88 (s_502424_element keeps a long there) */
struct s_joint_7f_view
{
	byte unknown00[0x88];
	bool started;
};

/* the slot the evaluation hands over to (type 0x3a) */
struct s_slot_3a_request
{
	s_slot_header header;
	short timer;
	byte unknown0e[0x1c - 0xe];
	long target_index;
	bool unknown20;
};

short g_470c58 = -1;
short g_470c5c = -2;

real function_30bf0(vector3f *v);
real normalize2d(point2f *v);
bool actor_has_joint_invitation(long actor_index, short type);
bool __stdcall function_1f4280(long actor_index);

short __stdcall function_257630(long actor_index);
short __stdcall function_2576c0(long actor_index, s_slot *slot, s_slot *next);
void __stdcall function_257cb0(long actor_index, s_slot *slot, long index);
long __stdcall function_257870(long actor_index, s_slot *slot);
void __stdcall function_257a40(long actor_index, s_slot *slot, long index);
bool __stdcall function_257a90(long actor_index, s_slot *slot, s_502424_element *joint);
void __stdcall function_257b60(long actor_index, s_slot *slot, s_slot_target_list *list);
short __stdcall function_257c60(long actor_index, long joint_index, s_slot *slot, long unknown);

s_slot_handler_2x g_47faa8 =
{
	{
		{
			0x7f, 2, 0, -2, 0,
			function_257630, (t_slot_evaluate)function_2576c0, joint_initiate, joint_leave, NONE, {0},
			function_257cb0, 0, 0, 0, 0, 0, 1
		},
		(t_slot_proc)joint_update, joint_activate, joint_deactivate
	},
	(t_slot_proc)function_257870, function_257a40, (t_slot_release)function_257a90, 0, function_257b60, (t_slot_proc4)function_257c60,
	2, 2, 1.0f, 0
};

PRIVATE __forceinline s_actor_view *function_257631(long arg_0)
{
	return (s_actor_view *)(g_4f55f0->data + (arg_0 & 0xffff) * sizeof(s_actor_view));
}

PRIVATE __forceinline s_clump_7f_view *function_257632(long arg_0)
{
	return (s_clump_7f_view *)(g_502420->data + (arg_0 & 0xffff) * sizeof(s_502420_element));
}

// score 19 (was 19): the pool lookup starts in the other register (ours
// mov eax,[esp+4], retail mov ecx,[esp+4]); the callee's body decides
// that and it lives in src/unknown_26e370.cpp, outside this lane.
// @retail 0x257630
short __stdcall function_257630(long actor_index)
{
	short result = 0;
	s_actor_view *actor = function_257631(actor_index);

	if (actor->unknown07c != NONE)
	{
		s_clump_7f_view *clump = function_257632(actor->unknown07c);

		if (actor->unknown004 == 2)
		{
			if (clump->time == NONE || (g_510c54->game_time - clump->time) * g_510c54->rate > 30.f)
			{
				result = 3;
			}
		}
		else if (actor_has_joint_invitation(actor_index, 0x7f))
		{
			result = 3;
		}
	}
	return result;
}

bool function_1f57f0(long actor_index, long animation, long const *target);

// @retail 0x2576c0
short __stdcall function_2576c0(long actor_index, s_slot *slot, s_slot *next)
{
	short result = g_470c5c;
	s_actor_view *actor = actor_get(actor_index);
	s_slot_7f_state *state = (s_slot_7f_state *)slot;
	s_502424_element *joint = element_502424_get(state->joint_index);
	long leader_index = joint->target.unknown0;

	if (leader_index == NONE)
	{
		result = g_470c58;
		goto local_9;
	}
	long target_index = joint->target.unknown4;
	if (target_index == NONE)
	{
		result = g_470c58;
		goto local_9;
	}

	if (state->following)
	{
		if (((s_joint_7f_view *)joint)->started)
		{
			s_slot_3a_request *request = (s_slot_3a_request *)next;
			long timer = g_510c54->field_2_3 * 3;

			request->unknown20 = true;
			request->target_index = target_index;
			request->timer = (short)timer;
			result = 0x3a;
			goto local_9;
		}
	}
	else
	{
		point3f *leader_position = &actor_get(leader_index)->position;
		point2f direction;

		direction.x = leader_position->x - actor->position.x;
		direction.y = leader_position->y - actor->position.y;
		normalize2d(&direction);
		if (direction.y * actor->unknown290.j + direction.x * actor->unknown290.i > 0.8 &&
			function_1f57f0(actor_index, 0x700002c, (long const *)&direction))
		{
			((s_joint_7f_view *)joint)->started = true;
			result = g_470c58;
			goto local_9;
		}
	}
local_9:
	return result;
}

struct s_actor_iterator_257870
{
	long index;
	long next;
};

static __forceinline s_actor_view *actor_iterator_next_257870(s_actor_iterator_257870 *iterator)
{
	s_actor_view *result = NULL;
	long index = iterator->next;

	if (index != NONE)
	{
		result = actor_get(index);
		iterator->index = index;
		iterator->next = result->next_index;
	}
	return result;
}

// @retail 0x257870
long __stdcall function_257870(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	long result = NONE;

	if (actor->unknown004 == 2)
	{
		if (actor->unknown07c != NONE)
		{
			s_502420_element *clump = element_502420_get(actor->unknown07c);
			real best_distance = 3.4028235e38f;
			long best_index = NONE;
			s_actor_iterator_257870 iterator;
			s_actor_view *other;

			iterator.next = clump->first_actor_index;
			while ((other = actor_iterator_next_257870(&iterator)) != NULL)
			{
				if (other != actor && other->unknown004 == 0)
				{
					vector3f delta;
					real distance;

					vector3d_from_points3d(&actor->position, &other->position, &delta);
					distance = (real)sqrt(delta.j * delta.j + delta.i * delta.i + delta.k * delta.k);
					if (distance < 1.f && distance < best_distance)
					{
						best_distance = distance;
						best_index = iterator.index;
					}
				}
			}

			if (best_index != NONE)
			{
				long joint_index = function_26e940(actor_index);

				if (joint_index != NONE)
				{
					s_slot_7f_state *state = (s_slot_7f_state *)slot;
					s_502424_element *joint;

					state->following = true;
					state->joint_index = joint_index;
					joint = element_502424_get(joint_index);
					joint->target.unknown4 = best_index;
					joint->target.unknown0 = actor_index;
					((s_clump_7f_view *)clump)->time = g_510c54->game_time;
				}
				result = joint_index;
			}
		}
	}
	else if (actor->unknown004 == 0)
	{
		s_slot_entry_iterator iterator;

		iterator.actor_index = actor_index;
		iterator.reference.unknown2 = 0x7f;
		iterator.reference.unknown0 = NONE;
		for (s_slot_memory_entry *entry = function_26f0c0(&iterator); entry; entry = function_26f0c0(&iterator))
		{
			if (function_26ecc0(actor_index, iterator.reference.unknown0, (s_joint_behavior_state *)slot))
			{
				result = entry->unknown4;
				break;
			}
		}
	}
	return result;
}

// @retail 0x257a40
void __stdcall function_257a40(long actor_index, s_slot *slot, long index)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown07c != NONE)
	{
		((s_clump_7f_view *)element_502420_get(actor->unknown07c))->time = g_510c54->game_time;
	}
}

// @retail 0x257a90
bool __stdcall function_257a90(long actor_index, s_slot *slot, s_502424_element *joint)
{
	bool result = true;

	if (joint->target.unknown0 == NONE || joint->target.unknown4 == NONE)
	{
		result = false;
	}
	else if (((s_slot_7f_state *)slot)->following)
	{
		if (actor_get(actor_index)->unknown040)
		{
			s_actor_view *leader = (s_actor_view *)record_pool_lookup(g_4f55f0, joint->target.unknown4);

			if (leader)
			{
				function_26c180(joint->target.unknown4);
				result = function_1f4460(actor_index, &leader->unknown27c.point, leader->unknown27c.unknown10, NONE, false);
			}
			else
			{
				result = false;
			}
		}
	}
	else
	{
		s_actor_view *leader = (s_actor_view *)record_pool_lookup(g_4f55f0, joint->target.unknown0);

		function_1f4280(actor_index);
		if (!leader)
		{
			result = false;
		}
	}
	return result;
}

// @retail 0x257b60
void __stdcall function_257b60(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	s_slot_7f_state *state = (s_slot_7f_state *)slot;
	s_actor_view *actor = actor_get(actor_index);
	s_502424_element *joint = (s_502424_element *)list;
	long target_index = state->following ? joint->target.unknown4 : joint->target.unknown0;
	s_actor_7f_view *target = (s_actor_7f_view *)datum_get_inlined(g_4f55f0, target_index);

	if (target)
	{
		vector3f direction;

		direction.i = target->unknown22c.x - ((s_actor_7f_view *)actor)->unknown22c.x;
		direction.j = target->unknown22c.y - ((s_actor_7f_view *)actor)->unknown22c.y;
		direction.k = target->unknown22c.z - ((s_actor_7f_view *)actor)->unknown22c.z;
		function_30bf0(&direction);
		actor->unknown41c = 3;
		actor->unknown420 = 4;
		actor->unknown424.vector = direction;
	}
}

// @retail 0x257c60
short __stdcall function_257c60(long actor_index, long joint_index, s_slot *slot, long unknown)
{
	s_502424_element *joint = element_502424_get(joint_index);
	short result = 0;

	if (function_26eae0(joint_index, joint->target.unknown4, 3, 1.0f))
	{
		result = 1;
	}
	return result;
}

// @retail 0x257cb0
void __stdcall function_257cb0(long actor_index, s_slot *slot, long index)
{
	s_slot_7f_state *state = (s_slot_7f_state *)slot;
	s_502424_element *joint = element_502424_get(state->joint_index);

	if (joint->target.unknown0 == index || joint->target.unknown4 == index)
	{
		joint->target.unknown0 = NONE;
		joint->target.unknown4 = NONE;
	}
}
