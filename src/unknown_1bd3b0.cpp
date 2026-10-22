// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "slot_handler.h"
#include "unknown_26e370.h"
#include "unknown_26c380.h"
#include "unit_requests.h"
#include "unknown_1f4460.h"
#include <math.h>

/* slot type 0x5d */

struct s_slot_5d
{
	s_slot_header header;
	bool active;
	byte unknown0d[3];
	long element_index;
	byte unknown14[0x1c - 0x14];
	short flags;
	byte unknown1e[2];
	long unknown20;
	short unknown24;
	byte unknown26[0x38 - 0x26];
	long unknown38;
	byte unknown3c[0x40 - 0x3c];
};

short __stdcall function_1bd850(long actor_index);
short __stdcall function_1bda60(long actor_index, s_slot *slot, bool active);
long __stdcall function_1bd890(long actor_index, s_slot *slot);
bool __stdcall function_1bdad0(long actor_index, s_slot *slot, long index);
short __stdcall function_1bde80(long actor_index, long joint_index, long a, long b);
point3f *function_b9dd0(long object_index, point3f *result);

/* an element of g_502424 as slot type 0x5d sees it */
struct s_5d_element
{
	short unknown00;
	short unknown02;
	byte unknown04[0x7c - 0x4];
	short unknown7c;
	byte unknown7e[2];
	long object_index;
	short unknown84;
	short unknown86;
	byte unknown88[0x94 - 0x88];
	short unknown94;
};

/* the tag of the object a slot 0x5d joint is about */
struct s_5d_object_tag
{
	byte unknown000[0x244];
	short unknown244;
};

__declspec(noinline) bool function_f5dc0(long object_index);

real function_30bf0(vector3f *v);
long function_1fa7f0(void);

struct s_trace_path_globals
{
	byte unknown00[0xc4];
	long count;
	s_pathfinding_data *first;
};

// @retail 0x1bd400
bool function_1bd400(long object_index, long actor_index, s_type_c3b527 const *point,
	long node_index, vector3f const *direction, short selected, bool check_sides,
	s_type_c3b527 *out_point, long *out_node)
{
	s_slot_object_view *object = object_get(object_index);
	bool result = false;
	s_tag_element *element = function_1e5450(actor_index, object->tag_index);
	vector3f local_direction;
	s_sector_trace_result traces[3];
	function_210770(point->output_index, direction, &local_direction);
	if (element)
	{
		real distance = *(real *)((byte *)element + 0x14) * 0.8f;
		s_trace_path_globals *globals = (s_trace_path_globals *)g_4e0348;
		s_pathfinding_data *pathfinding = NULL;
		if (globals->count > 0)
			pathfinding = globals->first;
		if (!function_26c590(pathfinding, &point->point, node_index, NONE, &local_direction, distance, NULL,
			(s_path_trace_result *)&traces[0]))
		{
			if (check_sides)
			{
				function_210770(point->output_index, &object->forward, &local_direction);
				local_direction.k = 0.0f;
				if (function_30bf0(&local_direction) != 0.0f)
				{
					pathfinding = (s_pathfinding_data *)function_1fa7f0();
					if (!function_26c590(pathfinding, &traces[0].point, traces[0].sector_index,
						NONE, &local_direction, 0.5f, NULL, (s_path_trace_result *)&traces[1]))
					{
						local_direction.i *= -1.0f;
						local_direction.j *= -1.0f;
						local_direction.k *= -1.0f;
						pathfinding = (s_pathfinding_data *)function_1fa7f0();
						result = !function_26c590(pathfinding, &traces[0].point, traces[0].sector_index,
							NONE, &local_direction, 0.5f, NULL, (s_path_trace_result *)&traces[2]);
					}
				}
			}
			else
				result = true;
			if (result)
			{
				if (out_point)
				{
					out_point->point = traces[selected].point;
					out_point->output_index = point->output_index;
				}
				if (out_node)
					*out_node = traces[selected].sector_index;
			}
		}
	}
	return result;
}

/* the object's velocity is under 0.2 */
// @retail 0x1bd3b0
bool function_1bd3b0(long object_index)
{
	bool result = false;

	if (magnitude3d(&object_get(object_index)->velocity) < 0.2f)
		result = true;
	return result;
}

