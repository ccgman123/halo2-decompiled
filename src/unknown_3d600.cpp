// @flags /O2 /Gr /arch:SSE
#include "unknown_11c920.h"
#include "globals.h"
#include "target_cache_storage.h"
#include "geometry_cache.h"
#include "unknown_03bcb0.h"


// @retail 0x3d600
void function_3d600(void)
{
	for (long cache = 0; cache < g_4c1bd4; ++cache)
	{
		byte *definition = g_4e3b44[g_4c1bd8[cache].tag & 0xffff].bytes;
		for (long section = 0; section < *(long *)(definition + 0x24); ++section)
		{
			byte *sections = *(byte **)(definition + 0x28);
			function_12de70((s_geometry_block_info *)(sections + section * 0x5c + 0x38), 10);
		}
		for (long material = 0; material < *(long *)(definition + 0x60); ++material)
		{
			byte *entry = *(byte **)(definition + 0x64) + material * 0x20;
			if (*(long *)(entry + 0xc) == NONE) continue;
			byte *shader = *(byte **)(g_4e3b44[*(long *)(entry + 0xc) & 0xffff].bytes + 0x24);
			for (long texture = 0; texture < *(long *)(shader + 4); ++texture)
			{
				long const &bitmap_tag = *(long *)(*(byte **)(shader + 8) + texture * 0xc);
				if (bitmap_tag == NONE) continue;
				byte *bitmap = g_4e3b44[bitmap_tag & 0xffff].bytes;
				for (long index = 0; index < *(long *)(bitmap + 0x44); ++index)
				{
					s_bitmap_predict_view *image = (s_bitmap_predict_view *)(*(byte **)(bitmap + 0x48) + index * 0x74);
					if (image) bitmap_predict_inline(image, 14);
				}
			}
		}
	}
}
