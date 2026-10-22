// @flags /O2 /Gr /arch:SSE
#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"
#include "unknown_11cc90.h"
#include <math.h>

struct s_29ab90
{
	byte field_0[0x1c];
	real field_1c;
	byte field_20[0x2c];
	real field_4c;
	real field_50;
	byte field_54[0x10];
	real field_64;
	real field_68;
	real field_6c;
};
long function_1f5f30(long arg_0);
bool function_1f9760(long arg_0, long arg_1, point3f const *arg_2, real arg_3,
	bool *arg_4, bool *arg_5, vector3f *arg_6);
real function_1f9e70(long arg_0, long arg_1, point2f const *arg_2,
	vector2f const *arg_3, vector2f const *arg_4, vector2f const *arg_5,
	vector2f const *arg_6, real arg_7, bool arg_8);
point3f *function_b9dd0(long arg_0, point3f *arg_1);
real function_30bf0(vector3f *arg_0);
bool __stdcall function_2922b0(long arg_0, long arg_1, vector3f *arg_2,
	vector3f *arg_3, real *arg_4, real *arg_5, bool arg_6);
void function_29b2e0(long arg_0, long arg_1, vector3f const *arg_2,
	vector3f const *arg_3, real arg_4, vector3f *arg_5);

PRIVATE __forceinline real function_29ab91(vector3f const *arg_0)
{
	real local_0 = (real)sqrt((double)arg_0->k * arg_0->k + (double)arg_0->j * arg_0->j + (double)arg_0->i * arg_0->i);
	return 0.0001f > fabs((double)local_0) ? 0.0f : local_0;
}
PRIVATE __forceinline real function_29ab92(vector2f *arg_0)
{
	real local_0 = (real)sqrt((double)arg_0->j * arg_0->j + (double)arg_0->i * arg_0->i);
	if (!(0.0001f > fabs((double)local_0)))
	{
		real local_1 = 1.0f / local_0;
		arg_0->i *= local_1;
		arg_0->j *= local_1;
	}
	else
		local_0 = 0.0f;
	return local_0;
}
PRIVATE __forceinline real function_29ab93(real arg_0, real arg_1, real arg_2)
{
	return arg_1 > arg_0 ? arg_1 : arg_0 > arg_2 ? arg_2 : arg_0;
}
PRIVATE __forceinline void function_29ab94(vector3f const *arg_0, real arg_1, vector3f *arg_2)
{
	arg_2->i = arg_0->i * arg_1;
	arg_2->j = arg_0->j * arg_1;
	arg_2->k = arg_0->k * arg_1;
}

