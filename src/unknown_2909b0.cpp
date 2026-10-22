#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
// @flags /O2 /arch:SSE /Gr

struct s_swarm_spawn_slot;
extern s_swarm_spawn_slot *g_5044c4;

struct s_2909b0
{
	byte field_0[0x40];
	real field_40;
	real field_44;
	byte field_48[8];
};

struct s_2909b1
{
	byte field_0[0x14];
	long field_14;
	char field_18;
	byte field_19[0x70 - 0x19];
	vector3f field_70;
	byte field_7c[0x116 - 0x7c];
	short field_116;
};

struct s_2909b2
{
	byte field_0[8];
	s_2909b1 *field_8;
};

void function_b7740(long object_index, vector3f const *linear_velocity, vector3f const *angular_velocity, bool skip_update);
void function_1c4b00(long object_index, void *linear, void *angular, long force);
void __stdcall function_b9b90(long object_index, bool disable);
void function_b7360(long object_index);
void function_bba20(long object_index);

__forceinline void function_290a16(transform4x3f const *arg_0, vector3f const *arg_1, vector3f *arg_2)
{
	real local_0 = arg_1->i;
	real local_1 = arg_1->j;
	real local_2 = arg_1->k;
	arg_2->i = arg_0->up.i * local_2 + arg_0->left.i * local_1 + arg_0->forward.i * local_0;
	arg_2->j = arg_0->up.j * local_2 + arg_0->left.j * local_1 + arg_0->forward.j * local_0;
	arg_2->k = arg_0->up.k * local_2 + arg_0->left.k * local_1 + arg_0->forward.k * local_0;
}

// @retail 0x2909b0
void function_2909b0(short arg_0, long arg_1)
{
	s_2909b0 *local_0 = &((s_2909b0 *)g_5044c4)[arg_0];
	s_2909b2 *local_1 = (s_2909b2 *)g_4e0300->data;
	s_2909b1 *local_2 = local_1[arg_1 & 0xffff].field_8;
	vector3f local_3;
	if (local_2->field_14 == NONE)
		local_3 = local_2->field_70;
	else
	{
		s_2909b1 *local_4 = local_1[local_2->field_14 & 0xffff].field_8;
		transform4x3f *local_5 = (transform4x3f *)((byte *)local_4 + local_4->field_116) + local_2->field_18;
		function_290a16(local_5, &local_2->field_70, &local_3);
	}
	real local_6 = function_259d0(&g_4e7408->unknown0, NULL, 0, local_0->field_40, local_0->field_44);
	local_3.i *= local_6;
	local_3.j *= local_6;
	local_3.k *= local_6;
	function_b7740(arg_1, &local_3, NULL, false);
	function_1c4b00(arg_1, &local_3, NULL, 1);
	real local_7 = local_3.k * local_3.k;
	local_7 += local_3.j * local_3.j;
	local_7 += local_3.i * local_3.i;
	if (local_7 > 0.0001f)
	{
		function_b9b90(arg_1, false);
		function_b7360(arg_1);
		function_bba20(arg_1);
	}
}
