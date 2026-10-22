#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include <math.h>

// @flags /O2 /arch:SSE /Gr

struct s_prism_245270
{
	long field_00;
	long field_04;
	long field_08;
	byte field_0c;
	byte field_0d;
	short field_0e;
	plane3f field_10;
	real field_20;
	short field_24;
	byte field_26;
	byte field_27;
	long field_28;
	point2f field_2c[8];
};

struct s_collection_245270
{
	short field_00;
	short field_02;
	short field_04;
	byte field_06[0x4c08 - 6];
	s_prism_245270 field_4c08[256];
};

short function_120850(vector3f const *v);

PRIVATE inline void function_245270_project(point3f const *point, short axis,
	byte side, point2f *out)
{
	real x = point->n[g_440b94[axis * 2 + side][0]];
	real y = point->n[g_440b94[axis * 2 + side][1]];
	out->x = x;
	out->y = y;
}

PRIVATE __forceinline void function_24526f(real thickness, short count, point3f const *points, plane3f const *plane,
	real height, long field_00, long field_04, long field_08, byte field_0c,
	byte field_0d, short field_0e, s_collection_245270 *collection)
{
	point3f const *const *local_points = &points;
	if (collection->field_04 < 256)
	{
		s_prism_245270 *prism = &collection->field_4c08[collection->field_04++];
		prism->field_00 = field_00;
		prism->field_04 = field_04;
		prism->field_08 = field_08;
		prism->field_0c = field_0c;
		prism->field_0d = field_0d;
		prism->field_0e = field_0e;
		prism->field_10 = *plane;
		prism->field_20 = thickness;
		prism->field_24 = function_120850(&prism->field_10.n);
		prism->field_26 = prism->field_10.n.n[prism->field_24] > 0.f ? 1 : 0;
		prism->field_28 = count;
		for (short i = 0; i < prism->field_28; ++i)
		{
			function_245270_project(&(*local_points)[i], prism->field_24, prism->field_26, &prism->field_2c[i]);
		}
		if (height > 0.f && plane->k < 0.f)
		{
			prism->field_10.d -= prism->field_10.k * height;
			if (prism->field_24 != 2)
			{
				short axis = g_440b94[prism->field_24 * 2 + prism->field_26][1] == 2;
				for (short i = 0; i < prism->field_28; ++i)
					((real *)&prism->field_2c[i])[axis] -= height;
			}
		}
	}
}

// @retail 0x245270
void function_245270(real thickness, short count, point3f const *points, plane3f const *plane,
	real height, long field_00, long field_04, long field_08, byte field_0c,
	byte field_0d, short field_0e, s_collection_245270 *collection)
{
	function_24526f(thickness, count, points, plane, height, field_00, field_04,
		field_08, field_0c, field_0d, field_0e, collection);
}

struct s_244de0
{
	long field_00;
	long field_04;
	long field_08;
	byte field_0c;
	byte field_0d;
	short field_0e;
	point3f field_10;
	vector3f field_1c;
	real field_28;
};

PRIVATE __forceinline void function_244de2(real thickness, short count, point3f const *points, real arg_0, real arg_1, real arg_2, real arg_3,
	real height, long field_00, long field_04, long field_08, byte field_0c,
	byte field_0d, short field_0e, s_collection_245270 *collection)
{
	if (collection->field_04 < 256)
	{
		s_prism_245270 *prism = &collection->field_4c08[collection->field_04++];
		prism->field_00 = field_00;
		prism->field_04 = field_04;
		prism->field_08 = field_08;
		prism->field_0c = field_0c;
		prism->field_0d = field_0d;
		prism->field_0e = field_0e;
		vector3f *normal = &prism->field_10.n;
		normal->i = arg_0;
		normal->j = arg_1;
		normal->k = arg_2;
		prism->field_10.d = arg_3;
		prism->field_20 = thickness;
		prism->field_24 = function_120850(&prism->field_10.n);
		prism->field_26 = prism->field_10.n.n[prism->field_24] > 0.f ? 1 : 0;
		prism->field_28 = count;
		for (short i = 0; i < prism->field_28; ++i)
		{
			function_245270_project(&points[i], prism->field_24, prism->field_26, &prism->field_2c[i]);
		}
		if (height > 0.f && arg_2 < 0.f)
		{
			prism->field_10.d -= prism->field_10.k * height;
			if (prism->field_24 != 2)
			{
				short axis = g_440b94[prism->field_24 * 2 + prism->field_26][1] == 2;
				for (short i = 0; i < prism->field_28; ++i)
					((real *)&prism->field_2c[i])[axis] -= height;
			}
		}
	}
}

