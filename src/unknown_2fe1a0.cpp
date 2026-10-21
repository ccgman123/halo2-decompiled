// @flags /O2 /GL-
// Havok (third-party physics library): two queries passed on to the object
// held at +0x10.

struct s_2fe1a0_result
{
	float values[4];
};

struct s_2fe1a0_range
{
	long field_0;
	long field_4;
};

struct s_2fe1a0_input
{
	long field_0;
	s_2fe1a0_range range;
};

class class_2fe1a0_target;

struct s_2fe1a0_query
{
	class_2fe1a0_target *target;
	s_2fe1a0_range range;
	const s_2fe1a0_input *input;
};

class class_2fe1a0_target
{
public:
	virtual void slot0() = 0;
	virtual void slot1() = 0;
	virtual void slot2() = 0;
	virtual void slot3() = 0;
	virtual void slot4() = 0;
	virtual void slot5() = 0;
	virtual void slot6() = 0;
	virtual void slot7() = 0;
	virtual s_2fe1a0_result slot8(long a, long b) = 0;
	virtual void slot9(long a, const s_2fe1a0_query *query, long c) = 0;
};

class class_2fe1a0
{
public:
	char unknown0[0x10];
	class_2fe1a0_target *target;

	s_2fe1a0_result function_2fe1a0(long a, long b);
	void function_2fe1c0(long a, const s_2fe1a0_input *input, long c);
};

// @retail 0x2fe1a0
s_2fe1a0_result class_2fe1a0::function_2fe1a0(long a, long b)
{
	return target->slot8(a, b);
}

// @retail 0x2fe1c0
// score 10 (was 17): the query is passed to the target through the member of
// this object rather than through the copy stored in the query, but retail
// loads the target once into edx and reuses it for the call where ours loads
// it twice, and retail stores the fields in the order q+8, q+0xc, q+0, q+4
// where ours stores q+0, q+4, q+8, q+0xc.
void class_2fe1a0::function_2fe1c0(long a, const s_2fe1a0_input *input, long c)
{
	s_2fe1a0_query query;
	query.target = target;
	query.range = input->range;
	query.input = input;
	target->slot9(a, &query, c);
}