struct s_1bd610
{
    union
    {
        byte field_0;
        struct
        {
            word : 4;
            word flag4 : 1;
            word : 11;
        };
    };
    byte field_2[2];
    long field_4;
    word field_8;
    byte field_a[2];
    s_type_c3b527 field_c;
    long field_1c;
};

typedef char assert_1bd610_size[(sizeof(s_1bd610) == 0x20) ? 1 : -1];

struct s_1bd611
{
    byte field_0[6];
    short field_6;
    vector3f field_8;
};

long function_25d810(long object_index, long actor_index, bool create);
void __stdcall function_26c2d0(long arg_0);

// @retail 0x1bd610
bool function_1bd610(s_1bd610 *arg_0, long arg_1, s_1bd611 *arg_2)
{
    bool local_0 = false;
    s_slot_object_view *local_1 = object_get(arg_0->field_4);
    long local_2 = function_25d810(arg_0->field_4, arg_1, false);
    if (local_2 != NONE)
    {
        s_prop_state_view *local_3 = prop_node_state(prop_node_get(local_2));
        function_26c2d0(local_2);
        if (*(long *)((byte *)local_3 + 0x44) != NONE)
        {
            vector3f const *local_4 = (vector3f const *)((byte *)local_1 + 0x7c);
            vector3f local_5 = *local_4;
            local_5.k = 0.0f;
            if (function_30bf0(&local_5) == 0.0f)
            {
                real up_j = local_4->j;
                local_5.i = up_j * local_1->forward.k - local_1->forward.j * local_4->k;
                local_5.j = local_1->forward.i * local_4->k - local_1->forward.k * local_4->i;
                local_5.k = local_1->forward.j * local_4->i - local_1->forward.i * local_4->j;
            }
            function_210770(local_3->unknown48.output_index, &local_5, &local_5);
            s_type_c3b527 local_6;
            long local_7;
            local_0 = function_1bd400(arg_0->field_4, arg_1, &local_3->unknown48,
                *(long *)((byte *)local_3 + 0x44), &local_5, arg_0->field_8,
                arg_2->field_6 > 1, &local_6, &local_7);
            if (!local_0)
            {
                local_5.i = local_5.i * -1.0f;
                local_5.j = local_5.j * -1.0f;
                local_5.k = local_5.k * -1.0f;
                local_0 = function_1bd400(arg_0->field_4, arg_1, &local_3->unknown48,
                    *(long *)((byte *)local_3 + 0x44), &local_5, arg_0->field_8,
                    arg_2->field_6 > 1, &local_6, &local_7);
            }
            if (local_0)
            {
                arg_0->field_c = local_6;
                arg_0->field_1c = local_7;
                arg_2->field_8.i = local_5.i * -1.0f;
                arg_2->field_8.j = local_5.j * -1.0f;
                arg_2->field_8.k = local_5.k * -1.0f;
            }
        }
    }
    if (local_0)
        arg_0->flag4 = true;
    else
        arg_0->flag4 = false;
    return local_0;
}

// @retail 0x1bd850
short __stdcall function_1bd850(long actor_index)
{
	s_slot_entry_iterator iterator;
	short result = 0;

	iterator.actor_index = actor_index;
	iterator.reference.unknown2 = 0x5d;
	iterator.reference.unknown0 = NONE;
	if (function_26f0c0(&iterator))
		result = 3;
	return result;
}

