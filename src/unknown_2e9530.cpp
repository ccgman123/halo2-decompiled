// @flags /O2 /GL- /GX
// Havok (third-party physics library): an object holding a value, an array of
// 12-byte elements and two vectors that start at the largest float, and a
// kind derived from it; with their creation functions (which take the value
// as their fourth argument).
#include "bounded_2e9530.h"
#include "child_refresh_2e9210.h"
#include "agent_table_2e5d50.h"
#include "pair_agents_2eab70.h"
#include "side_bounds_2e9140.h"
#include "growable_array_2d91d0.h"

// @retail 0x2e9530
bounded_2e9530::~bounded_2e9530()
{
}

// @retail 0x2e95a0 deleting bounded_2e9530

// @retail 0x2e95d0
bounded_2e9530::bounded_2e9530(long value_) : counted_2e9530(value_)
{
	minimum = maximum = _mm_set1_ps(3.402823466e+38f);
}

// @retail 0x2e9620
bounded_2e9530 *__cdecl function_2e9620(long arg_0, long arg_1, long arg_2, long value)
{
	return new bounded_2e9530(value);
}

// @retail 0x2e9680
void bounded_2e9530::function_2e9680(description_2e9210 const *description, typed_2e9210 **other, context_2e9210 const *context)
{
	function_2e9210(other, description, context);
}

// whether the kept objects are brought up to date in place (set) or rebuilt
// into a new array (clear)
bool g_480398;

// up to 128 keys kept in place, more in a block from the allocator
struct keys_2e96a0
{
	unsigned long *data;
	long size;
	unsigned long capacity_and_flags;
	unsigned long storage[128];

	keys_2e96a0() : data(storage), size(0), capacity_and_flags(128 | 0x80000000) {}
	~keys_2e96a0()
	{
		if ((capacity_and_flags & 0x80000000) == 0)
		{
			g_memory_2d88a0->release_block(data, (capacity_and_flags & 0x7fffffff) * sizeof(unsigned long), 0x12);
		}
	}
};

// a shape whose children sit in a tree: the keys of the children a box
// overlaps (slot 12), and the collection that hands the children out
struct tree_shape_2e96a0
{
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
	virtual void overlapping_keys(bounds_2e9140 const *bounds, keys_2e96a0 *keys) const {}
	char unknown4[8];
	collection_2e9210 const *container;
};

// one child of a side: its shape, its key, a placement and the side it
// belongs to
struct side_2e96a0
{
	typed_2e9210 const *shape;
	unsigned long key;
	long placement;
	void const *parent;
};

// the query as it runs here: it hands back the object made for the pair
typedef kept_child_2e9210 *(__cdecl *agent_query_2e96a0)(void const *a, void const *b, void const *input, long value);

// the collection that hands out a side's children, the child it last handed
// out, and room for that child; a child's parent is the side given, and its
// placement that side's
struct children_2e96a0
{
	collection_2e9210 const *container;
	side_2e96a0 child;
	child_buffer_2e9210 buffer;

	children_2e96a0(description_2e9210 const *owner)
	{
		child.parent = owner;
		child.placement = owner->field_8;
	}
};

// a new object for a child: from the query when the filter accepts the
// child, the empty one when it does not
__forceinline kept_child_2e9210 *new_agent_2e96a0(void const *a, children_2e96a0 *children,
	context_2e9210 const *context, unsigned long const *key, pair_query_2e90f0 query, long value)
{
	unsigned long const child_key = *key;
	children->child.shape = children->container->child(child_key, &children->buffer);
	children->child.key = child_key;
	if (context->filter->accept(context, (typed_2e9210 **)a, (description_2e9210 const *)&children->child, children->container, *key).value)
	{
		return ((agent_query_2e96a0)query)(a, &children->child, context, value);
	}
	return (kept_child_2e9210 *)function_301a40();
}

// takes element `index` out, the later ones moving down over it
__forceinline void remove_element_2e96a0(array_2e9530 *elements, element_2e9530 const *at)
{
	elements->size--;
	for (int i = at - elements->data; i < elements->size; i++)
	{
		elements->data[i] = elements->data[i + 1];
	}
}

// room for at least `request` elements, at least twice the old room
__forceinline void reserve_elements_2e96a0(array_2e9530 *elements, int request)
{
	int const capacity = elements->capacity_and_flags & 0x7fffffff;
	if (capacity < request)
	{
		int const doubled = capacity * 2;
		function_2d9160(elements, request < doubled ? doubled : request, sizeof(element_2e9530));
	}
}

// copies `count` elements, the last first
__forceinline void copy_elements_down_2e96a0(element_2e9530 *to, element_2e9530 const *from, int count)
{
	for (int i = count - 1; i >= 0; i--)
	{
		to[i] = from[i];
	}
}

