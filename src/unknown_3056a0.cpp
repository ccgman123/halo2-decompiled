// @flags /O2 /GL-
// Havok (third-party physics library): whether a manifold of box contacts
// still needs more: two-bit counters, one per box edge, of the contacts at
// the edges of each box's corners, and the manifold marked done when no edge
// was reached once only.

struct entry_305790
{
	unsigned short kind;
	unsigned short key;
	unsigned short id;
};

class manifold_305790
{
public:
	char unknown0[0x20];
	entry_305790 entries[8];
	long field_50;
	int count;
	bool done;

};



class pair_305790
{
public:
	void function_305790(manifold_305790 *manifold, long first_flags, long second_flags);
};

// one more for edge `edge`'s two-bit counter, which stops at 2
inline void count_edge_3056a0(long *counters, int edge)
{
	long const one = 1 << (edge * 2);
	if ((*counters & (one + one)) == 0)
	{
		*counters += one;
	}
}

// the edges of a corner along the two or three axes other than `axis`
// @retail 0x3056a0
static void __cdecl function_3056a0(int corner, long *counters, int axis)
{
	int const bits = corner >> 4;
	int const x = ~bits & 1;
	int const y = ~((unsigned int)bits >> 1) & 1;
	int const z = ~((unsigned int)bits >> 2) & 1;
	if (axis != 0)
	{
		count_edge_3056a0(counters, y + z * 2);
	}
	if (axis != 1)
	{
		count_edge_3056a0(counters, x + z * 2 + 4);
	}
	if (axis != 2)
	{
		count_edge_3056a0(counters, x + y * 2 + 8);
	}
}

// the edge of a corner along the axis in its low four bits
// @retail 0x305730
static void __cdecl function_305730(int edge_key, long *counters)
{
	int const axis = edge_key & 0xf;
	int const bits = edge_key >> 4;
	int const x = ~bits & 1;
	int const y = ~((unsigned int)bits >> 1) & 1;
	int const z = ~((unsigned int)bits >> 2) & 1;
	if (axis == 0)
	{
		count_edge_3056a0(counters, y + z * 2);
	}
	else if (axis == 1)
	{
		count_edge_3056a0(counters, x + z * 2 + 4);
	}
	else
	{
		count_edge_3056a0(counters, x + y * 2 + 8);
	}
}

// the axis a box's flags name: 0 for bit 4, else 1 for bit 5, else 2
inline int axis_305790(long flags)
{
	int const bits = flags >> 4;
	if (bits & 1)
	{
		return 0;
	}
	return (bits & 2) ? 1 : 2;
}

// score 4 (was 4): the final sete writes al in retail, dl in ours; the negated
// flag on its own, a named flag with and without const, an if and an else, a
// reference to the flag and a pointer to it all keep dl.
// @retail 0x305790
void pair_305790::function_305790(manifold_305790 *manifold, long first_flags, long second_flags)
{
	if (manifold->count < 3)
	{
		return;
	}
	if (manifold->field_50 >= 4)
	{
		manifold->done = true;
		return;
	}
	int const first_axis = axis_305790(first_flags);
	int const second_axis = axis_305790(second_flags);
	long first_counters = 0;
	long second_counters = 0;
	for (int i = 0; i < manifold->count; i++)
	{
		entry_305790 const *entry = &manifold->entries[i];
		if (entry->kind <= 2)
		{
			function_3056a0(entry->key, &second_counters, second_axis);
		}
		else if (entry->kind <= 5)
		{
			function_3056a0(entry->key, &first_counters, first_axis);
		}
		else
		{
			function_305730(entry->kind, &first_counters);
			function_305730(entry->key, &second_counters);
		}
	}
	bool const reached_once = (first_counters & 0x555555) > 0 || (second_counters & 0x555555) > 0;
	manifold->done = !reached_once;
}
