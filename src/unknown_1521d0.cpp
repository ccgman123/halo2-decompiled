// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"

struct s_timer_player
{
    byte unknown00[0x2c];
    long unit_index;
    byte unknown30[0x198 - 0x30];
    short effect_timers[2];
    byte unknown19c[0x21c - 0x19c];
};

void function_d0e00(long unit_index, real rate);

static inline s_timer_player *timer_player_get(long player_index)
{
	return (s_timer_player *)(g_4e8c24->data + (player_index & 0xffff) * sizeof(s_timer_player));
}

// score 9 (was 46): retail forms the player's address with add eax, edi and
// then lea esi, [eax + 0x198]; ours uses one lea. The prologue order and the
// registers of the reread of g_4e8c24 before the call also differ. Counting
// the loop down brought that reread back. Other tested forms scored 9 to 15,
// an indexed do/while 35; the 600 s permutation run from here found no
// improvement. Needs 0xd0e00 to match: without it this is 194 bytes, score 147.
// @retail 0x1521d0
void function_1521d0(long player_index)
{
	s_timer_player *player = timer_player_get(player_index);
	short *timer = player->effect_timers;
	long effect = 0;

	for (long count = 2; count != 0; count--, effect++, timer++)
	{
		if (*timer > 0)
		{
			(*timer)--;
			if (*timer == 0 && effect == 0)
			{
				function_d0e00(timer_player_get(player_index)->unit_index, 1.0f);
			}
		}
	}
}
