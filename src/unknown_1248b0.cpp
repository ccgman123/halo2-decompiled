// @flags /O2 /Ob1 /Gr
/* UNKNOWN_1248B0.CPP: gamepads, rumble and memory units through XInput */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1248b0.h"
#include <xtl.h>
#include <string.h>

/* Bungie's macros, as in Halo CE's cseries.h */
#ifndef MIN
#define MIN(a,b) ((a)>(b)?(b):(a))
#endif
#ifndef CEILING
#define CEILING(n,ceiling) ((n)>(ceiling)?(ceiling):(n))
#endif
#ifndef PIN
#define PIN(n,floor,ceiling) ((n)<(floor) ? (floor) : CEILING((n),(ceiling)))
#endif

enum
{
	k_maximum_gamepads = 4,

	k_maximum_update_milliseconds = 100,
	k_thumbstick_dead_zone = 9000,
	k_analog_button_release_margin = 0x20,
	k_analog_button_press_margin = 0x40
};

struct s_gamepad_rumble
{
	word left;
	word right;
};

struct s_input_globals
{
	bool initialized;
	bool suppressed;
	bool feedback_suppressed;
	byte unknown03;
	dword update_time;
	word memory_units;
	char memory_unit_drive_letters[8];
	byte unknown12[2];
	HANDLE gamepads[k_maximum_gamepads];
	s_type_ff3a2a gamepad_states[k_maximum_gamepads];
	long gamepad_types[k_maximum_gamepads];
	s_type_ff3a2a suppressed_gamepad_state;
	s_gamepad_rumble rumble[k_maximum_gamepads];
	dword memory_unit_change_time;
};

XINPUT_FEEDBACK g_4e60a0[k_maximum_gamepads];
s_input_globals g_4e61b8;

/* which byte of XINPUT_GAMEPAD::bAnalogButtons each analog button reads, and
   the XINPUT_GAMEPAD::wButtons bit of each digital button */
byte const g_440c38[k_gamepad_analog_button_count] = { 0, 1, 2, 3, 4, 5, 6, 7 };
byte const g_440c40[k_gamepad_button_count] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

void function_2184a0(dword device_changes);

void function_124950(void);

// @retail 0x1248b0
bool function_1248b0(void)
{
	XDEVICE_PREALLOC_TYPE device_types[] =
	{
		{ XDEVICE_TYPE_GAMEPAD, k_maximum_gamepads },
		{ XDEVICE_TYPE_VOICE_MICROPHONE, k_maximum_gamepads },
		{ XDEVICE_TYPE_VOICE_HEADPHONE, k_maximum_gamepads }
	};

	XInitDevices(sizeof(device_types) / sizeof(device_types[0]), device_types);
	memset(&g_4e61b8, 0, sizeof(g_4e61b8));
	g_4e61b8.memory_units |= 1;
	g_4e61b8.initialized = true;
	function_124950();

	return g_4e61b8.initialized;
}

// @retail 0x124920
void function_124920(void)
{
	for (short i = 0; i < k_maximum_gamepads; i++)
	{
		if (g_4e61b8.gamepads[i])
		{
			XInputClose(g_4e61b8.gamepads[i]);
			g_4e61b8.gamepads[i] = NULL;
		}
	}
}

