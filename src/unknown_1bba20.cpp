// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1BBA20.CPP: what the ai does when units and players act (called
   from the unit code) */

#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"
#include "unknown_1fb7e0.h"
#include "unknown_0d0690.h"

s_ai_player *ai_player_get(long player_index);

/* a player (0x21c bytes, g_4e8c24) */
struct s_ai_event_player
{
	byte unknown000[0x2c];
	long unit_index;
	byte unknown030[0x21c - 0x30];
};

/* a seat of a unit definition (0xb0 bytes) */
struct s_ai_event_seat
{
	dword flag0 : 1;
	dword flag1 : 1;
	dword flag2 : 1;
	dword flag3 : 1;
	dword flag4 : 1;
	dword flag5 : 1;
	dword flag6 : 1;
	dword flag7 : 1;
	dword flag8 : 1;
	dword flag9 : 1;
	dword flag10 : 1;
	dword flag11 : 1;
	dword unknown : 20;
	byte unknown04[0xb0 - 0x4];
};

struct s_ai_event_unit_definition
{
	byte unknown000[0x1cc];
	s_ai_event_seat *seats;
};

// @retail 0x1bba20
void function_1bba20(long actor_index)
{
	if (g_4f55d0->active)
	{
		s_actor_view *actor = actor_get(actor_index);

		if (actor->unknown018 != NONE)
			function_20ba60(0x69, actor->unknown018, actor_get(actor_index)->unknown26c, NONE, NONE, NULL);
	}
}

/* a player's unit got into a vehicle */
// @retail 0x1bbc00
void function_1bbc00(long player_index, long vehicle_index)
{
	if (g_4f55d0->active)
	{
		s_ai_event_player *player = (s_ai_event_player *)(g_4e8c24->data + (player_index & 0xffff) * sizeof(s_ai_event_player));
		s_ai_player *ai_player = ai_player_get(player_index);
		long unit_index;
		short seat_index;

		if (ai_player)
		{
			ai_player->unit_index = NONE;
			ai_player->unknown08 = NONE;
			ai_player->unknown0a = 0;
		}
		unit_index = player->unit_index;
		s_slot_object_view *unit = object_get(unit_index);
		seat_index = *(volatile short *)&unit->unknown1fc;
		if (seat_index != NONE)
		{
			s_ai_event_unit_definition *definition = (s_ai_event_unit_definition *)g_4e3b44[object_get(vehicle_index)->tag_index & 0xffff].bytes;

			if (!TEST_FIELD_BIT(definition->seats[seat_index].flag11))
				function_20ba60(0x66, unit_index, vehicle_index, NONE, NONE, NULL);
		}
	}
}

static __forceinline long real_to_long(real value);

// score 42 (was 57): the game clock is kept in a local so that the player
// index stays in EBX as in retail, but the clock is loaded before the child
// loop instead of after it, and the child iterator is passed on the stack
// instead of in EBX.
// @retail 0x1bbcc0
void function_1bbcc0(long player_index, long vehicle_index, long seat_index)
{
	if (g_4f55d0->active)
	{
		s_game_time_globals *game_clock = g_510c54;
		long player_slot = NONE;
		s_ai_player *player = ai_player_get(player_index);

		for (long i = 0; i < MAXIMUM_AI_PLAYERS; i++)
		{
			if (g_4f55cc[i].player_index == player_index)
			{
				player_slot = i;
				break;
			}
		}
		if (player)
		{
			real time;
			long ticks;
			s_object_child_iterator iterator;

			player->unit_index = vehicle_index;
			player->unknown08 = (short)seat_index;
			time = (real)game_clock->field_2_3 * 10.0f;
			ticks = real_to_long(time);
			player->unknown0a = (short)ticks;

			function_d0620(vehicle_index, &iterator);
			while (function_d0690(&iterator))
			{
				long actor_index = object_get(iterator.child_index)->actor_index;

				if (actor_index != NONE)
				{
					s_actor_view *actor = actor_get(actor_index);

					actor->unknown31c = (short)player_slot;
					time = (real)game_clock->field_2_3 * 2.0f;
					ticks = real_to_long(time);
					actor->unknown31e = (short)ticks;
					*(short *)actor->unknown320 = 0;
				}
			}
		}
	}
}


