// @flags /O2 /Gr /arch:SSE
#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"
#include "unknown_11cc90.h"
#include <math.h>

real g_47ffd0 = 0.5f;

struct s_29bab0
{
	byte field_0[0x14];
	real field_14;
	byte field_18[4];
	real field_1c;
	real field_20;
	byte field_24[0x20];
	real field_44;
	byte field_48[8];
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
real normalize2d(point2f *arg_0);
bool __stdcall function_2922b0(long arg_0, long arg_1, vector3f *arg_2,
	vector3f *arg_3, real *arg_4, real *arg_5, bool arg_6);
void function_29b2e0(long arg_0, long arg_1, vector3f const *arg_2,
	vector3f const *arg_3, real arg_4, vector3f *arg_5);
void function_29b990(long arg_0, long arg_1, vector3f const *arg_2,
	vector3f const *arg_3, vector3f *arg_4);
void __stdcall function_1f8510(bool arg_0, vector3f const *arg_1, vector3f const *arg_2, vector3f *arg_3);
void __stdcall function_1f3e40(long arg_0, vector3f const *arg_1, vector3f *arg_2);
bool __stdcall function_110ab0(long arg_0);
void function_ba1d0(long arg_0, vector3f *arg_1, vector3f *arg_2);
void function_1e4290(long arg_0, bool arg_1);

PRIVATE __forceinline real function_29bab1(vector3f *arg_0)
{
	real local_0 = (real)sqrt((double)arg_0->i * arg_0->i + (double)arg_0->k * arg_0->k + (double)arg_0->j * arg_0->j);
	if (0.0001f > fabs((double)local_0)) return 0.0f;
	real local_1 = 1.0f / local_0;
	arg_0->i *= local_1;
	arg_0->j *= local_1;
	arg_0->k *= local_1;
	return local_0;
}
PRIVATE __forceinline real function_29bab2(real arg_0, real arg_1, real arg_2)
{
	return arg_1 > arg_0 ? arg_1 : arg_0 > arg_2 ? arg_2 : arg_0;
}
PRIVATE __forceinline real function_29bab3(real arg_0, real arg_1, real arg_2)
{
	long local_0 = arg_1 < 0.0f ? -1 : 1;
	return (real)(sqrt(fabs((double)arg_1)) * local_0 * arg_2 + arg_0);
}
PRIVATE __forceinline real function_29bab4(vector3f const *arg_0, vector3f const *arg_1)
{
	return arg_0->k * arg_1->k + arg_0->j * arg_1->j + arg_1->i * arg_0->i;
}

// @retail 0x29bab0
void __stdcall function_29bab0(long arg_0, long arg_1, vector3f const *arg_2,
	vector3f *arg_3, short *arg_4, vector3f *arg_5, bool *arg_6, bool *arg_7, short arg_8)
{
	vector3f const *const * input_reference = &arg_2;
	byte *local_0 = (byte *)actor_get(arg_0);
	s_slot_object_view *local_1 = object_get(arg_1);
	s_29bab0 const *local_2 = (s_29bab0 const *)function_1e5450(arg_0, local_1->tag_index);
	bool local_3 = false;
	*(bool *)(local_0 + 0x6d1) = false;
	if (!arg_8)
	{
		*(real *)(local_0 + 0x658) = *(real *)(local_0 + 0x65c);
		*(real *)(local_0 + 0x660) = *(real *)(local_0 + 0x664);
		*(vector3f *)(local_0 + 0x678) = *(vector3f *)(local_0 + 0x66c);
	}
	if (*(long *)(local_0 + 0x26c) == NONE)
		*(long *)(local_0 + 0x7fc) = function_1f5f30(arg_0);
	vector3f const *local_4 = (vector3f const *)(local_0 + 0x290);
	vector3f local_5 = *local_4;
	vector3f local_6 = *g_4687a4;
	vector3f local_7 = *g_4687a4;
	point3f local_8;
	function_b9dd0(arg_1, &local_8);
	vector3f local_9 = *(*input_reference);
	real local_10 = function_29bab1(&local_9);
	if (!local_2)
		*(bool *)(local_0 + 0x654) = false;
	else if (!*(bool *)(local_0 + 0x5d0) || *arg_7)
	{
		*(bool *)(local_0 + 0x655) = true;
		*(bool *)(local_0 + 0x654) = false;
		function_29b990(arg_0, arg_1, local_4, NULL, &local_5);
	}
	else
	{
		bool local_11;
		vector3f local_13;
		if (function_1f9760(arg_0, arg_1, &local_8, local_10, &local_11, &local_3, &local_13))
		{
			*(bool *)(local_0 + 0x655) = true;
			*(bool *)(local_0 + 0x654) = false;
			*arg_7 = true;
			function_29b990(arg_0, arg_1, local_4, NULL, &local_5);
		}
		else
		{
			function_26c180(arg_0);
			point2f local_14 = { local_8.x + (*input_reference)->i, local_8.y + (*input_reference)->j };
			vector2f local_15 = { (*input_reference)->i, (*input_reference)->j };
			real local_16 = normalize2d((point2f *)&local_15);
			vector2f local_17 = { local_4->i, local_4->j };
			normalize2d((point2f *)&local_17);
			real local_18 = (real)((double)local_2->field_1c * 0.5);
			real local_19 = 0.0f >= local_2->field_14 ? local_1->unknown03c : local_2->field_14;
			real local_20 = 1.0f;
			bool local_21 = true;
			if (*(bool *)(local_0 + 0x654))
			{
				if ((double)local_15.i * local_17.i + (double)local_15.j * local_17.j < 0.99)
					local_21 = false;
				else if (!*(bool *)(local_0 + 0x4ae) && local_18 > local_16)
					local_21 = false;
			}
			else
			{
				bool local_22 = (*input_reference)->i * -local_17.j + (*input_reference)->j * local_17.i > 0.0f;
				real local_23 = local_2->field_20;
				real local_24 = local_22 ? local_23 : -local_23;
				point2f local_25 = { local_8.x + -local_17.j * local_24, local_8.y + local_17.i * local_24 };
				vector2f local_26 = { local_14.x - local_25.x, local_14.y - local_25.y };
				double local_27 = sqrt((double)local_26.i * local_26.i + (double)local_26.j * local_26.j);
				if (local_23 > local_27 && (*(short *)(local_0 + 0x656) != 2 || !*(bool *)(local_0 + 0x4ae)))
					local_21 = false;
				else if (!(local_23 > local_27) && !*(bool *)(local_0 + 0x4ae) && local_18 > local_16)
					local_21 = false;
				else if ((*input_reference)->j * local_17.j + (*input_reference)->i * local_17.i > 0.0f)
					local_21 = true;
				else
				{
					local_21 = local_26.j * local_17.i + local_26.i * -local_17.j > 0.0f;
					if (!local_22) local_21 = !local_21;
				}
			}
			local_5 = *(*input_reference);
			short local_22;
			real local_23;
			if (!(local_2->field_44 > local_16) && local_21 && !*(bool *)(local_0 + 0x4d5) && !*(bool *)(local_0 + 0x5d1))
			{
				bool local_24 = function_2922b0(arg_0, arg_1, &local_5, &local_7, &local_20, &local_23, true);
				*(bool *)(local_0 + 0x655) = false;
				if (!local_24)
				{
					local_22 = 3;
					*(bool *)(local_0 + 0x654) = true;
				}
				else
				{
					local_22 = 2;
					if (!*(bool *)(local_0 + 0x4ae) && local_2->field_1c > local_16)
					{
						real local_25 = (local_2->field_1c - local_16) / (local_2->field_1c - local_18);
						local_25 = local_25 < 1.0f ? local_25 : 1.0f;
						local_5.k *= 1.0f - local_25;
					}
					if (function_30bf0(&local_5) > 0.0f && fabs((double)local_5.k) > g_47ffd0)
					{
						real local_25 = (real)sqrt(1.0 - (double)g_47ffd0 * g_47ffd0);
						if (normalize2d((point2f *)&local_5) > 0.0f)
						{
							local_5.i *= local_25;
							local_5.j *= local_25;
							local_5.k = function_29bab2(local_5.k, -g_47ffd0, g_47ffd0);
						}
					}
				}
				if (function_30bf0(&local_5) == 0.0f) local_5 = *local_4;
			}
			else
			{
				local_22 = 3;
				*(bool *)(local_0 + 0x655) = false;
				*(bool *)(local_0 + 0x654) = true;
				if (local_19 > local_16) local_5 = *local_4;
				function_2922b0(arg_0, arg_1, &local_5, &local_7, &local_20, &local_23, false);
			}
			real local_24 = 0.0f;
			if (local_22 == 2)
			{
				if (*(real *)(local_0 + 0x47c) != 0.0f)
					local_24 = *(real *)(local_0 + 0x47c);
				else
				{
					real local_25 = local_16 * local_16;
					if (!(local_25 > 0.001f)) local_25 = 0.001f;
					real local_26 = local_2->field_1c * 0.5f;
					local_26 = local_26 * local_26 / local_25;
					real local_27 = local_15.i * local_17.i + local_15.j * local_17.j;
					if (!(local_27 > 0.0f)) local_27 = 0.0f;
					local_24 = (1.0f - local_27) * 0.5f * (local_26 * 0.5f) * 4.0f;
				}
				local_24 = function_29bab2(local_24, 0.05f, 1.0f);
				if (!(local_24 > local_23)) local_24 = local_23;
			}
			else if (local_22 == 1) local_24 = 1.0f;
			vector3f local_25 = local_5;
			if (local_22 == 3)
				function_29b990(arg_0, arg_1, local_4, &local_5, &local_5);
			else
				function_29b2e0(arg_0, arg_1, &local_5, &local_5, local_24, &local_5);
			*(real *)(local_0 + 0x668) = local_24;
			if (local_22 == 1)
			{
				*arg_6 = false;
				*(bool *)(local_0 + 0x654) = true;
			}
			else
			{
				vector2f local_26 = { local_25.i, local_25.j };
				normalize2d((point2f *)&local_26);
				real local_27 = function_1f9e70(arg_0, arg_1, (point2f const *)&local_8, &local_15, &local_26,
					&local_17, NULL, local_10, local_22 != 2) * local_2->field_50;
				function_1f8510(true, &local_9, local_4, &local_6);
				if (local_22 == 2)
				{
					*(bool *)(local_0 + 0x654) = false;
					if (local_2->field_1c > local_16)
					{
						real local_28 = (local_2->field_1c - local_16) / (local_2->field_1c - local_18);
						local_28 = local_28 < 1.0f ? local_28 : 1.0f;
						local_6.k *= local_28;
					}
					else local_6.k = 0.0f;
					local_6.i = 1.0f;
					local_6.j = 0.0f;
					*arg_6 = false;
				}
				if (local_27 > local_20) local_27 = local_20;
				local_6.i *= local_27;
				local_6.j *= local_27;
				local_6.k *= local_27;
				if (local_2->field_68 > 0.0f)
				{
					vector3f const *local_28 = (vector3f const *)((byte *)local_1 + 0x7c);
					vector3f local_29;
					local_29.i = local_28->j * local_4->k - local_28->k * local_4->j;
					local_29.j = local_28->k * local_4->i - local_28->i * local_4->k;
					local_29.k = local_28->i * local_4->j - local_28->j * local_4->i;
					real local_30 = 1.0f / local_2->field_68;
					real local_31 = local_6.i - local_30 * function_29bab4(&local_1->velocity, local_4);
					real local_32 = local_6.j - local_30 * function_29bab4(&local_1->velocity, &local_29);
					real local_33 = local_6.k - local_30 * function_29bab4(&local_1->velocity, local_28);
					local_6.i = function_29bab3(local_6.i, local_31, local_2->field_6c);
					local_6.j = function_29bab3(local_6.j, local_32, local_2->field_6c);
					local_6.k = function_29bab3(local_6.k, local_33, local_2->field_6c);
				}
				local_6.i += local_7.i;
				local_6.j += local_7.j;
				local_6.k += local_7.k;
				function_1f3e40(arg_1, &local_5, &local_6);
				local_6.i = *(real *)(local_0 + 0x678) * local_2->field_64 + local_6.i * (1.0f - local_2->field_64);
				local_6.j = *(real *)(local_0 + 0x67c) * local_2->field_64 + local_6.j * (1.0f - local_2->field_64);
				local_6.k = *(real *)(local_0 + 0x680) * local_2->field_64 + local_6.k * (1.0f - local_2->field_64);
				local_6.i = function_29bab2(local_6.i, -1.0f, 1.0f);
				local_6.j = function_29bab2(local_6.j, -1.0f, 1.0f);
				local_6.k = function_29bab2(local_6.k, -1.0f, 1.0f);
			}
			if (*(bool *)(local_0 + 0x482) && *(bool *)(local_0 + 0x5d0) && !function_110ab0(arg_1) && local_10 > 0.0f && (local_22 == 2 || local_22 == 3))
			{
				function_ba1d0(*(long *)(local_0 + 0x18), &local_7, NULL);
				if (function_30bf0(&local_7) > 0.0f && function_29bab4(&local_9, &local_7) > 0.85f)
					function_1e4290(arg_0, true);
			}
			*(short *)(local_0 + 0x656) = local_22;
		}
	}
	if (*arg_7 && !local_3 && *(short *)(local_0 + 0x4ac) == 6)
		*(bool *)(local_0 + 0x4bc) = true;
	*(vector3f *)(local_0 + 0x66c) = local_6;
	*arg_4 = 0;
	*arg_3 = local_5;
	*arg_5 = local_6;
}
