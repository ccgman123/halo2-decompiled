// @flags /O2 /Ob1 /Gr
#include "unknown_11c920.h"
#include "unknown_058dd0.h"
#include "unknown_0662e0.h"
#include "globals.h"
#include <xtl.h>
#include <string.h>

struct s_surface_description;
struct s_matchmaking_ratings;
struct s_match_player_list;
struct s_session_property_value
{
	long kind;
	long value04;
	long value08;
	long value0c;
	long value10;
	long value14;
	long value18;
};
struct s_71730
{
	long field_0[3];
	long field_c;
	long field_10;
	byte field_14[0xc0];
	long field_d4[16];
	long field_114[16];
	long field_154[16];
	long field_194[16];
	byte field_1d4[0x358 - 0x1d4];
	long field_358;
	dword field_35c;
	dword field_360;
};

void function_71e00(c_session_state *arg_0);
s_surface_description *function_192e60(long arg_0);
long function_193400(s_surface_description *arg_0);
long function_1931d0(s_surface_description *arg_0, long arg_1);
bool function_1934b0(s_surface_description *arg_0);
long function_1933a0(s_surface_description *arg_0);
long function_1933d0(s_surface_description *arg_0);
bool function_1934d0(s_surface_description *arg_0);
long function_193370(s_surface_description *arg_0);
long function_193440(s_surface_description *arg_0);
bool function_193470(s_surface_description *arg_0);
bool function_7e210(c_class_58d20 *arg_0, s_matchmaking_ratings *arg_1);
bool network_session_players_match(c_class_58d20 *arg_0, c_class_58d20 *arg_1);
long network_session_get_maximum_players(c_class_58d20 *arg_0);
long network_session_find_player(c_class_58d20 *arg_0, const dword *arg_1);
bool network_session_members_ready(c_class_58d20 *arg_0, dword *arg_1);
bool function_73ae0(unsigned __int64 *arg_0);
bool network_session_host_set_id49f0(c_class_58d20 *arg_0, const s_session_id *arg_1);
bool network_session_host_set_data49cc(c_class_58d20 *arg_0, const dword *arg_1);
void session_property_value_set_kind2(s_session_property_value *arg_0, long arg_1);
void session_property_value_set_kind3(s_session_property_value *arg_0, long arg_1);
bool function_72140(c_session_state_matchmaking *arg_0);
bool balance_teams_by_count(long arg_0, long const *arg_1, long arg_2, long arg_3,
	bool arg_4, long arg_5, long arg_6, bool arg_7, long *arg_8);
bool balance_teams(long arg_0, long arg_1, bool arg_2, long arg_3, long arg_4,
	bool arg_5, long arg_6, long const *arg_7, long const *arg_8, bool arg_9, long *arg_10);
void function_71ff0(c_session_state_matchmaking *arg_0, long arg_1,
	const s_match_player_list *arg_2, const long *arg_3);

