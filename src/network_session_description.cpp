// @flags /O2 /Ob1 /Gr
/* NETWORK_SESSION_DESCRIPTION.CPP: checking a session description and
   writing it to and reading it from a bit stream */

#include "unknown_11c920.h"
#include "globals.h"
#include "bitstream.h"
#include "network_session_description.h"
#include <xtl.h>
#include <string.h>

void utf8_string_to_utf16_string(const char *source, word *destination, long destination_count);
void utf16_string_to_utf8_string(const word *source, char *destination, long destination_count);
bool function_7efa0(s_bitstream *stream, s_player_appearance *appearance);
void function_7ee10(s_bitstream *stream, const s_player_appearance *appearance);
void function_1947a0(s_bitstream *stream);

/* whether every field of a description is in its range */
// @retail 0x7b880
bool session_description_valid(const s_session_description *description)
{
	if (!description)
	{
		return false;
	}
	if (description->unlisted < 0 || description->unlisted >= 2)
	{
		return false;
	}
	if (description->protocol < 0 || description->protocol >= 5)
	{
		return false;
	}
	if (description->build != -1 && (description->build <= 0 || description->build > 0xfffe))
	{
		return false;
	}
	/* retail checks build's upper bound again here, not compatible_build's */
	if (description->compatible_build != -1 && (description->compatible_build <= 0 || description->build > 0xfffe))
	{
		return false;
	}
	if (description->type < 0 || description->type >= 3)
	{
		return false;
	}
	if (description->local < 0 || description->local >= 3)
	{
		return false;
	}
	if (description->mode < 0 || description->mode >= 3)
	{
		return false;
	}
	if (description->public_open < 0 || description->public_open > k_session_description_maximum_players)
	{
		return false;
	}
	if (description->private_open < 0 || description->private_open > k_session_description_maximum_players)
	{
		return false;
	}
	if (description->public_used < 0 || description->public_used > k_session_description_maximum_players)
	{
		return false;
	}
	if (description->private_used < 0 || description->private_used > k_session_description_maximum_players)
	{
		return false;
	}
	if (description->variant < 0 || description->variant >= 5)
	{
		return false;
	}
	if (description->state < 0 || description->state >= 10)
	{
		return false;
	}
	if (description->scoring_kind < 0 || description->scoring_kind >= 3)
	{
		return false;
	}
	if (description->score_limit < 0)
	{
		return false;
	}
	if (description->player_count < 0 || description->player_count > k_session_description_maximum_players)
	{
		return false;
	}
	if (description->team_mask & 0xffffff00)
	{
		return false;
	}
	if (description->option != -1 && (description->option < 0 || description->option >= 4))
	{
		return false;
	}

	return true;
}

/* reads a description; false when a field is out of range or the stream ran out */
// @retail 0x7c110
bool __stdcall session_description_decode(s_bitstream *stream, void *session)
{
	s_session_description *description = (s_session_description *)session;
	description->version = (word)function_1959c0(stream, 8);
	description->unlisted = (short)function_1959c0(stream, 2);
	description->protocol = function_1959c0(stream, 3);
	description->build = function_1959c0(stream, 16) - 1;
	description->compatible_build = function_1959c0(stream, 16) - 1;
	description->type = (short)function_1959c0(stream, 2);
	description->local = (short)function_1959c0(stream, 2);
	description->mode = (short)function_1959c0(stream, 2);
	char name[32];
	function_195820(stream, name, 256);
	utf8_string_to_utf16_string(name, (word *)description->name, 32);
	function_195820(stream, &description->session_id, 64);
	function_195820(stream, &description->key, 128);
	function_195820(stream, &description->host_address, 288);
	description->public_open = (short)function_1959c0(stream, 5);
	description->private_open = (short)function_1959c0(stream, 5);
	description->public_used = (short)function_1959c0(stream, 5);
	description->private_used = (short)function_1959c0(stream, 5);
	description->variant = (short)function_1959c0(stream, 3);
	description->state = (short)function_1959c0(stream, 4);
	description->game_type = function_1959c0(stream, 4);
	description->map = function_1959c0(stream, 32);
	description->option = function_1959c0(stream, 3) - 1;
	description->map_variant = function_1959c0(stream, 32);
	description->teams = function_1957d0(stream);
	description->scoring_kind = function_1959c0(stream, 2);
	description->score_limit = function_1959c0(stream, 32);
	description->player_count = (short)function_1959c0(stream, 5);
	bool valid = description->player_count >= 0 && description->player_count <= k_session_description_maximum_players;
	long count = function_1959c0(stream, 5);
	valid = valid && count >= description->player_count && count <= k_session_description_maximum_players;
	if (count < description->player_count) count = description->player_count;
	else if (count > k_session_description_maximum_players) count = k_session_description_maximum_players;
	for (long i = 0; valid && i < count; i++)
	{
		s_session_player_identifier identifier;
		char name_buffer[32];
		s_player_appearance appearance;
		function_195820(stream, &identifier, 96);
		function_195820(stream, name_buffer, 256);
		long score = function_1959c0(stream, 32);
		long team = function_1959c0(stream, 5);
		function_7efa0(stream, &appearance);
		if (i < description->player_count)
		{
			description->player_id_list[i] = identifier;
			utf8_string_to_utf16_string(name_buffer, (word *)description->player_names[i], 32);
			description->player_scores[i] = score;
			description->player_teams[i] = (short)(team - 1);
			description->player_appearances[i] = appearance;
		}
	}
	description->team_mask = function_1959c0(stream, 8);
	dword sent_mask = function_1959c0(stream, 8);
	for (long team = 0; team < k_session_description_maximum_teams; team++)
	{
		if (sent_mask & (1 << team))
		{
			long score = function_1959c0(stream, 32);
			if (description->team_mask & (1 << team))
				description->team_scores[team] = score;
		}
	}
	description->optional_data_present = stream_read_bit(stream);
	if (description->optional_data_present)
		function_195820(stream, description->optional_data, 96);
	else
		memset(description->optional_data, 0, 12);
	if (valid && !stream_overflowed(stream) && session_description_valid(description))
		return true;
	return false;
}

