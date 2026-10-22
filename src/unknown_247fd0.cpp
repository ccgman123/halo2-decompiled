#include "unknown_11c920.h"
#include "effects.h"

// @flags /O2 /arch:SSE /Gr

struct s_247fd0
{
	real field_00[17];
	dword field_44;
	void *field_48;
	void *field_4c;
	void const *field_50;
};

struct s_247fd3
{
	word field_00;
	word field_02;
	long field_04;
};

struct s_248000
{
	word field_00;
	union
	{
		word field_02;
		struct
		{
			word : 13;
			word field_02_13 : 3;
		};
	};
	long field_04;
	real field_08;
	real field_0c;
	real field_10;
	word field_14;
	word field_16;
	word field_18;
	word field_1a;
	point3f field_1c;
	vector3f field_28;
	real field_34;
	real field_38;
	dword field_3c;
};

class c_248228
{
public:
	virtual void function_248228() = 0;
	virtual void function_248229() = 0;
	virtual void function_24822a() = 0;
	virtual void function_24822b() = 0;
	virtual void function_24822c() = 0;
	virtual void function_24822d() = 0;
	virtual long function_24822e() = 0;
	virtual s_effect_particle_system_definition *function_24822f(word arg_0) = 0;
};

struct s_particle_spawn_definition;
struct s_particle_spawn_state;
void function_249aa0(s_particle_spawn_definition const *arg_0, s_particle_spawn_state *arg_1,
	point3f const *arg_2, real const *arg_3, vector3f const *arg_4);
void __stdcall function_173ba0(dword arg_0, void *arg_1, void *arg_2, void const *arg_3, real *arg_4);

PRIVATE __forceinline void function_248076(dword *arg_0, real &arg_1)
{
	arg_1 = function_x82e52f(arg_0, 0, 0);
}

// @retail 0x247fd0
void function_247fd0(s_247fd3 *arg_1, s_particle_system_datum *arg_2,
	point3f const *arg_3, vector3f const *arg_4, dword arg_5, s_247fd0 *arg_0)
{
	long local_0 = record_pool_allocate(g_51ec84);
	if (local_0 != NONE)
	{
		s_248000 *local_1 = &((s_248000 *)g_51ec84->data)[local_0 & 0xffff];
		s_effect_particle_system_definition *local_2 = arg_2->function_1751d0();
		s_particle_spawn_definition const *local_3 = *(s_particle_spawn_definition const **)local_2->unknown34;
		local_1->field_04 = arg_1->field_04;
		arg_1->field_04 = local_0;
		local_1->field_02 &= 0xe004;
		local_1->field_02 |= 4;
		local_1->field_02_13 = (_random(&g_4e7408->seed, 0, 0) * 5) >> 16;
		local_1->field_08 = 0.f;
		real local_9;
		function_248076(&g_4e7408->seed, local_9);
		local_1->field_14 = (word)real_truncate(local_9 * 65535.f);
		function_248076(&g_4e7408->seed, local_9);
		local_1->field_16 = (word)real_truncate(local_9 * 65535.f);
		function_248076(&g_4e7408->seed, local_9);
		local_1->field_18 = (word)real_truncate(local_9 * 65535.f);
		function_248076(&g_4e7408->seed, local_9);
		local_1->field_1a = (word)real_truncate(local_9 * 65535.f);
		local_1->field_10 = arg_2->unknown04;
		local_1->field_38 = function_x82e52f(&g_4e7408->seed, 0, 0);
		if (arg_0->field_50 != local_1)
		{
			dword flags = arg_0->field_44 & ~0xf80f;
			arg_0->field_50 = local_1;
			arg_0->field_44 = flags;
		}
		function_173ba0(~arg_0->field_44 & 0xf80f, arg_0->field_48, arg_0->field_4c, arg_0->field_50, arg_0->field_00);
		arg_0->field_44 |= 0xf80f;
		function_249aa0(local_3, (s_particle_spawn_state *)local_1, arg_3, arg_0->field_00, arg_4);
		local_1->field_3c = arg_5;
		++arg_1->field_02;
		c_248228 *local_4 = (c_248228 *)function_137bd0(local_2->tag_index);
		for (long local_5 = 0; local_5 < local_4->function_24822e(); ++local_5)
		{
			long effect_index = arg_2->effect_index;
			long tag_index = local_2->tag_index;
			s_effect_particle_system_definition *child_settings = local_4->function_24822f((word)local_5);
			long local_6 = function_173fd0(child_settings, effect_index,
				tag_index, (short)local_5, NONE);
			local_4 = (c_248228 *)function_137bd0(local_2->tag_index);
			if (local_6 != NONE)
			{
				s_particle_system_datum *local_7 = &((s_particle_system_datum *)g_510c74->data)[local_6 & 0xffff];
				particle_system_link(local_7, &arg_2->first_child_index, &arg_2->last_child_index);
				local_7->parent = arg_2;
				local_7->unknown4c = local_0;
				real local_8 = local_1->field_0c > 0.f ? 1.f / local_1->field_0c : 0.f;
				local_7->flag0 = true;
				local_7->unknown04 = 0.f;
				if (local_8 > 0.f) local_7->unknown08 = 1.f / local_8;
				else local_7->unknown08 = 1.f;
			}
		}
	}
}