// @retail 0x29ab90
void __stdcall function_29ab90(long arg_0, long arg_1, vector3f *arg_2,
	vector3f *arg_3, short *arg_4, vector3f *arg_5, bool *arg_6, bool *arg_7, short arg_8)
{
	byte *local_0 = (byte *)actor_get(arg_0);
	s_slot_object_view *local_1 = object_get(arg_1);
	s_29ab90 const *local_2 = (s_29ab90 const *)function_1e5450(arg_0, local_1->tag_index);
	*(bool *)(local_0 + 0x6d1) = false;
	if (!arg_8)
	{
		*(real *)(local_0 + 0x658) = *(real *)(local_0 + 0x65c);
		*(real *)(local_0 + 0x660) = *(real *)(local_0 + 0x664);
		*(vector3f *)(local_0 + 0x678) = *(vector3f *)(local_0 + 0x66c);
	}
	if (*(long *)(local_0 + 0x26c) == NONE)
		*(long *)(local_0 + 0x7fc) = function_1f5f30(arg_0);
	vector3f const *local_3 = (vector3f const *)(local_0 + 0x290);
	vector3f local_4 = *local_3;
	vector3f local_5 = *g_4687a4;
	vector3f local_6 = *g_4687a4;
	point3f local_7;
	function_b9dd0(arg_1, &local_7);
	real local_9 = function_29ab91(arg_2);
	if (local_2)
	{
		if (!*(bool *)(local_0 + 0x5d0) || *arg_7)
			function_29ab94(local_3, 2.0f, arg_2);
		bool local_10;
		bool local_11;
		vector3f local_12;
		if (function_1f9760(arg_0, arg_1, &local_7, local_9, &local_10, &local_11, &local_12))
		{
			*arg_7 = true;
			function_29ab94(local_3, 2.0f, arg_2);
		}
		function_26c180(arg_0);
		vector2f local_13 = { arg_2->i, arg_2->j };
		real local_14 = function_29ab92(&local_13);
		vector2f local_15 = { local_3->i, local_3->j };
		function_29ab92(&local_15);
		local_4 = *arg_2;
		real local_16 = 1.0f;
		real local_17;
		function_2922b0(arg_0, arg_1, &local_4, &local_6, &local_16, &local_17, true);
		if (function_30bf0(&local_4) == 0.0f) local_4 = *local_3;
		real local_18;
		if (*(real *)(local_0 + 0x47c) != 0.0f)
			local_18 = *(real *)(local_0 + 0x47c);
		else
		{
			real local_19 = local_14 * local_14;
			if (!(local_19 > 0.001f)) local_19 = 0.001f;
			real local_20 = local_2->field_1c * 0.5f;
			local_20 = local_20 * local_20 / local_19;
			real local_21 = local_13.i * local_15.i + local_13.j * local_15.j;
			if (!(local_21 > 0.0f)) local_21 = 0.0f;
			local_18 = (1.0f - local_21) * 0.5f * (local_20 * 0.5f) * 4.0f;
		}
		real local_19 = local_2->field_4c > 0.05f ? local_2->field_4c : 0.05f;
		local_18 = function_29ab93(local_18, local_19, 1.0f);
		if (!(local_18 > local_17)) local_18 = local_17;
		vector2f local_20 = { local_4.i, local_4.j };
		function_29b2e0(arg_0, arg_1, &local_4, &local_4, local_18, &local_4);
		*(real *)(local_0 + 0x668) = local_18;
		function_29ab92(&local_20);
		local_18 = function_1f9e70(arg_0, arg_1, (point2f const *)&local_7,
			&local_13, &local_15, &local_20, NULL, local_9, false) * local_2->field_50;
		*arg_6 = false;
		if (local_18 > local_16) local_18 = local_16;
		vector3f local_22;
		local_22.i = local_18;
		local_22.j = local_18 * 0.0f;
		local_22.k = local_18 * 0.0f;
		if (local_2->field_68 > 0.0f)
		{
			vector3f const *local_23 = &local_1->velocity;
			real local_24 = local_23->k * local_3->k + local_23->j * local_3->j + local_23->i * local_3->i;
			real local_25 = local_18 - local_24 / local_2->field_68;
			long local_26 = local_25 < 0.0f ? -1 : 1;
			local_22.i = (real)(sqrt(fabs((double)local_25)) * local_2->field_6c * local_26 + local_22.i);
		}
		local_5.i = (local_6.i + local_22.i) * (1.0f - local_2->field_64) + *(real *)(local_0 + 0x678) * local_2->field_64;
		local_5.j = (local_6.j + local_22.j) * (1.0f - local_2->field_64) + *(real *)(local_0 + 0x67c) * local_2->field_64;
		local_5.k = (local_6.k + local_22.k) * (1.0f - local_2->field_64) + *(real *)(local_0 + 0x680) * local_2->field_64;
		local_5.i = function_29ab93(local_5.i, -1.0f, 1.0f);
		local_5.j = function_29ab93(local_5.j, -1.0f, 1.0f);
		local_5.k = function_29ab93(local_5.k, -1.0f, 1.0f);
		*(short *)(local_0 + 0x656) = NONE;
	}
	*(vector3f *)(local_0 + 0x66c) = local_5;
	*arg_4 = 0;
	*arg_3 = local_4;
	*arg_5 = local_5;
}
