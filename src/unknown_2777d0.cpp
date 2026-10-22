// @flags /O2 /Gr /arch:SSE
#include "unknown_11c920.h"
#include "globals.h"
#include "havok_reference.h"
#include "unknown_1cec30.h"
#include "object_list.h"
#include "slot_handler.h"
#include <xmmintrin.h>
#include <math.h>

/* Interfaces whose destructors restore their retail virtual tables. */
struct s_278a70_event;
struct s_278a70_body;
struct s_278da0_group;
class c_library_30c470
{
public:
	void detach(void *callback);
};
class c_interface_2777d0
{
public:
	virtual ~c_interface_2777d0();
	virtual void slot1(hkEntity *entity) { }
	virtual void slot2(hkEntity *entity) { }
};

class c_interface_2777f0
{
public:
	virtual ~c_interface_2777f0();
	virtual void slot1() { }
	virtual void slot2(s_278a70_event *event) { }
	virtual void slot3() { }
	virtual void slot4(void *context) {}
	virtual void slot5() { }
	virtual long slot6() { return 0; }
	long unknown04;
	c_library_30c470 *library;
};

class c_interface_278b40
{
public:
	virtual ~c_interface_278b40();
	virtual void slot1(s_278da0_group *group);
	virtual void slot2(s_278da0_group *group);
};

// @retail 0x2777d0 deleting
c_interface_2777d0::~c_interface_2777d0()
{
}

// @retail 0x277800 deleting c_interface_2777f0
// @retail 0x2777f0
c_interface_2777f0::~c_interface_2777f0()
{
}

// @retail 0x278b40 deleting
c_interface_278b40::~c_interface_278b40()
{
}

void __cdecl function_2d91d0(void *array, long element_size);

struct s_277890_buffer
{
	long *data;
	long count;
	long capacity;

	s_277890_buffer() : data(NULL), count(0), capacity(0x80000000) {}

	__forceinline void append(long value)
	{
		if (count == (capacity & 0x7fffffff))
			function_2d91d0(this, sizeof(long));
		data[count++] = value;
	}

	~s_277890_buffer()
	{
		if (!(capacity & 0x80000000))
			g_480118->allocate((long)data, (capacity & 0x7fffffff) * sizeof(long), 0x12);
	}
};

class c_callback_277820_target
{
public:
	virtual void slot0() {}
	virtual void slot1() {}
	virtual void slot2() {}
	virtual bool test(void *first, void *second) { return false; }
};

class c_reference_277820 : public c_havok_reference_counted
{
public:
	c_reference_277820() { reference_count = 1; }
};

class c_callback_277820 : public c_reference_277820
{
public:
	c_callback_277820() { target = NULL; }
	virtual hkBool test(void *first, void *second);
	c_callback_277820_target *target;
};

class c_interface_277840
{
public:
	virtual void event0(void *event) {}
	virtual void event1(void *event) {}
	virtual void event2(void *event) {}
};

class c_interface_277890 : public c_interface_2777d0, public c_interface_277840
{
public:
	c_interface_277890();
	s_277890_buffer buffer;
	long index;
	byte active;
	c_callback_277820 reference;
	virtual bool test(s_278a70_body *first, s_278a70_body *second);
	virtual void event0(void *event);
	virtual void event1(void *arg_0);
	virtual void event2(void *event);
	virtual void slot1(hkEntity *entity);
	virtual void slot2(hkEntity *entity);

};

struct s_278030
{
	byte field_0[8];
	hkEntity *field_8;
	hkEntity *field_c;
	hkEntity *field_10;
};

void function_1d05d0(hkEntity const *arg_0, hkEntity const *arg_1,
	s_havok_component *arg_2, short const *arg_3);

// @retail 0x278030
void c_interface_277890::event1(void *arg_0)
{
	s_278030 *local_0 = (s_278030 *)arg_0;
	hkEntity *local_1 = local_0->field_8;
	hkEntity *local_2 = local_0->field_c;
	long local_3 = havok_entity_property_get(local_1, 0x2001);
	long local_4 = havok_entity_property_get(local_2, 0x2001);
	if (local_0->field_10 == local_1)
	{
		s_havok_component *local_5 = havok_component_get(local_3);
		if (*(byte *)((byte *)local_5 + 4) & 1)
			function_1d05d0(local_1, local_2, local_5, (short const *)local_0);
	}
	else
	{
		s_havok_component *local_6 = havok_component_get(local_4);
		if (*(byte *)((byte *)local_6 + 4) & 1)
			function_1d05d0(local_2, local_1, local_6, (short const *)local_0);
	}
}

