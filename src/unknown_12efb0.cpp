// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "globals.h"
#include <string.h>

struct s_fog_cluster_distance
{
	long cluster_index;
	real distance;
	byte unknown08[0xc];
	long previous_index;
};

struct s_12ec00;
struct s_12eb00
{
	byte field_0[0xd8];
	long field_d8;
	s_12ec00 *field_dc;
	byte field_e0[0x14];
};
struct s_palette_owner;
struct s_fog_accumulator
{
	color3f color;
	real intensity;
	real distance;
	real height;
	real weight;
	real remaining;
};
struct s_12efb6
{
	color3f field_0;
	real field_c;
	real field_10;
	real field_14;
};
struct s_weighted_accumulator
{
	real a[3];
	real b[3];
	real c[4];
	real weight;
	real remainder;
};
struct s_12efb0
{
	byte field_0[0x1c];
	real field_1c[6];
	real field_34[6];
	real field_4c[4];
	byte field_5c[0x10];
	long field_6c;
	real field_70[10];
	byte field_98[0x30];
	real field_c8[6];
	real field_e0[4];
	long field_f0;
};

long scenario_fog_clusters_find(long cluster_index, point3f const *point, s_fog_cluster_distance *clusters);
long function_12eb00(long arg_0, s_fog_cluster_distance *arg_1, s_12eb00 *arg_2);
long function_130f60(s_palette_owner const *owner, long *palette_index);
void function_131030(long tag_index, vector3f const *view_direction, s_fog_accumulator *first, s_fog_accumulator *second, s_fog_accumulator *third);
void function_1317a0(real const *a, real const *b, real c0, real c1, real c2, real c3, real t, s_weighted_accumulator *acc);

real g_547f80 = 1.0f;

static __forceinline real function_12efb1(real arg_0)
{
	if (arg_0 < 0.0f) arg_0 = 0.0f;
	else if (arg_0 > 1.0f) arg_0 = 1.0f;
	return arg_0;
}

static __forceinline void function_12efb2(s_fog_accumulator *arg_0, real const *arg_1, real arg_2, real arg_3, real arg_4, real arg_5)
{
	arg_5 = function_12efb1(arg_5);
	arg_0->color.red += arg_1[0] * arg_5;
	arg_0->color.green += arg_1[1] * arg_5;
	arg_0->color.blue += arg_1[2] * arg_5;
	arg_0->intensity += arg_2 * arg_5;
	arg_0->distance += arg_3 * arg_5;
	arg_0->height += arg_4 * arg_5;
	arg_0->weight += arg_5;
	arg_0->remaining *= 1.0f - arg_5;
}

static __forceinline void function_12efb3(s_fog_accumulator *arg_0, s_fog_accumulator const *arg_1, real arg_2)
{
	arg_2 = function_12efb1(arg_2);
	if (arg_1->weight > 0.0f)
	{
		real local_0 = arg_2 / arg_1->weight;
		arg_0->color.red += arg_1->color.red * local_0;
		arg_0->color.green += arg_1->color.green * local_0;
		arg_0->color.blue += arg_1->color.blue * local_0;
		arg_0->intensity += arg_1->intensity * local_0;
		arg_0->distance += arg_1->distance * local_0;
		arg_0->height += arg_1->height * local_0;
	}
	arg_0->weight += arg_2;
	arg_0->remaining *= 1.0f - arg_2;
}

static __forceinline void function_12efb4(s_fog_accumulator const *arg_0, real *arg_1)
{
	if (arg_0->weight > 0.0f && arg_0->intensity > 0.0f)
	{
		real local_0 = 1.0f / arg_0->weight;
		arg_1[0] = arg_0->color.red * local_0;
		arg_1[1] = arg_0->color.green * local_0;
		arg_1[2] = arg_0->color.blue * local_0;
		arg_1[3] = arg_0->intensity * local_0;
		arg_1[4] = arg_0->distance * local_0;
		arg_1[5] = arg_0->height * local_0;
	}
}

static __forceinline void function_12efb5(s_12efb6 *arg_0, real const *arg_1, real arg_2)
{
	arg_2 = function_12efb1(arg_2);
	arg_0->field_0.red += arg_1[0] * arg_2;
	arg_0->field_0.green += arg_1[1] * arg_2;
	arg_0->field_0.blue += arg_1[2] * arg_2;
	arg_0->field_c += (arg_1[3] == 0.0f ? 1.0f : arg_1[3]) * arg_2;
	arg_0->field_10 += arg_2;
	arg_0->field_14 *= 1.0f - arg_2;
}

