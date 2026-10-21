// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_180B60.CPP: vectors packed into 32 bits (11, 11 and 10 signed
   bits, rounded down) */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "globals.h"
#include <math.h>

#define PIN(value, minimum, maximum) ((value) < (minimum) ? (minimum) : ((value) > (maximum) ? (maximum) : (value)))

static __forceinline long float_to_int_down(real value)
{
	__asm
	{
		movss xmm0, value
		cvttss2si eax, xmm0
		cvtsi2ss xmm1, eax
		cmpneqss xmm1, xmm0
		cmpltss xmm0, g_45dbd8
		andps xmm0, xmm1
		movmskps ecx, xmm0
		sub eax, ecx
	}
}

struct s_cluster_query
{
	byte unknown00[8];
	point3f point;
	byte unknown14[0xc];
	short cluster_index;
};

struct s_cluster_reference
{
	byte unknown00[0x3c];
	long next;
};

#include "cluster_marks_storage.h"

short __stdcall function_14a5b0(short cluster_index, point3f const *point, real radius, short maximum_count, short *clusters);
long __stdcall function_17d100(long cluster_index, long datum_index);

// @retail 0x180b60
void __stdcall function_180b60(s_cluster_query const *query, real radius_squared, long datum_index)
{
	short clusters[0x200];
	real radius = (real)sqrt(radius_squared);
	short count = 0;

	if (query->cluster_index != NONE)
	{
		if (radius > 0.0f)
		{
			g_4e7414++;
			g_4e7411 = true;
			count = function_14a5b0(query->cluster_index, &query->point, radius, 0x200, clusters);
			g_4e7411 = false;
		}
		else
		{
			count = 1;
			clusters[0] = query->cluster_index;
		}
	}

	if (count > 0x200)
		count = 0x200;

	long cluster_count = count;
	s_cluster_reference *reference = (s_cluster_reference *)g_4ea950->data + (datum_index & 0xffff);
	for (long i = 0; i < cluster_count; i++)
	{
		long cluster_index = clusters[i];
		if (cluster_index != query->cluster_index)
		{
			long next = function_17d100(cluster_index, datum_index);
			if (next == NONE)
			{
				break;
			}
			reference->next = next;
			reference = (s_cluster_reference *)g_4ea950->data + (next & 0xffff);
		}
	}
}

// @retail 0x180c60
dword vector3d_pack(vector3f const *vector)
{
	long i = float_to_int_down(PIN(vector->i, -1.0f, 1.0f) * 1023.5f) & 0x7ff;
	long j = float_to_int_down(PIN(vector->j, -1.0f, 1.0f) * 1023.5f) & 0x7ff;
	long k = float_to_int_down(PIN(vector->k, -1.0f, 1.0f) * 511.5f);

	return (((k << 11) | j) << 11) | i;
}