// @retail 0x1bd890
long __stdcall function_1bd890(long actor_index, s_slot *slot)
{
	s_slot_5d *state = (s_slot_5d *)slot;
	s_actor_view *actor = actor_get(actor_index);
	long result = NONE;
	long object_index = NONE;

	if (state->flags & 1)
		object_index = state->unknown20;
	if (actor->prop_index != NONE && prop_node_view(prop_node_get(actor->prop_index)))
	{
		s_slot_entry_iterator iterator;
		s_slot_memory_entry *entry;

		iterator.actor_index = actor_index;
		iterator.reference.unknown2 = 0x5d;
		iterator.reference.unknown0 = NONE;
		for (entry = function_26f0c0(&iterator); entry; entry = function_26f0c0(&iterator))
		{
			if ((object_index == NONE || ((s_5d_element *)element_502424_get(entry->unknown4))->object_index == object_index) &&
				function_26ecc0(actor_index, iterator.reference.unknown0, (s_joint_behavior_state *)slot))
			{
				result = entry->unknown4;
				break;
			}
		}
		if (result == NONE)
		{
			if (object_index == NONE)
				goto done;
			long created = function_26e940(actor_index);
			if (created != NONE)
			{
				state->active = true;
				state->element_index = created;
			}
			result = created;
		}
		if (result != NONE)
		{
			s_5d_element *element = (s_5d_element *)element_502424_get(result);

			if (state->active)
			{
				element->object_index = object_index;
				element->unknown84 = 0;
				element->unknown94 = 0;
				element->unknown86 = (((s_5d_object_tag *)g_4e3b44[object_get(object_index)->tag_index & 0xffff].bytes)->unknown244 > 0) ? 3 : 1;
			}
			state->flags = (state->flags & ~0x1e) | 1;
			state->unknown20 = element->object_index;
			state->unknown24 = NONE;
		}
	}
done:
	return result;
}
// @retail 0x1bda60
short __stdcall function_1bda60(long actor_index, s_slot *slot, bool active)
{
	short result = g_46fbe8;
	s_slot_5d *state = (s_slot_5d *)slot;
	s_5d_element *element = (s_5d_element *)element_502424_get(state->element_index);

	if (!(element->object_index != NONE && state->unknown20 != NONE && element->unknown7c >= element->unknown86 &&
		function_f5dc0(element->object_index)))
	{
		element->unknown02 = 2;
		result = g_46fbe4;
	}
	return result;
}

void function_1f86a0(long actor_index);
bool function_1f86f0(long actor_index);
bool __stdcall function_1badc0(long actor_index, long object_index, point3f const *point,
    s_type_c3b527 *out_point, long *out_node, bool create, bool *alternate);

struct s_1bdad0_target
{
	long object_index;
	short count;
	short required_count;
	vector3f direction;
	short next_id;
};

// @retail 0x1bdad0
bool __stdcall function_1bdad0(long actor_index, s_slot *slot, long index)
{
	bool result = true;
	s_actor_view *actor = actor_get(actor_index);
	s_5d_element *element = (s_5d_element *)index;
	s_1bdad0_target *target = (s_1bdad0_target *)&element->object_index;
	s_1bd610 *movement = (s_1bd610 *)((byte *)slot + 0x1c);
	long object_index = target->object_index;

	if (object_index == NONE || ((s_slot_5d *)slot)->unknown20 == NONE)
	{
		result = false;
		goto done;
	}
	{
	if ((short)movement->field_8 == NONE)
	{
		if (element->unknown02 == 1)
			movement->field_8 = target->next_id++;
		if ((short)movement->field_8 == NONE)
			goto done;
	}
	{
		object_index = target->object_index;
		bool stationary = function_1bd3b0(object_index);
		if (stationary)
		{
			if ((*(short *)movement) & 0x10)
			{
				if (!((*(short *)movement) & 2))
				{
					long prop_index = function_25d810(object_index, actor_index, false);
					real distance = function_210970((s_type_c3b527 const *)&actor->unknown27c,
						&movement->field_c);
					if (prop_index != NONE && *(short *)((byte *)prop_node_get(prop_index) + 0x1c) == 0 && distance < 0.4f)
						(*(short *)movement) |= 0xa;
					if (!((*(short *)movement) & 2) && ((*(short *)movement) & 8) && distance < 0.4f)
					{
						(*(short *)movement) |= 2;
						function_1f86a0(actor_index);
					}
					if (function_1f86f0(actor_index))
						(*(short *)movement) |= 0xa;
				}
				vector3f const *direction = &target->direction;
				if (((*(short *)movement) & 2) && actor->unknown290.k * direction->k +
					actor->unknown290.j * direction->j + actor->unknown290.i * direction->i > 0.95f && !((*(short *)movement) & 4))
				{
					target->count++;
					(*(short *)movement) |= 4;
				}
			}
			else
			{
				(*(short *)movement) &= ~2;
				if ((*(short *)movement) & 4)
				{
					target->count--;
					(*(short *)movement) &= ~4;
				}
			}
			if (target->count == element->unknown7c)
			{
				s_unit_request request;
				request.type = 0x21;
				*(long *)request.arguments = actor->unknown018;
				function_e6900(movement->field_4, &request);
			}
		}
		else
		{
			if ((*(short *)movement) & 4)
			{
				target->count--;
				(*(short *)movement) &= ~4;
			}
			(*(short *)movement) &= ~0x12;
		}
		if (actor->unknown040 && stationary && !((*(short *)movement) & 2))
		{
			if (!function_1bd610(movement, actor_index,
				(s_1bd611 *)target))
			{
				result = false;
				goto done;
			}
			point3f world;
			s_type_c3b527 destination;
			long node_index;
			bool alternate = false;
			function_210850(&movement->field_c, &world);
			if (function_1badc0(actor_index, movement->field_4, &world, &destination, &node_index, false, &alternate))
			{
				result = function_1f4460(actor_index, &movement->field_c, movement->field_1c,
					alternate ? NONE : movement->field_4, false);
				if (!result)
					actor_get(actor_index)->unknown040 = false;
			}
		}
	}	}
done:
	return result;
}


