#include "unknown_11c920.h"
#include "unknown_1d5120.h"
#include "object_queries.h"
#include <float.h>
#include <math.h>
// @flags /O2 /arch:SSE /Gr

extern hkWorld *g_51e9a4;
struct s_vehicle_ray;
bool __stdcall function_168f40(long arg_0, s_vehicle_ray const *arg_1, long arg_2, long arg_3);
bool function_182800(s_vehicle_ray *arg_0, long arg_1, void *arg_2);
void function_1d43d0(s_havok_component *arg_0, transform4x3f const *arg_1, transform4x3f *arg_2);
void function_11bed0(s_location *arg_0, point3f const *arg_1);

class c_contact_shape_view
{
public:
 virtual void slot0() = 0;
 virtual void slot1() = 0;
 virtual void slot2() = 0;
 virtual void slot3() = 0;
 virtual void slot4() = 0;
 virtual long type() = 0;
 long unknown04;
 long metadata;
};
struct s_1d48f0
{
 point3f field_0;
 vector3f field_c;
 real field_18;
 vector3f field_1c;
};
const dword g_455978[81] = {
 0x0, 0x0, 0x0, 0x3f800000, 0x0, 0x0, 0x0, 0x0, 0x3f800000,
 0x3f3504f3, 0x0, 0x3f3504f3, 0x3f13cd3a, 0x3f13cd3a, 0x3f13cd3a, 0x3f13cd3a, 0xbf13cd3a, 0x3f13cd3a,
 0x3f3504f3, 0x3f3504f3, 0x0, 0x3f3504f3, 0xbf3504f3, 0x0, 0x0, 0x3f3504f3, 0x3f3504f3,
 0x0, 0xbf3504f3, 0x3f3504f3, 0xbf800000, 0x0, 0x0, 0x0, 0x3f800000, 0x0,
 0x0, 0xbf800000, 0x0, 0xbf3504f3, 0xbf3504f3, 0x0, 0xbf3504f3, 0x3f3504f3, 0x0,
 0xbf3504f3, 0x0, 0x3f3504f3, 0xbf13cd3a, 0xbf13cd3a, 0x3f13cd3a, 0xbf13cd3a, 0x3f13cd3a, 0x3f13cd3a,
 0x0, 0x0, 0xbf800000, 0x3f3504f3, 0x0, 0xbf3504f3, 0xbf3504f3, 0x0, 0xbf3504f3,
 0x0, 0x3f3504f3, 0xbf3504f3, 0x0, 0xbf3504f3, 0xbf3504f3, 0x3f13cd3a, 0x3f13cd3a, 0xbf13cd3a,
 0x3f13cd3a, 0xbf13cd3a, 0xbf13cd3a, 0xbf13cd3a, 0xbf13cd3a, 0xbf13cd3a, 0xbf13cd3a, 0x3f13cd3a, 0xbf13cd3a
};

