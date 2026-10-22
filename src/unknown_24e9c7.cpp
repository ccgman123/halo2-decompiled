// @flags /O1 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include "view_context_storage.h"

struct s_float_rect;
struct s_widget_quad_2b11;
void function_22a664(s_widget_quad_2b11 const *arg_0, s_float_rect const *arg_1,
	long arg_2, long arg_3, long arg_4);
extern real g_4e69c0[4];


struct s_4e6950
{
	color3f field_0;
	point3f field_c;
	real value18;
	real value1c;
	real value20;
	real value24;
	byte unknown28[0x70 - 0x28];
	real user_values[4];
};
extern s_4e6950 g_4e6950;

/* the hud globals' marker kinds (+0x408) */
struct s_24e9c7_kind
{
	long unknown00;
	long bitmap_index;
	long unknown08;
	long shader_index;
	short sequence_index;
	byte unknown12[0x24 - 0x12];
};

struct s_24e9c7_kinds_view
{
	byte unknown00[0x408];
	long count;
	s_24e9c7_kind *entries;
};

struct s_24e9c7_sprite
{
	byte unknown00[8];
	real bounds[4];
};

struct s_24e9c7_sequence
{
	byte unknown00[0x20];
	short first_bitmap_index;
	byte unknown22[0x34 - 0x22];
	long sprite_count;
	s_24e9c7_sprite *sprites;
};

struct s_24e9c7_bitmap
{
	byte unknown00[4];
	short width;
	short height;
	byte unknown08[0x74 - 8];
};

struct s_24e9c7_bitmap_group
{
	byte unknown00[0x3c];
	long sequence_count;
	s_24e9c7_sequence *sequences;
	long bitmap_count;
	s_24e9c7_bitmap *bitmaps;
};

struct s_24e9c7_marker
{
	bool use_full_alpha;
	byte unknown01[3];
	long kind;
	byte unknown08[0x24 - 8];
	color3f color;
	point3f point;
	real alpha;
	real value40;
};

struct s_24e9c7_placement
{
	real forward_i;
	real forward_j;
	real x;
	real y;
	real size;
	bool hidden;
	byte unknown15[3];
	real fade;
	real unknown1c;
	real scale;
};

struct s_24e9c7_point
{
	real x, y;
};

struct s_24e9c7_points
{
	s_24e9c7_point n[4];
};

struct s_24e9c7_bounds
{
	real x0, x1, y0, y1;
};

/* Draws one marker's sprite at its placement; returns the sprite's height. */
// @retail 0x24e9c7
real function_24e9c7(s_24e9c7_marker const *marker, s_24e9c7_placement const *placement)
{
	real result = 0.0f;
	s_24e9c7_kinds_view *globals = (s_24e9c7_kinds_view *)g_510c94;
	if (globals && marker->kind >= 0 && marker->kind < globals->count)
	{
		s_24e9c7_kind *kind = &globals->entries[marker->kind];
		long bitmap_index = kind->bitmap_index;
		if (bitmap_index != NONE && kind->shader_index != NONE)
		{
			s_24e9c7_bitmap_group *group = (s_24e9c7_bitmap_group *)g_4e3b44[bitmap_index & 0xffff].bytes;
			short sequence_index = kind->sequence_index;
			s_24e9c7_bounds bounds;
			long first_bitmap;
			bounds.x0 = 0.0f;
			bounds.x1 = 1.0f;
			bounds.y0 = 0.0f;
			bounds.y1 = 1.0f;
			if (sequence_index < 0)
				return 0.0f;
			if (sequence_index < group->sequence_count)
			{
				s_24e9c7_sequence *sequence = &group->sequences[sequence_index];
				first_bitmap = sequence->first_bitmap_index;
				if (sequence->sprite_count)
					bounds = *(s_24e9c7_bounds *)sequence->sprites->bounds;
			}
			else
			{
				if (sequence_index != 0 || group->sequence_count != 0 || group->bitmap_count <= 0)
					return 0.0f;
				first_bitmap = 0;
			}
			if (first_bitmap != NONE)
			{
				s_24e9c7_bitmap *bitmap = &group->bitmaps[first_bitmap];
				real size = placement->size + 1.0f;
				real across_i = size * -placement->forward_j;
				real across_j = size * placement->forward_i;
				real scale = placement->scale;
				long height = (long)((bounds.y1 - bounds.y0) * bitmap->height * scale);
				result = (real)height;
				long half_width = (long)((bounds.x1 - bounds.x0) * bitmap->width * scale) / 2;
				s_24e9c7_point up;
				up.x = -(result * placement->forward_i);
				up.y = -(placement->forward_j * result);
				s_24e9c7_point side;
				side.x = (real)half_width * across_i;
				side.y = (real)half_width * across_j;
				s_24e9c7_point back;
				back.x = -side.x;
				back.y = -side.y;
				s_24e9c7_points points;
				points.n[0] = *(s_24e9c7_point const *)&placement->x;
				points.n[0].x += up.x;
				points.n[0].y += up.y;
				points.n[0].x += back.x;
				points.n[0].y += back.y;
				points.n[1] = *(s_24e9c7_point const *)&placement->x;
				points.n[1].x += up.x;
				points.n[1].y += up.y;
				points.n[1].x += side.x;
				points.n[1].y += side.y;
				points.n[2] = *(s_24e9c7_point const *)&placement->x;
				points.n[2].x += side.x;
				points.n[2].y += side.y;
				points.n[3] = *(s_24e9c7_point const *)&placement->x;
				points.n[3].x += back.x;
				points.n[3].y += back.y;
				g_4e6950.field_0 = marker->color;
				g_4e6950.field_c = marker->point;
				g_4e6950.value18 = 0.0f;
				g_4e6950.value24 = marker->value40;
				real alpha;
				if (marker->use_full_alpha)
					alpha = g_4e69c0[g_4b9ed8] * marker->alpha;
				else if (!placement->hidden)
					alpha = (1.0f - placement->fade) * g_4e69c0[g_4b9ed8] * marker->alpha;
				else
					alpha = 0.0f;
				g_4e6950.value1c = alpha;
				function_22a664((s_widget_quad_2b11 const *)&points, (s_float_rect const *)&bounds,
					bitmap_index, first_bitmap, kind->shader_index);
			}
		}
	}
	return result;
}