// @retail 0x124aa0
void input_update_device_changes(void)
{
	dword device_changes = 0;
	dword insertions;
	dword removals;
	dword memory_unit_insertions;
	dword memory_unit_removals;

	if (XGetDeviceChanges(XDEVICE_TYPE_GAMEPAD, &insertions, &removals))
	{
		for (short i = 0; i < k_maximum_gamepads; i++)
		{
			dword mask = 1 << i;

			if ((removals & mask) && g_4e61b8.gamepads[i])
			{
				XInputClose(g_4e61b8.gamepads[i]);
				g_4e61b8.gamepads[i] = NULL;
			}
			if (insertions & mask)
			{
				g_4e61b8.gamepad_types[i] = 0;
				g_4e61b8.gamepads[i] = XInputOpen(XDEVICE_TYPE_GAMEPAD, i, XDEVICE_NO_SLOT, NULL);
				if (g_4e61b8.gamepads[i])
				{
					XINPUT_CAPABILITIES capabilities;

					if (XInputGetCapabilities(g_4e61b8.gamepads[i], &capabilities) == ERROR_SUCCESS)
					{
						if (capabilities.SubType == XINPUT_DEVSUBTYPE_GC_GAMEPAD)
							g_4e61b8.gamepad_types[i] = 1;
						else if (capabilities.SubType == XINPUT_DEVSUBTYPE_GC_GAMEPAD_ALT)
							g_4e61b8.gamepad_types[i] = 2;
					}
				}
			}
		}

		if (removals & 1)
			device_changes = 0x1;
		if (removals & 2)
			device_changes |= 0x2;
		if (removals & 4)
			device_changes |= 0x4;
		if (removals & 8)
			device_changes |= 0x8;
		if (insertions & 1)
			device_changes |= 0x1000;
		if (insertions & 2)
			device_changes |= 0x2000;
		if (insertions & 4)
			device_changes |= 0x4000;
		if (insertions & 8)
			device_changes |= 0x8000;
	}

	if (XGetDeviceChanges(XDEVICE_TYPE_MEMORY_UNIT, &memory_unit_insertions, &memory_unit_removals))
		g_4e61b8.memory_unit_change_time = GetTickCount();

	function_2184a0(device_changes);
}

PRIVATE inline short input_thumbstick_dead_zone(short value)
{
	short result = 0;

	if (value > k_thumbstick_dead_zone)
		result = (short)((value - k_thumbstick_dead_zone) * 32767 / (32767 - k_thumbstick_dead_zone));
	if (value < -k_thumbstick_dead_zone)
		result = (short)((value + k_thumbstick_dead_zone) * -32768 / (-32768 + k_thumbstick_dead_zone));

	return result;
}

// @retail 0x124c00
void function_124c00(long milliseconds)
{
	for (short i = 0; i < k_maximum_gamepads; i++)
	{
		XINPUT_STATE state;

		if (!g_4e61b8.gamepads[i] || !SUCCEEDED(XInputGetState(g_4e61b8.gamepads[i], &state)))
			continue;

		s_type_ff3a2a *gamepad = &g_4e61b8.gamepad_states[i];
		short j;

		for (j = 0; j < k_gamepad_analog_button_count; j++)
		{
			bool down = gamepad->analog_buttons[j] > gamepad->analog_button_thresholds[j];
			byte threshold;

			gamepad->analog_buttons[j] = state.Gamepad.bAnalogButtons[g_440c38[j]];
			gamepad->analog_button_frames_down[j] = down ? MIN(gamepad->analog_button_frames_down[j] + 1, 0xff) : 0;
			gamepad->analog_button_msec_down[j] = down ? MIN(gamepad->analog_button_msec_down[j] + milliseconds, 0xffff) : 0;

			byte value = gamepad->analog_buttons[j];
			if (down)
			{
				threshold = value < k_analog_button_release_margin ? 0 :
					value - k_analog_button_release_margin;
				if (threshold > gamepad->analog_button_thresholds[j])
					gamepad->analog_button_thresholds[j] = threshold;
			}
			else
			{
				threshold = value > 0xff - k_analog_button_press_margin ? 0xff :
					value + k_analog_button_press_margin;
				if (threshold < gamepad->analog_button_thresholds[j])
					gamepad->analog_button_thresholds[j] = threshold;
			}
		}

		for (j = 0; j < k_gamepad_button_count; j++)
		{
			bool down = (word)(g_440c40[j] & state.Gamepad.wButtons) > 0;

			gamepad->button_frames_down[j] = down ? MIN(gamepad->button_frames_down[j] + 1, 0xff) : 0;
			gamepad->button_msec_down[j] = down ? MIN(gamepad->button_msec_down[j] + milliseconds, 0xffff) : 0;
		}

		gamepad->thumbsticks[0] = input_thumbstick_dead_zone(state.Gamepad.sThumbLX);
		gamepad->thumbsticks[1] = input_thumbstick_dead_zone(state.Gamepad.sThumbLY);
		gamepad->thumbsticks[2] = input_thumbstick_dead_zone(state.Gamepad.sThumbRX);
		gamepad->thumbsticks[3] = input_thumbstick_dead_zone(state.Gamepad.sThumbRY);
	}
}