void function_1e3400(long actor_index, long squad_index);
real function_204950(long actor_index, long squad_index, short mode);
void function_26def0(long actor_index, long owner_index);
__declspec(noinline) bool function_f5dc0(long object_index);

// score 67 (was 72): retail compares the squad field against -1 in memory and
// reads it back as a word, ours loads the field into EAX first and narrows it
// to a word; the arguments of the distance call are also set up in the other
// order.
// @retail 0x1bba70
void function_1bba70(long actor_index, long vehicle_index, long seat_index)
{
    if (g_4f55d0->active)
    {
        s_actor_view *actor = actor_get(actor_index);
        byte *vehicle = (byte *)object_get(vehicle_index);
        long tag_index = *(long *)vehicle;
        byte *definition = (byte *)g_4e3b44[tag_index & 0xffff].bytes;
        if (*(short *)(definition + 0x1f0) != 6 || *(long *)(vehicle + 0x14) != NONE)
        {
            s_tag_element *entry = function_1e5450(actor_index, tag_index);
            if (entry && (*(byte *)((byte *)entry + 0x10) & 1) && *(long *)((byte *)actor + 0x28) != NONE)
                function_1e3400(actor_index, *(word *)((byte *)actor + 0x28));
            else
            {
                real distance = 3.4028234663852886e+38f;
                if (*(long *)((byte *)actor + 0x28) != NONE)
                    distance = function_204950(actor_index, *(long *)((byte *)actor + 0x28), 0);
                if (*(long *)((byte *)actor + 0x28) != NONE && distance < 3.0f)
                    function_1e3400(actor_index, *(word *)((byte *)actor + 0x28));
                else
                {
                    long squad_index = *(long *)(vehicle + 0x3a0);
                    if (squad_index == NONE || !(3.0f > function_204950(actor_index, squad_index, 0)))
                    {
                        *((byte *)actor + 0x3c) = true;
                        *(short *)((byte *)actor + 0x2c) = g_510c54->field_2_3 * 10;
                        function_26def0(actor_index, vehicle_index);
                    }
                }
            }
        }
        if (!*((byte *)actor + 0x3c))
            *(long *)((byte *)actor + 0x28) = NONE;
        actor->unknown2ec = (short)seat_index;
        actor->unknown2e8 = vehicle_index;
        *(short *)actor->unknown2ee = g_510c54->field_2_3 * 15;
        actor->unknown2f0 = function_f5dc0(vehicle_index);
        actor = actor_get(actor_index);
        *((byte *)actor + 0x5d4) = false;
        *(dword *)((byte *)actor + 0x810) &= ~1;
    }
}

void __stdcall function_1e1a00(long actor_index, long value);
long __stdcall function_1e0160(long squad_index, long entry_index, long unit_index, bool flag);
bool function_1e11b0(long actor_index, bool active);
void function_1e1150(long actor_index, short team);

// @retail 0x1bbdf0
void function_1bbdf0(long object_index)
{
    if (g_4f55d0->active)
    {
        s_slot_object_view *parent = object_get(object_index);
        long child_index = *(long *)((byte *)parent + 0x10);
        while (child_index != NONE)
        {
            s_slot_object_view *child = object_get(child_index);
            if (((1 << *((char *)child + 0xaa)) & 3) && child->unknown1fc == NONE)
            {
                byte *definition = g_4e3b44[child->tag_index & 0xffff].bytes;
                if (*(long *)(definition + 0x138) != NONE)
                {
                    long owner_index = *(long *)((byte *)parent + 0x248);
                    if (owner_index != NONE)
                    {
                        s_slot_object_view *owner = object_get(owner_index);
                        long actor_index = child->actor_index;
                        if (actor_index != NONE && actor_get(actor_index)->unknown024 != *(short *)((byte *)owner + 0x138))
                            function_1e1a00(actor_index, 0);
                        if (child->actor_index == NONE)
                        {
                            child_index = function_1e0160(NONE, *(long *)(definition + 0x138), child_index, false);
                            if (child_index != NONE)
                            {
                                function_1e11b0(child_index, true);
                                function_1e1150(child_index, *(short *)((byte *)owner + 0x138));
                            }
                        }
                    }
                    else if (child->actor_index != NONE)
                        function_1e1a00(child->actor_index, 0);
                }
            }
            child_index = *(long *)((byte *)child + 0xc);
        }
    }
}
