// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "slot_handler.h"
#include "unknown_26e370.h"
#include "data_array.h"

/* slot type 0x25 */

struct s_slot_25
{
	s_slot_header header;
	bool unknown0c;
	byte unknown0d[3];
	long element_index;
	byte unknown14[0x1c - 0x14];
	short unknown1c;
	byte unknown1e[0x40 - 0x1e];
};

/* where a prop was seen (0x18 bytes) */
struct s_prop_sighting
{
	short unknown0;
	byte unknown2[2];
	dword unknown4[5];
};

/* the prop's view as slot type 0x25 reads it */
struct s_prop_view_25
{
	byte unknown00[0xc];
	long unknown0c;
	byte unknown10[0x70 - 0x10];
	s_prop_sighting unknown70;
	byte unknown88[0x94 - 0x88];
	vector3f unknown94;
};

struct s_joint_participant_25
{
	long actor_index;
	short status;
	short priority;
	real score;
};

/* an element of g_502424 (a joint) as slot type 0x25 uses it */
struct s_502424_element_25
{
	short salt;
	short state;
	s_joint_participant_25 participants[10];
	byte unknown7c[0x80 - 0x7c];
	bool unknown80;
	bool unknown81;
	byte unknown82[2];
	long object_index;
	long unknown88;
	short unknown8c;
	byte unknown8e[2];
	real unknown90;
	s_prop_sighting unknown94;
	real unknownac;
	vector3f unknownb0;
};

short __stdcall function_1b6450(long actor_index, s_slot *slot, bool active);
long __stdcall function_1b6120(long actor_index, s_slot *slot);
bool function_26ba60(long prop_index, long actor_index, long clump_index);

static __forceinline long real_to_long(real value);

// @retail 0x1b6120
long __stdcall function_1b6120(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_25 *state = (s_slot_25 *)slot;
	long joint_index = NONE;

	if (actor->prop_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);
		s_prop_view_25 *view = (s_prop_view_25 *)prop_node_view(node);

		if (view)
		{
			s_slot_entry_iterator iterator;
			s_slot_memory_entry *entry;

			iterator.actor_index = actor_index;
			iterator.reference.unknown2 = 0x25;
			iterator.reference.unknown0 = NONE;
			for (entry = function_26f0c0(&iterator); entry; entry = function_26f0c0(&iterator))
			{
				long current_reference = iterator.reference.unknown0;
				if (joint_index == NONE && element_502424_get(entry->unknown4)->target.unknown4 == node->object_index &&
					function_26ecc0(actor_index, current_reference, (s_joint_behavior_state *)slot))
				{
					joint_index = entry->unknown4;
				}
				else
				{
					joint_decline(actor_index, current_reference);
				}
			}

			if (joint_index == NONE)
			{
				joint_index = function_26e940(actor_index);
				if (joint_index != NONE)
				{
					state->unknown0c = true;
					state->element_index = joint_index;
				}
			}

			if (joint_index != NONE)
			{
				s_502424_element_25 *element = (s_502424_element_25 *)element_502424_get(joint_index);

				if (state->unknown0c)
				{
					element->unknown80 = false;
					element->unknown81 = false;
					element->unknown8c = 0;
					element->object_index = node->object_index;
					element->unknown90 = -1.0f;
					element->unknownac = -1.0f;
				}
				else if (function_26ba60(node->unknown08, actor_index, actor->unknown07c))
				{
					function_1fb7e0(0x4e, actor_index, NULL, node->object_index, NONE);
				}

				if (element->unknown90 == -1.0f || (real)view->unknown0c > element->unknown90)
				{
					element->unknown90 = (real)view->unknown0c;
					element->unknown94 = view->unknown70;
				}
				if (length_sq3f(&view->unknown94) > 0.0f)
				{
					if (element->unknownac == -1.0f || (real)view->unknown0c > element->unknownac)
					{
						element->unknownac = (real)view->unknown0c;
						element->unknownb0 = view->unknown94;
					}
				}
				real ticks = function_x82e52f(&g_4e7408->unknown0, __FILE__, __LINE__) * 3.0f * g_510c54->field_2_3;
				long value = real_to_long(ticks);
				state->unknown1c = (short)value;
			}
		}
	}
	return joint_index;
}
short __stdcall function_1b6740(long actor_index, long leader_index, long a, long b);
short actor_behavior_find_level(long owner_index, short type);

/* the state of slot type 0x22 another actor has */
struct s_slot_22
{
	s_slot_header header;
	byte unknown0c[0x18 - 0xc];
	long prop_index;
	byte unknown1c[0x40 - 0x1c];
};

/* the elements of g_50241c (0xc4 bytes) as slot type 0x25 reads them */
struct s_50241c_element_25
{
	byte unknown00[0x33];
	bool unknown33;
	byte unknown34[0xc4 - 0x34];
};

