// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1317A0.CPP: weighted accumulation, global table lookup, colour conversions */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "globals.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

struct s_weighted_accumulator
{
	real a[3];
	real b[3];
	real c[4];
	real weight;
	real remainder;
};

// @retail 0x1317a0
void function_1317a0(const real *a, const real *b, real c0, real c1, real c2, real c3, real t, s_weighted_accumulator *acc)
{
	if (t < 0.0f)
	{
		t = 0.0f;
	}
	else if (t > 1.0f)
	{
		t = 1.0f;
	}
	acc->a[0] += a[0] * t;
	acc->a[1] += a[1] * t;
	acc->a[2] += a[2] * t;
	acc->b[0] += b[0] * t;
	acc->b[1] += b[1] * t;
	acc->b[2] += b[2] * t;
	acc->c[0] += c0 * t;
	acc->c[1] += c1 * t;
	acc->c[2] += c2 * t;
	acc->c[3] += c3 * t;
	acc->weight += t;
	acc->remainder *= 1.0f - t;
}

struct s_color_table_globals
{
	byte unknown00[0x348];
	long count;
	byte *table;
};

// @retail 0x1318a0
byte *function_1318a0(long index)
{
	s_color_table_globals *globals = (s_color_table_globals *)g_4e0350;
	byte *result = 0;

	if (globals && index >= 0 && index < globals->count)
	{
		result = globals->table + index * 16;
	}
	return result;
}

// @retail 0x1318d0
hsv3f *function_1318d0(const color3f *rgb, hsv3f *hsv)
{
	real max_value = MAX(rgb->red, MAX(rgb->green, rgb->blue));
	real min_value = MIN(rgb->red, MIN(rgb->green, rgb->blue));
	real delta = max_value - min_value;
	real saturation;

	hsv->value = max_value;
	if (max_value == 0.0f)
	{
		saturation = 0.0f;
	}
	else
	{
		saturation = delta / max_value;
	}
	hsv->saturation = saturation;
	if (saturation == 0.0f)
	{
		hsv->hue = 0.0f;
		return hsv;
	}

	if (rgb->red == max_value)
	{
		hsv->hue = (rgb->green - rgb->blue) / delta;
	}
	else if (rgb->green == max_value)
	{
		hsv->hue = (rgb->blue - rgb->red) / delta + 2.0f;
	}
	else
	{
		hsv->hue = (rgb->red - rgb->green) / delta + 4.0f;
	}
	hsv->hue *= 1.0f / 6.0f;
	if (hsv->hue < 0.0f)
	{
		hsv->hue += 1.0f;
	}
	return hsv;
}

// @retail 0x131a00
color3f *function_131a00(const hsv3f *hsv, color3f *rgb)
{
	real h = hsv->hue * 6.0f;

	if (hsv->saturation == 0.0f)
	{
		rgb->red = rgb->green = rgb->blue = hsv->value;
	}
	else
	{
		long sector = (long)h;
		real f;
		if ((real)sector > h)
		{
			sector--;
		}
		f = h - (real)sector;
		real p = (1.0f - hsv->saturation) * hsv->value;
		real q = (1.0f - hsv->saturation * f) * hsv->value;
		real t = (1.0f - (1.0f - f) * hsv->saturation) * hsv->value;

		switch (sector)
		{
		case 0: rgb->red = hsv->value; rgb->green = t; rgb->blue = p; break;
		case 1: rgb->red = q; rgb->green = hsv->value; rgb->blue = p; break;
		case 2: rgb->red = p; rgb->green = hsv->value; rgb->blue = t; break;
		case 3: rgb->red = p; rgb->green = q; rgb->blue = hsv->value; break;
		case 4: rgb->red = t; rgb->green = p; rgb->blue = hsv->value; break;
		case 5: rgb->red = hsv->value; rgb->green = p; rgb->blue = q; break;
		default: __assume(0);
		}
	}
	return rgb;
}

// @retail 0x131b20
color4f *unpack_color4f(dword pixel, color4f *color)
{
	color->alpha = (real)(pixel >> 24) / 255.0f;
	color->red = (real)((pixel >> 16) & 0xff) / 255.0f;
	color->green = (real)((pixel >> 8) & 0xff) / 255.0f;
	color->blue = (real)(pixel & 0xff) / 255.0f;
	return color;
}

// @retail 0x131bb0
color3f *unpack_color3f(dword pixel, color3f *color)
{
	color->red = (real)((pixel >> 16) & 0xff) / 255.0f;
	color->green = (real)((pixel >> 8) & 0xff) / 255.0f;
	color->blue = (real)(pixel & 0xff) / 255.0f;
	return color;
}
