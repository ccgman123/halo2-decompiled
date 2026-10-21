// @flags /O2 /GL-
// Havok (third-party physics library): a reference-counted object's
// constructor: it takes a reference to the object it is given, and points
// its list's owner at itself.
#include "world_object_30b340.h"

// score 12 (was 12): retail stores the vtable and reference count first and the
// fields out of offset order (+0x14 and +0x18 before +0x10, +0x24 before +0x1c);
// the compiler emits the member stores in declaration order whatever order the
// initializer list is in, so the header would have to change
// tried and no better: the initializer list in retail's store order (the same
// bytes), the stores in the body instead of the initializer list (16: the base
// class's stores then come first as in retail, but the list's own stores move
// ahead of the fields)
// @retail 0x30b340
class_30b340::class_30b340(referenced_2d88a0 *object_, long value)
	: field_8(0), object(object_), field_10(-1), field_14(0), field_18(0), field_1c(0), field_20(0), field_24(value)
{
	list.owner = this;
	if (object_)
	{
		object_->reference_count++;
	}
}
