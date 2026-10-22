// @flags /O2 /Gr /arch:SSE
#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"
#include <math.h>

struct s_2922b0
{
	byte field_0[0x14];
	real field_14;
	byte field_18[0x18];
	real field_30;
	byte field_34[0x14];
	real field_48;
};
struct s_2922b1
{
	long field_0;
	real field_4;
	byte field_8[0x4c - 8];
};
struct s_collision_result_1697c0;
bool __stdcall function_1697c0(long arg_0, point3f const *arg_1, vector3f const *arg_2,
	long arg_3, long arg_4, s_collision_result_1697c0 *arg_5);
real function_30bf0(vector3f *arg_0);

PRIVATE __forceinline void function_2922b1(vector3f const *arg_0, vector3f const *arg_1, vector3f *arg_2)
{
	real local_0 = arg_0->j * arg_1->k - arg_0->k * arg_1->j;
	real local_1 = arg_0->k * arg_1->i - arg_0->i * arg_1->k;
	real local_2 = arg_0->i * arg_1->j - arg_0->j * arg_1->i;
	arg_2->i = local_0;
	arg_2->j = local_1;
	arg_2->k = local_2;
}
PRIVATE __forceinline void function_2922b2(point3f const *arg_0, vector3f const *arg_1, real arg_2, point3f *arg_3)
{
	arg_3->x = arg_0->x + arg_1->i * arg_2;
	arg_3->y = arg_0->y + arg_1->j * arg_2;
	arg_3->z = arg_0->z + arg_1->k * arg_2;
}
PRIVATE __forceinline real function_2922b3(real arg_0, real arg_1, real arg_2)
{
	return arg_1 > arg_0 ? arg_1 : arg_0 > arg_2 ? arg_2 : arg_0;
}
PRIVATE __forceinline real function_2922b4(real arg_0, real arg_1, real arg_2, real *arg_3)
{
	real local_0 = (real)sqrt(0.0f > 1.0f - arg_0 / arg_1 ? 0.0f : 1.0f - arg_0 / arg_1);
	*arg_3 = local_0;
	return function_2922b3(arg_2 * local_0, 0.0f, arg_2);
}
PRIVATE __forceinline void function_2922b5(vector3f const *arg_0, vector3f const *arg_1, real arg_2, vector3f *arg_3)
{
	real local_0 = (real)sin(arg_2);
	real local_1 = (real)cos(arg_2);
	real local_2 = (arg_0->i * arg_1->i + arg_0->k * arg_1->k + arg_1->j * arg_0->j) * (1.0f - local_1);
	vector3f local_3;
	function_2922b1(arg_0, arg_1, &local_3);
	real local_4 = local_1 * arg_0->i + arg_1->i * local_2 - local_3.i * local_0;
	real local_5 = local_1 * arg_0->j + arg_1->j * local_2 - local_3.j * local_0;
	real local_6 = local_1 * arg_0->k + arg_1->k * local_2 - local_3.k * local_0;
	arg_3->i = local_4;
	arg_3->j = local_5;
	arg_3->k = local_6;
}

