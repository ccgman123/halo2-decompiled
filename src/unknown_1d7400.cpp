#include "unknown_11c920.h"
#include "unknown_182d90.h"
#include "unknown_1cec30.h"
#include "havok_reference.h"
#include <new>
// @flags /O2 /arch:SSE /Gr

struct s_extent_bounds
{
	__m128 lower;
	__m128 upper;
};

class c_extent_shape
{
public:
	virtual void slot0() {}
	virtual void slot1() {}
	virtual void slot2() {}
	virtual void slot3() {}
	virtual void slot4() {}
	virtual long shape_kind() { return 0; }
	virtual void bounds(s_extent_transform const *transform, real tolerance, s_extent_bounds *result) {}
	virtual void slot7() {}
	virtual void slot8() {}
	virtual void slot9() {}
	virtual void slot10() {}
	virtual long first_key() { return NONE; }
	virtual long next_key(long key) { return NONE; }
	virtual c_extent_shape *child(long key, void *buffer) { return 0; }
	virtual void slot14() {}
	virtual long vertex_count() { return 0; }
};

class c_1d7390
{
public:
	virtual void slot0() = 0;
	virtual void slot1() = 0;
	virtual void slot2() = 0;
	virtual void slot3() = 0;
	virtual void *function_1d7390(long arg_0, long arg_1) = 0;
};


struct c_child_transform : c_havok_reference_counted
{
 dword user;
 long field_c;
 hkTransform transform;
 c_child_transform(c_havok_reference_counted *child);
};
class c_physics_shape_list
{
public:
 c_physics_shape_list(c_havok_reference_counted **shapes, long count);
};
class hkShape;
class c_2dbfc0;
bool function_182180(hkShape const *shape);
real __stdcall function_182aa0(c_extent_shape *shape, real *minimum, real *maximum);
c_2dbfc0 *function_1d7390(long arg_0, void *arg_1, real arg_2);
extern real g_47f05c;

struct s_1d7401
{
 byte field_0[0xc];
 c_extent_shape *field_c;
 hkTransform field_10;
};

struct s_1d7400
{
 c_extent_shape *function_1d7400(c_extent_shape *arg_0, void *arg_1, real arg_2);
};

// @retail 0x1d7400
c_extent_shape *s_1d7400::function_1d7400(c_extent_shape *arg_0, void *arg_1, real arg_2)
{
 c_extent_shape *result = arg_0;
 real local_1, local_0;
 function_182aa0(arg_0, &local_1, &local_0);
 real local_2 = (local_0 * arg_2 * 0.5f + g_47f05c) * g_510c54->field_2_3;
 if (shape_kind_is_list(arg_0->shape_kind()))
 {
  c_extent_shape *local_4[32];
  long local_5 = 0;
  for (long local_6 = arg_0->first_key(); local_6 != NONE; local_6 = arg_0->next_key(local_6))
  {
   __m128 local_7[16];
   c_extent_shape *local_8 = arg_0->child(local_6, local_7);
   if (local_8->shape_kind() == 0x11)
    local_4[local_5++] = local_8;
   else if (local_8->shape_kind() == 0x15)
   {
    /* retail calls the shape test here */
#pragma inline_depth(0)
    bool local_25 = function_182180((hkShape *)((s_1d7401 *)local_8)->field_c);
#pragma inline_depth()
    if (local_25)
    {
     c_havok_reference_counted *local_10 = (c_havok_reference_counted *)function_1d7390((long)((s_1d7401 *)local_8)->field_c, arg_1, local_2);
     c_child_transform *local_18 = (c_child_transform *)((c_1d7390 *)g_480118)->function_1d7390(0x50, 0x22);
     if (local_18)
      new (local_18) c_child_transform(local_10);
     local_18->transform = ((s_1d7401 *)local_8)->field_10;
     havok_reference_remove(local_10);
     local_4[local_5++] = (c_extent_shape *)local_18;
    }
    else
    {
     c_havok_reference_counted *local_11 = (c_havok_reference_counted *)function_1d7400(((s_1d7401 *)local_8)->field_c, arg_1, arg_2);
     void *local_19 = ((c_1d7390 *)g_480118)->function_1d7390(0x50, 0x22);
     c_child_transform *local_20 = local_19 ? new (local_19) c_child_transform(local_11) : NULL;
     local_20->transform = ((s_1d7401 *)local_8)->field_10;
     havok_reference_remove(local_11);
     local_4[local_5++] = (c_extent_shape *)local_20;
    }
   }
   else
   {
#pragma inline_depth(0)
    bool local_26 = function_182180((hkShape *)local_8);
#pragma inline_depth()
    if (local_26)
     local_4[local_5++] = (c_extent_shape *)function_1d7390((long)local_8, arg_1, local_2);
    else
     local_4[local_5++] = function_1d7400(local_8, arg_1, arg_2);
   }
  }
  void *local_12 = ((c_1d7390 *)g_480118)->function_1d7390(0x38, 0x22);
  ((c_havok_reference_counted *)local_12)->allocation_size = 0x38;
  c_extent_shape *local_13 = (c_extent_shape *)new (local_12) c_physics_shape_list((c_havok_reference_counted **)local_4, local_5);
  for (long local_14 = 0; local_14 < local_5; ++local_14)
   if (local_4[local_14]->shape_kind() != 0x11)
    havok_reference_remove((c_havok_reference_counted *)local_4[local_14]);
  result = local_13;
 }
 else if (arg_0->shape_kind() != 0x11)
 {
 if (arg_0->shape_kind() == 0x15)
 {
  c_havok_reference_counted *local_15 = (c_havok_reference_counted *)function_1d7390((long)((s_1d7401 *)arg_0)->field_c, arg_1, local_2);
  void *local_21 = ((c_1d7390 *)g_480118)->function_1d7390(0x50, 0x22);
  c_child_transform *local_22 = local_21 ? new (local_21) c_child_transform(local_15) : NULL;
  local_22->transform = ((s_1d7401 *)arg_0)->field_10;
  havok_reference_remove(local_15);
  result = (c_extent_shape *)local_22;
 }
 else
  result = (c_extent_shape *)function_1d7390((long)arg_0, arg_1, local_2);
 }
 return result;
 }

extern byte g_47f06f;

struct s_1d7300
{
 byte field_0[0xc];
 c_extent_shape *field_c;
 byte field_10[0x2c];
 c_havok_body_motion_state *field_3c;
};

// @retail 0x1d7300
s_1d7300 *function_1d7300(s_1d7300 *arg_0, s_1d7400 *arg_1, real arg_2)
{
 (void)&arg_1; (void)&arg_2;
 s_1d7400 *const *local_2 = &arg_1;
 if (g_47f06f && arg_0->field_3c->kind_code() != 7 && arg_0->field_3c->kind_code() != 6)
 {
  c_extent_shape *local_0 = arg_0->field_c;
  c_extent_shape *local_1 = (*local_2)->function_1d7400(local_0, arg_0, arg_2);
  havok_reference_remove((c_havok_reference_counted *)local_0);
  arg_0->field_c = local_1;
  ((s_havok_component *)*local_2)->unknown04 |= 0x10;
 }
 return arg_0;
}