// @retail 0x277890 deleting c_interface_277890

// @retail 0x277820
hkBool c_callback_277820::test(void *first, void *second)
{
	hkBool result;
	result.m_bool = target->test(first, second);
	return result;
}

struct s_278300_references
{
	byte unknown00[8];
	c_havok_reference_counted **data;
	long count;
	long capacity;
	long index;
};

// @retail 0x278300
void function_278300(s_278300_references *references)
{
	while (references->index + 1 < references->count)
	{
		references->index++;
		havok_reference_remove(references->data[references->index]);
	}
	references->index = NONE;
	references->count = 0;
}

// @retail 0x277840
c_interface_277890::c_interface_277890()
{
	reference.target = (c_callback_277820_target *)this;
	index = NONE;
	active = false;
	function_278300((s_278300_references *)this);
}

void function_b7360(long object_index);
void function_226a60(long component_index);

// @retail 0x277990
void c_interface_277890::slot2(hkEntity *entity)
{
	long index = havok_entity_property_get(entity, 0x2001);
	if (index != NONE)
	{
		s_havok_component *component = havok_component_get(index);
		component->unknown04 &= ~8;
		function_b7360(component->object_index);
		function_226a60(index);
	}
}

void __cdecl function_2d91d0(void *array, long element_size);
void havok_component_rigid_body_state_update(long index, s_havok_component *component);

__forceinline long property_2778e0(hkEntity const *entity)
{
	long count = entity->m_property_count;
	for (long i = 0; i < count; i++)
	{
		if (entity->m_properties[i].m_key == 0x2001)
			return entity->m_properties[i].m_value.m_data;
	}
	return 0;
}

// @retail 0x2778e0
void c_interface_277890::slot1(hkEntity *entity)
{
	if (active)
	{
		((c_havok_reference_counted *)entity)->reference_count++;
		buffer.append((long)entity);
	}
	long index = property_2778e0(entity);
	if (index != NONE)
	{
		s_havok_component *component = havok_component_get(index);
		if ((bool)component->flag5)
		{
			for (long i = 0; i < component->rigid_bodies.size; i++)
				havok_component_rigid_body_state_update(i, component);
		}
	}
}

struct s_279670_object
{
	byte unknown00[0xb8];
	long platform_index;
	long unknownbc;
	word flag0 : 1;
	word flag1 : 1;
	word flag2 : 1;
	word others : 13;
};

struct s_279670_header
{
	byte unknown00[8];
	s_279670_object *object;
};

// @retail 0x279670
bool __cdecl function_279670(hkEntity *entity)
{
	long index = havok_entity_property_get(entity, 0x2001);
	bool result = true;
	if (index != NONE)
	{
		long object_index = havok_component_get(index)->object_index;
		s_279670_object *object = ((s_279670_header *)g_4e0300->data)[object_index & 0xffff].object;
		if ((bool)object->flag1 || (bool)object->flag2)
		{
			long platform_index = object->platform_index;
			bool found = false;
			if (platform_index != NONE)
			{
				for (long i = 0; i < g_5107f0->object_count; i++)
				{
					if (g_5107f0->object_indices[i] == platform_index)
					{
						found = true;
						break;
					}
				}
			}
			result = !found;
		}
	}
	return result;
}

struct s_278a70_body
{
	byte unknown00[0x18];
	long type;
	long unknown1c;
	hkEntity *entity;
};

struct s_278a70_event
{
	long unknown00;
	s_278a70_body *body;
};

class c_callback_2789b0 : public c_havok_reference_counted, public c_interface_2777f0
{
public:
	virtual void slot2(s_278a70_event *event);
	virtual void slot4(void *context);
	static void operator delete(void *block)
	{
		g_480118->allocate((long)block, ((c_callback_2789b0 *)block)->allocation_size, 0x10);
	}
};

// @retail 0x278ae0
void c_callback_2789b0::slot4(void *context)
{
	library->detach(static_cast<c_interface_2777f0 *>(this));
	havok_reference_remove(this);
}

// @retail 0x278a70
void c_callback_2789b0::slot2(s_278a70_event *event)
{
	if (event->body->type == 1)
	{
		hkEntity *entity = event->body->entity;
		if (entity)
		{
			long index = havok_entity_property_get(entity, 0x2001);
			if (index != NONE)
			{
				s_havok_component *component = havok_component_get(index);
				component->flags12 &= ~1;
			}
		}
	}
}