// @retail 0x244de0
void function_244de0(point3f const *arg_0, vector3f const *arg_1, real arg_2,
	real arg_3, long arg_4, long arg_5, long arg_6, byte arg_7,
	byte arg_8, short arg_9, s_collection_245270 *arg_10)
{
	if (arg_10->field_02 < 256)
	{
		s_244de0 *local_0 = &((s_244de0 *)((byte *)arg_10 + 0x2008))[arg_10->field_02++];
		local_0->field_00 = arg_4;
		local_0->field_04 = arg_5;
		local_0->field_08 = arg_6;
		local_0->field_0c = arg_7;
		local_0->field_0d = arg_8;
		local_0->field_0e = arg_9;
		local_0->field_10 = *arg_0;
		local_0->field_1c = *arg_1;
		local_0->field_28 = arg_3;
	}
	if (arg_2 > 0.f)
	{
		if (arg_10->field_02 < 256)
		{
			s_244de0 *local_0 = &((s_244de0 *)((byte *)arg_10 + 0x2008))[arg_10->field_02++];
			local_0->field_00 = arg_4;
			local_0->field_04 = arg_5;
			local_0->field_08 = arg_6;
			local_0->field_0c = arg_7;
			local_0->field_0d = arg_8;
			local_0->field_0e = arg_9;
			real_point3d_set(&local_0->field_10, arg_0->x, arg_0->y, arg_0->z - arg_2);
			local_0->field_1c = *arg_1;
			local_0->field_28 = arg_3;
		}
		point2f local_1 = { -arg_1->j, arg_1->i };
		real local_2 = (real)sqrt(local_1.x * local_1.x + local_1.y * local_1.y);
		if (!(fabs(local_2) < 0.0001f))
		{
			real local_3 = 1.f / local_2;
			local_1.x = local_3 * local_1.x;
			local_1.y = local_3 * local_1.y;
		}
		else
			local_2 = 0.f;
		if (local_2 != 0.f)
		{
			real local_4 = arg_0->y * local_1.y + arg_0->x * local_1.x;
			point3f local_5[4];
			local_5[0] = *arg_0;
			local_5[1].x = arg_0->x + arg_1->i;
			local_5[1].y = arg_0->y + arg_1->j;
			local_5[1].z = arg_0->z + arg_1->k;
			local_5[2] = local_5[1];
			local_5[2].z -= arg_2;
			local_5[3] = *arg_0;
			local_5[3].z -= arg_2;
			function_244de2(arg_3, 4, local_5, local_1.x, local_1.y, 0.f, local_4, 0.f, arg_4, arg_5, arg_6, arg_7, arg_8, arg_9, arg_10);
			point3f local_6 = local_5[1];
			local_5[1] = local_5[3];
			local_5[3] = local_6;
			function_244de2(arg_3, 4, local_5, 0.f - local_1.x, 0.f - local_1.y, 0.f, 0.f - local_4, 0.f, arg_4, arg_5, arg_6, arg_7, arg_8, arg_9, arg_10);
		}
	}
}

struct s_material_245400
{
	long field_00;
	byte field_04;
	byte field_05;
	short field_06;
};

struct s_surface_245400
{
	byte field_00[8];
	short field_08;
	byte field_0a[2];
};

struct s_vertex_245400
{
	point3f field_00;
	word field_0c;
	word field_0e;
};

struct s_source_245400
{
	byte field_00[0x2c];
	s_material_245400 *field_2c;
	long field_30;
	s_surface_245400 *field_34;
	long field_38;
	s_vertex_245400 *field_3c;
};

struct s_line_list;
void function_244ca0(real height, real radius, s_line_list *list, long a, long b, long c,
	byte d, byte e, point3f const *position, short f);

