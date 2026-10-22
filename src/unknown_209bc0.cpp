// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_209BC0.CPP: the script globals' runtime values: refreshing an
   external global's value and reading a global (outside functions of lane I;
   retail calls them from the thread code, never inlines them) */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"
#include "hs.h"
#include "unknown_1dee50.h"

/* a global's runtime value (g_4f9380, 8 bytes) */
struct s_hs_global_value
{
	byte unknown0[4];
	script_value value;
};

/* an external global's definition (g_473468 points at one per global) */
struct s_hs_external_global
{
	short type;
	byte unknown2[2];
	void *address;
};

extern s_record_pool *g_4f9380;
extern s_hs_external_global *g_473468[1];
extern char const *g_470010;

void function_20a2e0(short global_index);

// @retail 0x209bc0
long function_209bc0(short global_index)
{
	function_20a2e0(global_index);

	long index = global_index;

	return ((s_hs_global_value *)g_4f9380->data)[(index & 0x8000) ? (index & 0x7fff) : (index & 0x7fff) + 0x41d].value.d;
}

// @retail 0x20a2e0
void function_20a2e0(short global_index)
{
	if (global_index & 0x8000)
	{
		long index = global_index;
		long slot = index & 0x7fff;

		if (!(index & 0x8000))
		{
			slot += 0x41d;
		}

		s_hs_global_value *value = &((s_hs_global_value *)g_4f9380->data)[slot];
		s_hs_external_global *global = g_473468[global_index & 0x7fff];

		switch (global->type)
		{
		case _hs_type_boolean:
			value->value.b = global->address ? *(byte *)global->address : (byte)0;
			goto done;
		case _hs_type_real:
			value->value.r = global->address ? *(real *)global->address : 0.f;
			goto done;
		case _hs_type_short_integer:
			value->value.s = global->address ? *(short *)global->address : (short)0;
			goto done;
		case _hs_type_long_integer:
			value->value.d = global->address ? *(dword *)global->address : 0;
			goto done;
		case _hs_type_string:
			value->value.d = global->address ? *(dword *)global->address : (dword)g_470010;
			goto done;
		case _hs_type_script:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_string_id:
			value->value.d = global->address ? *(dword *)global->address : 0;
			goto done;
		case _hs_type_unit_seat_mapping:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_trigger_volume:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_cutscene_flag:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_cutscene_camera_point:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_cutscene_title:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_cutscene_recording:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_device_group:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_ai:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_ai_command_list:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_ai_command_script:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_ai_behavior:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_ai_orders:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_starting_profile:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_conversation:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_structure_bsp:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_navpoint:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_point_reference:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_style:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_hud_message:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_object_list:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_sound:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_effect:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_damage:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_looping_sound:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_animation_graph:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_damage_effect:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_object_definition:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_bitmap:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_shader:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_render_model:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_structure_definition:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_lightmap_definition:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_game_difficulty:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_team:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_actor_type:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_hud_corner:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_model_state:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_network_event:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		case _hs_type_object:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_unit:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_vehicle:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_weapon:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_device:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_scenery:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			goto done;
		case _hs_type_object_name:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			goto done;
		}
done:
		;
	}
}

// @retail 0x20a490
// score 48 (was 48): the index is kept in ESI where retail keeps it in EAX, so the two halves of the
// split value and the jump table's index take the other registers.
void function_20a490(short global_index)
{
	long slot = (word)global_index & 0x7fff;
	word external = global_index & 0x8000;
	long index = global_index;
	short type = external ? g_473468[slot]->type : *(short *)(*(byte **)((byte *)g_4e0350 + 0x1c4) + slot * 0x28 + 0x20);
	long runtime_index = (index & 0x8000) ? (index & 0x7fff) : (index & 0x7fff) + 0x41d;
	s_hs_global_value *value = &((s_hs_global_value *)g_4f9380->data)[runtime_index];
	if (external)
	{
		s_hs_external_global *global = g_473468[slot];
		switch (type)
		{
		case 5:
			if (global->address)
				*(byte *)global->address = value->value.b;
			break;
		case 6:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 7:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 8:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 9:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 10:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 11:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 12:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 13:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 14:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 15:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 16:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 17:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 18:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 19:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 20:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 21:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 22:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 23:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 24:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 25:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 26:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 27:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 28:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 29:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 30:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 31:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 32:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 33:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 34:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 35:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 36:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 37:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 38:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 39:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 40:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 41:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 42:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 43:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 44:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 45:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 46:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 47:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 48:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 49:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;
		case 50:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 51:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 52:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 53:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 54:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 55:
			if (global->address)
				*(dword *)global->address = value->value.d;
			break;
		case 56:
			if (global->address)
				*(word *)global->address = value->value.w;
			break;

		}
	}
	if (type >= 50 && type <= 55)
	{
		long object_index = value->value.d;
		if (object_index != NONE)
		{
			dword *flags = (dword *)(*(byte **)(g_4e0300->data + (object_index & 0xffff) * 12 + 8) + 4);
			*flags |= 0x10000000;
		}
	}
	else if (type == _hs_type_object_list)
	{
		long reference_index;
		for (long object_index = object_list_get_first_inlined(value->value.d, &reference_index); object_index != NONE; object_index = function_x457076(&reference_index))
		{
			dword *flags = (dword *)(*(byte **)(g_4e0300->data + (object_index & 0xffff) * 12 + 8) + 4);
			*flags |= 0x10000000;
		}
	}
}
