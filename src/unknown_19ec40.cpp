// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_19EC40.CPP: finds entries of the 4e0350 globals' second table by
   position and by three optional 16-bit keys */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
#include "unknown_19ec40.h"
#include <math.h>

real distance_sq3f(point3f const *a, point3f const *b);

// @retail 0x19ec40
long function_19ec40(
	point3f const *point,
	real height,
	short key_a,
	short key_b,
	short key_c,
	long maximum_count,
	long *results,
	real radius)
{
	point3f const *const *point_reference = &point;
	s_palette_source_globals *globals = g_4e0350;
	real radius_squared = radius * radius;
	real best = 0.0f;
	long count = 0;
	short i = 0;

	if (globals->marker_count > 0)
	{
		do
		{
			short key = key_a;
			s_marker_entry *entry = &globals->marker_entries[i];

			if (key == -1 || key == entry->key_a)
			{
				key = key_b;
				if (key == -1 || key == entry->key_b)
				{
					key = key_c;
					if (key == -1 || key == entry->key_c)
					{
						real distance_squared;

						if (*point_reference)
						{
							point3f position = g_4e0350->marker_entries[i].position;
							distance_squared = distance_sq3f(&position, point);
							if (radius >= 0.0f && distance_squared > radius_squared)
								goto next;
							if (height > 0.0f && fabs(entry->position.z - point->z) > height)
								goto next;
						}
						else
							distance_squared = 0.0f;

						if (count < maximum_count)
							results[count++] = i;
						else if (count == 1 && best > distance_squared)
						{
							results[0] = i;
							best = distance_squared;
						}
					}
				}
			}
next:
			i++;
		}
		while (i < globals->marker_count);
	}

	return count;
}
