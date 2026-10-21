// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "slot_handler.h"
#include "unknown_26e370.h"
#include "props.h"

/* slot type 0x12 */

struct s_slot_12
{
	s_slot_header header;
	bool unknown0c;
	byte unknown0d[3];
	long element_index;
	byte unknown14[0x1c - 0x14];
	long unknown1c;
	long unknown20;
	byte unknown24[0x40 - 0x24];
};

short __stdcall function_1b7e40(long actor_index);
short __stdcall function_1b81c0(long actor_index, s_slot *slot, bool active);
long __stdcall function_1b8070(long actor_index, s_slot *slot);
bool __stdcall function_1b82d0(long actor_index, s_slot *slot, long index);
struct s_invite_data;
short __stdcall function_1b83b0(long actor_index, long leader_index, s_slot *slot, s_invite_data *data);
void function_265c30(long prop_index, long actor_index, bool unknown);

struct s_invite_data
{
	long unknown0;
	long unknown4;
};


// @retail 0x1b8360
void __stdcall function_1b8360(long actor_index, s_slot *slot, long index)
{
	s_slot_12 *state = (s_slot_12 *)slot;

	state->unknown20--;
}

// @retail 0x1b8370
void __stdcall function_1b8370(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown41c = 4;
	actor->unknown420 = 2;
	actor->unknown449 = true;
	actor->unknown44a = true;
}