// @retail 0x2922b0
bool __stdcall function_2922b0(long arg_0, long arg_1, vector3f *arg_2,
	vector3f *arg_3, real *arg_4, real *arg_5, bool arg_6)
{
	bool local_0 = true;
	s_slot_object_view *local_1 = object_get(arg_1);
	s_2922b0 const *local_2 = (s_2922b0 const *)function_1e5450(arg_0, local_1->tag_index);
	vector3f local_3 = *arg_2;
	real local_4 = 3.402823466e38f;
	*arg_5 = 0.0f;
	bool local_5[4];
	local_5[0] = false;
	local_5[1] = false;
	local_5[2] = false;
	local_5[3] = false;
	if (local_2->field_48 > 0.0f)
	{
		arg_3->i = 0.0f;
		arg_3->j = 0.0f;
		arg_3->k = 0.0f;
		real local_6 = local_2->field_48;
		vector3f local_7;
		vector3f local_8;
		vector3f local_9;
		vector3f const *local_10;
		vector3f const *local_11;
		if (arg_6)
		{
			local_10 = &local_1->forward;
			local_11 = (vector3f const *)((byte *)local_1 + 0x7c);
			function_2922b1(local_11, local_10, &local_9);
		}
		else
		{
			local_7 = *arg_2;
			if (function_30bf0(&local_7) == 0.0f) local_7 = *g_4687a8;
			function_2922b1(g_4687b0, &local_7, &local_9);
			if (function_30bf0(&local_9) == 0.0f) local_9 = *g_4687ac;
			function_2922b1(&local_7, &local_9, &local_8);
			if (function_30bf0(&local_8) == 0.0f) local_8 = *g_4687b0;
			local_10 = &local_7;
			local_11 = &local_8;
		}
		point3f local_12 = local_1->unknown030;
		*arg_4 = 1.0f;
		point3f local_13[4];
		function_2922b2(&local_12, local_11, local_2->field_14, &local_13[0]);
		function_2922b2(&local_12, &local_9, local_2->field_14, &local_13[1]);
		function_2922b2(&local_12, local_11, -local_2->field_14, &local_13[2]);
		function_2922b2(&local_12, &local_9, -local_2->field_14, &local_13[3]);
		real local_14[4];
		local_14[0] = local_2->field_14 + local_6;
		local_14[1] = local_2->field_14 + local_6;
		local_14[2] = local_2->field_14 + local_6;
		local_14[3] = local_2->field_14 + local_6;
		real local_15[4];
		s_2922b1 local_16;
		*(short *)((byte *)&local_16 + 0x24) = NONE;
		long local_17 = 0;
		long remaining = 4;
		do
		{
			vector3f local_18;
			real ray_length = local_14[local_17];
			local_18.i = local_10->i * ray_length;
			local_18.j = local_10->j * ray_length;
			local_18.k = local_10->k * ray_length;
			if (function_1697c0(0x1808c2d, &local_13[local_17], &local_18, arg_1, NONE, (s_collision_result_1697c0 *)&local_16))
			{
				real local_19 = local_14[local_17] * local_16.field_4 - local_2->field_14;
				if (0.1f > local_19 / local_6)
				{
					local_19 = local_6 * 0.1f;
					local_0 = false;
				}
				local_15[local_17] = local_19;
				local_5[local_17] = true;
				if (local_4 > local_19) local_4 = local_19;
			}
			++local_17;
		} while (--remaining);
		if (local_0)
		{
			real local_17 = 0.0f;
			real local_18 = 0.0f;
			real local_19 = local_2->field_30 > 0.0f ? local_2->field_30 * 0.01745329238474369f : 1.5707963705062866f;
			if (3.402823466e38f > local_4)
			{
				*arg_4 = (1.0f / local_6) * local_4 * 0.7f + 0.3f;
				*arg_5 = 1.0f - function_2922b3((1.0f / local_6) * local_4, 0.0f, 1.0f);
			}
			real local_20;
			if (local_5[0])
			{
				local_17 = function_2922b4(local_15[0], local_6, local_19, &local_20);
				arg_3->k -= local_20;
			}
			if (local_5[1])
			{
				local_18 = -function_2922b4(local_15[1], local_6, local_19, &local_20);
				arg_3->j -= local_20;
			}
			if (local_5[2])
			{
				local_17 -= function_2922b4(local_15[2], local_6, local_19, &local_20);
				arg_3->k += local_20;
			}
			if (local_5[3])
			{
				local_18 += function_2922b4(local_15[3], local_6, local_19, &local_20);
				arg_3->j += local_20;
			}
			function_2922b5(&local_3, &local_9, local_17, &local_3);
			function_2922b5(&local_3, local_11, local_18, &local_3);
		}
		if (arg_6)
			*arg_2 = local_3;
		else
		{
			vector3f local_17;
			local_17.i = local_9.i * arg_3->j + local_10->i * arg_3->i + local_11->i * arg_3->k;
			local_17.j = local_9.j * arg_3->j + local_10->j * arg_3->i + local_11->j * arg_3->k;
			local_17.k = local_9.k * arg_3->j + local_10->k * arg_3->i + local_11->k * arg_3->k;
			vector3f local_18;
			vector3f const *local_19 = (vector3f const *)((byte *)local_1 + 0x7c);
			function_2922b1(local_19, &local_1->forward, &local_18);
			arg_3->i = local_1->forward.k * local_17.k + local_1->forward.j * local_17.j + local_1->forward.i * local_17.i;
			arg_3->j = local_18.k * local_17.k + local_18.j * local_17.j + local_18.i * local_17.i;
			arg_3->k = local_19->k * local_17.k + local_19->j * local_17.j + local_19->i * local_17.i;
		}
	}
	return local_0;
}