// @retail 0x1d48f0
bool function_1d48f0(point3f *arg_11, s_havok_component *arg_0, long arg_1, long arg_2, point3f const *arg_3,
 vector3f const *arg_4, s_location *arg_5, real arg_6, real arg_7, bool arg_8,
 point3f const *arg_9, long arg_10)
{
 c_contact_query_bounds_volume *local_1 = NULL;
 long local_2 = NONE;
 _control87(0x9001f, 0x8001f);
 _mm_setcsr(_mm_getcsr() | 0x1f80);
 if (arg_9)
 {
  c_contact_query_bounds_info local_3;
  real local_4 = arg_7 + 0.001f;
  local_3.lower.set(arg_3->x - local_4, arg_3->y - local_4, arg_3->z - local_4);
  local_3.upper.set(arg_3->x + local_4, arg_3->y + local_4, arg_3->z + local_4);
  for (long local_5 = 0; local_5 < 3; ++local_5)
  {
   if (((real *)&local_3.lower.m_quad)[local_5] + 0.001f > arg_9->n[local_5])
    ((real *)&local_3.lower.m_quad)[local_5] = arg_9->n[local_5] - 0.001f;
   if (arg_9->n[local_5] > ((real *)&local_3.upper.m_quad)[local_5] - 0.001f)
    ((real *)&local_3.upper.m_quad)[local_5] = arg_9->n[local_5] + 0.001f;
  }
  local_3.filter = ((arg_0->object_index + 1) << 16) | arg_2;
  local_1 = new c_contact_query_bounds_volume(&local_3);
  ((c_contact_query_world *)g_51e9a4)->add(local_1);
  if (arg_10 != NONE) local_2 = havok_object_get(arg_10)->havok_component_index;
 }
 long local_6 = 18 + 9 * arg_8;
 bool local_0 = false;
 hkTransform local_7;
 c_contact_query_transform_volume *local_8;
 {
  c_contact_query_transform_info local_9;
  hkRigidBody *local_10 = havok_component_rigid_body_get(arg_1, arg_0);
  c_contact_shape_view *local_11 = *(c_contact_shape_view **)((byte *)local_10 + 0xc);
  if (local_11->type() == 0x17) local_11 = *(c_contact_shape_view **)((byte *)local_11 + 0x30);
  local_7.set(local_10->m_motion->m_transform);
  local_9.transform.set(local_7);
  local_9.shape = local_11;
  local_9.filter = ((arg_0->object_index + 1) << 16) | arg_2;
  local_8 = new c_contact_query_transform_volume(&local_9);
 }
 ((c_contact_query_world *)g_51e9a4)->add(local_8);
 c_contact_presence local_12;
 local_12.found = false;
 for (long local_13 = 0; local_13 < local_6; ++local_13)
 {
  vector3f local_14 = ((vector3f const *)g_455978)[local_13];
  if (arg_6 > 0.0f)
  {
   vector3f local_15 = ((vector3f const *)g_455978)[local_13];
   real local_16 = (real)sqrt((double)*(real const volatile *)&local_15.i * local_15.i +
    (double)*(real const volatile *)&local_15.j * local_15.j);
   if (fabs(local_16) < 0.0001f) continue;
   real local_17 = 1.0f / local_16;
   local_15.i *= local_17;
   local_15.j *= local_17;
   local_15.k = 0.0f * local_17;
   if (local_16 == 0.0f) continue;
   local_15.i *= arg_6;
   local_15.j *= arg_6;
   local_15.k *= arg_6;
   real local_18 = (arg_7 - arg_6) / arg_7;
   local_14.i = local_14.i * local_18 + local_15.i;
   local_14.j = local_14.j * local_18 + local_15.j;
   local_14.k = local_14.k * local_18 + local_15.k;
  }
  arg_11->x = local_14.i * arg_7 + arg_3->x;
  arg_11->y = local_14.j * arg_7 + arg_3->y;
  arg_11->z = local_14.k * arg_7 + arg_3->z;
  local_7.m_translation.set(arg_11->x, arg_11->y, arg_11->z);
  local_12.found = false;
  ((hkRigidBody *)local_8)->pose_write(local_7);
  ((c_contact_query_dispatch *)local_8)->query(&local_12);
  if (local_12.found) continue;
  point3f local_19 = *arg_11;
  local_19.x += arg_4->i;
  local_19.y += arg_4->j;
  local_19.z += arg_4->k;
  if (function_168f40(0x1480040d, (s_vehicle_ray const *)&local_19, arg_0->object_index, NONE)) continue;
  if (local_1)
  {
   s_1d48f0 local_20;
   local_20.field_0 = *arg_9;
   local_20.field_c.i = local_19.x - local_20.field_0.x;
   local_20.field_c.j = local_19.y - local_20.field_0.y;
   local_20.field_c.k = local_19.z - local_20.field_0.z;
   if (function_182800((s_vehicle_ray *)&local_20, local_2, local_1) && !local_0) continue;
  }
  transform4x3f local_21;
  transform4x3f local_22;
  local_21.scale = 1.0f;
  local_21.forward.i = local_7.m_rotation.m_col0(0);
  local_21.forward.j = local_7.m_rotation.m_col0(1);
  local_21.forward.k = local_7.m_rotation.m_col0(2);
  local_21.left.i = local_7.m_rotation.m_col1(0);
  local_21.left.j = local_7.m_rotation.m_col1(1);
  local_21.left.k = local_7.m_rotation.m_col1(2);
  local_21.up.i = local_7.m_rotation.m_col2(0);
  local_21.up.j = local_7.m_rotation.m_col2(1);
  local_21.up.k = local_7.m_rotation.m_col2(2);
  local_21.position.x = local_7.m_translation(0);
  local_21.position.y = local_7.m_translation(1);
  local_21.position.z = local_7.m_translation(2);
  function_1d43d0(arg_0, &local_21, &local_22);
  *arg_11 = local_22.position;
  function_11bed0(arg_5, arg_11);
  local_0 = arg_5->cluster_index != NONE;
  if (local_0) break;
 }
 ((c_contact_query_world *)g_51e9a4)->remove(local_8);
 havok_reference_remove((c_havok_reference_counted *)local_8);
 if (local_1)
 {
  ((c_contact_query_world *)g_51e9a4)->remove(local_1);
  havok_reference_remove((c_havok_reference_counted *)local_1);
 }
 _mm_setcsr(_mm_getcsr() & 0xffffffc0);
 _clearfp();
 _control87(0x9001f, 0xfffff);
 return local_0;
}
