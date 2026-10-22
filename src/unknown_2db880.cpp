// @flags /O2 /GL-
// Havok (third-party physics library): a body's velocities set to reach a
// position and a rotation within a step. (Its own file: retail calls the
// rotation helpers of src/unknown_2db630.cpp without knowing their bodies.)
#include <xmmintrin.h>
#include <math.h>
#include "unknown_1cec30.h"

// a vector of four copies of a float
inline __m128 splat_2db630(float value)
{
	__m128 const single = _mm_set_ss(value);
	return _mm_shuffle_ps(single, single, 0);
}

struct rotation_2db630
{
	__m128 v;

	void function_2db630(rotation_2db630 const &a, rotation_2db630 const &b);
	void function_2db6f0(rotation_2db630 &out) const;
};

struct vector_2db7a0
{
	__m128 v;

	void function_2db7a0(rotation_2db630 const &rotation, vector_2db7a0 const &vector);
};

// a rotation held by the game's component (0x2da2d0 sets it)
class c_component_rotation
{
public:
	hkVector4 value;
	void set(hkRotation const &rotation);
};

// a rotation normalized in place (0x1d8d60, game code)
struct s_1d8d60
{
	__m128 field_0;
	void function_1d8d60();
};

// a body's motion as 0x2db880 sees it
class motion_2db880
{
public:
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void slot_2() {}
	virtual void slot_3() {}
	virtual void slot_4() {}
	virtual void slot_5() {}
	virtual void slot_6() {}
	virtual void slot_7() {}
	virtual void slot_8() {}
	virtual void slot_9() {}
	virtual void slot_10() {}
	virtual void slot_11() {}
	virtual void slot_12() {}
	virtual void slot_13() {}
	virtual void slot_14() {}
	virtual void slot_15() {}
	virtual void slot_16() {}
	virtual void slot_17() {}
	virtual void slot_18() {}
	virtual void slot_19() {}
	// set the linear and the angular velocity
	virtual void slot_20(__m128 const *velocity) {}
	virtual void slot_21(__m128 const *velocity) {}

	// (the table takes 16 bytes in a class holding vectors)
	char unknown10[0x20 - 0x10];
	// the centre of mass, in the body's own space
	vector_2db7a0 field_20;
	rotation_2db630 rotation;
	char unknown40[0x70 - 0x40];
	// the centre of mass, in the world
	__m128 field_70;
};

// the angle whose cosine is given, clamped to 0 or pi outside -1 to 1
static inline float angle_2db880(float cosine)
{
	if (fabs(cosine) >= 1.0f)
	{
		if (cosine > 0.0f)
		{
			return 0.0f;
		}
		return 3.14159274f;
	}
	return (float)acos(cosine);
}

// the body's velocities set to bring it to the position and the rotation
// within a step of 1 / frequency: the centre of mass's offset, and the
// rotation still to make (none below a thousandth of a radian)
// @retail 0x2db880
// score 13 (was 13): retail reads the position before the frequency and keeps
// the offset in xmm0, the position in xmm1 and the home position in xmm2;
// ours reads the frequency first and keeps the home position in xmm1, the
// position in xmm2 and the offset in xmm3. Reordering the statements, the
// operands of the sum and of the product, splitting the difference in two
// steps and reading the values into locals early all leave the code exactly
// as it was, so the scheduler picks the order on its own.
void __cdecl function_2db880(hkVector4 const *position, c_component_rotation const *rotation, real frequency, hkRigidBody *body)
{
	__m128 linear;
	motion_2db880 *motion = (motion_2db880 *)body->m_motion;
	{
		vector_2db7a0 center;
		center.function_2db7a0(*(rotation_2db630 const *)rotation, motion->field_20);
		__m128 const target = position->m_quad;
		__m128 const home = motion->field_70;
		__m128 delta = _mm_sub_ps(_mm_add_ps(center.v, target), home);
		linear = _mm_mul_ps(splat_2db630(frequency), delta);
	}
	if (!body->body_is_simulating().m_bool && body->m_simulation_island)
	{
		body->activate();
	}
	rotation_2db630 angular;
	((motion_2db880 *)body->m_motion)->slot_20(&linear);
	{
		rotation_2db630 change;
		change.function_2db630(*(rotation_2db630 const *)rotation, ((motion_2db880 *)body->m_motion)->rotation);
		((s_1d8d60 *)&change)->function_1d8d60();
		float const angle = angle_2db880((float)fabs(change.v.m128_f32[3])) * 2.0f;
		if (angle < 0.001f)
		{
			angular.v = _mm_setzero_ps();
		}
		else
		{
			change.function_2db6f0(angular);
			angular.v = _mm_mul_ps(splat_2db630(frequency * angle), angular.v);
		}
	}
	if (!body->body_is_simulating().m_bool && body->m_simulation_island)
	{
		body->activate();
	}
	((motion_2db880 *)body->m_motion)->slot_21(&angular.v);
}
