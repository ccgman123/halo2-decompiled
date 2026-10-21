// @flags /O2 /GL-
// Havok (third-party physics library): combines the mass properties of
// several placed parts into one: total volume and mass, the mass-weighted
// centre, and the inertia tensor of every part turned into the common frame
// and shifted to the common centre.
// (Its own file: in 0x2db9e0's file the compiler inlines that callee.)
#include <xmmintrin.h>

// the 3x3 rotation part of a transform, as the matched callee declares it
struct hkRotation;

// a point transformed by a matrix (0x2da610)
struct c_transformed_point
{
	__m128 value;
	void transform(const void *matrix, const __m128 *point);
};

// a 3x3 matrix turned by a rotation: R * M * R^T (0x2da9a0)
struct s_2da8c0
{
	__m128 columns[3];
	void function_2da9a0(hkRotation const *rotation);
};

// one 3x4 matrix added to another (0x2da880)
class class_2da880
{
public:
	__m128 rows[3];

	void add(const class_2da880 &other);
};

void __cdecl function_2db9e0(float const *point, float mass, float *inertia);

struct s_physics_mass
{
	float volume;
	float mass;
	char unknown08[8];
	__m128 center;
	__m128 inertia[3];
};

struct s_physics_mass_entry
{
	s_physics_mass mass;
	__m128 rotation[3];
	__m128 translation;
};

struct s_physics_mass_array
{
	s_physics_mass_entry *data;
	long size;
	long capacity;
};

static inline __m128 splat_2dba80(float value)
{
	__m128 single = _mm_set_ss(value);
	return _mm_shuffle_ps(single, single, 0);
}

// score 5 (was 76): in the second loop retail loads array->data before the
// subtraction and stores the offset before it pushes &inertia; ours loads it
// after and pushes first. Retail copies the total mass as bits, hence the
// long copy (a float copy goes through the x87 and scores 32). A 10-minute
// anneal found nothing.
// 1 when the parts have no positive total mass, otherwise 0 with the result
// filled in
// @retail 0x2dba80
long __cdecl function_2dba80(s_physics_mass_array const *array, s_physics_mass *result)
{
	__m128 const zero = _mm_setzero_ps();
	__m128 center = zero;
	float total_mass = 0.0f;
	float total_volume = 0.0f;
	c_transformed_point centre_in_world;
	for (int index = 0; index < array->size; index++)
	{
		centre_in_world.transform(array->data[index].rotation, &array->data[index].mass.center);
		total_mass += array->data[index].mass.mass;
		total_volume += array->data[index].mass.volume;
		center = _mm_add_ps(center, _mm_mul_ps(splat_2dba80(array->data[index].mass.mass), centre_in_world.value));
	}
	if (total_mass <= 0.0f)
	{
		return 1;
	}
	float const inverse_mass = 1.0f / total_mass;
	*(long *)&result->mass = *(long const *)&total_mass;
	result->volume = total_volume;
	center = _mm_mul_ps(splat_2dba80(inverse_mass), center);
	result->center = center;
	class_2da880 *sum = (class_2da880 *)result->inertia;
	sum->rows[0] = zero;
	sum->rows[1] = zero;
	sum->rows[2] = zero;
	for (int part = 0; part < array->size; part++)
	{
		s_2da8c0 inertia;
		inertia.columns[0] = array->data[part].mass.inertia[0];
		inertia.columns[1] = array->data[part].mass.inertia[1];
		inertia.columns[2] = array->data[part].mass.inertia[2];
		inertia.function_2da9a0((hkRotation const *)array->data[part].rotation);
		centre_in_world.transform(array->data[part].rotation, &array->data[part].mass.center);
		centre_in_world.value = _mm_sub_ps(centre_in_world.value, center);
		function_2db9e0((float const *)&centre_in_world, array->data[part].mass.mass, (float *)&inertia);
		sum->add(*(class_2da880 const *)&inertia);
	}
	return 0;
}
