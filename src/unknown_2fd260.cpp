// @flags /O2 /GL-
// Havok (third-party physics library): the support vertex of a convex vertex
// set stored four vertices to a block, one term at a time.
#include <float.h>
#include <xmmintrin.h>

// four vertices: their x terms, then their y terms, then their z terms
struct vertex_block_2fd260
{
	__m128 x;
	__m128 y;
	__m128 z;
};

class convex_vertices_2fd260
{
public:
	char unknown0[0x30];
	vertex_block_2fd260 const *blocks;
	int block_count;

	void function_2fd260(__m128 const *direction, __m128 *support) const;
	void function_2fd430(unsigned short const *indices, int count, __m128 *vertices) const;

	// a vertex by index, with its index in the fourth term tagged with the
	// bits of 0.5
	void fetch_vertex(int index, __m128 *vertex) const
	{
		vertex_block_2fd260 const *block = &blocks[index >> 2];
		int const lane = index & 3;
		vertex->m128_f32[0] = block->x.m128_f32[lane];
		vertex->m128_f32[1] = block->y.m128_f32[lane];
		vertex->m128_f32[2] = block->z.m128_f32[lane];
		((int *)vertex)[3] = index | 0x3f000000;
	}
};

// the vertex furthest along a direction, with its index in the fourth term
// tagged with the bits of 0.5
// score 50 (was 50): the block's z, y and x terms are read from one block
// earlier than retail's, the largest float is set up after the shift in retail
// and before it here, and the direction's terms come out of the registers in
// another order.
// @retail 0x2fd260
void convex_vertices_2fd260::function_2fd260(__m128 const *direction, __m128 *support) const
{
	vertex_block_2fd260 const *first = blocks;
	vertex_block_2fd260 const *block = first;
	int const vertex_count = block_count * 4;
	__m128 best = _mm_set_ps1(-FLT_MAX);
	int best_index = 0;
	for (int i = 0; i < vertex_count; i += 4, block++)
	{
		__m128 const direction_z = _mm_set_ps1(direction->m128_f32[2]);
		__m128 const direction_y = _mm_set_ps1(direction->m128_f32[1]);
		__m128 const direction_x = _mm_set_ps1(direction->m128_f32[0]);
		__m128 const z = _mm_mul_ps(direction_z, block->z);
		__m128 const y = _mm_mul_ps(direction_y, block->y);
		__m128 const x = _mm_mul_ps(direction_x, block->x);
		__m128 dots = _mm_add_ps(_mm_add_ps(x, y), z);
		int const better = _mm_movemask_ps(_mm_cmplt_ps(best, dots));
		if (better != 0)
		{
			float best_dot;
			switch (better)
			{
			default:
				if (dots.m128_f32[0] > dots.m128_f32[1])
				{
					if (dots.m128_f32[0] > dots.m128_f32[2])
					{
						if (dots.m128_f32[0] > dots.m128_f32[3])
						{
							best_dot = dots.m128_f32[0];
							best_index = i;
						}
						else
						{
							best_dot = dots.m128_f32[3];
							best_index = i + 3;
						}
					}
					else if (dots.m128_f32[2] > dots.m128_f32[3])
					{
						best_dot = dots.m128_f32[2];
						best_index = i + 2;
					}
					else
					{
						best_dot = dots.m128_f32[3];
						best_index = i + 3;
					}
				}
				else if (dots.m128_f32[1] > dots.m128_f32[2])
				{
					if (dots.m128_f32[1] > dots.m128_f32[3])
					{
						best_dot = dots.m128_f32[1];
						best_index = i + 1;
					}
					else
					{
						best_dot = dots.m128_f32[3];
						best_index = i + 3;
					}
				}
				else if (dots.m128_f32[2] > dots.m128_f32[3])
				{
					best_dot = dots.m128_f32[2];
					best_index = i + 2;
				}
				else
				{
					best_dot = dots.m128_f32[3];
					best_index = i + 3;
				}
				break;
			case 0:
			case 1:
				best_dot = dots.m128_f32[0];
				best_index = i;
				break;
			case 2:
				best_dot = dots.m128_f32[1];
				best_index = i + 1;
				break;
			case 4:
				best_dot = dots.m128_f32[2];
				best_index = i + 2;
				break;
			case 8:
				best_dot = dots.m128_f32[3];
				best_index = i + 3;
				break;
			case 13:
				if (dots.m128_f32[0] > dots.m128_f32[2])
				{
					if (dots.m128_f32[0] > dots.m128_f32[3])
					{
						best_dot = dots.m128_f32[0];
						best_index = i;
					}
					else
					{
						best_dot = dots.m128_f32[3];
						best_index = i + 3;
					}
				}
				else if (dots.m128_f32[2] > dots.m128_f32[3])
				{
					best_dot = dots.m128_f32[2];
					best_index = i + 2;
				}
				else
				{
					best_dot = dots.m128_f32[3];
					best_index = i + 3;
				}
				break;
			case 14:
				if (dots.m128_f32[1] > dots.m128_f32[2])
				{
					if (dots.m128_f32[1] > dots.m128_f32[3])
					{
						best_dot = dots.m128_f32[1];
						best_index = i + 1;
					}
					else
					{
						best_dot = dots.m128_f32[3];
						best_index = i + 3;
					}
				}
				else if (dots.m128_f32[2] > dots.m128_f32[3])
				{
					best_dot = dots.m128_f32[2];
					best_index = i + 2;
				}
				else
				{
					best_dot = dots.m128_f32[3];
					best_index = i + 3;
				}
				break;
			}
			best = _mm_set_ps1(best_dot);
		}
	}
	vertex_block_2fd260 const *found = &first[best_index >> 2];
	int const lane = best_index & 3;
	support->m128_f32[0] = found->x.m128_f32[lane];
	support->m128_f32[1] = found->y.m128_f32[lane];
	support->m128_f32[2] = found->z.m128_f32[lane];
	((int *)support)[3] = best_index | 0x3f000000;
}

// the vertices for a list of indices
// score 147 (was 147): retail takes four vertices per step and writes each
// vertex's four words through a register, where ours writes them straight to
// memory and takes one vertex per step.
// @retail 0x2fd430
void convex_vertices_2fd260::function_2fd430(unsigned short const *indices, int count, __m128 *vertices) const
{
	for (int i = count - 1; i >= 0; i--)
	{
		fetch_vertex(*indices, vertices);
		indices++;
		vertices++;
	}
}
