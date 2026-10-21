// @flags /O2 /GL-
// Havok (third-party physics library): a push at a point; accumulating a vector and setting
// its w to one; and a step of a body's velocities: the accumulated push and
// turn applied and cleared, gravity added, then both velocities damped.
#include <xmmintrin.h>

// a vector of four copies of a float
inline __m128 splat_319cd0(float value)
{
	__m128 const single = _mm_set_ss(value);
	return _mm_shuffle_ps(single, single, 0);
}

inline float larger_319cd0(float a, float b)
{
	return a > b ? a : b;
}

// a vector updated in place
struct vector_319cd0
{
	__m128 value;

	void add_product(__m128 const &a, __m128 const &b) { value = _mm_add_ps(value, _mm_mul_ps(a, b)); }
	void scale(__m128 const &a) { value = _mm_mul_ps(a, value); }
};

class class_319ca0
{
public:
	char unknown0[0x10];
	float inverse_inertia;
	float linear_damping;
	float angular_damping;
	char unknown1c[0x10];
	float inverse_mass;
	char unknown30[0x10];
	vector_319cd0 linear_velocity;
	vector_319cd0 angular_velocity;
	char unknown60[0x10];
	__m128 position;
	char unknown80[0x40];
	__m128 push;
	__m128 sum;

	void push_at(const __m128 &force, const __m128 &point);
	void add(const __m128 &vector);
	void function_319cd0(float const *step, __m128 const *gravity);
};

// the cross product of two vectors
inline __m128 cross_319c20(__m128 const &a, __m128 const &b)
{
	return _mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(a, a, 0xc9), _mm_shuffle_ps(b, b, 0xd2)),
		_mm_mul_ps(_mm_shuffle_ps(a, a, 0xd2), _mm_shuffle_ps(b, b, 0xc9)));
}

// a push at a point: the push, and the turn it gives about the position
// @retail 0x319c20
void class_319ca0::push_at(const __m128 &force, const __m128 &point)
{
	push = _mm_add_ps(push, force);
	__m128 const &here = position;
	add(cross_319c20(_mm_sub_ps(point, here), force));
}
// @retail 0x319ca0
void class_319ca0::add(const __m128 &vector)
{
	sum = _mm_add_ps(sum, vector);
	sum.m128_f32[3] = 1.0f;
}

// @retail 0x319cd0
void class_319ca0::function_319cd0(float const *step, __m128 const *gravity)
{
	float const t = *step;
	linear_velocity.add_product(splat_319cd0(t * inverse_mass), push);
	linear_velocity.add_product(splat_319cd0(t), *gravity);
	linear_velocity.scale(splat_319cd0(larger_319cd0(0.0f, 1.0f - t * linear_damping)));
	push = _mm_setzero_ps();
	angular_velocity.add_product(splat_319cd0(t * inverse_inertia), sum);
	sum = _mm_setzero_ps();
	angular_velocity.scale(splat_319cd0(larger_319cd0(0.0f, 1.0f - t * angular_damping)));
}
