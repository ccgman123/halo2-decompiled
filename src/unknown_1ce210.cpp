#include "unknown_11c920.h"
#include <xmmintrin.h>
// @flags /O2 /arch:SSE /Gr

__declspec(align(16)) const dword g_455940[4] = {0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff};
__declspec(align(16)) const dword g_4124a0[4] = {0x80000000, 0x80000000, 0x80000000, 0x80000000};

// score 13 (was 13): our build gives this function a custom convention (the first two
// arguments in EAX/EDX) while retail passes all five arguments on the stack.
// @retail 0x1ce210
void __cdecl function_1ce210(const __m128 *arg_0, const __m128 *arg_1, const __m128 *arg_2, real arg_3, __m128 *arg_4)
{
 const __m128 *local_7 = *(const __m128 *volatile const *)&arg_0;
 const __m128 *local_8 = arg_1;
 __m128 local_0 = *local_8;
 __m128 local_1 = *(__m128 *)g_455940;
 __m128 local_2 = local_7[2];
 __m128 local_5 = _mm_set1_ps(*(volatile real const *)&arg_3);
 __m128 local_3 = _mm_shuffle_ps(local_0, local_0, 0xaa);
 local_3 = _mm_mul_ps(local_3, local_2);
 __m128 local_4 = _mm_shuffle_ps(local_0, local_0, 0);
 local_2 = _mm_and_ps(local_3, local_1);
 local_3 = _mm_shuffle_ps(local_0, local_0, 0x55);
 local_2 = _mm_add_ps(local_5, local_2);
 local_3 = _mm_and_ps(_mm_mul_ps(local_3, local_7[1]), local_1);
 local_4 = _mm_and_ps(_mm_mul_ps(local_4, local_7[0]), local_1);
 arg_4[1] = _mm_add_ps(_mm_add_ps(local_4, local_3), local_2);
 arg_4[0] = _mm_xor_ps(arg_4[1], *(__m128 *)g_4124a0);
 local_0 = *arg_2;
 local_2 = _mm_add_ps(_mm_mul_ps(local_7[2], _mm_shuffle_ps(local_0, local_0, 0xaa)), local_7[3]);
 local_3 = _mm_mul_ps(local_7[1], _mm_shuffle_ps(local_0, local_0, 0x55));
 local_4 = _mm_mul_ps(local_7[0], _mm_shuffle_ps(local_0, local_0, 0));
 local_0 = _mm_add_ps(_mm_add_ps(local_4, local_3), local_2);
 arg_4[1] = _mm_add_ps(arg_4[1], local_0);
 arg_4[0] = _mm_add_ps(arg_4[0], local_0);
}
