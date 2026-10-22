// @flags /O1 /Oi /Gr /arch:SSE
#include "unknown_11c920.h"
#include "unknown_24ecae.h"
#include "globals.h"
#include "view_context_storage.h"

// @retail 0x24f0fc
void function_24f0fc(s_marker_list const *list, s_24ecae_placement const *placement, real offset, long count)
{
	if (!g_510c94 || list->l4 < 0 || list->l4 >= ((s_24ecae_globals *)g_510c94)->label_count || !list->b1)
		return;
	s_24f0fc_kind const *kind = &((s_24ecae_globals *)g_510c94)->labels[list->l4];
	if (kind->bitmap_index == NONE || kind->shader_index == NONE || kind->border_bitmap == NONE ||
		placement->hidden || count == 0)
		return;
	real center_x = placement->position.x;
	real center_y = placement->position.y;
	center_y -= placement->scale * 16.0f + offset + 1.0f;
	real left_width = count <= 1 ? 16.0f : 32.0f;
	real right_width = count <= 1 ? 16.0f : 32.0f;
	real left = center_x - (placement->scale * left_width + 1.0f);
	real right = placement->scale * right_width + 1.0f + center_x;
	real top = center_y - (placement->scale * 16.0f + 1.0f);
	real bottom = placement->scale * 16.0f + 1.0f + center_y;
	g_4e6950.field_0 = *(color3f const *)&list->color24;
	g_4e6950.field_c = *(point3f const *)&list->color30;
	g_4e6950.value18 = 0.0f;
	g_4e6950.value24 = list->r40;
	if (list->b0)
		g_4e6950.value1c = g_4e69c0[g_4b9ed8] * list->r3c;
	else if (placement->hidden)
		g_4e6950.value1c = 0.0f;
	else
		g_4e6950.value1c = (1.0f - placement->fade) * g_4e69c0[g_4b9ed8] * list->r3c;
	box2f bounds;
	bounds.x0 = 0.0f;
	bounds.x1 = 1.0f;
	bounds.y0 = 0.0f;
	bounds.y1 = 0.5f;
	point2f points[4];
	for (short corner = 0; corner < 4; ++corner)
	{
		bool upper = corner < 2;
		bool right_side = corner == 1 || corner == 2;
		real x = right_side ? right : left;
		real y = upper ? top : bottom;
		points[0].x = x + (right_side ? 2.0f : -2.0f);
		points[0].y = y + (upper ? -2.0f : 2.0f);
		points[1].x = x;
		points[1].y = y + (upper ? -2.0f : 2.0f);
		points[2].x = x;
		points[2].y = y;
		points[3].x = x + (right_side ? 2.0f : -2.0f);
		points[3].y = y;
		function_22a664((s_widget_quad_2b11 const *)points, (s_float_rect const *)&bounds,
			kind->border_bitmap, 0, kind->shader_index);
	}
	bounds.x0 = 0.0f;
	bounds.x1 = 1.0f;
	bounds.y0 = 0.5f;
	bounds.y1 = 1.0f;
	points[0].x = left;
	points[0].y = top - 2.0f;
	points[1].x = right;
	points[1].y = top - 2.0f;
	points[2].x = right;
	points[2].y = top;
	points[3].x = left;
	points[3].y = top;
	function_22a664((s_widget_quad_2b11 const *)points, (s_float_rect const *)&bounds,
		kind->border_bitmap, 0, kind->shader_index);
	points[0].x = right;
	points[0].y = top;
	points[1].x = right + 2.0f;
	points[1].y = top;
	points[2].x = right + 2.0f;
	points[2].y = bottom;
	points[3].x = right;
	points[3].y = bottom;
	function_22a664((s_widget_quad_2b11 const *)points, (s_float_rect const *)&bounds,
		kind->border_bitmap, 0, kind->shader_index);
	points[0].x = left;
	points[0].y = top;
	points[1].x = left - 2.0f;
	points[1].y = top;
	points[2].x = left - 2.0f;
	points[2].y = bottom;
	points[3].x = left;
	points[3].y = bottom;
	function_22a664((s_widget_quad_2b11 const *)points, (s_float_rect const *)&bounds,
		kind->border_bitmap, 0, kind->shader_index);
	real split = center_x - placement->scale * 16.0f + 1.0f;
	real edge = bottom + 2.0f;
	points[0].x = left;
	points[0].y = edge;
	points[1].x = split;
	points[1].y = edge;
	points[2].x = split;
	points[2].y = bottom;
	points[3].x = left;
	points[3].y = bottom;
	function_22a664((s_widget_quad_2b11 const *)points, (s_float_rect const *)&bounds,
		kind->border_bitmap, 0, kind->shader_index);
	split = placement->scale * 16.0f + center_x;
	points[0].x = right;
	points[0].y = edge;
	points[1].x = split;
	points[1].y = edge;
	points[2].x = split;
	points[2].y = bottom;
	points[3].x = right;
	points[3].y = bottom;
	function_22a664((s_widget_quad_2b11 const *)points, (s_float_rect const *)&bounds,
		kind->border_bitmap, 0, kind->shader_index);
}
