// @flags /O2 /GL-
// Havok (third-party physics library): the closest points of two infinite
// lines, each given by a point and a direction.
#include <math.h>
#include <xmmintrin.h>

// three-term dot product of two vectors
inline float dot3_2dddf0(__m128 const &a, __m128 const &b)
{
	__m128 const product = _mm_mul_ps(a, b);
	float result;
	_mm_store_ss(&result, _mm_add_ss(_mm_shuffle_ps(product, product, 0xaa), _mm_add_ss(_mm_shuffle_ps(product, product, 0x55), product)));
	return result;
}

class line_query_2dddf0
{
public:
	void function_2dddf0(__m128 const *first_point, __m128 const *first_direction, __m128 const *second_point, __m128 const *second_direction,
		float *distance_squared, float *first_fraction, float *second_fraction, __m128 *first_closest, __m128 *second_closest);
};

// the fractions along each direction of the closest points, the points and
// the square of their distance; nearly parallel lines take the first line's
// point as its closest
// score 21 (was 35): the first closest point is added with the scaled direction
// as the first operand, so the direction is loaded first; retail still loads the
// second scale's value before the first multiply and keeps the two points in
// different registers.
// @retail 0x2dddf0
void line_query_2dddf0::function_2dddf0(__m128 const *first_point, __m128 const *first_direction, __m128 const *second_point, __m128 const *second_direction,
	float *distance_squared, float *first_fraction, float *second_fraction, __m128 *first_closest, __m128 *second_closest)
{
	__m128 const between = _mm_sub_ps(*second_point, *first_point);
	float const directions = dot3_2dddf0(*first_direction, *second_direction);
	float const first_along = dot3_2dddf0(*first_direction, between);
	float const second_along = dot3_2dddf0(*second_direction, between);
	float const first_length_squared = dot3_2dddf0(*first_direction, *first_direction);
	float const second_length_squared = dot3_2dddf0(*second_direction, *second_direction);
	float const lengths = second_length_squared * first_length_squared;
	float const directions_squared = directions * directions;
	float const determinant = lengths - directions_squared;
	if (fabs(determinant) > (fabs(lengths) + directions_squared) * 9.5367431640625e-7f)
	{
		*first_fraction = (second_length_squared * first_along - second_along * directions) / determinant;
	}
	else
	{
		*first_fraction = 0.0f;
	}
	float const second = (directions * *first_fraction - second_along) / second_length_squared;
	*second_fraction = second;
	__m128 const first_scale = _mm_set_ss(*first_fraction);
	__m128 const first_on_line = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(first_scale, first_scale, 0), *first_direction), *first_point);
	__m128 const second_scale = _mm_set_ss(second);
	__m128 const second_on_line = _mm_add_ps(*second_point, _mm_mul_ps(_mm_shuffle_ps(second_scale, second_scale, 0), *second_direction));
	*first_closest = first_on_line;
	__m128 const offset = _mm_sub_ps(first_on_line, second_on_line);
	*second_closest = second_on_line;
	*distance_squared = dot3_2dddf0(offset, offset);
}