// @retail 0x12efb0
void function_12efb0(long arg_0, point3f const *arg_1, vector3f const *arg_2, s_12efb0 *arg_3)
{
	s_fog_cluster_distance local_0[512];
	s_12eb00 local_1[24];
	long local_2 = NONE;
	real local_3 = 0.0f;
	long local_4 = scenario_fog_clusters_find(arg_0, arg_1, local_0);
	function_12eb00(local_4, local_0, local_1);
	s_fog_accumulator local_5;
	s_fog_accumulator local_6;
	s_fog_accumulator local_7;
	s_fog_accumulator local_8;
	s_12efb6 local_9;
	s_weighted_accumulator local_10;
	memset(&local_5, 0, sizeof(local_5));
	memset(&local_6, 0, sizeof(local_6));
	memset(&local_7, 0, sizeof(local_7));
	memset(&local_8, 0, sizeof(local_8));
	memset(&local_9, 0, sizeof(local_9));
	memset(&local_10, 0, sizeof(local_10));
	local_5.remaining = 1.0f;
	local_6.remaining = 1.0f;
	local_7.remaining = 1.0f;
	local_8.remaining = 1.0f;
	local_10.remainder = 1.0f;
	local_9.field_14 = 1.0f;
	for (long local_11 = 0; local_11 < local_4; local_11++)
	{
		s_fog_cluster_distance *local_12 = &local_0[local_11];
		byte *local_13 = *(byte **)((byte *)g_4e0348 + 0xa0) + local_12->cluster_index * 0xb0;
		long local_14 = NONE;
		long local_15 = function_130f60((s_palette_owner const *)local_13, &local_14);
		bool local_16 = false;
		long *local_17 = (long *)local_12->unknown08;
		local_17[0] = 0;
		local_17[1] = NONE;
		*(real *)&local_17[2] = 0.0f;
		if (*(char *)(local_13 + 0x6f) != NONE)
		{
			s_12eb00 *local_18 = NULL;
			if (local_12->previous_index != NONE)
				local_18 = &local_1[local_12->previous_index];
			else if (g_4e0350 && *(char *)(local_13 + 0x6f) >= 0 && *(char *)(local_13 + 0x6f) < *(long *)((byte *)g_4e0350 + 0x340))
				local_18 = (s_12eb00 *)(*(byte **)((byte *)g_4e0350 + 0x344) + *(char *)(local_13 + 0x6f) * 0xf4);
			if (local_18)
			{
				real const *local_19 = (real const *)local_18;
				real local_20 = local_19[4] > 0.0f ? (local_19[4] < 10.0f ? local_19[4] : 10.0f) : g_547f80;
				real local_21 = function_12efb1(1.0f - local_12->distance / local_20);
				if (local_12->cluster_index == arg_0)
					arg_3->field_f0 = *(word *)((byte *)local_18 + 0xf0);
				if (local_21 > 0.0f)
				{
					function_12efb2(&local_5, local_19 + 1, local_19[6], local_19[7], local_19[8], local_21);
					function_12efb2(&local_6, local_19 + 9, local_19[13], local_19[14], local_19[15], local_21);
					function_12efb2(&local_7, local_19 + 17, local_19[20], local_19[22], local_19[21], local_21);
					if (local_19[0x29] > 0.0f && *(long *)((byte *)local_18 + 0xd4) != NONE)
					{
						real const *local_22 = local_19 + 0x22;
						if (local_22[7] > 0.0f)
						{
							function_1317a0(local_22, local_22 + 3, local_22[6], local_22[7], local_22[8], local_22[9], local_21, &local_10);
							if (local_12->cluster_index == arg_0 || local_21 > local_3)
							{
								local_2 = *(long *)((byte *)local_18 + 0xd4);
								local_3 = local_21;
							}
							local_16 = true;
						}
					}
					if (local_19[0x38] > 0.0f)
						function_12efb5(&local_9, local_19 + 0x38, local_21);
					local_17[0] = 2;
					local_17[1] = *(char *)(local_13 + 0x6f);
					*(real *)&local_17[2] = local_21;
				}
			}
			else
				goto local_30;
		}
		else if (local_15 != NONE)
		{
			byte *local_23 = g_4e3b44[local_15 & 0xffff].bytes;
			real local_24 = *(real *)(local_23 + 0x44);
			local_24 = local_24 > 0.0f ? (local_24 < 10.0f ? local_24 : 10.0f) : g_547f80;
			real local_25 = function_12efb1(1.0f - local_12->distance / local_24);
			if (local_25 > 0.0f)
			{
				s_fog_accumulator local_26, local_27, local_28;
				function_131030(local_15, arg_2, &local_26, &local_27, &local_28);
				function_12efb3(&local_5, &local_26, local_25);
				function_12efb3(&local_6, &local_27, local_25);
				function_12efb3(&local_8, &local_28, local_25);
				local_7.weight += local_25;
				if (*(long *)(local_23 + 0x60) > 0)
				{
					real const *local_29 = *(real **)(local_23 + 0x64);
					if (*(long *)((byte *)local_29 + 0x4c) != NONE && local_29[7] > 0.0f)
					{
						function_1317a0(local_29, local_29 + 3, local_29[6], local_29[7], local_29[8], local_29[9], local_25, &local_10);
						if (local_12->cluster_index == arg_0 || local_25 > local_3)
						{
							local_2 = *(long *)((byte *)local_29 + 0x4c);
							local_3 = local_25;
						}
						local_16 = true;
					}
				}
				if (*(real *)(local_23 + 0x68) > 0.0f)
					function_12efb5(&local_9, (real const *)(local_23 + 0x68), local_25);
				local_17[0] = 3;
				local_17[1] = local_14;
				*(real *)&local_17[2] = local_25;
			}
		}
		else
		{
local_30:
			real local_31 = 1.0f - local_12->distance / g_547f80;
			if (local_31 < 0.0f) local_31 = 0.0f;
			local_5.weight += local_31;
			local_6.weight += local_31;
			local_7.weight += local_31;
			local_8.weight += local_31;
			local_10.weight += local_31;
			local_17[0] = 1;
			local_17[1] = NONE;
			*(real *)&local_17[2] = local_31;
		}
		if (!local_16)
		{
			real local_32 = 1.0f - local_12->distance / g_547f80;
			if (local_32 < 0.0f) local_32 = 0.0f;
			local_10.weight += local_32;
		}
	}
	function_12efb4(&local_5, arg_3->field_1c);
	function_12efb4(&local_6, arg_3->field_34);
	function_12efb4(&local_7, arg_3->field_c8);
	if (local_8.weight > 0.0f && local_8.intensity > 0.0f)
	{
		real local_33 = 1.0f / local_8.weight;
		arg_3->field_4c[0] = local_8.color.red * local_33;
		arg_3->field_4c[1] = local_8.color.green * local_33;
		arg_3->field_4c[2] = local_8.color.blue * local_33;
		arg_3->field_4c[3] = local_8.intensity * local_33;
	}
	if (local_2 != NONE && local_10.weight > 0.0f && local_10.c[1] > 0.0f)
	{
		real local_34 = 1.0f / local_10.weight;
		arg_3->field_70[0] = local_10.a[0] * local_34;
		arg_3->field_70[1] = local_10.a[1] * local_34;
		arg_3->field_70[2] = local_10.a[2] * local_34;
		arg_3->field_70[3] = local_10.b[0] * local_34;
		arg_3->field_70[4] = local_10.b[1] * local_34;
		arg_3->field_70[5] = local_10.b[2] * local_34;
		arg_3->field_70[6] = local_10.c[0] * local_34;
		arg_3->field_70[7] = local_10.c[1] * local_34;
		arg_3->field_70[8] = local_10.c[2] * local_34;
		arg_3->field_70[9] = local_10.c[3] * local_34;
		arg_3->field_6c = local_2;
	}
	if (local_9.field_10 > 0.0f)
	{
		real local_35 = 1.0f / local_9.field_10;
		arg_3->field_e0[0] = function_12efb1((1.0f - local_9.field_14) * local_9.field_0.red * local_35);
		arg_3->field_e0[1] = function_12efb1(local_9.field_0.green * local_35);
		arg_3->field_e0[2] = function_12efb1(local_9.field_0.blue * local_35);
		arg_3->field_e0[3] = local_9.field_c * local_35;
	}
}
