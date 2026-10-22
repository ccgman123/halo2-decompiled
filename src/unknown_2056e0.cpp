// @flags /O2 /Oi /arch:SSE /Gr
/* UNKNOWN_2056E0.CPP: applying a vehicle's physics step: the forces of its
   contacts and the caller's force and torque, a braking that pulls its
   velocities back (capped), and the flags of which contacts touch */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1cec30.h"
#include "unknown_1428b0.h"
#include <math.h>

struct s_vehicle_physics_state;

/* the physics state as this step reads it (vehicles.cpp) */
struct s_vehicle_step_definition
{
	byte unknown00[0x44];
	long contact_count;
};

struct s_vehicle_step_contact
{
	byte unknown00[0x80];
	bool touching;
	bool lifted;
	byte unknown82[0xd8 - 0x82];
};

struct s_vehicle_step_state
{
	byte unknown000[4];
	s_vehicle_step_definition *definition;
	real scale;
	matrix3x3 rotation;
	byte unknown030[0x6c - 0x30];
	real mass;
	byte unknown070[0xb10 - 0x70];
	s_vehicle_step_contact contacts[16];
};

/* the vehicle object as this step reads it */
struct s_vehicle_step_object
{
	byte unknown000[0xb4];
	long havok_component_index;
	byte unknown0b8[0x38e - 0xb8];
	word contact_flags;
};

struct s_vehicle_step_header
{
	byte unknown00[8];
	s_vehicle_step_object *object;
};

/* the motion's virtual slot that gives its local inverse inertia */
class c_vehicle_step_motion
{
public:
	virtual void slot00(void) {}
	virtual void slot01(void) {}
	virtual void slot02(void) {}
	virtual void slot03(void) {}
	virtual void slot04(void) {}
	virtual void slot05(void) {}
	virtual void slot06(void) {}
	virtual void slot07(void) {}
	virtual void slot08(void) {}
	virtual void slot09(void) {}
	virtual void slot0a(void) {}
	virtual void slot0b(void) {}
	virtual void slot0c(void) {}
	virtual void inverse_inertia_local(hkRotation &inertia) const {}
};

bool __stdcall function_206090(s_vehicle_physics_state *state, vector3f *linear, vector3f *angular);
void function_ba1d0(long object_index, vector3f *linear_velocity, vector3f *angular_velocity);
void __stdcall function_b77d0(long object_index, vector3f const *linear_velocity, vector3f const *angular_velocity);
matrix3x3 *function_142eb0(matrix3x3 const *right, matrix3x3 const *left, matrix3x3 *out);

/* scales a vector down to the given length when it is longer */
PRIVATE __forceinline void vector_limit_length(vector3f *v, real maximum)
{
	real length_squared = v->k * v->k + v->j * v->j + v->i * v->i;
	if (length_squared > maximum * maximum)
	{
		real scale = (real)(maximum / sqrt(length_squared));
		v->i = (real)(v->i * scale);
		v->j = (real)(v->j * scale);
		v->k = (real)(v->k * scale);
	}
}

/* the motion's local inverse inertia as a matrix3x3; the 16-byte aligned
   Havok rotation it reads lives in this inlined helper, not in the caller's
   own locals (written in the caller, it changes the conventions LTCG gives
   the caller's callees) */
PRIVATE __forceinline void motion_inverse_inertia_local(c_vehicle_step_motion *motion, matrix3x3 *out)
{
	hkRotation inertia;
	motion->inverse_inertia_local(inertia);
	out->forward.i = inertia.m_col0(0);
	out->forward.j = inertia.m_col0(1);
	out->forward.k = inertia.m_col0(2);
	out->left.i = inertia.m_col1(0);
	out->left.j = inertia.m_col1(1);
	out->left.k = inertia.m_col1(2);
	out->up.i = inertia.m_col2(0);
	out->up.j = inertia.m_col2(1);
	out->up.k = inertia.m_col2(2);
}