PRIVATE inline point3f *function_245400_transform(transform4x3f const *matrix,
	point3f const *point, point3f *out)
{
	real x = point->x;
	real y = point->y;
	real z = point->z;
	if (matrix->scale != 1.f)
	{
		x = matrix->scale * x;
		y = matrix->scale * y;
		z = matrix->scale * z;
	}
	out->x = matrix->up.i * z + matrix->left.i * y + matrix->forward.i * x + matrix->position.x;
	out->y = matrix->up.j * z + matrix->left.j * y + matrix->forward.j * x + matrix->position.y;
	out->z = matrix->up.k * z + matrix->left.k * y + matrix->forward.k * x + matrix->position.z;
	return out;
}

// @retail 0x245400
void function_245400(s_source_245400 const *source, long index,
	transform4x3f const *matrix, real height, real radius, long field_00,
	long field_04, s_line_list *list)
{
	s_vertex_245400 const *vertex = &source->field_3c[index];
	s_surface_245400 const *surface = &source->field_34[vertex->field_0c];
	s_material_245400 const *material = &source->field_2c[surface->field_08];
	point3f transformed;
	point3f const *point = matrix ?
		function_245400_transform(matrix, &vertex->field_00, &transformed) : &vertex->field_00;
	function_244ca0(height, radius, list, field_00, field_04, surface->field_08,
		material->field_04, material->field_05, point, material->field_06);
}

// @retail 0x245540
void function_245540(long arg_0, s_source_245400 const *arg_1,
	transform4x3f const *arg_2, real arg_3, real arg_4,
	long arg_5, long arg_6, s_collection_245270 *arg_7)
{
	s_surface_245400 const *local_0 = &arg_1->field_34[arg_0];
	long local_1 = local_0->field_08;
	s_material_245400 const *materials = arg_1->field_2c;
	short first_material = (short)materials[local_1].field_00;
	short second_material = (short)materials[*(short *)local_0->field_0a].field_00;
	if (first_material != second_material)
	{
		point3f const *local_4 = &arg_1->field_3c[((word const *)local_0)[0]].field_00;
		point3f const *local_5 = &arg_1->field_3c[((word const *)local_0)[1]].field_00;
		vector3f local_6;
		vector3d_from_points3d(local_4, local_5, &local_6);
		long local_2 = first_material;
		long local_3 = second_material;
		plane3f const *local_7 = *(plane3f const *const *)((byte const *)arg_1 + 0xc);
		plane3f const *local_8 = &local_7[local_2 & 0x7fff];
		plane3f const *local_9 = &local_7[local_3 & 0x7fff];
		bool local_10 = (bool)((local_2 >> 15) & 1);
		bool local_11 = (bool)((local_3 >> 15) & 1);
		if ((local_2 & 0x7fff) != (local_3 & 0x7fff))
		{
			vector3f local_12;
			local_12.i = local_8->j * local_9->k - local_8->k * local_9->j;
			local_12.j = local_8->k * local_9->i - local_8->i * local_9->k;
			local_12.k = local_8->i * local_9->j - local_8->j * local_9->i;
			real local_13 = local_12.k * local_6.k + local_12.j * local_6.j + local_12.i * local_6.i;
			if (local_10 == local_11)
			{
				if (!(local_13 > -0.0001f))
					return;
			}
			else if (!(local_13 < 0.0001f))
				return;
		}
		point3f local_14;
		if (arg_2)
		{
			real local_15 = local_6.i;
			real local_16 = local_6.j;
			real local_17 = local_6.k;
			if (arg_2->scale != 1.f)
			{
				local_15 = arg_2->scale * local_15;
				local_16 = arg_2->scale * local_16;
				local_17 = arg_2->scale * local_17;
			}
			local_6.i = arg_2->forward.i * local_15 + arg_2->left.i * local_16 + arg_2->up.i * local_17;
			local_6.j = arg_2->up.j * local_17 + local_15 * arg_2->forward.j + arg_2->left.j * local_16;
			local_6.k = arg_2->forward.k * local_15 + arg_2->left.k * local_16 + arg_2->up.k * local_17;
			local_4 = function_245400_transform(arg_2, local_4, &local_14);
		}
		function_244de0(local_4, &local_6, arg_3, arg_4, arg_5, arg_6, local_1,
			materials[local_1].field_04, materials[local_1].field_05,
			materials[local_1].field_06, arg_7);
	}
}