// @retail 0x1b8460
void __stdcall function_1b8460(long actor_index, s_slot *slot, long index)
{
	s_slot_12 *state = (s_slot_12 *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if (element->target.unknown0 == index)
		element->target.unknown0 = NONE;
}

void function_ba1d0(long object_index, vector3f *linear_velocity, vector3f *angular_velocity);

bool actor_has_joint_invitation(long actor_index, short type);
long function_25d810(long object_index, long actor_index, bool create);
bool function_25d9b0(long prop_index);

// @retail 0x1b7e40
short __stdcall function_1b7e40(long actor_index)
{
	s_actor_view *actor = (s_actor_view *)(g_4f55f0->data + (actor_index & 0xffff) * sizeof(s_actor_view));
	long prop_index = actor->prop_index;
	s_prop_node_view *node = NULL;
	if (prop_index != NONE)
		node = (s_prop_node_view *)(g_502418->data + (prop_index & 0xffff) * sizeof(s_prop_node_view));
	short priority;
	if (actor_has_joint_invitation(actor_index, 0x12))
	{
		s_slot_entry_iterator iterator;
		iterator.actor_index = actor_index;
		iterator.reference.unknown2 = 0x12;
		iterator.reference.unknown0 = NONE;
		s_slot_memory_entry *local_1;
		while ((local_1 = function_26f0c0(&iterator)) != NULL)
		{
			short invitation_index = iterator.reference.unknown0;
			s_502424_element *joint = (s_502424_element *)(g_502424->data + (((s_slot_owner_entry *)actor)->joint_invitations[invitation_index].joint_index & 0xffff) * sizeof(s_502424_element));
			if (!node || joint->target.unknown0 != node->object_index)
			{
				long new_prop;
				if (actor->unknown086 >= 4 || (new_prop = function_25d810(joint->target.unknown0, actor_index, false)) == NONE)
				{
					joint_decline(actor_index, iterator.reference.unknown0);
					continue;
				}
				node = (s_prop_node_view *)(g_502418->data + (new_prop & 0xffff) * sizeof(s_prop_node_view));
			}
			priority = *(short *)((byte *)joint + ((s_slot_owner_entry *)actor)->joint_invitations[invitation_index].participant_index * 12 + 0xa);
			if (priority <= 0)
				return 0;
			goto evaluate;
		}
		return (short)(long)local_1;
	}
	else
	{
		if (actor->unknown07c == NONE || !node)
			return 0;
		node = (s_prop_node_view *)(g_502418->data + (prop_index & 0xffff) * sizeof(s_prop_node_view));
		if (node->unknown27 < 2)
			return 0;
		priority = 3;
	}
evaluate:
	s_type_5cfb45 *state = function_25d690((s_prop_datum *)node);
	s_prop_view_fields *view = NULL;
	if (node->view_index != NONE)
	{
		byte *local_0 = g_502414->data + (node->view_index & 0xffff) * 0x124;
		if (local_0)
			view = (s_prop_view_fields *)(local_0 + 0x70);
	}
	vector3f velocity;
	function_ba1d0(node->object_index, &velocity, NULL);
	if (node->unknown28 > 1.6f && (real)sqrt((double)(velocity.i * velocity.i + velocity.j * velocity.j + velocity.k * velocity.k)) < 1.0f &&
		view->unknown54 < 0.31 && node->unknown28 <= 15.0f &&
		!state->unknown64 && !function_25d9b0(actor->prop_index))
		return priority;
	return 0;
}

// score 73 (was 73): retail calls function_25d740, our build inlines it
// through the
// prop_node_view wrapper, so the two blocks are laid out differently.
// @retail 0x1b81c0
short __stdcall function_1b81c0(long actor_index, s_slot *slot, bool active)
{
	s_slot_12 *state = (s_slot_12 *)slot;
	short result = g_46fbe4;

	if (state->unknown20 > 0)
	{
		long prop_index = actor_get(actor_index)->prop_index;

		if (prop_index == state->unknown1c)
		{
			s_prop_node_view *node = prop_node_get(prop_index);

			if (0.8f > node->unknown28)
			{
				result = 0x10;
				return result;
			}

			s_prop_view_fields *view = prop_node_view(node);
			vector3f velocity;

			function_ba1d0(node->object_index, &velocity, NULL);
			if (view && sqrt(length_sq3f(&velocity)) < 1.0f && view->unknown54 < 0.31 && node->unknown28 <= 15.0f)
				return g_46fbe8;
		}
	}
	return result;
}

// @retail 0x1b82d0
bool __stdcall function_1b82d0(long actor_index, s_slot *slot, long index)
{
	bool result = true;
	long local_0 = ((s_slot_12 *)slot)->unknown1c;
	s_actor_view *actor = actor_get(actor_index);

	if (actor->prop_index != local_0)
		return false;

	bool local_1 = actor->unknown040;
	if (local_1)
	{
		if (((s_slot_12 *)slot)->unknown0c)
			local_1 = function_1f4810(actor_index, actor->prop_index, 0.3f, 0);
		else
			local_1 = function_1f4810(actor_index, actor->prop_index, 3.5f, 0);
		if (!local_1)
		{
			function_265c30(actor->prop_index, actor_index, true);
			actor_get(actor_index)->unknown040 = local_1;
		}
		result = local_1;
	}
	return result;
}

PRIVATE __forceinline s_actor_view *actor_step_1b83b0(long *index, long *current)
{
    if (*index == NONE)
        return NULL;
    *current = *index;
    s_actor_view *actor = actor_get(*index);
    *index = actor->next_index;
    return actor;
}

// @retail 0x1b83b0
short __stdcall function_1b83b0(long actor_index, long leader_index, s_slot *slot, s_invite_data *data)
{
	long count = 0;
	s_actor_view *actor = actor_get(actor_index);

	data->unknown4 = ((s_slot_12 *)slot)->unknown1c;
	if (actor->unknown07c == NONE)
		goto done;
	{
		long index = element_502420_get(actor->unknown07c)->first_actor_index;

		long other_index;
		s_actor_view *other;
		while ((other = actor_step_1b83b0(&index, &other_index)) != NULL)
		{
			if (actor != other && function_26eae0(leader_index, other_index, 3, 1.0f))
				count++;
		}
	}
done:
	return (short)count;
}

s_slot_handler_2x g_47e898 =
{
	{
		{
			0x12, 2, 0, -2, 0,
			function_1b7e40, function_1b81c0, joint_initiate, joint_leave, NONE, {0},
			0, 0, function_1b8460, 0, 0, 0, 1
		},
		(t_slot_proc)joint_update, joint_activate, joint_deactivate
	},
	(t_slot_proc)function_1b8070, 0, (t_slot_release)function_1b82d0, function_1b8360, function_1b8370, (t_slot_proc4)function_1b83b0,
	1, 10, 1.0f, 0
};


void function_26ee40(s_joint_behavior_state *behavior);
void function_267770(long prop_index, long actor_index);

// @retail 0x1b8070
long __stdcall function_1b8070(long actor_index, s_slot *slot)
{
    s_actor_view *actor = actor_get(actor_index);
    s_slot_entry_iterator iterator;
    iterator.actor_index = actor_index;
    iterator.reference.unknown2 = 0x12;
    iterator.reference.unknown0 = NONE;
    s_slot_memory_entry *entry;
    register s_slot_12 *state = (s_slot_12 *)slot;
    long joint_index = NONE;
    while ((entry = function_26f0c0(&iterator)) != NULL)
    {
        if (function_26ecc0(actor_index, iterator.reference.unknown0, (s_joint_behavior_state *)slot))
        {
            joint_index = entry->unknown4;
            break;
        }
    }
    if (joint_index == NONE)
    {
        long candidate = function_26e940(actor_index);
        if (candidate != NONE)
        {
            state->unknown0c = true;
            state->element_index = candidate;
        }
        joint_index = candidate;
    }
    if (joint_index != NONE)
    {
        s_502424_element *element = element_502424_get(joint_index);
        if (state->unknown0c)
        {
            element->target.unknown0 = prop_node_get(actor->prop_index)->object_index;
            state->unknown1c = actor->prop_index;
        }
        else
        {
            long prop_index = function_25d810(element->target.unknown0, actor_index, false);
            if (prop_index != NONE)
            {
                state->unknown1c = prop_index;
                if (prop_index != actor->prop_index)
                    function_267770(prop_index, actor_index);
            }
            else
                goto failed;
        }
        real time = (real)g_510c54->field_2_3 * 10.0f;
        long ticks;
        __asm
        {
            fld time
            fistp ticks
        }
        state->unknown20 = ticks;
    }
done:
    return joint_index;
failed:
    if (joint_index != NONE)
    {
        function_26ee40((s_joint_behavior_state *)slot);
        return NONE;
    }
    goto done;
}
