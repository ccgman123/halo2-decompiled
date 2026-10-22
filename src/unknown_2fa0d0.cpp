// @flags /O2 /GL-
// Havok (third-party physics library): moving to the end of a range every
// active entry whose surface fails the ray test, shrinking the range by one
// each time.
#include <xmmintrin.h>

struct answer_2f97e0
{
	char value;

	answer_2f97e0(bool value_) : value((char)value_) {}
};

struct ray_2f97e0
{
	__m128 start;
	__m128 direction;
};

struct surface_2f97e0
{
	__m128 point;
	__m128 direction;
};

struct limits_2f97e0;

// a vector copied as a whole
struct vector_2fa0d0
{
	__m128 value;

	vector_2fa0d0 &operator=(vector_2fa0d0 const &other)
	{
		value = other.value;
		return *this;
	}
};

struct entry_2fa0d0
{
	vector_2fa0d0 field_0;
	vector_2fa0d0 field_10;
	float field_20;
	float field_24;
	vector_2fa0d0 field_30;
	vector_2fa0d0 field_40;
	surface_2f97e0 surface;
	long field_70;
	long field_74;
	long field_78;
	char state;
	char field_7d;
};

struct entries_2fa0d0
{
	entry_2fa0d0 *data;
};

class ray_test_2f97e0
{
public:
	answer_2f97e0 function_2f97e0(ray_2f97e0 const *ray, surface_2f97e0 const *surface, limits_2f97e0 const *limits);
	int function_2fa0d0(ray_2f97e0 const *ray, entries_2fa0d0 *entries, int first, int end, limits_2f97e0 const *limits);
};

// score 144 (was 144): retail's frame is 0xb4 and it keeps the entries' array
// in esi with the entry's offset in edi; ours has a 0x94 frame and keeps the
// array in ebx, so both loops differ in their registers
// returns the range's new end
// @retail 0x2fa0d0
int ray_test_2f97e0::function_2fa0d0(ray_2f97e0 const *ray, entries_2fa0d0 *entries, int first, int end, limits_2f97e0 const *limits)
{
	for (int i = first; i < end;)
	{
		entry_2fa0d0 *data = entries->data;
		if (data[i].state == 1 && !function_2f97e0(ray, &data[i].surface, limits).value)
		{
			entry_2fa0d0 kept;
			kept = data[i];
			data[i] = data[end - 1];
			data[end - 1] = kept;
			end--;
		}
		else
		{
			i++;
		}
	}
	return end;
}
