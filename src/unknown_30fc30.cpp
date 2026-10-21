// @flags /O2 /GL-
// Havok (third-party physics library): a world flushes its list of entities
// waiting to be removed: each one still in an island is removed, and the list
// drops its reference to each; and the collision and integration parts of a
// world's step.
#include "growable_array_2d91d0.h"
#include "timer_mark_2ebc80.h"

class hkBool
{
public:
	hkBool() {}

	char m_bool;
};

bool __cdecl function_279740(long value);

class deletable_30fc30
{
public:
	virtual void destroy(unsigned int flags) = 0;
};

class hkEntity
{
public:
	unsigned short unknown0[3];
	unsigned short reference_count;
	char unknown8[0x44 - 8];
	void *island;
};

ELEMENTS_2D91D0(entities_30fc30, hkEntity *);

class c_havok_body_group;

class hkWorld
{
public:
	char unknown0[0xf0];
	entities_30fc30 pending_removals;

	hkBool entity_world_remove(hkEntity *entity);
	void function_30fc30();
	void world_island_discard(c_havok_body_group *island);
	void function_30ce20();
};

// @retail 0x30fc30
void hkWorld::function_30fc30()
{
	for (long i = 0; i < pending_removals.size; i++)
	{
		hkEntity *entity = pending_removals.data[i];
		if (entity->island != 0)
		{
			entity_world_remove(entity);
		}
		entity = pending_removals.data[i];
		entity->reference_count--;
		if (entity->reference_count == 0)
		{
			if (function_279740((long)entity))
			{
				entity->reference_count = 1;
			}
			else
			{
				// the deleting destructor, first in the vtable, without the
				// null test `delete` adds
				((deletable_30fc30 *)entity)->destroy(1);
			}
		}
	}
	pending_removals.size = 0;
}

// the parts of a world's step run by other classes' code
class c_1d1261
{
public:
	void function_30cda0();
};

class class_30f030
{
public:
	void function_30f6b0();
};

class narrow_phase_317c00
{
public:
	void function_317d30(void const *step, void const *input);
};

struct listener_world_3187a0;
struct listener_world_318830;
void __cdecl function_3187a0(listener_world_3187a0 *world, void *value);
void __cdecl function_3188c0(listener_world_318830 *world, void *first, void *second);

// a step's length and a second term, from the caller
struct s_311100
{
	float delta;
	float field_4;
};

struct step_terms_311100;

// the terms a step's terms give: their two scaled terms and two products
struct step_products_311100
{
	float scaled_delta;
	float scaled_field_4;
	float field_8;
	float field_c;

	inline void set_from(step_terms_311100 const *terms);
};

// the terms a step works out from its length (at 0x170 in the world)
struct step_terms_311100
{
	float scaled_delta;
	float scaled_field_4;
	char unknown8[0xc - 8];
	long field_c;
	float field_10;
	char unknown14[0x28 - 0x14];
	step_products_311100 products;

	void scale_by(s_311100 const *description)
	{
		scaled_delta = description->delta * field_10;
		scaled_field_4 = field_c * description->field_4;
		products.set_from(this);
	}
};

// the two scaled terms, and each multiplied by the other term's factor (read
// through the terms' pointer, so after each store)
inline void step_products_311100::set_from(step_terms_311100 const *terms)
{
	scaled_delta = terms->scaled_delta;
	scaled_field_4 = terms->scaled_field_4;
	field_8 = terms->field_c * terms->scaled_delta;
	field_c = terms->field_10 * terms->scaled_field_4;
}
struct step_info_311100
{
	long field_0;
	long field_4;
	long field_8;
	long field_c;
};

// an object the world holds a reference to until the step's end
class released_30fef0
{
public:
	virtual void destroy(unsigned int flags) = 0;

	unsigned short unknown4;
	unsigned short reference_count;
};

// an island as the integration sees it
struct island_30fef0
{
	char unknown0[0x30];
	bool field_30;
};

struct answer_316b30
{
	char value;

	answer_316b30() {}
	answer_316b30(bool value_) : value((char)value_) {}
};

class timers_316b30
{
public:
	answer_316b30 function_316b30(float const *step);
};

struct step_316a20;
class simulation_316a20
{
public:
	void function_316a20(step_316a20 *step);
};

class class_30d320
{
public:
	void function_30d400();
};

struct class_30c180;
struct event_318700;
void __cdecl function_318700(class_30c180 *source, event_318700 *event);
void __cdecl function_318830(listener_world_318830 *world, void *first, void *second);
void function_175f40();