// copies `count` elements, the first first
__forceinline void copy_elements_up_2e96a0(element_2e9530 *to, element_2e9530 const *from, int count)
{
	for (int i = 0; i < count; i++)
	{
		to[i] = from[i];
	}
}

// makes room for `count` elements at `index`, the later ones moving up;
// gives the first of them
__forceinline element_2e9530 *open_gap_2e96a0(array_2e9530 *elements, int index, int count)
{
	int const new_size = count + elements->size;
	int const tail = elements->size - index;
	if (new_size > (int)(elements->capacity_and_flags & 0x7fffffff))
	{
		reserve_elements_2e96a0(elements, new_size);
	}
	copy_elements_down_2e96a0(elements->data + index + count, elements->data + index, tail);
	elements->size = new_size;
	return elements->data + index;
}

__forceinline void set_element_count_2e96a0(array_2e9530 *elements, int count)
{
	reserve_elements_2e96a0(elements, count);
	elements->size = count;
}

// gives up the array's storage when it owns it
__forceinline void free_elements_2e96a0(array_2e9530 *elements)
{
	if ((elements->capacity_and_flags & 0x80000000) == 0)
	{
		element_2e9530 *const block = elements->data;
		g_memory_2d88a0->release_block(block, (elements->capacity_and_flags & 0x7fffffff) * sizeof(element_2e9530), 0x12);
	}
}

// makes `to` a copy of `from`
__forceinline void assign_elements_2e96a0(array_2e9530 *to, array_2e9530 const *from)
{
	if ((int)(to->capacity_and_flags & 0x7fffffff) < from->size)
	{
		free_elements_2e96a0(to);
		to->data = (element_2e9530 *)g_memory_2d88a0->allocate_block(from->size * sizeof(element_2e9530), 0x12);
		to->capacity_and_flags = from->size;
	}
	to->size = from->size;
	copy_elements_up_2e96a0(to->data, from->data, from->size);
}

// sorts `size` keys from low to high
__forceinline void sort_keys_by_2e96a0(unsigned long *data, int size, key_order_2e9360 order)
{
	if (size > 1)
	{
		int const first = 0;
		int const last = size - 1;
		function_2e9360(data, first, last, order);
	}
}

__forceinline void sort_keys_2e96a0(unsigned long *data, int size)
{
	sort_keys_by_2e96a0(data, size, key_order_2e9360());
}

// the kept objects brought up to date in place: the object of each key no
// longer found let go and its element removed, then an element made for each
// new key where it sorts
__forceinline void refresh_in_place_2e96a0(array_2e9530 *elements, keys_2e96a0 *keys, void const *a,
	children_2e96a0 *children, context_2e9210 const *context, pair_query_2e90f0 query, long value)
{
	{
		element_2e9530 *entry = elements->data;
		element_2e9530 *end = elements->data + elements->size;
		unsigned long *key = keys->data;
		unsigned long *const keys_end = keys->data + keys->size;
		for (; entry != end; entry++)
		{
			if (key != keys_end && (unsigned long)entry->value_0 == *key)
			{
				key++;
				continue;
			}
			for (key = keys->data; key != keys_end; key++)
			{
				if ((unsigned long)entry->value_0 == *key)
				{
					key++;
					goto kept;
				}
			}
			((kept_child_2e9210 *)entry->value_8)->release();
			remove_element_2e96a0(elements, entry);
			entry--;
			end--;
		kept:;
		}
	}
	if (keys->size != elements->size)
	{
		element_2e9530 *entry = elements->data;
		element_2e9530 *entries_end = elements->data + elements->size;
		unsigned long *key = keys->data;
		unsigned long *const keys_end = keys->data + keys->size;
		for (; key != keys_end; key++)
		{
			if (entry != entries_end && (unsigned long)entry->value_0 == *key)
			{
				entry++;
				continue;
			}
			int const index = key - keys->data;
			entry = open_gap_2e96a0(elements, index, 1);
			entry->value_8 = (long)new_agent_2e96a0(a, children, context, key, query, value);
			entry->value_0 = *key;
			entries_end = elements->data + elements->size;
			entry++;
		}
	}
}