class c_278da0_resource
{
public:
	virtual void slot0() {}
	virtual void slot1() {}
	virtual void slot2() {}
	virtual void slot3() {}
	virtual void finish() {}
	virtual void release() {}
	long unknown04;
	c_278da0_resource *owner;
};

class c_278da0_kind
{
public:
	virtual void slot0() {}
	virtual void slot1() {}
	virtual void slot2() {}
	virtual void slot3() {}
	virtual void slot4() {}
	virtual void slot5() {}
	virtual void slot6() {}
	virtual long type() { return 0; }
};

class c_library_311690
{
public:
	void update(long value);
	byte unknown00[0x54];
	c_278da0_kind *kind;
};

struct s_278da0_entry
{
	byte *first;
	byte *second;
	c_278da0_resource *resource;
	long unknown0c;
};

struct s_278da0_group
{
	byte unknown00[0x20];
	class c_library_30c190 *context;
	byte unknown24[0x3c - 0x24];
	c_library_311690 **items;
	long item_count;
	byte unknown44[0x24];
	s_278da0_entry *entries;
	long count;
};

__forceinline hkRigidBody *body_278da0(byte *contact)
{
	s_278a70_body *body = (s_278a70_body *)(contact - 0x10);
	return body->type == 1 ? (hkRigidBody *)body->entity : NULL;
}

// @retail 0x278da0
void c_interface_278b40::slot2(s_278da0_group *group)
{
	for (long i = 0; i < group->count; i++)
	{
		if (body_278da0(group->entries[i].first)->m_fixed || body_278da0(group->entries[i].second)->m_fixed)
		{
			c_278da0_resource *resource = group->entries[i].resource;
			if (resource)
			{
				c_278da0_resource *owner = resource->owner;
				resource->release();
				owner->finish();
			}
			group->entries[i].resource = NULL;
		}
	}
	for (long i = 0; i < group->item_count; i++)
	{
		if (group->items[i]->kind->type() == 2)
			group->items[i]->update(1);
	}
}

class c_278b60_filter
{
public:
	virtual hkBool test0(s_278a70_body *first, s_278a70_body *second) { return hkBool(); }
	virtual hkBool test1(s_278a70_body *first, s_278a70_body *second) { return hkBool(); }
};

struct s_278b60_filter
{
	byte unknown00[8];
	byte interface08[4];
};

struct s_278b60_dispatch;

class c_278b60_factory
{
public:
	virtual void slot0() {}
	virtual c_278da0_resource *create(s_278a70_body *first, s_278a70_body *second,
		s_278b60_dispatch *dispatch) { return NULL; }
};

typedef c_278da0_resource *(__cdecl *function_278b60_create)(s_278a70_body *, s_278a70_body *,
	s_278b60_dispatch *, c_278da0_resource *);

struct s_278b60_table
{
	byte unknown00[0xc];
	c_278b60_factory *factories[64];
	byte unknown10c[0x80];
	function_278b60_create callbacks[1024];
};

struct s_278b60_dispatch
{
	s_278b60_table *table;
};

class c_library_30c190
{
public:
	c_278b60_filter *filter();
	byte unknown00[0xcc];
	s_278b60_dispatch *dispatch;
	s_278b60_filter *filter_object;
};

class c_278b60_shape
{
public:
	virtual void slot0() {}
	virtual void slot1() {}
	virtual void slot2() {}
	virtual void slot3() {}
	virtual void slot4() {}
	virtual long type() { return 0; }
};

struct c_transformed_point
{
	__m128 value;
	void transform(const void *matrix, const __m128 *point);
};

struct s_2792c0_contact
{
	c_278b60_shape *shape;
	long unknown04;
	byte *transform;
};

struct s_2792c0_context
{
	byte unknown00[8];
	s_2792c0_contact *first;
	s_2792c0_contact *second;
	long unknown10;
	__m128 *plane;
};

struct s_2792c0_vertices
{
	byte unknown00[0x20];
	__m128 points[3];
};

