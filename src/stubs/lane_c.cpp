// stubs for lane C (0x1c0000..0x1cffff): callees outside the region that are
// not decompiled yet, and the library (Havok) functions the region calls
#include "unknown_11c920.h"
#include "unknown_1cec30.h"


class c_1d1261
{
public:
 hkBool function_30ca30(void *arg_0);
 void function_30bd10(void *arg_0);
};

class c_2df9d0
{
public:
 virtual hkBool function_2df9d0(void const *arg_0, void const *arg_1);
};

class c_2df8f0
{
public:
 virtual hkBool function_2df8f0(void const *arg_0, void const *arg_1);
};

class c_2dfae0
{
public:
 virtual hkBool function_2dfae0(void const *arg_0, void const *arg_1);
};

class c_2dfbf0
{
public:
 c_2dfbf0();
 void function_2df950(long arg_0, long arg_1);
 void function_2df990(long arg_0, long arg_1);
};

struct s_3111d0
{
 s_3111d0();
};

struct s_30cb70
{
 byte field_0;
};
class c_interface_278b40;
class c_3101c0
{
public:
 c_3101c0(s_3111d0 const &arg_0, long arg_1);
 void function_30c1a0(byte *arg_0);
 void function_30cb70(s_30cb70 arg_0);
 void function_30c500();
 void function_310900(c_2dfbf0 *arg_0, s_30cb70 arg_1);
 void function_30cab0(c_interface_278b40 *arg_0);
};



struct s_311100;
class c_311100
{
public:
 void function_311100(s_311100 const *arg_0);
};

// @stub 0x2d8780
void __cdecl function_2d8780() {}
// @stub 0x2d7f00
void __cdecl function_2d7f00() {}
class c_278370
{
public:
 virtual void function_a9ef0(void *arg_0);
};
void c_278370::function_a9ef0(void *arg_0) {}

// @stub 0x2d8890
void __cdecl function_2d8890() {}

class c_library_30c470
{
public:
	void detach(void *callback);
};



class c_278b60_filter;
struct s_278b60_dispatch;
struct s_278b60_filter;
class c_library_30c190
{
public:
	c_278b60_filter *filter();
	byte unknown00[0xcc];
	s_278b60_dispatch *dispatch;
	s_278b60_filter *filter_object;
};

#include "slot_owner.h"
#include "unknown_1c62f0.h"
#include "unknown_0259a0.h"
#include "unknown_1765e0.h"



// @stub 0x312530
void hkEntity::entity_property_put(dword key, s_havok_property_word value) { }

/* slot handler callbacks of the region not decompiled yet (their handler
   structs hold their addresses) */

/* game functions outside the region called by the ai lifecycle callbacks */





/* the animation graph lookups (0x1d9000..0x1de000) */


/* the physics callees of the havok components */
struct s_havok_component;


/* callees of the slot handler callbacks (unknown_0259a0.h) */






/* callees of the physics code (unknown_1c25a0.cpp, unknown_1cec30.cpp) */









/* in the region, not decompiled yet */







/* the rigid body accessors of the havok components (unknown_1cec30.cpp) */




/* callees of ai.cpp's 0x1caa40 */



struct real_quaternion_transform;










class c_component_rotation
{
public:
	hkVector4 value;
	void set(hkRotation const &rotation);
};



class c_contact_rule_fallback
{
public:
 hkBool accepts(long a, long b);
};


class c_component_joint_strength
{
public:
    void set_strength(real strength);
};

// @stub 0x312b80
void c_component_joint_strength::set_strength(real strength) {}

#include <xmmintrin.h>
struct c_component_joint_snapshot_0
{
    __m128 data[6];
    c_component_joint_snapshot_0();
};
struct c_component_joint_reader_0
{
    void read(c_component_joint_snapshot_0 *output);
};

struct c_component_joint_snapshot_1
{
    __m128 data[9];
    c_component_joint_snapshot_1();
};
struct c_component_joint_reader_1
{
    void read(c_component_joint_snapshot_1 *output);
};

struct c_component_joint_snapshot_2
{
    __m128 data[9];
    c_component_joint_snapshot_2();
};
struct c_component_joint_reader_2
{
    void read(c_component_joint_snapshot_2 *output);
};

struct c_component_joint_snapshot_4
{
    __m128 data[3];
    c_component_joint_snapshot_4();
};
struct c_component_joint_reader_4
{
    void read(c_component_joint_snapshot_4 *output);
};

// @stub 0x313b30
c_component_joint_snapshot_0::c_component_joint_snapshot_0() {}

// @stub 0x313c50
void c_component_joint_reader_0::read(c_component_joint_snapshot_0 *output) {}

// @stub 0x313320
c_component_joint_snapshot_1::c_component_joint_snapshot_1() {}

// @stub 0x3134a0
void c_component_joint_reader_1::read(c_component_joint_snapshot_1 *output) {}

// @stub 0x312920
c_component_joint_snapshot_2::c_component_joint_snapshot_2() {}

// @stub 0x312ab0
void c_component_joint_reader_2::read(c_component_joint_snapshot_2 *output) {}

// @stub 0x3140c0
c_component_joint_snapshot_4::c_component_joint_snapshot_4() {}

// @stub 0x3141c0
void c_component_joint_reader_4::read(c_component_joint_snapshot_4 *output) {}


class c_world_callback_registration
{
public:
    void remove(void *callback);
};


