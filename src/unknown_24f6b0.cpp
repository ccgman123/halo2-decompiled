#include "unknown_11c920.h"
#include "unknown_1946f0.h"

// @flags /O1 /Oi /Ob1 /arch:SSE /Gr

PRIVATE __forceinline void direction_face_point(s_direction_face const *face, vector3f const *point, vector3f *out)
{
	real local_0 = face->scale;
	real x = point->i;
	real y = point->j;
	real z = point->k;
	if (local_0 != 1.f)
	{
		x *= local_0;
		y *= local_0;
		z = local_0 * z;
	}
	out->i = face->axes[2].i * z + face->axes[1].i * y + face->axes[0].i * x + face->origin.i;
	out->j = face->axes[2].j * z + face->axes[1].j * y + face->axes[0].j * x + face->origin.j;
	out->k = face->axes[2].k * z + face->axes[1].k * y + face->axes[0].k * x + face->origin.k;
}

/* Expands a face index and two six-bit coordinates to a unit direction. */
// score 19 (was 50): the components are stored in retail's order (i, j then
// k), so our zero is materialised after the multiplies where retail keeps it
// in xmm3 from the top of the function; the low six bits also come from a
// byte move here instead of retail's full copy of the index.
// @retail 0x24f6b0
real __fastcall function_24f6b0(dword index, vector3f *direction)
{
	int value = (int)index;
	short x_coordinate = (short)(value & 0x3f);
	long y_coordinate = (value >> 6) & 0x3f;
	direction->i = (2.f / 63.f) * (real)x_coordinate;
	direction->j = (2.f / 63.f) * (real)y_coordinate;
	direction->k = 0.f;
	direction_face_point(&g_475480[value >> 12], direction, direction);
	return function_30bf0(direction);
}