// @retail 0x71730
bool __stdcall function_71730(c_session_state_matchmaking *arg_0)
{
	bool local_3 = false;
	{
		s_session_owner *local_0 = arg_0->owner;
		c_class_58d20 *local_1 = local_0->session_a;
		c_class_58d20 *local_2 = local_0->session_b;
		if (local_1->type != 11)
			goto finished;
		long local_4 = local_1->state;
		if (local_4 != 5 && local_4 != 6 && local_4 != 7 && local_4 != 8)
		{
			function_71e00(arg_0);
			goto finished;
		}
		local_4 = local_2->state;
		if (local_4 != 5 && local_4 != 6 && local_4 != 7 && local_4 != 8)
		{
			arg_0->mode = 14;
			local_3 = true;
			goto finished;
		}
		long local_7 = NONE;
		if (local_2->state > 2 && local_2->state <= 8)
			local_7 = local_2->value49c8;
		s_surface_description *local_8 = function_192e60(local_7);
		byte *local_9 = local_2->get_data_4a00();
		long local_10 = (long)(g_510548 ? g_51054c : GetTickCount()) - arg_0->unknowna68;
		byte *local_11 = *(byte **)((byte *)local_0 + 0x3c);
		long local_12 = false;
		bool local_13 = false;
		long local_15[16];
		s_71730 local_16;
		if ((dword)local_2->player_count > *(dword *)((byte *)arg_0 + 0xa6c))
		{
			*(dword *)((byte *)arg_0 + 0xa6c) = local_2->player_count;
			*(long *)((byte *)arg_0 + 0xa70) = g_510548 ? g_51054c : GetTickCount();
		}
		long local_17 = (long)(g_510548 ? g_51054c : GetTickCount()) - *(long *)((byte *)arg_0 + 0xa70);
		s_session_property_value local_14 = {0};
		if (!local_8 || !local_9)
		{
			arg_0->mode = 7;
			local_3 = true;
			goto local_28;
		}
		if (!function_7e210(local_2, (s_matchmaking_ratings *)&local_16))
			goto local_28;
		if (local_16.field_358 > function_193400(local_8) &&
			*(long *)(local_9 + 0x184) == network_session_get_maximum_players(local_2))
			goto local_28;
		if (*(long *)(local_11 + 8) == 0 &&
			(local_10 > g_network_configuration.value1e4 * 1000 ||
			 *(long *)(local_11 + 0x10) > g_network_configuration.value1e0))
			*(long *)(local_11 + 8) = 2;
		if (!network_session_players_match(local_1, local_2) || local_16.field_360 ||
			local_2->player_count != *(long *)(local_9 + 0x184))
			goto local_29;
		{
			long local_18 = *(long *)(local_9 + 0x184);
			long local_19 = 0;
			for (long local_20 = 0; local_20 < local_18; local_20++)
				if (network_session_find_player(local_2, (const dword *)(local_9 + 0x188 + local_20 * 12)) != NONE)
					local_19++;
			if (local_19 != local_18)
				goto local_29;
			long local_21 = function_1931d0(local_8, local_2->player_count);
			if (local_21 == 0x7fffffff || local_21 > local_17)
			{
				session_property_value_set_kind3(&local_14, (local_21 - local_17) / 1000);
				goto local_29;
			}
			if (function_1934b0(local_8) && local_16.field_358 > function_193400(local_8))
				goto local_29;
			dword local_22;
			if (!network_session_members_ready(local_2, &local_22))
			{
				memset(&local_14, 0, sizeof(local_14));
				local_14.kind = 4;
				goto local_29;
			}
			if (!(local_2->state > 2 && local_2->state <= 8 && local_2->flag49e8))
			{
				s_session_id local_23;
				function_73ae0((unsigned __int64 *)&local_23);
				network_session_host_set_id49f0(local_2, &local_23);
			}
			if (*(long *)(local_11 + 8) != 1)
				*(long *)(local_11 + 8) = 1;
			local_13 = function_72140(arg_0);
			if (!local_13)
				goto local_30;
			if (function_1934b0(local_8))
			{
				long local_24;
				long local_25 = false;
				if (balance_teams_by_count(local_16.field_c, local_16.field_194,
					function_193440(local_8), function_193370(local_8), function_1934d0(local_8),
					function_1933d0(local_8), function_1933a0(local_8), true, &local_24) &&
					local_24 <= function_193400(local_8))
					local_25 = balance_teams(function_193440(local_8), function_193370(local_8),
						function_1934d0(local_8), function_1933d0(local_8), function_1933a0(local_8),
						true, local_16.field_c, local_16.field_194, local_16.field_d4, true, local_15);
				if (!local_25 && !function_193470(local_8))
				{
					if (balance_teams_by_count(local_16.field_c, local_16.field_194,
						function_193440(local_8), function_193370(local_8), function_1934d0(local_8),
						function_1933d0(local_8), function_1933a0(local_8), false, &local_24) &&
						local_24 <= function_193400(local_8))
						local_25 = balance_teams(function_193440(local_8), function_193370(local_8),
							function_1934d0(local_8), function_1933d0(local_8), function_1933a0(local_8),
							false, local_16.field_c, local_16.field_194, local_16.field_d4, true, local_15);
				}
				if (!local_25)
				{
					local_13 = false;
					local_12 = true;
					goto local_30;
				}
			}
			else
			{
				for (long local_26 = 0; local_26 < local_16.field_c; local_26++)
					local_15[local_26] = local_26;
			}
			function_71ff0(arg_0, local_7, (const s_match_player_list *)((byte *)&local_16 + 4), local_15);
			goto local_31;
		}
	local_29:
		if (local_2->state > 2 && local_2->state <= 8 && local_2->flag49e8)
			local_12 = true;
		goto local_30;
	local_28:
		if (arg_0->mode == 3)
			local_12 = true;
	local_30:
		{
			long local_27 = *(long *)((byte *)local_8 + 0x59c) + *(long *)((byte *)arg_0 + 0x974);
			if (local_27 <= local_17)
				local_12 = true;
			else
			{
				if (!local_14.kind)
					session_property_value_set_kind2(&local_14, (local_27 - local_17) / 1000);
				network_session_host_set_data49cc(local_2, (const dword *)&local_14);
			}
		}
		if (local_13)
		{
	local_31:
			network_session_set_mode(local_1, 12);
			local_3 = true;
		}
		if (local_12)
		{
			if (*(long *)(local_11 + 8) != 1)
				*(long *)(local_11 + 8) = 1;
			network_session_set_mode(local_1, 6);
			local_3 = true;
		}
	}
finished:
	return local_3;
}
