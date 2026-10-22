// @flags /O2 /GL- /GX
// Havok (third-party physics library): arrays whose storage comes from the
// allocator's stack area, for elements of 2, 16, 4 and 1 bytes: their
// constructors, which take the storage from the stack area, and their
// destructors, which return it. An array
// holds its storage, its size, and its capacity, whose top bit says the array
// does not own the storage. (The array types are stamped out by macro: the
// build compiles template bodies where they cannot be inlined.) And the
// three-axis sweep's constructor.
#include "reference_counted_2d88a0.h"
#include <xmmintrin.h>
#include <new>
#include "tagged_objects_2e4ff0.h"
#include "sweep_quantize_2e1090.h"

#define ARRAY_2E1850(name, T) 	struct name 	{ 		T *data; 		long size; 		unsigned long capacity_and_flags; 		name() : data(0), size(0), capacity_and_flags(0x80000000) {} 		~name() 		{ 			if ((capacity_and_flags & 0x80000000) == 0) 			{ 				g_memory_2d88a0->release_block(data, (capacity_and_flags & 0x7fffffff) * sizeof(T), 0x12); 			} 		} 	};

#define STACK_ARRAY_2E1850(name, base, T) 	struct name : base 	{ 		T *stack_block; 		long stack_capacity; 		name(long capacity); 		~name(); 	};

// a block from the top of the allocator's stack area, or from its slot 9 when
// the area is too small
inline void *stack_allocate_2e1850(long size)
{
	memory_2d88a0 *memory = g_memory_2d88a0;
	long free = memory->stack_free;
	if (size > free)
	{
		return memory->allocate_stack_overflow(size);
	}
	char *block = memory->stack_cursor;
	memory->stack_cursor = block + size;
	memory->stack_free = free - size;
	return block;
}

ARRAY_2E1850(array_shorts_2e1850, short)
STACK_ARRAY_2E1850(shorts_2e1850, array_shorts_2e1850, short)

// @retail 0x2e1850
shorts_2e1850::shorts_2e1850(long capacity)
{
	void *block = stack_allocate_2e1850((capacity * sizeof(short) + 15) & ~15);
	data = (short *)block;
	capacity_and_flags = capacity | 0x80000000;
	stack_block = (short *)block;
	stack_capacity = capacity;
}

// @retail 0x2e18e0
shorts_2e1850::~shorts_2e1850()
{
	g_memory_2d88a0->release_stack(stack_block, (stack_capacity * sizeof(short) + 15) & ~15);
}

ARRAY_2E1850(array_vectors_2e1980, __m128)
STACK_ARRAY_2E1850(vectors_2e1980, array_vectors_2e1980, __m128)

// @retail 0x2e1980
vectors_2e1980::vectors_2e1980(long capacity)
{
	void *block = stack_allocate_2e1850((capacity * sizeof(__m128) + 15) & ~15);
	data = (__m128 *)block;
	capacity_and_flags = capacity | 0x80000000;
	stack_block = (__m128 *)block;
	stack_capacity = capacity;
}

// @retail 0x2e1a10
vectors_2e1980::~vectors_2e1980()
{
	g_memory_2d88a0->release_stack(stack_block, (stack_capacity * sizeof(__m128) + 15) & ~15);
}

ARRAY_2E1850(array_longs_2e1ab0, long)
STACK_ARRAY_2E1850(longs_2e1ab0, array_longs_2e1ab0, long)

// @retail 0x2e1ab0
longs_2e1ab0::longs_2e1ab0(long capacity)
{
	void *block = stack_allocate_2e1850((capacity * sizeof(long) + 15) & ~15);
	data = (long *)block;
	capacity_and_flags = capacity | 0x80000000;
	stack_block = (long *)block;
	stack_capacity = capacity;
}

// @retail 0x2e1b40
longs_2e1ab0::~longs_2e1ab0()
{
	g_memory_2d88a0->release_stack(stack_block, (stack_capacity * sizeof(long) + 15) & ~15);
}

ARRAY_2E1850(array_bytes_2e1be0, char)
STACK_ARRAY_2E1850(bytes_2e1be0, array_bytes_2e1be0, char)

// @retail 0x2e1be0
bytes_2e1be0::bytes_2e1be0(long capacity)
{
	void *block = stack_allocate_2e1850((capacity * sizeof(char) + 15) & ~15);
	data = (char *)block;
	capacity_and_flags = capacity | 0x80000000;
	stack_block = (char *)block;
	stack_capacity = capacity;
}

// @retail 0x2e1c70
bytes_2e1be0::~bytes_2e1be0()
{
	g_memory_2d88a0->release_stack(stack_block, (stack_capacity * sizeof(char) + 15) & ~15);
}

static __declspec(align(16)) const unsigned long sign_mask_2e1d10[4] = {0x80000000, 0x80000000, 0x80000000, 0x80000000};