inline s_50241c_element_25 *element_50241c_25_get(long index)
{
	return (s_50241c_element_25 *)(g_50241c->data + (index & 0xffff) * sizeof(s_50241c_element_25));
}

// @retail 0x1b6450
short __stdcall function_1b6450(long actor_index, s_slot *slot, bool active)
{
	short result = g_46fbe8;
	s_actor_view *actor = actor_get(actor_index);
	s_prop_node_view *node = prop_node_get(actor->prop_index);
	s_slot_25 *state = (s_slot_25 *)slot;
	s_502424_element_25 *joint = (s_502424_element_25 *)element_502424_get(state->element_index);

	if (joint->object_index == NONE)
		goto done;
	if (!joint->unknown80)
		goto evaluate;
	if (--state->unknown1c > 0)
		goto leave;

done:
	result = g_46fbe4;
	joint->unknown80 = true;
	if (actor->prop_index != NONE)
	{
		s_prop_view_25 *view = (s_prop_view_25 *)prop_node_view(node);

		if (view)
		{
			if (joint->unknown90 != -1.0f)
				view->unknown70 = joint->unknown94;
			if (joint->unknownac != -1.0f)
				view->unknown94 = joint->unknownb0;
		}
	}
leave:
	return result;

evaluate:
	{
		if (joint->unknown88 >= g_510c54->game_time)
			goto leave;

		joint->unknown88 = g_510c54->game_time;
		for (short i = 0; i < 10; i++)
		{
			s_joint_participant_25 *participant = &joint->participants[i];

			if (participant->actor_index != NONE && participant->status == 0)
			{
				s_actor_view *other = (s_actor_view *)record_pool_lookup(g_4f55f0, participant->actor_index);

				if (other && other->unknown221 && actor->unknown009)
				{
					if (joint->unknown8c > 10 * g_510c54->field_2_3)
						goto done;

					joint->unknown8c++;
					if (joint->state == 1 && !joint->unknown81 &&
						function_26ba60(node->unknown08, actor_index, actor->unknown07c) &&
						function_1fb7e0(0x4d, actor_index, NULL, joint->object_index, NONE))
					{
						joint->unknown81 = true;
					}
					goto leave;
				}
			}
		}

		s_prop_view_25 *view = (s_prop_view_25 *)prop_node_view(node);

		if (view && view->unknown70.unknown0 == 0 && function_26ba60(node->unknown08, actor_index, actor->unknown07c))
		{
			s_50241c_element_25 *element = element_50241c_25_get(node->unknown08);

			if (!element->unknown33)
				element->unknown33 = function_1fb7e0(0x4f, actor_index, NULL, joint->object_index, NONE);
		}
	}
	goto done;
}

/* invites the clump members whose slot 0x22 is at the actor's prop */
// @retail 0x1b6740
short __stdcall function_1b6740(long actor_index, long leader_index, long a, long b)
{
	long count = 0;
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown07c != NONE && actor->prop_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);
		s_element_502420_member_iterator iterator;
		s_actor_view *other;

		iterator.next_index = element_502420_get(actor->unknown07c)->first_actor_index;
		iterator.index = NONE;
		while ((other = element_502420_member_next(&iterator)) != 0)
		{
			long other_index = iterator.index;

			if (other != actor && other->unknown221)
			{
				short slot_index = actor_behavior_find_level(other_index, 0x22);

				if (slot_index != NONE)
				{
					s_slot_22 *state = (s_slot_22 *)&other->slots[slot_index];

					if (state->prop_index != NONE)
					{
						s_prop_node_view *other_node = (s_prop_node_view *)record_pool_lookup(g_502418, state->prop_index);

						if (other_node && node->object_index == other_node->object_index &&
							function_26eae0(leader_index, other_index, 3, 1.0f))
						{
							count++;
						}
					}
				}
			}
		}
	}
	return (short)count;
}
// @retail 0x1b6880
void __stdcall function_1b6880(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	actor_get(actor_index)->unknown4a1 = true;
}

// @retail 0x1b68b0
void __stdcall function_1b68b0(long actor_index, s_slot *slot, long index)
{
	s_slot_25 *state = (s_slot_25 *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if (element->target.unknown4 == index)
		element->target.unknown4 = NONE;
}

// @retail 0x1b68f0
void __stdcall function_1b68f0(long actor_index, s_slot *slot)
{
	s_slot_25 *state = (s_slot_25 *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if (element->target.unknown14 == 1)
		element->target.unknown4 = NONE;
}

s_slot_handler_2x g_47e648 =
{
	{
		{
			0x25, 2, 0, -2, 0,
			function_1adcd0, function_1b6450, joint_initiate, joint_leave, NONE, {0},
			0, 0, function_1b68b0, function_1b68f0, 0, 0, 1
		},
		(t_slot_proc)joint_update, joint_activate, joint_deactivate
	},
	(t_slot_proc)function_1b6120, 0, 0, 0, function_1b6880, (t_slot_proc4)function_1b6740,
	2, 10, 150.0f, 0
};