// @retail 0x2792c0
bool function_2792c0(s_2792c0_context *context)
{
	bool first = context->first->shape->type() == 0x18;
	bool second = context->second->shape->type() == 0x18;
	bool result = true;
	if (first || second)
	{
		s_2792c0_contact *contact = first ? context->first : context->second;
		s_2792c0_vertices *vertices = (s_2792c0_vertices *)contact->shape;
		__m128 edge0 = _mm_sub_ps(vertices->points[1], vertices->points[0]);
		__m128 edge1 = _mm_sub_ps(vertices->points[2], vertices->points[0]);
		__m128 right = _mm_mul_ps(_mm_shuffle_ps(edge0, edge0, 0xd2), _mm_shuffle_ps(edge1, edge1, 0xc9));
		__m128 left = _mm_mul_ps(_mm_shuffle_ps(edge0, edge0, 0xc9), _mm_shuffle_ps(edge1, edge1, 0xd2));
		c_transformed_point normal;
		normal.value = _mm_sub_ps(left, right);
		normal.transform(contact->transform + 0x20, &normal.value);
		__m128 squares = _mm_mul_ps(normal.value, normal.value);
		__m128 sum = _mm_add_ss(_mm_shuffle_ps(squares, squares, 0xaa), _mm_add_ss(_mm_shuffle_ps(squares, squares, 0x55), squares));
		__m128 inverse = _mm_rsqrt_ss(sum);
		__m128 correction = _mm_sub_ss(_mm_set_ss(3.0f), _mm_mul_ss(_mm_mul_ss(sum, inverse), inverse));
		__m128 scale = _mm_mul_ss(_mm_mul_ss(_mm_set_ss(0.5f), inverse), correction);
		normal.value = _mm_mul_ps(_mm_shuffle_ps(scale, scale, 0), normal.value);
		__m128 product = _mm_mul_ps(normal.value, context->plane[1]);
		__m128 dot = _mm_add_ss(_mm_shuffle_ps(product, product, 0xaa), _mm_add_ss(_mm_shuffle_ps(product, product, 0x55), product));
		real value;
		_mm_store_ss(&value, dot);
		if (0.577 > fabs(value))
			result = false;
		else
			result = true;
	}
	return result;
}

// @retail 0x278b60
void c_interface_278b40::slot1(s_278da0_group *group)
{
	for (long i = 0; i < group->count; i++)
	{
		s_278da0_entry *entry = &group->entries[i];
		if (!entry->resource)
		{
			s_278a70_body *first = (s_278a70_body *)(entry->first - 0x10);
			s_278a70_body *second = (s_278a70_body *)(group->entries[i].second - 0x10);
			hkRigidBody *first_body = first->type == 1 ? (hkRigidBody *)first->entity : NULL;
			hkRigidBody *second_body = second->type == 1 ? (hkRigidBody *)second->entity : NULL;
			c_library_30c190 *context = group->context;
			if (context->filter_object && !((c_278b60_filter *)context->filter_object->interface08)->test0(first, second).m_bool)
			{
				group->entries[i].resource = NULL;
				continue;
			}
			s_278b60_dispatch *dispatch = group->context->dispatch;
			c_278da0_resource *owner = dispatch->table->factories[*((char *)first_body + 0x48) * 8 + *((char *)second_body + 0x48)]->create(first, second, dispatch);
			dispatch = group->context->dispatch;
			s_278b60_table *table = dispatch->table;
			long first_shape = (*(c_278b60_shape **)first)->type();
			long second_shape = (*(c_278b60_shape **)second)->type();
			function_278b60_create create = table->callbacks[first_shape * 32 + second_shape];
			s_278da0_entry *destination = &group->entries[i];
			destination->resource = create(first, second, dispatch, owner);
			c_278da0_resource **resource = &group->entries[i].resource;
			if (*resource && (*resource)->owner)
			{
				hkRigidBody *active_first = first->type == 1 ? (hkRigidBody *)first->entity : NULL;
				hkRigidBody *active_second = second->type == 1 ? (hkRigidBody *)second->entity : NULL;
				if ((active_first->m_motion->kind_code() != 6 && !active_first->m_fixed) ||
					(active_second->m_motion->kind_code() != 6 && !active_second->m_fixed))
				{
					if (group->context->filter()->test1(first, second).m_bool)
						group->entries[i].unknown0c = 0;
					else
						group->entries[i].unknown0c = NONE;
				}
				else
					group->entries[i].unknown0c = NONE;
			}
			else
			{
				*resource = NULL;
				owner->finish();
			}
		}
	}
	for (long i = 0; i < group->item_count; i++)
	{
		if (group->items[i]->kind->type() == 1)
			group->items[i]->update(2);
	}
}

long function_147090(void);
void *g_55e508;
void *g_55e50c;
long g_55e504;
long g_55e500;
bool g_55e4fc;

struct s_278e80_state
{
	byte unknown00[0xae];
	bool changed;
};