struct c_contact_query_bounds_info { c_contact_query_bounds_info(); };
struct c_contact_query_transform_info { c_contact_query_transform_info(); };
class c_contact_query_bounds_volume
{
public:
    c_contact_query_bounds_volume(c_contact_query_bounds_info const *info);
};
#include "contact_volume_30bb50.h"
class c_contact_query_world
{
public:
    void *add(void *volume);
    void remove(void *volume);
};


class c_havok_abstract_allocator;
struct s_fixed_memory_statistics_16;
struct s_physics_pool_statistics;




struct s_physics_mass_array;
struct s_physics_mass;

class c_havok_reference_counted;


struct s_2dc030
{
    byte field_0[8];
    long field_8;
    real field_c;
    real field_10;
    s_2dc030();
};

class c_2dbfc0
{
public:
    byte field_0[4];
    word field_4;
    byte field_6[0x3a];
    c_2dbfc0(long arg_0, void *arg_1, real arg_2);
    void function_2dbcf0(s_2dc030 const *arg_0);
};

struct s_1d1870
{
 char field_0;
 s_1d1870() {}
 s_1d1870(bool arg_0) : field_0(arg_0) {}
};

class c_314710
{
public:
    byte field_0[0x60];
    c_314710(hkEntity *arg_0);
    s_1d1870 function_3144b0(void *arg_0);
    s_1d1870 function_314580(void *arg_0);
};

// @stub 0x314710
c_314710::c_314710(hkEntity *arg_0) {}

struct c_shape_library_base_a
{
 c_shape_library_base_a(c_havok_reference_counted *arg_0, long arg_1);
};

struct s_2e5a10 { byte field_0; s_2e5a10(const s_2e5a10 &arg_0) : field_0(arg_0.field_0) {} };

struct s_1c3a40
{
 void function_2e5e70(long arg_0, long arg_1);
 void function_2e5a10(s_2e5a10 arg_0);
};

// @stub 0x2e5e70
void s_1c3a40::function_2e5e70(long arg_0, long arg_1) {}


// @stub 0x2e6190
void __cdecl function_2e6190(s_1c3a40 *arg_0) {}

// @stub 0x2e6cd0
void __cdecl function_2e6cd0(s_1c3a40 *arg_0) {}

// @stub 0x2e7d10
void __cdecl function_2e7d10(s_1c3a40 *arg_0) {}

// @stub 0x2e9030
void __cdecl function_2e9030(s_1c3a40 *arg_0) {}

// @stub 0x3144b0
s_1d1870 c_314710::function_3144b0(void *arg_0) { return s_1d1870(); }

// @stub 0x314580
s_1d1870 c_314710::function_314580(void *arg_0) { return s_1d1870(); }

struct s_311340 { s_311340(); };
class c_311ba0 { public: c_311ba0(const s_311340 *arg_0); };

struct s_2da8c0
{
 void function_2da8c0(hkRotation const *arg_0, hkRotation const *arg_1);
};



struct s_1d1540 { void function_30be00(void *arg_0); };
struct s_1d1541 { void *function_30d2b0(void *arg_0); };
struct s_314320 { s_314320(); };
struct s_314450
{
 void function_314450(s_314320 *arg_0);
 void function_314480(s_314320 const *arg_0);
};
// @stub 0x314320
s_314320::s_314320() {}
// @stub 0x314450
void s_314450::function_314450(s_314320 *arg_0) {}
// @stub 0x314480
void s_314450::function_314480(s_314320 const *arg_0) {}



/* the two snapshots and the six constraints 0x1d77d0 builds */
struct c_component_joint_snapshot_3
{
    __m128 data[4];
    c_component_joint_snapshot_3();
};

struct c_component_joint_snapshot_5
{
    __m128 data[10];
    c_component_joint_snapshot_5();
};

class c_component_joint_0 { public: c_component_joint_0(c_component_joint_snapshot_0 const *info); };
class c_component_joint_1 { public: c_component_joint_1(c_component_joint_snapshot_1 const *info); };
class c_component_joint_2 { public: c_component_joint_2(c_component_joint_snapshot_2 const *info); };
class c_component_joint_3 { public: c_component_joint_3(c_component_joint_snapshot_3 const *info); };
class c_component_joint_4 { public: c_component_joint_4(c_component_joint_snapshot_4 const *info); };
class c_component_joint_5 { public: c_component_joint_5(c_component_joint_snapshot_5 const *info); };

// @stub 0x314860
c_component_joint_snapshot_3::c_component_joint_snapshot_3() {}

// @stub 0x314bb0
c_component_joint_snapshot_5::c_component_joint_snapshot_5() {}

// @stub 0x313de0
c_component_joint_0::c_component_joint_0(c_component_joint_snapshot_0 const *info) {}

// @stub 0x313550
c_component_joint_1::c_component_joint_1(c_component_joint_snapshot_1 const *info) {}

// @stub 0x312d50
c_component_joint_2::c_component_joint_2(c_component_joint_snapshot_2 const *info) {}

// @stub 0x314b00
c_component_joint_3::c_component_joint_3(c_component_joint_snapshot_3 const *info) {}

// @stub 0x314270
c_component_joint_4::c_component_joint_4(c_component_joint_snapshot_4 const *info) {}

// @stub 0x314ef0
c_component_joint_5::c_component_joint_5(c_component_joint_snapshot_5 const *info) {}