// @retail 0x124ee0
void function_124ee0(void)
{
	bool stopped = g_4e61b8.feedback_suppressed || g_4e61b8.suppressed;

	if (!g_4e6948 || !g_4e6948->flag1120 || g_510c54->active && g_510c54->unknown01)
		stopped = true;

	XINPUT_FEEDBACK *feedback = g_4e60a0;

	for (short i = 0; i < k_maximum_gamepads; i++, feedback++)
	{
		if (!g_4e61b8.gamepads[i])
			continue;

		if (feedback->Header.dwStatus == ERROR_SUCCESS)
		{
			feedback->Rumble.wLeftMotorSpeed = stopped ? 0 : g_4e61b8.rumble[i].left;
			feedback->Rumble.wRightMotorSpeed = stopped ? 0 : g_4e61b8.rumble[i].right;
			XInputSetState(g_4e61b8.gamepads[i], feedback);
		}
		else if (feedback->Header.dwStatus != ERROR_IO_PENDING)
		{
			feedback->Header.dwStatus = ERROR_SUCCESS;
		}
	}
}

// @retail 0x124950
void function_124950(void)
{
	if (!g_4e61b8.initialized)
		return;

	dword time = GetTickCount();
	long elapsed = time - g_4e61b8.update_time;
	long milliseconds = PIN(elapsed, 0, k_maximum_update_milliseconds);

	g_4e61b8.suppressed = false;
	g_4e61b8.update_time = time;
	input_update_device_changes();
	function_124c00(milliseconds);
	function_124ee0();
}

// @retail 0x1249a0
s_type_ff3a2a const *function_1249a0(short gamepad_index)
{
	s_type_ff3a2a const *result = NULL;

	if (g_4e61b8.gamepads[gamepad_index])
	{
		if (g_4e61b8.suppressed)
			result = &g_4e61b8.suppressed_gamepad_state;
		else
			result = &g_4e61b8.gamepad_states[gamepad_index];
	}
	return result;
}

/* 1 for a standard gamepad, 2 for the alternate one, 0 for none */
// @retail 0x1249d0
long function_1249d0(short gamepad_index)
{
	if (g_4e61b8.gamepads[gamepad_index])
		return g_4e61b8.gamepad_types[gamepad_index];
	return 0;
}

// @retail 0x1249f0
bool function_1249f0(long memory_unit, char *drive_letter)
{
	dword mask = 1 << memory_unit;

	if (g_4e61b8.memory_units & mask)
	{
		if (memory_unit)
			*drive_letter = g_4e61b8.memory_unit_drive_letters[memory_unit - 1];
		else
			*drive_letter = 'u';
	}
	else
	{
		*drive_letter = 0;
	}
	return (g_4e61b8.memory_units & mask) ? true : false;
}

// @retail 0x124a40
void function_124a40(short gamepad_index, word left, word right)
{
	bool enabled = true;

	if (TEST_FIELD_BIT(g_54e8e0[gamepad_index].flag4))
		enabled = !TEST_FIELD_BIT(g_54e8e0[gamepad_index].settings.vibration_disabled);

	if (enabled)
	{
		g_4e61b8.rumble[gamepad_index].left = left;
		g_4e61b8.rumble[gamepad_index].right = right;
	}
	else
	{
		g_4e61b8.rumble[gamepad_index].left = 0;
		g_4e61b8.rumble[gamepad_index].right = 0;
	}
}
