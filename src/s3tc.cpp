// @flags /O2 /Gr
/* S3TC.CPP: decoding one pixel of an S3TC (DXT1, DXT3 and DXT5) block
   (function_223e70, 0x223e70, is in src/unknown_223b60.cpp) */

#include "unknown_11c920.h"
#include <string.h>
#include "unknown_223b60.h"

void function_223e70(const word *rgb, S3TC_COLOR *out);

/* a DXT1 block: two 565 colors and 2 bit indices */
struct S3TCBlockRGB
{
	word color0;
	word color1;
	long indices;
};

/* a DXT3 block: 4 bit alphas, then the colors */
struct S3TCBlockRGBA_explicit
{
	word alpha[4];
	S3TCBlockRGB rgb;
};

/* a DXT5 block: two alphas and 3 bit alpha indices, then the colors */
struct S3TCBlockRGBA_interpolated
{
	byte alpha0;
	byte alpha1;
	byte alpha_indices[6];
	S3TCBlockRGB rgb;
};

// @retail 0x223ed0
void function_223ed0(S3TCBlockRGB const *block, S3TC_COLOR *out, short x, short y)
{
	S3TC_COLOR colors[4];
	long i;

	if (!block)
	{
		memset(out, 0, 16 * sizeof(S3TC_COLOR));
		return;
	}

	function_223e70(&block->color0, &colors[0]);
	function_223e70(&block->color1, &colors[1]);
	colors[0].a = colors[1].a = colors[2].a = 0xff;
	if (block->color0 > block->color1)
	{
		for (i = 0; i < 3; i++)
		{
			word color0 = ((byte *)&colors[0])[i];
			word color1 = ((byte *)&colors[1])[i];

			((byte *)&colors[2])[i] = (byte)((2 * color0 + color1 + 1) / 3);
			((byte *)&colors[3])[i] = (byte)((color0 + 2 * color1 + 1) / 3);
		}
		colors[3].a = 0xff;
	}
	else
	{
		i = 0;
		do
		{
			((byte *)&colors[2])[i] = (byte)((((byte *)&colors[0])[i] + ((byte *)&colors[1])[i]) / 2);
			((byte *)&colors[3])[i] = 0;
			i++;
		}
		while (i < 3);
		colors[3].a = 0;
	}
	*out = colors[(block->indices >> (2 * (x + 4 * y))) & 3];
}

// @retail 0x224040
void DecodeBlockRGBA_explicit__single_pixel(S3TCBlockRGBA_explicit const *block, S3TC_COLOR *out, short x, short y)
{
	word alpha;

	function_223ed0(&block->rgb, out, x, y);
	alpha = block->alpha[y];
	alpha = (alpha >> (x * 4)) & 0xf;
	out->a = (byte)((alpha << 4) | alpha);
}

// @retail 0x224080
// score 19 (was 19): the interpolated colour is built in EDX where retail builds it in EAX, so the
// shift and the lookup of the palette entry use the other register.
void DecodeBlockRGBA_interpolated__single_pixel(S3TCBlockRGBA_interpolated const *block, S3TC_COLOR *out, short x, short y)
{
	word alphas[8];
	long bits;
	short index;

	function_223ed0(&block->rgb, out, x, y);
	alphas[0] = block->alpha0;
	alphas[1] = block->alpha1;
	if (alphas[0] > alphas[1])
	{
		alphas[2] = (word)((6 * alphas[0] + 1 * alphas[1]) / 7);
		alphas[3] = (word)((5 * alphas[0] + 2 * alphas[1]) / 7);
		alphas[4] = (word)((4 * alphas[0] + 3 * alphas[1]) / 7);
		alphas[5] = (word)((3 * alphas[0] + 4 * alphas[1]) / 7);
		alphas[6] = (word)((2 * alphas[0] + 5 * alphas[1]) / 7);
		alphas[7] = (word)((1 * alphas[0] + 6 * alphas[1]) / 7);
	}
	else
	{
		alphas[2] = (word)((4 * alphas[0] + 1 * alphas[1]) / 5);
		alphas[3] = (word)((3 * alphas[0] + 2 * alphas[1]) / 5);
		alphas[4] = (word)((2 * alphas[0] + 3 * alphas[1]) / 5);
		alphas[5] = (word)((1 * alphas[0] + 4 * alphas[1]) / 5);
		alphas[6] = 0;
		alphas[7] = 0xff;
	}
	if (y < 2)
	{
		bits = (block->alpha_indices[2] << 16) | (block->alpha_indices[1] << 8) | block->alpha_indices[0];
		index = x + 4 * y;
	}
	else
	{
		bits = (block->alpha_indices[5] << 16) | (block->alpha_indices[4] << 8) | block->alpha_indices[3];
		index = x + 4 * y - 8;
	}
	out->a = (byte)alphas[(bits >> (index * 3)) & 7];
}
