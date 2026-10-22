// @flags /O2 /GL-
// Havok (third-party physics library): the timing stream saved: the label of
// every mark is kept once (0x2d89d0), and the stream itself is then copied
// whole, between a header and an end label, after the kept strings.
#include "growable_array_2d91d0.h"
#include "scratch_2d88a0.h"
#include "string_helpers_2d8c90.h"

struct name_2d89d0
{
	char const *text;
	char const *key;
};

ELEMENTS_2D91D0(names_2d89d0, name_2d89d0);
ELEMENTS_2D91D0(bytes_2d89d0, char);

// what comes before the saved stream
struct stream_header_2d8ab0
{
	char const *label;
	char const *field_20;
	char const *field_24;
	long field_18;
	long field_1c;
};

// room for the header and the `length` bytes that follow it, and where they
// start
__forceinline char *expand_2d8ab0(bytes_2d89d0 &bytes, long length)
{
	long offset = bytes.size;
	long needed = offset + length + (long)sizeof(stream_header_2d8ab0) + 4;
	bytes.ensure_capacity(needed);
	char *start = bytes.data + offset;
	bytes.size = needed;
	return start;
}

class strings_2d89d0
{
public:
	names_2d89d0 names;
	bytes_2d89d0 bytes;

	void function_2d89d0(char const *text, unsigned int length);
};

// the labels that open and close a saved stream
char const *g_461e14 = "Ii";
char const *g_461e18 = "Fd";

class c_2d8ab0 : public strings_2d89d0
{
public:
	long field_18;
	long field_1c;
	char const *field_20;
	char const *field_24;

	void function_2d8ab0();
};

// each mark's label kept, a mark's size read from its label's first letter;
// a full block, or a label not known, ends the saving
// @retail 0x2d8ab0
void c_2d8ab0::function_2d8ab0()
{
	char *end = g_480154;
	char *mark = g_480150;
	while (mark < end)
	{
		if ((bytes.capacity_and_flags & 0x7fffffff) - bytes.size < 0x400)
		{
			return;
		}
		char const *label = *(char const **)mark;
		switch (*label)
		{
		case 'E':
		case 'S':
		case 'T':
		case 'l':
			function_2d89d0(label, 0);
			mark += 12;
			break;
		case 'L':
		{
			// two strings, one after the other
			unsigned int first = string_length_2d8d80(label);
			unsigned int second = string_length_2d8d80(*(char const **)mark + first + 1);
			unsigned int total = first + second + 2;
			function_2d89d0(*(char const **)mark, total);
			mark += 12;
			break;
		}
		case 'M':
			function_2d89d0(label, 0);
			mark += 8;
			break;
		case 'P':
		case 'p':
			function_2d89d0(label, 0);
			mark += 4;
			break;
		default:
			return;
		}
	}
	function_2d89d0(g_461e18, 0);
	function_2d89d0(g_461e14, 0);
	function_2d89d0(field_20, 0);
	function_2d89d0(field_24, 0);
	long room = (bytes.capacity_and_flags & 0x7fffffff) - bytes.size;
	if (room < 0x400)
	{
		return;
	}
	long length = g_480154 - g_480150;
	if (room < length + (long)sizeof(stream_header_2d8ab0) + 4)
	{
		return;
	}
	stream_header_2d8ab0 *header = (stream_header_2d8ab0 *)expand_2d8ab0(bytes, length);
	header->label = g_461e14;
	header->field_20 = field_20;
	header->field_24 = field_24;
	header->field_18 = field_18;
	header->field_1c = field_1c;
	function_2d8db0(header + 1, g_480150, length);
	*(char const **)((char *)(header + 1) + length) = g_461e18;
}