// @retail 0x2056e0
void __stdcall function_2056e0(long vehicle_index, s_vehicle_physics_state *state, real braking,
	vector3f const *force, vector3f const *torque)
{
	real dt = g_510c54->rate;
	s_vehicle_step_object *vehicle = ((s_vehicle_step_header *)g_4e0300->data)[vehicle_index & 0xffff].object;
	s_havok_component *component = havok_component_get(vehicle->havok_component_index);
	s_vehicle_step_state *step = (s_vehicle_step_state *)state;
	vector3f linear_change;
	vector3f angular_change;
	if (!function_206090(state, &linear_change, &angular_change))
		braking = 0.0f;
	if (force)
	{
		linear_change.i = force->i + linear_change.i;
		linear_change.j = force->j + linear_change.j;
		linear_change.k = force->k + linear_change.k;
	}
	if (torque)
	{
		angular_change.i = torque->i + angular_change.i;
		angular_change.j = torque->j + angular_change.j;
		angular_change.k = torque->k + angular_change.k;
	}
	vector3f linear_velocity;
	vector3f angular_velocity;
	function_ba1d0(vehicle_index, &linear_velocity, &angular_velocity);
	c_vehicle_step_motion *motion = (c_vehicle_step_motion *)component->rigid_bodies.data[0].rigid_body->m_motion;
	real inverse_mass = 1.0f / step->mass;
	linear_change.i *= inverse_mass;
	linear_change.j *= inverse_mass;
	linear_change.k *= inverse_mass;
	matrix3x3 inverse_inertia;
	motion_inverse_inertia_local(motion, &inverse_inertia);
	{
		matrix3x3 rotation = step->rotation;
		function_142eb0(&rotation, &inverse_inertia, &inverse_inertia);
		function_142eb0(&inverse_inertia, function_142b80(&rotation, &rotation), &inverse_inertia);
	}
	vector3f local_461a4b_2;
	for (long axis = 0; axis < 3; axis++)
		local_461a4b_2.n[axis] = inverse_inertia.up.n[axis] * angular_change.k +
			inverse_inertia.left.n[axis] * angular_change.j + inverse_inertia.forward.n[axis] * angular_change.i;
	linear_velocity.i += linear_change.i * dt;
	linear_velocity.j += linear_change.j * dt;
	linear_velocity.k += linear_change.k * dt;
	angular_velocity.i += local_461a4b_2.i * dt;
	angular_velocity.j += local_461a4b_2.j * dt;
	angular_velocity.k += local_461a4b_2.k * dt;
	if (braking > 0.0f)
	{
		real scale = 0.0f - braking / dt;
		vector3f linear_brake;
		linear_brake.i = linear_velocity.i * scale;
		linear_brake.j = linear_velocity.j * scale;
		linear_brake.k = 0.0f;
		vector_limit_length(&linear_brake, 270.0f);
		vector3f angular_brake;
		angular_brake.i = angular_velocity.i * scale;
		angular_brake.j = angular_velocity.j * scale;
		angular_brake.k = angular_velocity.k * scale;
		vector_limit_length(&angular_brake, 18.0f);
		linear_velocity.i += linear_brake.i * dt;
		linear_velocity.j += linear_brake.j * dt;
		linear_velocity.k += linear_brake.k * dt;
		angular_velocity.i += angular_brake.i * dt;
		angular_velocity.j += angular_brake.j * dt;
		angular_velocity.k += angular_brake.k * dt;
	}
	function_b77d0(vehicle_index, &linear_velocity, &angular_velocity);
	vehicle->contact_flags = 0;
	for (long i = 0; i < step->definition->contact_count; i++)
	{
		if (i >= 0x10000)
			break;
		if (!step->contacts[i].lifted && step->contacts[i].touching)
			vehicle->contact_flags &= ~(1 << i);
		else
			vehicle->contact_flags |= 1 << i;
	}
}
