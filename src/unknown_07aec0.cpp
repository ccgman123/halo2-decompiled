// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_07AEC0.CPP: network addresses */

#include "unknown_11c920.h"
#include <string.h>

/* Bungie's macro, as in Halo CE's cseries.h */
#ifndef MIN
#define MIN(a,b) ((a)>(b)?(b):(a))
#endif

enum
{
	k_ipv4_address_length = 4,
	k_ipv6_address_length = 16
};

struct s_type_99af70
{
	union
	{
		dword ipv4_address;
		word ipv6_address[8];
	};
	word port;
	short address_length;
};

bool function_7af40(s_type_99af70 const *address);

/* an IPv4 address in host byte order whose last octet is zero */
// score 55 (was 55): retail pushes esi after the first branch, ours pushes it first, and the
// two comparisons of the field value are ordered differently.
// @retail 0x7aec0
bool function_07aec0(s_type_99af70 const *address, dword *ipv4_address)
{
	dword &output = *ipv4_address;
	bool result = false;

	short length = address ? address->address_length : 0;
	if (function_7af40(address) && length == k_ipv4_address_length)
	{
		output = (((address->ipv4_address & 0xff0000) | (address->ipv4_address >> 16)) >> 8) | (((address->ipv4_address << 16) | (address->ipv4_address & 0xff00)) << 8);
		if (!(byte)output)
			result = true;
	}
	return result;
}

// @retail 0x7af40
bool function_7af40(s_type_99af70 const *address)
{
	bool result = false;

	if (address)
	{
		switch (address->address_length)
		{
		case NONE:
		case k_ipv4_address_length:
			result = address->ipv4_address != 0;
			break;
		case k_ipv6_address_length:
			for (long i = 0; i < 8; i++)
			{
				if (address->ipv6_address[i])
					return true;
			}
			break;
		}
	}
	return result;
}

// @retail 0x7af80
bool function_7af80(s_type_99af70 const *a, s_type_99af70 const *b, bool compare_ports)
{
	short length = MIN(a->address_length, b->address_length);

	if (a->address_length > 0 && a->address_length == b->address_length && memcmp(a, b, length) == 0 &&
		(!compare_ports || a->port == b->port))
		return true;
	return false;
}