/* writes a description, its players and the scores of the teams in its mask */
// @retail 0x7ba10
void __stdcall session_description_encode(s_bitstream *stream, void *session)
{
	const s_session_description *description = (const s_session_description *)session;
	char name[32];
	utf16_string_to_utf8_string((const word *)description->name, name, 32);
	stream_write_checked(stream, description->version, 8);
	stream_write_checked(stream, description->unlisted, 2);
	stream_write_checked(stream, description->protocol, 3);
	stream_write_checked(stream, description->build + 1, 16);
	stream_write_checked(stream, description->compatible_build + 1, 16);
	stream_write_checked(stream, description->type, 2);
	stream_write_checked(stream, description->local, 2);
	stream_write_checked(stream, description->mode, 2);
	function_1955d0(stream, name, 256);
	function_1955d0(stream, &description->session_id, 64);
	function_1955d0(stream, &description->key, 128);
	function_1955d0(stream, &description->host_address, 288);
	stream_write_checked(stream, description->public_open, 5);
	stream_write_checked(stream, description->private_open, 5);
	stream_write_checked(stream, description->public_used, 5);
	stream_write_checked(stream, description->private_used, 5);
	stream_write_checked(stream, description->variant, 3);
	stream_write_checked(stream, description->state, 4);
	stream_write_checked(stream, description->game_type, 4);
	function_195720(stream, description->map, 32);
	stream_write_checked(stream, description->option + 1, 3);
	function_195720(stream, description->map_variant, 32);
	stream_write_bit(stream, description->teams);
	stream_write_checked(stream, description->scoring_kind, 2);
	function_195720(stream, description->score_limit, 32);
	stream_write_checked(stream, description->player_count, 5);
	long count = description->player_count;
	stream_write_checked(stream, count, 5);
	for (long i = 0; i < count; i++)
	{
		s_session_player_identifier identifier;
		s_player_appearance appearance;
		char name_buffer[32];
		long score = 0;
		short team = NONE;
		if (i < description->player_count)
		{
			identifier = description->player_id_list[i];
			memcpy(name_buffer, description->player_names[i], sizeof(name_buffer));
			utf16_string_to_utf8_string((const word *)description->player_names[i], name_buffer, 32);
			score = description->player_scores[i];
			team = description->player_teams[i];
			appearance = description->player_appearances[i];
		}
		else
		{
			memset(&identifier, 0, sizeof(identifier));
			memset(name_buffer, 0, sizeof(name_buffer));
			memset(&appearance, 0, sizeof(appearance));
		}
		function_1955d0(stream, &identifier, 96);
		function_1955d0(stream, name_buffer, 256);
		function_195720(stream, score, 32);
		stream_write_checked(stream, team + 1, 5);
		function_7ee10(stream, &appearance);
	}
	stream_write_checked(stream, description->team_mask, 8);
	dword sent_mask = description->team_mask;
	stream_write_checked(stream, sent_mask, 8);
	for (long team = 0; team < k_session_description_maximum_teams; team++)
	{
		dword score = 0;
		if (description->team_mask & (1 << team)) score = description->team_scores[team];
		if (sent_mask & (1 << team)) function_195720(stream, score, 32);
	}
	stream_write_bit(stream, description->optional_data_present);
	if (description->optional_data_present) function_1955d0(stream, description->optional_data, 96);
}

/* reads a description from a buffer, such as the data a QoS probe returned */
// @retail 0x7c530
bool session_description_decode_buffer(const byte *data, long size, void *description)
{
	bool result = false;
	if (size > 0 && data)
	{
		s_bitstream stream;
		stream.unknown08 = 1;
		stream_set_buffer(&stream, data, size);
		function_1947a0(&stream);
		result = session_description_decode(&stream, description);
		if (result && (stream.bit_position > (stream.size_in_bytes << 3) || stream.error))
			result = false;
	}
	return result;
}
