// @flags /O1 /Oi /Gr /arch:SSE
#include "unknown_11c920.h"
#include "unknown_24ecae.h"
#include "globals.h"
#include "view_context_storage.h"

// @retail 0x24ecae
long __stdcall function_24ecae(s_marker_list const *list, s_24ecae_placement const *placement, real offset)
{
	long count = 0;
	dword mask = 0;
	s_24ecae_globals *globals = (s_24ecae_globals *)g_510c94;
	long index = 0;
	long item_count = list->count;
	s_list_item const *item = list->items;
	for (; index < item_count; ++index, ++item)
	{
		long kind_index = item->kind;
		if (globals && kind_index >= 0 && kind_index < globals->image_count)
		{
			s_24ecae_kind const *kind = &globals->images[kind_index];
			if (kind->bitmap_index != NONE && kind->shader_index != NONE &&
				(!placement->hidden || kind->hidden_sequence != NONE) &&
				(!placement->alternate || kind->alternate_sequence != NONE) &&
				kind->sequence_index != NONE)
			{
				mask |= 1UL << (index & 31);
				++count;
			}
		}
	}
	if (count > 0 && count <= 2)
	{
		point2f center = placement->position;
		long drawn = 0;
		if (placement->hidden)
			offset = (real)(sqrt(512.0) * placement->scale + offset);
		else
			offset = placement->scale * 16.0f + offset + 1.0f;
		center.x += -(placement->forward_i * offset);
		center.y += -(placement->forward_j * offset);
		item = list->items;
		for (index = 0; index < list->count; ++index, ++item)
		{
			if (mask & (1UL << (index & 31)))
			{
				s_24ecae_kind const *kind = &globals->images[item->kind];
				s_24ecae_bitmap_group const *group = (s_24ecae_bitmap_group *)g_4e3b44[kind->bitmap_index & 0xffff].bytes;
				short sequence_index;
				if (placement->hidden)
					sequence_index = kind->hidden_sequence;
				else if (placement->alternate)
					sequence_index = kind->alternate_sequence;
				else
					sequence_index = kind->sequence_index;
				if (sequence_index < 0)
					continue;
				long bitmap_index;
				box2f bounds;
				point2f pivot;
				if (sequence_index < group->sequence_count)
				{
					s_24ecae_sequence const *sequence = &group->sequences[sequence_index];
					bitmap_index = sequence->first_bitmap;
					s_24ecae_bitmap const *bitmap = &group->bitmaps[bitmap_index];
					if (sequence->sprite_count > 0)
					{
						bounds = sequence->sprites->bounds;
						pivot.x = (bounds.x1 + bounds.x0) * 0.5f;
						pivot.y = (bounds.y1 + bounds.y0) * 0.5f;
					}
					else
					{
						bounds.x0 = bounds.y0 = 0.0f;
						bounds.x1 = bounds.y1 = 1.0f;
						pivot.x = (real)bitmap->field_10 / bitmap->width;
						pivot.y = (real)bitmap->field_12 / bitmap->height;
					}
				}
				else if (sequence_index == 0 && group->sequence_count == 0 && group->bitmap_count > 0)
				{
					bitmap_index = 0;
					s_24ecae_bitmap const *bitmap = group->bitmaps;
					bounds.x0 = bounds.y0 = 0.0f;
					bounds.x1 = bounds.y1 = 1.0f;
					pivot.x = (real)bitmap->field_10 / bitmap->width;
					pivot.y = (real)bitmap->field_12 / bitmap->height;
				}
				else
					continue;
				if (bitmap_index == NONE)
					continue;
				s_24ecae_bitmap const *bitmap = &group->bitmaps[bitmap_index];
				real alpha = 1.0f;
				if (placement->fading && kind->hidden_sequence == NONE)
					alpha = 1.0f - placement->fade;
				point2f origin;
				if (!placement->hidden && count != 1)
				{
					real side = drawn == 0 ? -0.5f : 0.5f;
					origin.x = placement->scale * side * 32.0f + center.x;
					origin.y = center.y;
				}
				else
					origin = center;
				real height = (real)bitmap->height * placement->scale;
				real width = (real)bitmap->width * placement->scale;
				point2f points[4];
				for (long vertex = 0; vertex < 4; ++vertex)
				{
					bool top = vertex == 0 || vertex == 1;
					bool right = vertex == 1 || vertex == 2;
					points[vertex] = origin;
					real y = top ? bounds.y0 : bounds.y1;
					real x = right ? bounds.x1 : bounds.x0;
					points[vertex].x += width * (x - pivot.x);
					points[vertex].y += height * (y - pivot.y);
				}
				g_4e6950.field_0 = *(color3f const *)&item->a;
				g_4e6950.field_c = *(point3f const *)&item->b;
				g_4e6950.value18 = 0.0f;
				g_4e6950.value1c = g_4e69c0[g_4b9ed8] * list->r3c * alpha;
				g_4e6950.value20 = item->r;
				g_4e6950.value3c = 1.0f;
				function_1392c5(item->index);
				function_22a664((s_widget_quad_2b11 const *)points, (s_float_rect const *)&bounds,
					kind->bitmap_index, bitmap_index, kind->shader_index);
				++drawn;
				globals = (s_24ecae_globals *)g_510c94;
			}
		}
	}
	return count;
}
