/* CHANNEL_START_OUT_OF_LINE.H: starting an animation channel through
   s_animation_state::channel_start (0x1cb410) as an out-of-line call, for the
   callers where retail calls it instead of expanding it */

#ifndef CHANNEL_START_OUT_OF_LINE_H
#define CHANNEL_START_OUT_OF_LINE_H

#include "unknown_1cafc0.h"

#pragma inline_depth(0)
static __forceinline bool channel_start_out_of_line(s_animation_state *state, c_animation_channel *channel,
	c_type_709360 animation_id, long unknown08, char unknown0c, char unknown0d, char unknown0e,
	dword channel_flags)
{
	return state->channel_start(channel, animation_id, unknown08, unknown0c, unknown0d, unknown0e, channel_flags);
}
#pragma inline_depth(255)

#endif
