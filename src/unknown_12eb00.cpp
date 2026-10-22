// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include <string.h>

struct s_fog_cluster_distance
{
	long cluster_index;
	real distance;
	byte unknown08[0xc];
	long previous_index;
};

struct s_12ec00
{
	byte field_0[0xc];
	short field_c;
	short field_e;
};

struct s_12eb00
{
	byte field_0[0xd8];
	long field_d8;
	s_12ec00 *field_dc;
	byte field_e0[0x14];
};

struct s_12ed00
{
	byte field_0[0x340];
	long field_340;
	s_12eb00 *field_344;
	byte field_348[0x3c0 - 0x348];
	long field_3c0;
};

struct s_interpolator_globals;
extern s_interpolator_globals *g_4e6740;
real __stdcall scenario_ramp_evaluate(long arg_0, real arg_1);

/* the entry at index in the table, or NULL when the index is out of range */
static inline s_12eb00 *entry_at_index(s_12ed00 *table, long index)
{
	s_12eb00 *entry = NULL;
	if (index >= 0 && index < table->field_340)
		entry = &table->field_344[index];
	return entry;
}

#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

// @retail 0x12eb00
long function_12eb00(long count, s_fog_cluster_distance *clusters, s_12eb00 *output)
{
	long local_1 = 0;
	long local_0[18][2] = {
		{4, 12}, {0x10, 4}, {0x18, 4}, {0x1c, 4}, {0x20, 4}, {0x24, 12},
		{0x34, 4}, {0x38, 4}, {0x3c, 4}, {0x44, 12}, {0x50, 4}, {0x54, 4},
		{0x58, 4}, {0x88, 24}, {0xa0, 4}, {0xa4, 4}, {0xa8, 4}, {0xac, 4}
	};
	s_12ed00 *local_2 = (s_12ed00 *)g_4e0350;
	if (local_2 && count > 0)
	{
		s_12eb00 *arg_2 = output;
		s_fog_cluster_distance *arg_1 = clusters;
		long arg_0 = count;
		do
		{
			byte *local_3 = *(byte **)((byte *)g_4e0348 + 0xa0) + arg_1->cluster_index * 0xb0;
			signed char local_4 = *(signed char *)(local_3 + 0x6f);
			s_12eb00 *local_5 = NULL;
			if (local_4 != NONE)
				local_5 = entry_at_index(local_2, local_4);
			if (local_5 && local_5->field_d8 > 0)
			{
				real local_6 = 0.0f;
				real local_7 = 1.0f;
				real local_8[2];
				memset(local_8, 0, sizeof(local_8));
				for (long local_9 = 0; local_9 < local_5->field_d8; local_9++)
				{
					s_12ec00 *local_10 = &local_5->field_dc[local_9];
					if (PIN(local_10->field_c, 0, local_2->field_340 - 1) == local_10->field_c &&
						PIN(local_10->field_e, 0, local_2->field_3c0 - 1) == local_10->field_e)
					{
						real *local_11 = (real *)((byte *)g_4e6740 + local_10->field_e * 0x20 + 4);
						real local_12 = local_11 ? scenario_ramp_evaluate(local_10->field_e, *local_11) : 0.0f;
						if (local_12 < 0.0f) local_12 = 0.0f;
						else if (local_12 > 1.0f) local_12 = 1.0f;
						local_6 += local_12;
						local_8[local_9] = local_12;
						local_7 *= 1.0f - local_12;
					}
				}
				if (local_6 > 0.0f)
				{
					s_12eb00 local_13;
					memset(&local_13, 0, sizeof(local_13));
					real local_14 = 1.0f / local_6;
					for (long local_15 = 0; local_15 < local_5->field_d8; local_15++)
					{
						real weight = local_8[local_15];
						if (local_8[local_15] > 0.0f)
						{
							long local_16 = local_5->field_dc[local_15].field_c;
							s_12eb00 *local_18 = entry_at_index((s_12ed00 *)g_4e0350, local_16);
							for (long local_19 = 0; local_19 < 18; local_19++)
							{
								for (long local_20 = 0; local_20 < local_0[local_19][1]; local_20 += 4)
								{
									long local_21 = local_0[local_19][0] + local_20;
									*(real *)((byte *)&local_13 + local_21) += *(real *)((byte *)local_18 + local_21) * weight * local_14;
								}
							}
						}
					}
					arg_1->previous_index = local_1;
					*arg_2 = *local_5;
					for (long local_22 = 0; local_22 < 18; local_22++)
					{
						for (long local_23 = 0; local_23 < local_0[local_22][1]; local_23 += 4)
						{
							long local_24 = local_0[local_22][0] + local_23;
							*(real *)((byte *)arg_2 + local_24) = *(real *)((byte *)&local_13 + local_24) * (1.0f - local_7) + *(real *)((byte *)local_5 + local_24) * local_7;
						}
					}
					local_1++;
					arg_2++;
					local_2 = (s_12ed00 *)g_4e0350;
				}
			}
			arg_1++;
		} while (--arg_0);
	}
	return local_1;
}