struct s_248cc0
{
	long field_00;
	long field_04;
	long field_08;
};

struct s_248ce2
{
	short field_00;
	word field_02;
	long field_04;
	long field_08;
	real field_0c;
	byte field_10[0x4c - 0x10];
};

struct s_particle_emitter_datum;
void function_2483f0(s_particle_emitter_datum *arg_0, long *arg_1, long *arg_2);

// @retail 0x248cc0
void function_248cc0(s_248cc0 *arg_0, s_247fd0 *arg_1, s_particle_system_datum *arg_2,
	point3f const *arg_3, vector3f const *arg_4)
{
	s_record_pool *local_0 = g_51ec88;
	if (arg_0->field_04 == NONE)
	{
		long local_1 = record_pool_allocate(local_0);
		if (local_1 == NONE)
			return;
		s_248ce2 *local_2 = &((s_248ce2 *)local_0->data)[local_1 & 0xffff];
		local_2->field_08 = NONE;
		local_2->field_02 = 0;
		local_2->field_04 = NONE;
		local_2->field_0c = 0.f;
		function_2483f0((s_particle_emitter_datum *)&((s_248ce2 *)local_0->data)[local_1 & 0xffff], &arg_0->field_04, &arg_0->field_08);
	}
	if (arg_0->field_04 != NONE)
		function_247fd0((s_247fd3 *)&((s_248ce2 *)local_0->data)[arg_0->field_04 & 0xffff], arg_2, arg_3, arg_4, arg_2->color, arg_1);
}

#include "unknown_0259a0.h"

struct s_247950
{
	word field_0;
	word field_2;
	long field_4;
	byte field_8[8];
	matrix3x3 field_10;
	point3f field_34;
	point3f field_40;
};

struct s_placement
{
	point3f position;
	vector3f direction;
	vector3f up;
	short value24;
};

class c_247951
{
public:
	virtual void function_247952() = 0;
	virtual void function_247953() = 0;
	virtual void function_247954() = 0;
	virtual void function_247955() = 0;
	virtual dword function_247956() = 0;
	virtual long function_247957() = 0;
	virtual long function_247958() = 0;
	virtual s_effect_particle_system_definition *function_247959(word arg_0) = 0;
};

struct s_object_246eb0;
struct s_particle_impact;
void function_249170(real const *arg_0, s_particle_spawn_state *arg_1, s_particle_spawn_definition const *arg_2, s_particle_system_datum const *arg_3);
vector3f *function_143070(matrix3x3 const *arg_1, vector3f const *arg_0, vector3f *arg_2);
void placement_set(s_placement *arg_0, vector3f const *arg_1, point3f const *arg_2, vector3f const *arg_3, short arg_4);
void function_246e60(long arg_0, dword arg_1, s_particle_impact const *arg_2, s_object_246eb0 const *arg_3, long arg_4);