// score 215 (was 235): the halving loop now goes through the two-level box
// quantizer and matches retail, the 32-bit `below 12` compare included. Still:
// retail keeps the first axis array's address (the vector constructor
// iterator's argument) in edi and the element array's in ebx, re-making the
// latter with lea after the loop; ours keeps the element array in edi and a
// zero in ebx, so the appends and pushes differ in registers (the axes or
// elements reached through this class's own members compile alike or worse,
// 257). In the scale block retail stores the 65532 splat to the member and
// reloads it before multiplying; a reference to the member gives that reload
// but scores 275, so the forwarded form is kept. One byte longer than retail.
// the sweep over the box from the corner at `arg_0` to the corner at
// `arg_1`, with a grid of 2 to the power `bits`, less one, cells: the
// quantizing scale and offsets, the largest value that still quantizes below
// 12 (found by halving), the first box and the axes' first endpoints, and a
// box with two x endpoints for each cell, then the axes' last endpoints
// @retail 0x2e1d10
class_2e4ff0::class_2e4ff0(long arg_0, long arg_1, int bits)
{
	class_2e1090 *sweep = (class_2e1090 *)this;
	sweep->elements.ensure_capacity(0x100);
	{
		sweep->lower_offset = _mm_setzero_ps();
		sweep->upper_offset = _mm_setzero_ps();
		sweep->scale = _mm_set_ps1(1.0f);
		float low = 10.0f;
		float high = 11.0f;
		bounds_2e0020 probe;
		quantized_box_2e1090 quantized;
		for (int step = 0; step < 23; step++)
		{
			float const middle = (low + high) * 0.5f;
			probe.lower = _mm_set_ps1(middle);
			probe.upper = _mm_set_ps1(middle + 1.0f);
			quantize_box_2e1090(sweep, probe, quantized);
			if (quantized.high[0] < 12)
			{
				low = middle;
			}
			else
			{
				high = middle;
			}
		}
		sweep->field_7c = (low + high) * 0.5f - 11.0f;
	}
	{
		__m128 const extent = _mm_sub_ps(*(__m128 const *)arg_1, *(__m128 const *)arg_0);
		__m128 inverse;
		inverse.m128_f32[0] = 1.0f / extent.m128_f32[0];
		inverse.m128_f32[1] = 1.0f / extent.m128_f32[1];
		inverse.m128_f32[2] = 1.0f / extent.m128_f32[2];
		inverse.m128_f32[3] = 0.0f;
		sweep->scale = _mm_set_ps1(65532.0f);
		sweep->scale = _mm_mul_ps(sweep->scale, inverse);
		sweep->lower_offset = _mm_xor_ps(*(__m128 const *)arg_0, *(__m128 const *)sign_mask_2e1d10);
		__m128 const margin = _mm_mul_ps(_mm_set_ps1(1.52597204e-05f), extent);
		sweep->upper_offset = _mm_add_ps(sweep->lower_offset, margin);
		sweep->scale.m128_f32[3] = 0.0f;
		sweep->lower_offset.m128_f32[3] = 0.0f;
		sweep->upper_offset.m128_f32[3] = 0.0f;
	}
	element_2e1090 &first = sweep->elements.next_slot();
	first.slots[4] = first.slots[0] = first.slots[1] = 0;
	sweep->axes[0].ensure_capacity(0x202);
	sweep->axes[1].ensure_capacity(0x202);
	sweep->axes[2].ensure_capacity(0x202);
	endpoint_2e0f80 start;
	start.id = 0;
	start.value = 0;
	sweep->axes[0].data[sweep->axes[0].size++] = start;
	sweep->axes[1].data[sweep->axes[1].size++] = start;
	sweep->axes[2].data[sweep->axes[2].size++] = start;
	sweep->cells = 0;
	sweep->cell_shift = bits;
	sweep->cells_used = (1 << bits) - 1;
	if (sweep->cells_used != 0)
	{
		sweep->cells = (cell_2e1090 *)g_memory_2d88a0->allocate(sweep->cells_used * sizeof(cell_2e1090), 0x1c);
	}
	for (long cell = 0; cell < sweep->cells_used; cell++)
	{
		entry_2e4ff0 *entry = new (&entries[cell]) entry_2e4ff0;
		endpoint_2e0f80 endpoint;
		endpoint.id = (unsigned short)sweep->elements.size;
		((unsigned short *)&entry->field_0)[0] = endpoint.id;
		endpoint.value = (unsigned short)((cell + 1) << (16 - sweep->cell_shift));
		((unsigned short *)&entry->field_0)[1] = endpoint.value;
		element_2e1090 &element = sweep->elements.next_slot();
		element.slots[4] = (unsigned short)sweep->axes[0].size;
		sweep->axes[0].data[sweep->axes[0].size++] = endpoint;
		endpoint.value |= 1;
		element.slots[5] = (unsigned short)sweep->axes[0].size;
		sweep->axes[0].data[sweep->axes[0].size++] = endpoint;
		element.owner = (cell * sizeof(cell_2e1090)) | 1;
		element.slots[0] = element.slots[1] = 0;
		element.slots[2] = element.slots[3] = 1;
	}
	first.slots[5] = (unsigned short)sweep->axes[0].size;
	first.slots[2] = (unsigned short)sweep->axes[1].size;
	first.slots[3] = (unsigned short)sweep->axes[2].size;
	endpoint_2e0f80 end;
	end.id = 0;
	end.value = 0xfffd;
	sweep->axes[0].data[sweep->axes[0].size++] = end;
	sweep->axes[1].data[sweep->axes[1].size++] = end;
	sweep->axes[2].data[sweep->axes[2].size++] = end;
}