// @retail 0x278e60
void __cdecl function_278e60(void *context)
{
	g_55e508 = context;
	g_55e504 = function_147090();
}

// @retail 0x278e80
void __cdecl function_278e80(s_278e80_state *state)
{
	g_55e508 = NULL;
	if (function_147090() - g_55e504)
	{
		state->changed = true;
		g_55e4fc = true;
	}
}

// @retail 0x278eb0
void __cdecl function_278eb0(void *context)
{
	g_55e50c = context;
	g_55e500 = function_147090();
}

// @retail 0x278ed0
void __cdecl function_278ed0(s_278e80_state *state)
{
	g_55e50c = NULL;
	if (function_147090() - g_55e500)
	{
		state->changed = true;
		g_55e4fc = true;
	}
}

struct s_2797a0_group
{
	byte unknown00[0x40];
	long count;
};

struct s_2797a0_groups
{
	byte unknown00[8];
	s_2797a0_group **first;
	long first_count;
	long unknown10;
	s_2797a0_group **second;
	long second_count;
};

struct s_2797a0_iterator
{
	byte unknown00;
	bool second;
	byte unknown02[2];
	long group;
	long index;
	s_2797a0_groups *groups;
	long mode;
};

// @retail 0x2797a0
bool function_2797a0(s_2797a0_iterator *iterator)
{
	bool result = false;
	if (iterator->group == NONE)
	{
		iterator->group = 0;
		iterator->index = 0;
		if (iterator->mode == 1)
			iterator->second = true;
	}
	else
		iterator->index++;
	if (!iterator->second)
	{
		s_2797a0_groups *groups = iterator->groups;
		while (iterator->group < iterator->groups->first_count)
		{
			if (iterator->index < groups->first[iterator->group]->count)
			{
				result = true;
				break;
			}
			iterator->group++;
			iterator->index = 0;
		}
		if (!result)
		{
			iterator->second = true;
			iterator->group = 0;
			iterator->index = 0;
		}
	}
	if (iterator->second && (iterator->mode == 2 || iterator->mode == 1))
	{
		s_2797a0_groups *groups = iterator->groups;
		while (iterator->group < iterator->groups->second_count)
		{
			if (iterator->index < groups->second[iterator->group]->count)
			{
				result = true;
				break;
			}
			iterator->group++;
			iterator->index = 0;
		}
	}
	return result;
}

class c_278200_motion
{
public:
	virtual void slot0() {}
	virtual void slot1() {}
	virtual void slot2() {}
	virtual void slot3() {}
	virtual void slot4() {}
	virtual void slot5() {}
	virtual long type() { return 0; }
};

long havok_entity_component_index_get(hkEntity const *entity);

// @retail 0x278200
bool c_interface_277890::test(s_278a70_body *first, s_278a70_body *second)
{
	hkRigidBody *first_body = first->type == 1 ? (hkRigidBody *)first->entity : NULL;
	hkRigidBody *second_body = second->type == 1 ? (hkRigidBody *)second->entity : NULL;
	bool result = false;
	if (first_body && second_body)
	{
		if (first_body->m_fixed || second_body->m_fixed ||
			((c_278200_motion *)first_body->m_motion)->type() == 6 ||
			((c_278200_motion *)second_body->m_motion)->type() == 6)
			result = true;
		else
		{
			long first_index = havok_entity_component_index_get((hkEntity *)first_body);
			if (first_index != NONE)
			{
				long second_index = havok_entity_component_index_get((hkEntity *)second_body);
				if (second_index != NONE)
				{
					long first_object = havok_component_get(first_index)->object_index;
					// Retail checks both indices, then uses the first for both lookups.
					long second_object = havok_component_get(first_index)->object_index;
					s_object_header_view *first_header = &((s_object_header_view *)g_4e0300->data)[first_object & 0xffff];
					s_object_header_view *second_header = &((s_object_header_view *)g_4e0300->data)[second_object & 0xffff];
					bool neither = first_header->type != 1 && second_header->type != 1;
					if ((first_header->type == 1 || second_header->type == 1) &&
						first_header->type != second_header->type || neither)
						result = true;
					else
						result = false;
				}
			}
		}
	}
	return result;
}

