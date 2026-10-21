// @flags /O2 /GL- /GX
// Havok (third-party physics library): the object kept for a pair of convex
// shapes: the second's frame against the first's, and the pair's search
// state started from it, with the triangle's own start when the second shape
// is a triangle.
#include "pair_agent_2ed170.h"

struct c_query_transform
{
	__m128 axes[3];
	__m128 position;

	void inverse_product(const c_query_transform *a, const c_query_transform *b);
};

class convex_2fefd0;
struct triangle_2ff0c0;

struct pair_state_2fefd0
{
	unsigned short first_id;
	unsigned short second_id;
	unsigned short field_4;
	unsigned short field_6;
	bool field_8;
	unsigned char field_9;
	bool first_flag;
	unsigned char second_flag;

	void start(convex_2fefd0 const *first, convex_2fefd0 const *second, c_query_transform const *transform);
	void function_2ff0c0(convex_2fefd0 const *first, triangle_2ff0c0 const *triangle, c_query_transform const *transform);
};

class convex_pair_2ee2e0 : public sized_2ed170
{
public:
	pair_state_2fefd0 state;
	long field_18;
	char field_1c[0x2c - 0x1c];
	float field_2c;

	convex_pair_2ee2e0(description_2ed170 const *first, description_2ed170 const *second, long arg_3);
};

// @retail 0x2ee2e0
convex_pair_2ee2e0::convex_pair_2ee2e0(description_2ed170 const *first, description_2ed170 const *second, long arg_3) :
	sized_2ed170(arg_3)
{
	shape_2ed170 const *first_shape = first->shape;
	shape_2ed170 const *second_shape = second->shape;
	c_query_transform transform;
	transform.inverse_product((c_query_transform const *)&first->placement->transform, (c_query_transform const *)&second->placement->transform);
	if (((typed_2ed170 *)second_shape)->type() == 5)
	{
		state.function_2ff0c0((convex_2fefd0 const *)first_shape, (triangle_2ff0c0 const *)second_shape, &transform);
	}
	else
	{
		state.start((convex_2fefd0 const *)first_shape, (convex_2fefd0 const *)second_shape, &transform);
	}
	long &field_18_ref = field_18;
	field_18_ref = 0;
	field_2c = -1.0f;
}