// @retail 0x247950
void function_247950(s_particle_system_datum *arg_2, s_particle_spawn_definition const *arg_3,
	dword arg_4, real arg_5, real arg_6, real arg_7, s_247950 *arg_1, s_247fd0 *arg_0)
{
	struct s_247961
	{
		point3f field_0;
		point3f field_c;
		vector3f field_18;
		s_collision_result_1697c0 field_24;
		s_placement field_70;
		real field_98;
	} local_23;
	volatile bool local_18;
	long local_0 = arg_2->effect_index;
	if (local_0 != NONE)
	{
		byte local_24 = *(byte const *)((byte const *)DATUM(g_4ea93c, s_effect_datum, local_0) + 2);
		local_24 >>= 3;
		local_24 &= 1;
		if (local_18 = local_24 != 0)
			return;
	}
	long local_1 = record_pool_allocate(g_51ec84);
	if (local_1 != NONE)
	{
		s_248000 *local_2 = DATUM(g_51ec84, s_248000, local_1);
		s_effect_particle_system_definition *local_3 = arg_2->function_1751d0();
		local_2->field_04 = arg_1->field_4;
		arg_1->field_4 = local_1;
		local_2->field_02 &= 0xe004;
		local_2->field_02 |= 4;
		local_2->field_02_13 = (_random(&g_4e7408->seed, 0, 0) * 5) >> 16;
		local_2->field_08 = 0.f;
		function_248076(&g_4e7408->seed, local_23.field_98);
		local_2->field_14 = (word)real_truncate(local_23.field_98 * 65535.f);
		function_248076(&g_4e7408->seed, local_23.field_98);
		local_2->field_16 = (word)real_truncate(local_23.field_98 * 65535.f);
		function_248076(&g_4e7408->seed, local_23.field_98);
		local_2->field_18 = (word)real_truncate(local_23.field_98 * 65535.f);
		function_248076(&g_4e7408->seed, local_23.field_98);
		local_2->field_1a = (word)real_truncate(local_23.field_98 * 65535.f);
		local_2->field_10 = arg_2->unknown04;
		if (arg_0->field_50 != local_2)
		{
			arg_0->field_44 &= ~0xf80f;
			arg_0->field_50 = local_2;
		}
		function_173ba0(~arg_0->field_44 & 0xf80f, arg_0->field_48, arg_0->field_4c, arg_0->field_50, arg_0->field_00);
		arg_0->field_44 |= 0xf80f;
		function_249170(arg_0->field_00, (s_particle_spawn_state *)local_2, arg_3, arg_2);
		local_2->field_1c.x = arg_7 * local_2->field_1c.x;
		local_2->field_1c.y = arg_7 * local_2->field_1c.y;
		local_2->field_1c.z = arg_7 * local_2->field_1c.z;
		local_2->field_08 += arg_5 * arg_6 * local_2->field_0c;
		if (local_3->unknown0c == 0)
		{
			vector3f const *local_5 = local_0 != NONE ? &DATUM(g_4ea93c, s_effect_datum, arg_2->effect_index)->velocity : g_4687a4;
			local_23.field_0.x = arg_1->field_34.x * arg_5 + arg_1->field_40.x * (1.f - arg_5);
			local_23.field_0.y = arg_1->field_40.y * (1.f - arg_5) + arg_1->field_34.y * arg_5;
			local_23.field_0.z = arg_1->field_40.z * (1.f - arg_5) + arg_1->field_34.z * arg_5;
			function_143070(&arg_1->field_10, (vector3f *)&local_2->field_1c, (vector3f *)&local_2->field_1c);
			local_2->field_1c.x = local_23.field_0.x + local_2->field_1c.x;
			local_2->field_1c.y = local_23.field_0.y + local_2->field_1c.y;
			local_2->field_1c.z = local_23.field_0.z + local_2->field_1c.z;
			function_143070(&arg_1->field_10, &local_2->field_28, &local_2->field_28);
			if (*(long *)((byte const *)arg_3 + 0x78) == 8)
			{
				local_23.field_c = local_2->field_1c;
				local_23.field_18.i = 0.f;
				local_23.field_18.j = 0.f;
				local_23.field_18.k = -1.f;
				local_23.field_c.z += 1.f;
				local_23.field_24.unknown24 = NONE;
				if (function_1697c0(0x800005, &local_23.field_c, &local_23.field_18, NONE, NONE, &local_23.field_24))
				{
					vector3f const *local_10 = (vector3f const *)((byte const *)&local_23.field_24 + 0x28);
					local_2->field_1c.x = local_10->i * 0.005f + local_23.field_24.point.x;
					local_2->field_1c.y = local_10->j * 0.005f + local_23.field_24.point.y;
					local_2->field_1c.z = local_10->k * 0.005f + local_23.field_24.point.z;
					local_2->field_02 |= 0xa;
					local_2->field_28 = *local_10;
				}
				else
					local_2->field_02 |= 1;
			}
			if ((bool)((*(byte const *)((byte const *)local_3 + 0x16) >> 4) & 1))
			{
				local_2->field_28.i += local_5->i;
				local_2->field_28.j = local_5->j + local_2->field_28.j;
				local_2->field_28.k = local_5->k + local_2->field_28.k;
			}
		}
		else
		{
			real local_11 = (1.f - arg_5) * arg_6;
			local_2->field_1c.x = local_2->field_28.i * local_11 + local_2->field_1c.x;
			local_2->field_1c.y = local_2->field_28.j * local_11 + local_2->field_1c.y;
			local_2->field_1c.z = local_2->field_28.k * local_11 + local_2->field_1c.z;
		}
		local_2->field_3c = arg_4;
		++arg_1->field_2;
		local_23.field_70.value24 = NONE;
		c_247951 *local_12 = (c_247951 *)function_137bd0(local_3->tag_index);
		for (long local_13 = 0; local_13 < local_12->function_247958(); ++local_13)
		{
			long local_14 = function_173fd0(local_12->function_247959((word)local_13), arg_2->effect_index, local_3->tag_index, local_13, NONE);
			local_12 = (c_247951 *)function_137bd0(local_3->tag_index);
			if (local_14 != NONE)
			{
				s_particle_system_datum *local_15 = DATUM(g_510c74, s_particle_system_datum, local_14);
				particle_system_link(local_15, &arg_2->first_child_index, &arg_2->last_child_index);
				local_15->parent = arg_2;
				local_15->unknown4c = local_1;
				real local_16 = local_2->field_0c > 0.f ? 1.f / local_2->field_0c : 0.f;
				local_15->flag0 = true;
				local_15->unknown04 = 0.f;
				local_15->unknown08 = local_16 > 0.f ? 1.f / local_16 : 1.f;
			}
		}
		if (local_12->function_247957() != NONE)
		{
			placement_set(&local_23.field_70, &local_2->field_28, &local_2->field_1c, g_4687b0, 0);
			local_3 = arg_2->function_1751d0();
			long local_19 = local_3->tag_index;
			long local_20 = local_12->function_247957();
			dword local_21 = local_12->function_247956();
			function_246e60(local_20, local_21, (s_particle_impact const *)&local_23.field_70, (s_object_246eb0 const *)local_2, local_19);
		}
	}
}
