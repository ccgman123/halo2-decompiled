// @flags /O2 /Gr /arch:SSE
#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_3dfe0.h"

extern s_record_pool *g_509434;
extern dword g_4ba034;
extern bool g_467464;
long function_d1850(long object_index, bool value, s_effect_color_query *query);
void function_d4080(long type, s_effect_color_query const *query, s_lighting_record *record, long flag);
void function_ba1d0(long object_index, vector3f *linear_velocity, vector3f *angular_velocity);
real magnitude3d(vector3f const *vector);
real distance3d(point3f const *first, point3f const *second);
struct s_object;
s_object *function_badc0(long object_index, dword mask);
void function_3e5a0(vector3f *current, vector3f const *target, real step);
void function_3e4e0(dword *current, dword const *target, real step);

// @retail 0x3dfe0
void function_3dfe0(long cache_index, long object_index, real priority)
{
    s_3dfe0_cache *cache = (s_3dfe0_cache *)(g_509434->data + (cache_index & 0xffff) * 0x100);
    long elapsed = g_4ba034 - cache->sample_time;
    if (!elapsed) return;
    long update_elapsed = g_4ba034 - cache->update_time;
    byte *object = ((s_3dd10_object_header *)g_4e0300->data)[object_index & 0xffff].object;
    bool force = !cache->initialized || !cache->valid;
    bool update = false;
    if (elapsed < 0) elapsed = 1;
    if (force) update = true;
    else if (object[0xc1] & 1) return;
    bool sample = true;
    if (cache->sample_time != (dword)NONE)
    {
        if (priority > 400.0f) sample = elapsed > 3;
        else if (priority > 200.0f) sample = elapsed > 6;
        else if (priority > 100.0f) sample = elapsed > 9;
        else if (priority > 50.0f) sample = elapsed > 15;
        else if (priority > 10.0f) sample = elapsed > 45;
        else sample = elapsed > 60;
    }
    if (cache->update_time == (dword)NONE || update_elapsed > 1) update = true;
    if (sample)
    {
        bool stationary = false;
        if (cache->valid)
        {
            vector3f delta;
            delta.i = *(real *)(object + 0x30) - cache->position.x;
            delta.j = *(real *)(object + 0x34) - cache->position.y;
            delta.k = *(real *)(object + 0x38) - cache->position.z;
            stationary = delta.k * delta.k + delta.i * delta.i + delta.j * delta.j < 0.002500000176951289f;
        }
        if (!stationary)
        {
            cache->sample_time = g_4ba034;
            if ((byte)function_d1850(object_index, force, &cache->target))
            {
                cache->valid = true;
                cache->position = *(point3f *)(object + 0x30);
            }
        }
    }
    if (!cache->valid) return;
    s_effect_color_query *current = &cache->current;
    if (cache->initialized && g_467464 && !update)
    {
        s_effect_color_query const *target = &cache->target;
        vector3f velocity;
        function_ba1d0(object_index, &velocity, 0);
        real blend = (magnitude3d(&velocity) - 0.15000000596046448f) * 0.03350083902478218f;
        if (blend < 0.0f) blend = 0.0f;
        else if (blend > 1.0f) blend = 1.0f;
        if (blend > 0.0f ||
            (distance3d(&cache->position, (point3f *)(object + 0x30)) > 0.05000000074505806f && blend > 0.0f) ||
            function_badc0(object_index, 0x80))
        {
            function_3e5a0((vector3f *)&(*current), (vector3f const *)target, blend * 0.009999999776482582f);
            function_3e4e0(&(*current).color_b, &target->color_b, blend * 0.5f);
            function_3e4e0(&(*current).color_a, &target->color_a, blend * 0.5f);
        }
    }
    else (*current) = cache->target;
    long type = NONE;
    long special = false;
    if (cache->parent != NONE)
    {
        byte *parent = ((s_3dd10_object_header *)g_4e0300->data)[cache->parent & 0xffff].object;
        byte *definition = g_4e3b44[*(long *)parent & 0xffff].bytes;
        type = *(short *)definition;
        special = (bool)(((*(dword *)(parent + 4)) >> 13) & 1) || (bool)((*(word *)(definition + 2) >> 1) & 1);
    }
    function_d4080(type, &(*current), &cache->lighting, special);
    cache->initialized = true;
}
