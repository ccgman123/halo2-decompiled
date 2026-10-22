#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include <string.h>
// @flags /O1 /Oi /arch:SSE /Gr

long g_4ba048;
extern long g_4ba04c;

struct s_13c051
{
	short field_0;
	short field_2;
	short field_4;
	short field_6;
};

void interface_rectangle_fill(struct short_rectangle2d const *arg_2, dword arg_1);

// @retail 0x13e635
void function_13e635(void)
{
	if (g_4ba04c > 1)
	{
		s_13c051 local_1;
		if (g_4ba048 == 1)
		{
			local_1.field_0 = 239;
			local_1.field_2 = 0;
			local_1.field_4 = 241;
			local_1.field_6 = 640;
			interface_rectangle_fill((short_rectangle2d const *)&local_1, 0xff000000);
			if (g_4ba04c > 2)
			{
				if (g_4ba04c == 3)
				{
					local_1.field_0 = 240;
					local_1.field_2 = 319;
					local_1.field_4 = 480;
					local_1.field_6 = 321;
					interface_rectangle_fill((short_rectangle2d const *)&local_1, 0xff000000);
				}
				else
				{
					local_1.field_0 = 0;
					local_1.field_2 = 319;
					local_1.field_4 = 480;
					local_1.field_6 = 321;
					interface_rectangle_fill((short_rectangle2d const *)&local_1, 0xff000000);
				}
			}
		}
		else
		{
			local_1.field_0 = 0;
			local_1.field_2 = 319;
			local_1.field_4 = 480;
			local_1.field_6 = 321;
			interface_rectangle_fill((short_rectangle2d const *)&local_1, 0xff000000);
			if (g_4ba04c > 2)
			{
				if (g_4ba04c == 3)
				{
					local_1.field_2 = 320;
					local_1.field_0 = 239;
					local_1.field_4 = 241;
					local_1.field_6 = 640;
					interface_rectangle_fill((short_rectangle2d const *)&local_1, 0xff000000);
				}
				else
				{
					local_1.field_2 = 0;
					local_1.field_0 = 239;
					local_1.field_4 = 241;
					local_1.field_6 = 640;
					interface_rectangle_fill((short_rectangle2d const *)&local_1, 0xff000000);
				}
			}
		}
	}
}

struct s_13e703
{
	short field_0;
	short field_2;
};

struct s_13c052
{
	point2f field_0;
	point2f field_8;
	dword field_10;
};

struct s_13c050
{
	dword field_0;
	byte field_4[8];
	byte *field_c;
	byte field_10[0x18];
	real field_28;
	real field_2c;
	byte field_30[0x10];
	real field_40;
	real field_44;
	byte field_48[0x48];
	__int64 field_90;
};

void __stdcall function_52040(void const *arg_1, void const *arg_2);

// @retail 0x13e703
void function_13e703(byte const *arg_1, s_13e703 const *arg_2, box2f const *arg_3, real arg_4, real arg_5, real arg_6)
{
	box2f local_1 = { 0.0f, 1.0f, 0.0f, 1.0f };
	real local_2 = (real)sin(arg_5);
	real cosine = (real)cos(arg_5);
	if (!arg_3)
		arg_3 = &local_1;
	dword local_3 = ((dword)(arg_6 * 255.0f) << 24) | 0xffffff;
	s_13c052 local_10[4];
	for (short local_11 = 0; local_11 < 4; local_11++)
	{
		real local_12 = ((local_11 + 1) & 2) ? arg_3->x1 : arg_3->x0;
		real local_13 = local_11 > 1 ? arg_3->y1 : arg_3->y0;
		real local_4 = *(short const *)(arg_1 + 4);
		real local_6 = *(short const *)(arg_1 + 0x10);
		real local_5 = *(short const *)(arg_1 + 6);
		real local_7 = *(short const *)(arg_1 + 0x12);
		real local_8 = arg_2->field_0;
		real local_9 = arg_2->field_2;
		point2f local_19;
		local_19.x = (local_4 * local_12 - local_6) * arg_4;
		local_19.y = (local_5 * local_13 - local_7) * arg_4;
		local_10[local_11].field_0.x = local_19.x * cosine + local_8 - local_19.y * local_2;
		local_10[local_11].field_0.y = local_19.y * cosine + local_19.x * local_2 + local_9;
		local_10[local_11].field_8.x = local_12;
		local_10[local_11].field_8.y = local_13;
		local_10[local_11].field_10 = local_3;
	}
	s_13c050 local_16;
	memset(&local_16, 0, sizeof(local_16));
	local_16.field_0 = 0;
	*((byte *)&local_16 + 0x96) = false;
	local_16.field_44 = 1.0f;
	local_16.field_40 = 1.0f;
	local_16.field_2c = 1.0f;
	local_16.field_28 = 1.0f;
	*(word *)((byte *)&local_16 + 0x94) = 7;
	local_16.field_c = (byte *)arg_1;
	function_52040(&local_16, local_10);
}