// a world seen as the object that runs a step
class c_311100
{
public:
	char unknown0[8];
	narrow_phase_317c00 **islands;
	long island_count;
	char unknown10[0x24 - 0x10];
	long pending_count;
	char unknown28[0xa0 - 0x28];
	void *integrate_listener;
	char unknowna4[0xac - 0xa4];
	void *island_listener;
	char unknownb0[0xc4 - 0xb0];
	void const *step;
	char unknownc8[0xcc - 0xc8];
	step_info_311100 const *info;
	char unknownd0[0xf4 - 0xd0];
	long pending_removal_count;
	char unknownf8[0x100 - 0xf8];
	long collected_count;
	char unknown104[0x10c - 0x104];
	long integrated_count;
	char unknown110[0x114 - 0x110];
	released_30fef0 **released;
	long released_count;
	char unknown11c[0x12c - 0x11c];
	bool stepping;
	bool collision_pass_running;
	char unknown12e[0x140 - 0x12e];
	step_info_311100 input;
	s_311100 description;
	char unknown158[0x170 - 0x158];
	step_terms_311100 terms;
	char unknown1a8[0x23c - 0x1a8];
	bool flag_23c;
	// islands may be discarded after integrating
	bool flag_23d;

	void function_30fcb0(s_311100 const *description);
	void function_30fef0(s_311100 const *description);
	void function_30e720();
};

// score 5 (was 110): retail loads info before description_, and its last
// product loads field_10 first (fld field_10, fmul scaled_field_4); 0x30fef0
// matches with the same inline. Copy orders, an explicit scale and two
// anneals found nothing better.
// runs the collision part of a world's step: the step's terms set from the
// description, each island's contacts worked out and the island listeners
// told, the pending removals and other work flushed, and the post-collide
// listeners told; marked in the timing stream throughout
// @retail 0x30fcb0
void c_311100::function_30fcb0(s_311100 const *description_)
{
	timer_add_2ebc80("LtSimulate");
	step_info_311100 const *source = info;
	description = *description_;
	input = *source;
	terms.scale_by(description_);
	stepping = true;
	if (pending_count > 0)
	{
		((class_30f030 *)this)->function_30f6b0();
	}
	collision_pass_running = true;
	for (int i = 0; i < island_count; i++)
	{
		islands[i]->function_317d30(step, &input);
		if (island_listener != 0)
		{
			timer_add_2ebc80("TtIslandPostCollideCb");
			collision_pass_running = false;
			function_3188c0((listener_world_318830 *)this, islands[i], (void *)description_);
			collision_pass_running = true;
			timer_add_2ebc80("Et");
		}
	}
	collision_pass_running = false;
	stepping = false;
	if (pending_count > 0)
	{
		((class_30f030 *)this)->function_30f6b0();
	}
	if (pending_removal_count != 0)
	{
		((hkWorld *)this)->function_30fc30();
	}
	if (collected_count != 0)
	{
		((c_1d1261 *)this)->function_30cda0();
	}
	if (pending_count > 0)
	{
		((class_30f030 *)this)->function_30f6b0();
	}
	timer_add_2ebc80("StPostCollideCB");
	function_3187a0((listener_world_3187a0 *)this, (void *)description_);
	timer_add_2ebc80("lt");
}

// runs the integration part of a world's step: the step's terms set from the
// description, each island integrated (or discarded when it asks to be) and
// the post-integrate listeners told, pending work flushed, the references
// held until the step's end dropped, and the post-simulate listeners told;
// marked in the timing stream throughout
// @retail 0x30fef0
void c_311100::function_30fef0(s_311100 const *description_)
{
	timer_add_2ebc80("LtSimulate");
	description = *description_;
	terms.scale_by(description_);
	function_175f40();
	if (pending_count > 0)
	{
		((class_30f030 *)this)->function_30f6b0();
	}
	if (integrated_count != 0)
	{
		((hkWorld *)this)->function_30ce20();
	}
	integrated_count = 0;
	stepping = true;
	for (int i = island_count - 1; i >= 0; i--)
	{
		narrow_phase_317c00 *island = islands[i];
		if (!(flag_23d && ((timers_316b30 *)island)->function_316b30(&description_->delta).value))
		{
			((simulation_316a20 *)island)->function_316a20((step_316a20 *)&input);
			((island_30fef0 *)island)->field_30 = false;
		}
		else
		{
			((hkWorld *)this)->world_island_discard((c_havok_body_group *)island);
		}
		if (integrate_listener != 0)
		{
			timer_add_2ebc80("TtPostIntegrateCb");
			function_318830((listener_world_318830 *)this, island, (void *)description_);
			timer_add_2ebc80("Et");
		}
	}
	if (flag_23c)
	{
		function_30e720();
	}
	stepping = false;
	if (pending_removal_count != 0)
	{
		((hkWorld *)this)->function_30fc30();
	}
	if (collected_count != 0)
	{
		((c_1d1261 *)this)->function_30cda0();
	}
	for (long k = 0; k < released_count; k++)
	{
		((class_30d320 *)released[k])->function_30d400();
		released_30fef0 *object = released[k];
		object->reference_count--;
		if (object->reference_count == 0)
		{
			if (function_279740((long)object))
			{
				object->reference_count = 1;
			}
			else
			{
				object->destroy(1);
			}
		}
	}
	released_count = 0;
	timer_add_2ebc80("lt");
	timer_add_2ebc80("TtPostSimulateCb");
	function_318700((class_30c180 *)this, (event_318700 *)description_);
	timer_add_2ebc80("Et");
}