// the kept objects rebuilt into a new array in key order: an object kept for
// each key still found, made for each new key, let go for each key gone
__forceinline void rebuild_2e96a0(bounded_2e9530 *self, keys_2e96a0 *keys, void const *a,
	children_2e96a0 *children, context_2e9210 const *context, pair_query_2e90f0 query, long value)
{
	element_2e9530 *old = self->elements.data;
	element_2e9530 *const old_end = self->elements.data + self->elements.size;
	unsigned long *key = keys->data;
	unsigned long *const keys_end = keys->data + keys->size;
	stack_elements_2e93e0 fresh(keys->size);
	set_element_count_2e96a0(&fresh, keys->size);
	element_2e9530 *out = fresh.data;
	while (old != old_end && key != keys_end)
	{
		if (*key == (unsigned long)old->value_0)
		{
			*out = *old;
			out++;
			old++;
			key++;
			continue;
		}
		if (*key < (unsigned long)old->value_0)
		{
			out->value_8 = (long)new_agent_2e96a0(a, children, context, key, query, value);
			out->value_0 = *key;
			out++;
			key++;
			continue;
		}
		{
			if (old->value_8)
			{
				((kept_child_2e9210 *)old->value_8)->release();
			}
			old++;
		}
	}
	while (old != old_end)
	{
		if (old->value_8)
		{
			((kept_child_2e9210 *)old->value_8)->release();
		}
		old++;
	}
	while (key != keys_end)
	{
		out->value_8 = (long)new_agent_2e96a0(a, children, context, key, query, value);
		out->value_0 = *key;
		out++;
		key++;
	}
	assign_elements_2e96a0(&self->elements, &fresh);
}

// three axes and a position (0x2da4d0 makes one side's in the other's frame)
struct c_query_transform
{
	__m128 axes[3];
	__m128 position;
	void inverse_product(const c_query_transform *a, const c_query_transform *b);
};

// where a side sits
struct placement_2e96a0
{
	char unknown0[0x20];
	c_query_transform transform;
};

// a shape that gives its bounds in a frame, widened by a tolerance (slot 6)
struct bounded_shape_2e96a0
{
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void slot_2() {}
	virtual void slot_3() {}
	virtual void slot_4() {}
	virtual void slot_5() {}
	virtual void bounds_in(c_query_transform const *transform, float tolerance, bounds_2e9140 *bounds) const {}
};

// one side of a pair query: its shape, its key, its placement and the side
// it belongs to
struct pair_side_2e96a0
{
	void const *shape;
	long key;
	placement_2e96a0 const *placement;
	void const *parent;
};

// the query's input as read here: its tolerance follows the dispatcher and
// the filter
struct query_input_2e96a0
{
	void *dispatcher;
	void *filter;
	float tolerance;
};

// whether the kept bounds may be kept: when set, bounds that moved less
// than half the tolerance on every axis leave the kept objects as they are
bool g_461f08 = true;

// every bit but the sign bit, in four lanes
__declspec(align(16)) static const unsigned long absolute_mask_455940[4] = {
	0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff
};

struct flag_2e96a0
{
	bool value;
};

// whether two corners lie within `limit` of each other on x, y and z
__forceinline flag_2e96a0 corners_close_2e96a0(__m128 const &kept, __m128 fresh, float limit)
{
	__m128 difference = _mm_sub_ps(kept, fresh);
	difference = _mm_and_ps(difference, *(__m128 const *)absolute_mask_455940);
	__m128 const limits = _mm_set1_ps(limit);
	int const apart = _mm_movemask_ps(_mm_cmplt_ps(limits, difference)) & 7;
	flag_2e96a0 close;
	close.value = apart == 0;
	return close;
}

