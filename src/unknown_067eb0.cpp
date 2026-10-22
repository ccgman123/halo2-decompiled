// @flags /O2 /Gr
/* UNKNOWN_067EB0.CPP: lifecycle callbacks (entry 23) */

#include "unknown_11c920.h"
#include "globals.h"

long g_4cf784;

// @retail 0x67eb0
void function_067eb0(void)
{
	if (g_4cf770)
	{
		g_4cf77c = 0;
		g_4cf780 = 0;
		g_4cf784 = 0;
		g_4cf770 = false;
	}
}

#include <string.h>
#include "unknown_067e10.h"

struct s_simulation_definition_registry
{
	long entity_count;
	void *entities[32];
	long event_count;
	void *events[32];
};

c_class_6a600 g_544b20;
s_simulation_world_owner g_545d40;
s_simulation_definition_registry g_546974;

void __stdcall function_82240(s_simulation_definition_registry *registry,
	long *entity_count, long *event_count);

// score 42 (was 42): retail keeps the two -1 stores to the entity and event counts and
// reloads the registry pointer after the call, because it cannot see what the callee does
// to the global; ours drops the -1 stores as dead and reuses the address it just stored,
// since the callee's body is visible to LTCG in the reduced image. The callee lives in
// another source file, so this difference is out of reach here.
// @retail 0x67e30
void function_67e30(void)
{
	g_4cf784 = (long)&g_546974;
	g_4cf77c = (s_simulation_world *)&g_544b20;
	g_4cf780 = (s_47f048_object *)&g_545d40;
	g_546974.entity_count = NONE;
	memset(g_546974.entities, 0, sizeof(g_546974.entities));
	g_546974.event_count = NONE;
	memset(g_546974.events, 0, sizeof(g_546974.events));
	long entity_count = NONE;
	long event_count = NONE;
	function_82240((s_simulation_definition_registry *)g_4cf784, &entity_count, &event_count);
	s_simulation_definition_registry *registry = (s_simulation_definition_registry *)g_4cf784;
	registry->entity_count = entity_count;
	registry->event_count = event_count;
	g_4cf770 = true;
}
