// @flags /O2 /GL-
// Havok (third-party physics library): reporting an error message through
// the error handler the game installs.
#include "stream_holder_2d8e70.h"

// the game's error handler; its third slot reports a message
class error_handler_2d97d0
{
public:
	virtual void slot_0() {}
	virtual void slot_1() {}
	virtual void report(char const *title, unsigned long id, char const *message, char const *file, long line) {}
};

error_handler_2d97d0 *g_480178;

// @retail 0x2d97d0
// score 29 (was 29): retail's frame is four bytes larger and holds the
// address of the flag, which is how MSVC records an exception scope; our
// build has no /EHsc and so never writes that address, and adding /EHsc
// would add the handler registration retail does not have. Everything else
// (the calls, the global and the string pointers) matches.
void __cdecl function_2d97d0(char const *message)
{
	char buffer[0x200];
	flag_2d8e70 zero(true);
	stream_holder_2d8e70 stream(buffer, sizeof(buffer), zero);
	stream.function_2d8f10(message);
	g_480178->report("Error", 0x2636fe25, buffer, ".\\error\\hkError.cpp", 27);
}