// the objects kept for the children of the second side's tree that the
// first side's bounds (in the second side's frame, widened by one and a half
// times the tolerance) overlap, brought up to date in key order; left as they
// are while the bounds moved less than half the tolerance
// score 5 (was 412): with the bounds in a block of their own, the key array
// before the children, and the array steps as inlines, only the removal of
// an element differs: retail holds the decremented size in ecx and the data
// in eax, ours in eax and edx. Passing the removal an int index computed by
// the caller (instead of the element) reads the data before the decrement
// and scores 444.
// @retail 0x2e96a0
void class_2e96a0::function_2e96a0(void const *a, void const *b, void const *input, pair_query_2e90f0 query)
{
	bounded_2e9530 *self = (bounded_2e9530 *)this;
	pair_side_2e96a0 const *side_a = (pair_side_2e96a0 const *)a;
	pair_side_2e96a0 const *side_b = (pair_side_2e96a0 const *)b;
	bounds_2e9140 bounds;
	{
		c_query_transform transform;
		{
			placement_2e96a0 const *const placement_b = side_b->placement;
			placement_2e96a0 const *const placement_a = side_a->placement;
			transform.inverse_product(&placement_b->transform, &placement_a->transform);
		}
		{
			float const half = ((query_input_2e96a0 const *)input)->tolerance * 0.5f;
			((bounded_shape_2e96a0 const *)side_a->shape)->bounds_in(&transform, ((query_input_2e96a0 const *)input)->tolerance + half, &bounds);
			if (g_461f08 && corners_close_2e96a0(self->maximum, bounds.maximum, half).value && corners_close_2e96a0(self->minimum, bounds.minimum, half).value)
			{
				return;
			}
			self->minimum = bounds.minimum;
			self->maximum = bounds.maximum;
		}
	}
	tree_shape_2e96a0 const *tree = (tree_shape_2e96a0 const *)side_b->shape;
	keys_2e96a0 keys;
	{
		tree->overlapping_keys(&bounds, &keys);
	}
	{
		collection_2e9210 const *const container = tree->container;
		children_2e96a0 children((description_2e9210 const *)b);
		children.container = container;
		if (g_480398)
		{
			refresh_in_place_2e96a0(&self->elements, &keys, a, &children, (context_2e9210 const *)input, query, self->value);
		}
		else
		{
			sort_keys_2e96a0(keys.data, keys.size);
			rebuild_2e96a0(self, &keys, a, &children, (context_2e9210 const *)input, query, self->value);
		}
	}
}
// the objects kept for the children of the second side's tree that the
// first side's bounds overlap, brought up to date in key order
// score 51 (was 512): the shared inlines of 0x2e96a0 and a named pointer to
// the elements for the in-place refresh (without it the keys' count takes
// ebx and the score is 500). Still: retail keeps the first side in ebx, the
// second in edi and `this` in esi (ours edi, esi, ebx), reloads the keys'
// count after the second sort, and differs in the removal's registers as
// 0x2e96a0 does.
// @retail 0x2e9d90
void class_2e9d90::function_2e9d90(void const *a, void const *b, void const *input, pair_query_2e90f0 query)
{
	bounded_2e9530 *self = (bounded_2e9530 *)this;
	bounds_2e9140 bounds;
	function_2e9140(a, b, input, &bounds);
	self->minimum = bounds.minimum;
	self->maximum = bounds.maximum;
	tree_shape_2e96a0 const *tree = *(tree_shape_2e96a0 const *const *)b;
	keys_2e96a0 keys;
	{
		tree->overlapping_keys(&bounds, &keys);
	}
	collection_2e9210 const *const container = tree->container;
	{
		sort_keys_2e96a0(keys.data, keys.size);
		children_2e96a0 children((description_2e9210 const *)a);
		children.container = container;
		if (g_480398)
		{
			array_2e9530 *const elements = &self->elements;
			refresh_in_place_2e96a0(elements, &keys, a, &children, (context_2e9210 const *)input, query, self->value);
		}
		else
		{
			sort_keys_2e96a0(keys.data, keys.size);
			rebuild_2e96a0(self, &keys, a, &children, (context_2e9210 const *)input, query, self->value);
		}
	}
}

// @retail 0x2eb4e0
derived_2eb4e0 *__cdecl function_2eb4e0(long arg_0, long arg_1, long arg_2, long value)
{
	return new derived_2eb4e0(value);
}

struct body_2ea940;
class bounded_2e9530;
class derived_2eb4e0;
struct input_2ea940;
derived_2eb4e0 *__cdecl function_2eb4e0(long arg_0, long arg_1, long arg_2, long value);
void __cdecl function_2eb1d0(void const *a, void const *b, void const *input, void *collector);
void __cdecl function_2eb290(body_2ea940 const *a, body_2ea940 const *b, input_2ea940 const *input, long value);
void __cdecl function_2eb2f0(void const *a, void const *b, void const *input, void *collector, void *second_collector);
bounded_2e9530 *__cdecl function_2e9620(long arg_0, long arg_1, long arg_2, long value);
void __cdecl function_2ead90(void const *a, void const *b, void const *input, void *collector);
void __cdecl function_2ea940(body_2ea940 const *a, body_2ea940 const *b, input_2ea940 const *input, void *collector);
void __cdecl function_2ea640(void const *a, void const *b, void const *input, void *collector, void *second_collector);

// registers handlers for pairs of shape types, each pair both ways round
// where the pair differs
// @retail 0x2eb540
void __cdecl function_2eb540(s_1c3a40 *table)
{
	handlers_2e5d50 handlers;
	handlers.handler[0] = (handler_2e5d50)function_2eb4e0;
	handlers.handler[1] = (handler_2e5d50)function_2eb1d0;
	handlers.handler[2] = (handler_2e5d50)function_2eb290;
	handlers.handler[3] = (handler_2e5d50)function_2eb2f0;
	table->function_2e5d50(&handlers, 3, -1);
	handlers.handler[0] = (handler_2e5d50)function_2e9620;
	handlers.handler[1] = (handler_2e5d50)function_2ead90;
	handlers.handler[2] = (handler_2e5d50)function_2ea940;
	handlers.handler[3] = (handler_2e5d50)function_2ea640;
	table->function_2e5d50(&handlers, -1, 3);
}

// @retail 0x2ebae0 destructor derived_2eb4e0
