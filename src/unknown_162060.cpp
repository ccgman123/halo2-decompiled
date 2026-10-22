// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_162060.CPP: the game engine's update after a round starts (part
   of unknown_1523c0.cpp in the original): it refreshes the netgame entries and
   the teams, creates the engine's simulation entities when the simulation
   world is the authority, then starts the round's timers and ends the game
   for teams or players already past the score limit */

#include "unknown_11c920.h"
#include "globals.h"
#include "engine_peer.h"
#include "unknown_157450.h"
#include "unknown_xa19f52.h"

/* the players, as read here (0x21c bytes, unknown_157450.cpp) */
struct s_162060_player
{
	byte unknown000[0x2c];
	long unit_index;
	byte unknown030[0x164 - 0x30];
	long respawn_ticks;
	byte unknown168[0x170 - 0x168];
	long respawn_seconds;
};

struct s_162060_player_iterator
{
	s_162060_player *player;
	s_record_pool *data;
	long datum_index;
	long index;
};

/* the fields of the game options and the engine globals read here */
struct s_162060_options_view
{
	byte unknown000[0x190];
	long time_limit;
};

struct s_162060_globals_view
{
	byte unknown00[0x70];
	long round_start_time;
};

/* the entity table of the simulation world (unknown_08ad30.cpp) */
struct c_entry_table
{
	void function_08a400(dword identifier);
};

long entity_table_new_entity(c_entry_table *table, long handler_index);
void function_15adb0(s_netgame_entry_state *entries);
void function_1579f0(void);
void function_15c000(void);
void function_a77c0(void);
long function_1587f0(long team);
void function_15b3a0(long team, bool a);
bool player_iterator_next(long *iterator);

#define MACRO_0A8D68 ((s_simulation_world_view *)g_4cf77c)

static inline bool simulation_world_is_authority(void)
{
	long state = *(long *)((byte *)MACRO_0A8D68 + 8);

	return state == 4 || state == 5;
}

static inline bool simulation_world_is_playing(void)
{
	long state = *(long *)((byte *)MACRO_0A8D68 + 8);

	return state != 3 && state != 5;
}

/* stores a new entity's identifier and activates it */
static inline void simulation_entity_activate(c_entry_table *table, long *identifier_reference, long identifier)
{
	*identifier_reference = identifier;
	table->function_08a400(identifier);
}

/* creates an entity of a type in the world's entity table */
static inline long simulation_entity_create(s_simulation_world_view *world, long type)
{
	long identifier = NONE;

	if (simulation_world_is_authority() && simulation_world_is_playing())
	{
		s_simulation_entity_table *table = &world->database->table;

		identifier = entity_table_new_entity((c_entry_table *)table, type);
		if (identifier != NONE)
		{
			table->entities[identifier & 0x3ff].object_index = NONE;
		}
	}
	return identifier;
}

static inline bool game_engine_round_in_progress(void)
{
	return game_engine_get() && function_xaee93d()->value6c == 1 &&
		(g_4e6948->mode == 4 || function_xaee93d()->value_c04 == 1);
}

static inline bool engine_has_teams(void)
{
	bool result = false;

	if (game_engine_get())
	{
		volatile bool teams;

		result = (g_4e6948->flags184.bit0 & 1) != 0;
		teams = result;
	}
	return result;
}

static inline bool engine_team_is_playing(long team)
{
	bool result = false;

	if (engine_has_teams() && team >= 0 && team < 8)
	{
		result = (function_xaee93d()->playing_teams & (1 << team)) != 0;
	}
	return result;
}

// @retail 0x162060
void __stdcall function_162060(void *engine)
{
	long slot_index;

	function_15adb0(function_xaee93d()->netgame_entries);
	function_1579f0();
	function_15c000();
	game_engine_get()->p40();
	if (!game_engine_get() || function_xaee93d()->index24 == NONE)
	{
		function_a77c0();
	}

	for (slot_index = 0; slot_index < 16; slot_index++)
	{
		if (!game_engine_get() || function_xaee93d()->slots[(short)slot_index] == NONE)
		{
			s_simulation_world_view *world = MACRO_0A8D68;
			long identifier = simulation_entity_create(world, 6);

			if (identifier != NONE)
			{
				simulation_entity_activate((c_entry_table *)&world->database->table,
					&function_xaee93d()->slots[(short)slot_index], identifier);
			}
		}
	}
	if (!game_engine_get() || function_xaee93d()->index28 == NONE)
	{
		long identifier = simulation_entity_create(MACRO_0A8D68, 7);

		if (identifier != NONE)
		{
			simulation_entity_activate((c_entry_table *)&MACRO_0A8D68->database->table,
				&function_xaee93d()->index28, identifier);
		}
	}

	if (game_engine_round_in_progress() && ((s_162060_options_view *)g_4e6948)->time_limit)
	{
		long minutes = ((s_162060_options_view *)g_4e6948)->time_limit - function_xaee93d()->value_e0;

		if (minutes < 1)
		{
			minutes = 1;
		}
		((s_162060_globals_view *)function_xaee93d())->round_start_time = g_510c54->game_time - g_510c54->field_2_3 * minutes;
	}
	if (game_engine_round_in_progress())
	{
		s_162060_player_iterator iterator;

		iterator.data = g_4e8c24;
		iterator.index = NONE;
		iterator.datum_index = NONE;
		while (player_iterator_next((long *)&iterator))
		{
			s_162060_player *player = iterator.player;

			if (player->unit_index == NONE)
			{
				player->respawn_ticks = g_510c54->field_2_3 > g_510c54->field_2_3 * player->respawn_seconds ?
					g_510c54->field_2_3 : g_510c54->field_2_3 * player->respawn_seconds;
			}
		}
	}
	if (game_engine_round_in_progress())
	{
		if (engine_has_teams())
		{
			long team;

			for (team = 0; team < 8; team++)
			{
				if (engine_team_is_playing(team) && g_4e6948->score_to_win &&
					function_1587f0(team) >= g_4e6948->score_to_win)
				{
					function_15b3a0(team, 0);
				}
			}
		}
		else
		{
			s_162060_player_iterator iterator;

			iterator.data = g_4e8c24;
			iterator.index = NONE;
			iterator.datum_index = NONE;
			while (player_iterator_next((long *)&iterator))
			{
				long score_to_win = g_4e6948->score_to_win;

				if (score_to_win)
				{
					long score = 0;
					s_statborg *statborg = game_engine_statborg_inline();
					if (statborg)
						score = statborg->players[iterator.datum_index & 0xffff].score;

					if (score >= score_to_win)
					{
						function_15b3a0(iterator.datum_index, 0);
					}
				}
			}
		}
	}
}
