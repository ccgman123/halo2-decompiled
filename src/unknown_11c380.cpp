// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_11C380.CPP: the scenario's trigger volumes, as boxes placed in the
   world or on an object's node (outside functions lane A's script functions
   need) */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
#include "object_queries.h"

/* a trigger volume of the scenario (0x44 bytes) */
struct s_trigger_volume
{
	byte unknown00[4];
	short object_name_index;
	byte unknown06[2];
	long node_name;
	vector3f forward;
	vector3f up;
	point3f position;
	vector3f extents;
	real radius;
	byte unknown40[0x44 - 0x40];
};

struct s_scenario_trigger_volumes_11c380
{
	byte unknown000[0x108];
	long trigger_volume_count;
	s_trigger_volume *trigger_volumes;
};

struct s_object_11c380
{
	long definition_index;
	byte unknown004[0x30 - 4];
	point3f center;
	byte unknown03c[0x116 - 0x3c];
	short node_matrices_offset;
};

struct s_object_header_11c380
{
	byte unknown00[8];
	s_object_11c380 *object;
};

struct s_object_definition_11c380
{
	byte unknown00[0x38];
	long model_index;
};

#define OBJECT_GET_11C380(index) (((s_object_header_11c380 *)g_4e0300->data)[(index) & 0xffff].object)

long function_bb760(short index);
long render_model_find_named_entry(long render_model_index, long name);
void function_1420f0(transform4x3f *out, point3f const *position, vector3f const *forward, vector3f const *up);
int __fastcall function_142a60(transform4x3f const *a, transform4x3f const *b, transform4x3f *result);

/* the matrix of a trigger volume: placed in the world, or on a node of the
   object it names */
// @retail 0x11c380
bool function_11c380(long trigger_volume_index, transform4x3f *matrix)
{
	s_trigger_volume *trigger_volume = &((s_scenario_trigger_volumes_11c380 *)g_4e0350)->trigger_volumes[trigger_volume_index];
	bool result = false;

	if (trigger_volume->object_name_index != NONE)
	{
		long object_index = function_bb760(trigger_volume->object_name_index);
		if (object_index != NONE)
		{
			s_object_11c380 *object = OBJECT_GET_11C380(object_index);
			s_object_definition_11c380 *definition = (s_object_definition_11c380 *)g_4e3b44[object->definition_index & 0xffff].bytes;
			long node_index = render_model_find_named_entry(definition->model_index, trigger_volume->node_name);
			if (node_index != NONE)
			{
				function_1420f0(matrix, &trigger_volume->position, &trigger_volume->forward, &trigger_volume->up);
				object = OBJECT_GET_11C380(object_index);
				transform4x3f *field_50 = (transform4x3f *)((byte *)object + object->node_matrices_offset);
				function_142a60(&field_50[(short)node_index], matrix, matrix);
				result = true;
			}
		}
	}
	else
	{
		function_1420f0(matrix, &trigger_volume->position, &trigger_volume->forward, &trigger_volume->up);
		result = true;
	}
	return result;
}

PRIVATE __forceinline vector3f *function_11c471(real arg_0, vector3f const *arg_1, vector3f *arg_2)
{
	arg_2->i = arg_0 * arg_1->i;
	arg_2->j = arg_0 * arg_1->j;
	arg_2->k = arg_0 * arg_1->k;
	return arg_2;
}

/* whether a point is inside a trigger volume */
// @retail 0x11c470
bool function_11c470(long trigger_volume_index, point3f const *point)
{
	transform4x3f matrix;
	bool result = false;

	if (function_11c380(trigger_volume_index, &matrix))
	{
		s_trigger_volume *trigger_volume = &((s_scenario_trigger_volumes_11c380 *)g_4e0350)->trigger_volumes[trigger_volume_index];
		if (matrix.scale != 0.0f)
		{
			vector3f vector;
			vector3d_from_points3d(&matrix.position, point, &vector);
			if (matrix.scale != 1.0f)
			{
				real inverse_scale = 1.0f / matrix.scale;
				function_11c471(inverse_scale, &vector, &vector);
			}
			real x = dot3f(&vector, &matrix.forward);
			real y = dot3f(&vector, &matrix.left);
			real z = dot3f(&vector, &matrix.up);
			result = x > 0.0f && y > 0.0f && z > 0.0f &&
				trigger_volume->extents.i > x && trigger_volume->extents.j > y && trigger_volume->extents.k > z;
		}
	}
	return result;
}

long function_1ded60(void);
// @retail 0x11c5b0
bool function_11c5b0(long object_index, long trigger_volume_index)
{
	bool result = false;
	if (object_index != NONE && function_11c470(trigger_volume_index, &OBJECT_GET_11C380(object_index)->center))
		result = true;
	return result;
}

void function_1dedb0(long list_index, long object_index);
void function_11bed0(s_location *location, point3f const *point);
short __stdcall function_bb050(long a, unsigned long type_mask, void const *location, point3f const *position, float radius, long *objects, short maximum_count);

/* unknown_0259d0's inline point transform */
static inline point3f *transform4x3f_apply_point(transform4x3f const *matrix, point3f const *point, point3f *out)
{
	real x = point->x;
	real y = point->y;
	real z = point->z;

	if (matrix->scale != 1.0f)
	{
		x = matrix->scale * x;
		y = matrix->scale * y;
		z = matrix->scale * z;
	}
	real result_x = matrix->left.i * y;
	result_x += matrix->forward.i * x;
	result_x += matrix->up.i * z;
	out->x = result_x + matrix->position.x;
	out->y = matrix->up.j * z + matrix->left.j * y + matrix->forward.j * x + matrix->position.y;
	out->z = matrix->up.k * z + matrix->left.k * y + matrix->forward.k * x + matrix->position.z;
	return out;
}

/* a new object list of the objects (of the types of a mask) inside a trigger
   volume */
// @retail 0x11c5f0
long function_11c5f0(long trigger_volume_index, long type_mask)
{
	long list_index = function_1ded60();
	transform4x3f matrix;

	if (function_11c380(trigger_volume_index, &matrix))
	{
		s_trigger_volume *trigger_volume = &((s_scenario_trigger_volumes_11c380 *)g_4e0350)->trigger_volumes[trigger_volume_index];
		point3f center;
		s_location location;

		function_11c471(0.5f, &trigger_volume->extents, (vector3f *)&center);
		transform4x3f_apply_point(&matrix, &center, &center);
		function_11bed0(&location, &center);
		if (location.cluster_index != NONE)
		{
			long objects[128];
			long count = function_bb050(0, type_mask, &location, &center, trigger_volume->radius, objects, 128);
			for (long i = 0; i < count; i++)
			{
				long object_index = objects[i];
				if (object_index != NONE &&
					function_11c470(trigger_volume_index, &OBJECT_GET_11C380(object_index)->center))
				{
					function_1dedb0(list_index, object_index);
				}
			}
		}
	}
	return list_index;
}
