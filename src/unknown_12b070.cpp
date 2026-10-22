// @flags /O2 /arch:SSE /Gr
#include "counters_storage.h"
#include "device_setup_storage.h"
/* UNKNOWN_12B070.CPP: the main loop's timing: the time it started, and the
   lengths of the last 15 vertical blanks reported by the rasterizer */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0820f0.h"
#include <xtl.h>
#include <string.h>

long __fastcall function_14280(void const *data);

extern s_connection_counter g_4e6398;
extern byte g_4e6388;
extern byte g_4e6389;

/* the tick count, carried past its wrap to 64 bits */
union u_tick_count
{
	__int64 value;
	struct
	{
		dword low;
		dword high;
	} parts;
};

u_tick_count g_4e6390;
/* the lengths of the last vertical blanks, a ring of 15 */
struct s_vblank_history
{
	short next;
	short counts[15];
};

s_vblank_history g_4e6400;

// @retail 0x12b2a0
void __cdecl main_vblank_callback(D3DVBLANKDATA *data)
{
	long count = function_14280(data);

	if (count > 0)
	{
		g_4e6400.counts[g_4e6400.next] = (short)count;
		g_4e6400.next = (g_4e6400.next + 1) % 15;
	}
}

// @retail 0x12b070
void function_12b070(void)
{
	g_4e6390.parts.low = GetTickCount();
	g_4e6390.parts.high = 0;
	g_4e6398.low = 0;
	g_4e6398.high = 0;
	D3DDevice_SetVerticalBlankCallback(main_vblank_callback);
	g_4e6400.next = 0;
	memset(g_4e6400.counts, 0, sizeof(g_4e6400.counts));
}

/* the vertical blank count (unknown_123b30.cpp) and the one of the last frame */

#define VBLANK_COUNT (*(__int64 volatile *)&g_485ab0)
#define LAST_FRAME_VBLANK_COUNT (*(__int64 *)&g_4e6398)

__int64 g_4e63a8;
__int64 g_4e63b0;
short g_4e63b8;
bool g_4e63bc;

/* waits for the vertical blank before the one the last frame asked for, and
   notes whether a frame is due */
// @retail 0x12b2e0
void main_time_wait_for_vblank(void)
{
	__int64 elapsed;
	short frames;

	g_4e63a8 = VBLANK_COUNT + 1;
	if (VBLANK_COUNT < LAST_FRAME_VBLANK_COUNT - 1)
	{
		while (VBLANK_COUNT < LAST_FRAME_VBLANK_COUNT - 1)
		{
		}
	}
	g_4e63b0 = VBLANK_COUNT + 1;
	elapsed = g_4e63b0 - LAST_FRAME_VBLANK_COUNT;
	frames = (short)(elapsed < 0 ? 0 : (elapsed > 0x7fff ? 0x7fff : elapsed));
	if (g_4e63b8 > 0 && !frames)
	{
		g_4e63bc = true;
	}
	else
	{
		g_4e63bc = false;
	}
}

// @retail 0x12b3c0
bool function_12b3c0(void)
{
	bool result = true;

	if (!g_485aca)
	{
		result = false;
	}
	else
	{
		if (!g_4e6948)
		{
			return false;
		}
		short local_0 = g_4e6948->flag1120;
		if (!local_0)
		{
			return false;
		}
		if (g_4e6388)
		{
			result = *(bool *)&g_4e6389;
		}
	}
	return result;
}

short g_485ac0;
short g_4e63ba;
long g_4e638c;
__int64 g_4e63a0;

bool window_manager_main_menu_focused(void);

/* the frame's timing: the vertical blank it ends on (no earlier than the
   one the last frame asked for, plus the interval the game asks for), the
   tick count, and the frame's length in seconds (at most 10) */
// @retail 0x12b0e0
real function_12b0e0(void)
{
	real local_0;
	dword now = GetTickCount();
	__int64 target = g_4e63b0;
	__int64 next;
	__int64 ticks;
	long game_time;
	long rate;
	real elapsed;

	if (LAST_FRAME_VBLANK_COUNT > target)
		target = LAST_FRAME_VBLANK_COUNT;
	if (g_4e6948 && g_4e6948->flag1120)
		game_time = g_510c54->game_time;
	else
		game_time = 0;
	next = target;
	if (function_12b3c0())
	{
		g_4e63ba = g_485aca;
		if (g_4e6948->flag1120 && (g_4e6948->state != 3 || window_manager_main_menu_focused()))
			g_4e63ba = g_4e63ba > 2 ? g_4e63ba : 2;
		g_4e63b8 = g_4e63ba;
		next = target + g_4e63b8;
	}
	now = GetTickCount();
	ticks = (g_4e6390.value & 0xffffffff00000000) | now;
	if (ticks < g_4e6390.value)
		ticks += 0x100000000;
	if (VBLANK_COUNT > next)
		next = VBLANK_COUNT;
	rate = g_485ac0;
	if (rate <= 0)
		rate = 60;
	real frequency = rate;
	elapsed = (real)(next - LAST_FRAME_VBLANK_COUNT) / frequency;
	local_0 = 0.0f > elapsed ? 0.0f : (elapsed > 10.0f ? 10.0f : elapsed);
	LAST_FRAME_VBLANK_COUNT = next;
	g_4e6390.parts.low = now;
	g_4e6390.parts.high = (dword)(ticks >> 32);
	g_4e638c = game_time;
	g_4e63a0 = VBLANK_COUNT;
	return local_0;
}