struct s_277ec0;
class c_1cf520;
class c_277b80;
struct s_scale_definition;
struct s_scale_owner
{
	byte unknown00[0x3c];
	s_scale_definition *definition;
	real get_inverse_scale() const;
};
struct s_277a00
{
	void *field_0;
	byte field_4[8];
	s_277a00 *field_c;
	byte field_10[8];
	long field_18;
	long field_1c;
	hkEntity *field_20;
};
struct s_277a01
{
	short field_0;
	byte field_2[2];
	c_1cf520 *field_4;
	s_277a00 *field_8;
	s_277a00 *field_c;
	hkEntity *field_10;
	byte field_14[0xc];
	long field_20;
};
bool function_181e90(real arg_0, real arg_1);
void __cdecl function_181ee0(s_scale_owner const *arg_0, s_scale_owner const *arg_1, real *arg_2, real *arg_3);
void __cdecl function_315340(void *arg_0, hkEntity *arg_1, hkEntity *arg_2, real arg_3, real arg_4);
long __stdcall function_277b80(void *arg_0, bool arg_1, real arg_2,
	c_1cf520 *arg_3, short arg_4, s_277ec0 *arg_5, s_277ec0 *arg_6,
	hkEntity *arg_7, hkEntity *arg_8, c_277b80 **arg_9);

PRIVATE __forceinline hkEntity *function_277a25(s_277a00 *arg_0)
{
	while (arg_0->field_c) arg_0 = arg_0->field_c;
	return arg_0->field_18 == 1 ? arg_0->field_20 : NULL;
}

// @retail 0x277a00
void c_interface_277890::event0(void *arg_0)
{
	s_277a01 *local_0 = (s_277a01 *)arg_0;
	if (local_0->field_20 != 1)
	{
		hkEntity *local_1 = function_277a25(local_0->field_8);
		hkEntity *local_2 = function_277a25(local_0->field_c);
		if (local_1 && local_2)
		{
			real local_3 = 1.0f;
			real local_4 = 1.0f;
			if (*((byte *)local_1 + 0x48) == 1 && *((byte *)local_2 + 0x48) == 1 &&
				((hkRigidBody *)local_1)->m_motion->kind_code() != 6 &&
				((hkRigidBody *)local_1)->m_motion->kind_code() != 7 &&
				((hkRigidBody *)local_2)->m_motion->kind_code() != 6 &&
				((hkRigidBody *)local_2)->m_motion->kind_code() != 7 &&
				function_181e90(((s_scale_owner *)local_1)->get_inverse_scale(),
					((s_scale_owner *)local_2)->get_inverse_scale()))
			{
				function_181ee0((s_scale_owner *)local_1, (s_scale_owner *)local_2, &local_3, &local_4);
				function_315340(local_0->field_4, local_1, local_2, local_3, local_4);
			}
			if (local_0->field_10 == local_1)
				function_277b80(this, false, local_3, local_0->field_4, local_0->field_0,
					(s_277ec0 *)local_0->field_8, (s_277ec0 *)local_0->field_c,
					local_1, local_2, (c_277b80 **)local_0->field_c);
			else
				function_277b80(this, true, local_4, local_0->field_4, local_0->field_0,
					(s_277ec0 *)local_0->field_c, (s_277ec0 *)local_0->field_8,
					local_2, local_1, (c_277b80 **)local_0->field_8);
		}
	}
}

struct s_278100
{
    byte field_0[4];
    s_277a00 *field_4;
    s_277a00 *field_8;
    byte field_c[4];
    hkEntity *field_10;
};
struct s_1d0770;
void function_1d0770(s_1d0770 *arg_0, s_havok_component *arg_1, long arg_2,
    long arg_3, long arg_4, long arg_5);
long havok_entity_property_2002_get(hkEntity const *arg_0);

// @retail 0x278100
void c_interface_277890::event2(void *arg_0)
{
    s_278100 *local_0 = (s_278100 *)arg_0;
    hkEntity *local_1 = function_277a25(local_0->field_4);
    hkEntity *local_2 = function_277a25(local_0->field_8);
    if (local_1 && local_2)
    {
        long local_3 = havok_entity_component_index_get(local_1);
        long local_4 = havok_entity_component_index_get(local_2);
        long local_5 = havok_entity_property_2002_get(local_1);
        long local_6 = havok_entity_property_2002_get(local_2);
        if (local_0->field_10 == local_1)
        {
            s_havok_component *local_7 = havok_component_get(local_3);
            if (local_7->unknown04 & 1)
                function_1d0770((s_1d0770 *)local_0, local_7, local_3, local_4, local_5, local_6);
        }
        else
        {
            s_havok_component *local_8 = havok_component_get(local_4);
            if (local_8->unknown04 & 1)
                function_1d0770((s_1d0770 *)local_0, local_8, local_4, local_3, local_6, local_5);
        }
    }
}