// @retail 0x1bdd70
void __stdcall function_1bdd70(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_5d *state = (s_slot_5d *)slot;
	s_502424_target *target = &element_502424_get(state->element_index)->target;

	if (list->count == 0)
	{
		actor->unknown41c = 2;
		actor->unknown420 = 2;
		if (actor->unknown086 >= 7)
		{
			actor->unknown488 = true;
			actor->unknown41c = 3;
			actor->unknown420 = 2;
		}
	}
	else if ((state->flags & 2) && (state->flags & 0x10))
	{
		actor->unknown41c = 3;
		actor->unknown420 = 4;
		actor->unknown424.point = target->point;
		actor->unknown44d = true;
		actor->unknown4cc = 0.2f;
	}
	else
	{
		actor->unknown430 = 2;
		actor->unknown434 = 6;
		actor->unknown438.object_index = state->unknown20;
		actor->unknown444 = NONE;
	}
}

/* the joint behavior (g_502424) as function_1bde80 reads it */
struct s_1bde80_joint
{
	byte unknown00[0x80];
	long object_index;
	short unknown84;
	short participant_count;
};

/* invites the clump members within 10 of the joint's object, the nearest
   first */
// score 48 (was 59): retail tests the loop condition at the top of the loop
// and jumps back to
// that test, ours tests it at the bottom, so the preamble and the register
// allocation differ.
// @retail 0x1bde80
short __stdcall function_1bde80(long actor_index, long joint_index, long a, long b)
{
	long count = 0;
	s_actor_view *actor = actor_get(actor_index);
	s_1bde80_joint *joint = (s_1bde80_joint *)element_502424_get(joint_index);
	point3f position;

	function_b9dd0(joint->object_index, &position);
	if (joint->participant_count > 1)
	{
		long index = element_502420_get(actor->unknown07c)->first_actor_index;

		while (index != NONE)
		{
			s_actor_view *other = actor_get(index);
			long other_index = index;

			if (other != actor)
			{
				real i = position.x - other->position.x;
				real j = position.y - other->position.y;
				real k = position.z - other->position.z;
				real distance = (real)sqrt(j * j + (i * i + k * k));

				if (10.0f > distance && function_26eae0(joint_index, other_index, 3, 1.0f / (distance + 0.1f)))
					count++;
			}
			index = other->next_index;
		}
	}
	return (short)count;
}
// @retail 0x1bdfd0
void __stdcall function_1bdfd0(long actor_index, s_slot *slot, long index)
{
	s_slot_5d *state = (s_slot_5d *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if ((state->flags & 1) && state->unknown20 == index || element->target.unknown0 == index)
	{
		state->unknown20 = NONE;
		state->flags &= ~0x11;
		element->target.unknown0 = NONE;
	}
}

// @retail 0x1be020
void __stdcall function_1be020(long actor_index, s_slot *slot)
{
	s_slot_5d *state = (s_slot_5d *)slot;

	state->flags &= ~0x10;
	state->unknown38 = 0;
}

s_slot_handler_2x g_47ec48 =
{
	{
		{
			0x5d, 2, 0, -2, 0,
			function_1bd850, function_1bda60, joint_initiate, joint_leave, 1, {0},
			0, 0, function_1bdfd0, function_1be020, 0, 0, 1
		},
		(t_slot_proc)joint_update, joint_activate, joint_deactivate
	},
	(t_slot_proc)function_1bd890, 0, (t_slot_release)function_1bdad0, slot_release_nothing, function_1bdd70, (t_slot_proc4)function_1bde80,
	1, 3, 0.2f, 0
};
