// @flags /O2 /Gr /arch:SSE
/* the render state constants of a projected source: built without /Op, which
   keeps sqrt and tan inline (src/unknown_0494b0.cpp is /Op) */

#include "unknown_11c920.h"
#include "globals.h"
#include "render_source_storage.h"
#include "texture_stage_storage.h"
#include "unknown_0494b0.h"
#include <xtl.h>
#include <math.h>

void function_1cf50(void);
void function_01ddd0(long index, dword width, dword height);
long function_48e70(real value);
extern real g_485a34, g_485a40;
extern byte g_485a47;
extern real g_485a0c[8];
extern real g_4c9798[36];

// @retail 0x48ee0
void function_48ee0(byte const *state, byte const *matrix, byte const *tag)
{
    real aspect = (real)sqrt(*(real const *)(state + 8));
    real distance = *(real const *)(state + 0x64);
    real offset = 0.0f;
    if (*(short const *)(tag + 0x8e) == 1)
        offset = *(real const *)(state + 0x28);
    real inverse = distance != 0.0f ? 1.0f / distance : -3.4028234663852886e38f;
    vector3f const &direction = *(vector3f const *)(matrix + 0x28);
    g_4c9798[0] = direction.i * inverse;
    g_4c9798[1] = direction.j * inverse;
    g_4c9798[2] = direction.k * inverse;
    g_4c9798[3] = 0.0f - inverse * offset;
    g_4c9798[4] = 0.0f - direction.i;
    g_4c9798[5] = 0.0f - direction.j;
    g_4c9798[6] = 0.0f - direction.k;
    g_4c9798[7] = 0.0f;
    long mode = !(g_4c1a80 & 0x400) ? 2 : g_485a40 > 0.0f && *(short const *)(tag + 0x14) == 0 ? 1 : 0;
    real extent = *(real const *)(state + 4);
    if (*(real const *)(state + 0xc) > 0.0f)
        extent += (real)tan(*(real const *)(state + 0xc) * 0.5f) * distance * 2.0f;
    long max_width = (bool)((g_4ba014 >> 4) & 1) && !(bool)((g_4ba014 >> 5) & 1) ? 128 : 512;
    long max_height = (bool)((g_4ba014 >> 4) & 1) && !(bool)((g_4ba014 >> 5) & 1) ? 128 : 512;
    real maximum = (real)max_width;
    real size = ((real)sqrt(extent * extent * *(real const *)(state + 8)) * 0.33333334f + *(real const *)(tag + 0x10) * 0.125f) *
        (maximum - 128.0f) * g_485a34 + 128.0f;
    if (size < 128.0f) size = 128.0f;
    else if (size > maximum) size = maximum;
    long width = (function_48e70(size / aspect) + 63) & ~63;
    long height = (function_48e70(size * aspect) + 63) & ~63;
    if (width * height > max_width * max_height)
    {
        if (width > height) width = (max_width * max_height / height) & ~63;
        else height = (max_width * max_height / width) & ~63;
    }
    g_485a47 = (byte)mode;
    g_51f3c8[1][0] = 0;
    g_51f3c8[1][1] = 0;
    g_51f3c8[1][2] = 0;
    g_51f3c8[1][3] = 0;
    function_1cf50();
    if ((bool)((g_4ba014 >> 4) & 1) && !(bool)((g_4ba014 >> 5) & 1))
    {
        g_4c9798[8] = maximum;
        g_4c9798[9] = (real)max_height;
        g_4c9798[10] = 16777215.0f;
        g_4c9798[11] = 1.0f;
    }
    else
    {
        function_01ddd0(9, width, height);
        g_4c9798[8] = (real)width;
        g_4c9798[9] = (real)height;
        g_4c9798[10] = 16777215.0f;
        g_4c9798[11] = 1.0f;
    }
    g_4c9798[32] = 1.0f;
    g_4c9798[33] = 1.0f;
    g_4c9798[34] = 0.0f;
    g_4c9798[35] = 0.0f;
    real border = mode > 0 ? (real)width * 0.75f / maximum : 0.0f;
    g_4c9798[12] = border + 0.5f;
    g_4c9798[13] = 0.5f - border;
    g_4c9798[15] = 0.0f;
    point3f const &origin = *(point3f const *)matrix;
    plane3f *plane = (plane3f *)&g_4c9798[16];
    vector3f normal;
    point3f point;
    point.x = direction.i * 0.01f + origin.x;
    point.y = direction.j * 0.01f + origin.y;
    point.z = direction.k * 0.01f + origin.z;
    normal.i = 0.0f - direction.i;
    normal.j = 0.0f - direction.j;
    normal.k = 0.0f - direction.k;
    plane->n = normal;
    plane->d = dot3f(&plane->n, (vector3f const *)&point);
    plane->i *= 100.0f;
    plane->j *= 100.0f;
    plane->k *= 100.0f;
    plane->d *= -100.0f;
    real lower = *(real const *)(tag + 0x2c);
    real upper = *(real const *)(tag + 0x30);
    real scale = upper > lower ? 1.0f / (upper - lower) : 1000.0f;
    plane = (plane3f *)&g_4c9798[20];
    point.x = direction.i * lower + origin.x;
    point.y = direction.j * lower + origin.y;
    point.z = direction.k * lower + origin.z;
    plane->n = direction;
    plane->d = dot3f(&plane->n, (vector3f const *)&point);
    plane->i *= scale;
    plane->j *= scale;
    plane->k *= scale;
    plane->d = 0.0f - plane->d * scale;
    ((vector4f *)&g_4c9798[24])[0] = ((vector4f const *)g_485a0c)[0];
    ((vector4f *)&g_4c9798[24])[1] = ((vector4f const *)g_485a0c)[1];
    D3DDevice_SetVertexShaderConstantFast(-56, g_4c9798, 9);
}
