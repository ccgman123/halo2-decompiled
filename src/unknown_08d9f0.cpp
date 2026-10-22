// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_075870.h"
#include "unknown_059ad0.h"
#include "unknown_0662e0.h"
#include "network_message_types.h"
#include "online_attributes.h"
#include <xtl.h>
#include <string.h>

class c_class_93590;
class c_class_938e0;
struct s_network_message_gateway;
struct s_network_session_list
{
	c_class_58d20 *sessions[3];
};
struct s_system_link_globals
{
	bool active;
	byte unknown01[7];
	s_session_id session_id;
};
#include "system_link_globals.h"
struct s_online_search_globals
{
	bool active;
	byte unknown01[3];
	s_range_input input;
	long task_index;
	long qos_handle;
	long capacity;
	void *entries;
	long start_time;
};
struct s_search_value_globals;
struct s_0b35e0_entry;
struct s_08d9f0_link
{
	byte unknown00[0x1c];
	void *consumer;
};
struct s_08d9f0_gateway
{
	void *vtable;
	bool initialized;
	byte unknown05[3];
	s_08d9f0_link *link;
	c_type_659ceb *messages;
	c_class_938e0 *handler;
	bool unknown14;
};
struct s_08d9f0_handler
{
	bool initialized;
	byte unknown01[3];
	c_class_93590 *link;
	c_type_659ceb *messages;
	s_network_message_gateway *gateway;
	s_network_observer *observer;
	s_network_session_list *sessions;
};

extern bool g_4d8ba0;
extern c_class_93590 *g_510560;
extern c_type_659ceb *g_510564;
extern s_network_message_gateway *g_510568;
extern c_class_938e0 *g_51056c;
extern s_network_observer *g_510570;
extern c_class_58d20 *g_510574;
s_network_session_list *g_510578;
extern s_network_session_list *g_510550;
extern s_link g_528000;
extern byte g_528b28[4];
extern byte g_529188[4];
extern byte g_5291a0[4];
c_type_659ceb g_528588;
byte g_52e0f8[3 * 0x78b8];
s_network_session_list g_52e0e8;
extern void *g_4d8eb0;
extern s_online_search_globals g_4d8ecc;
extern long g_4d8f04;
extern bool g_4d8f08;
extern long g_4d8f0c;
extern c_data_allocator *g_4d8f10;
extern long g_4d8f14;
extern s_0b35e0_entry *g_4d8f18;
extern s_search_value_globals g_509450;
extern long g_510530;
extern bool g_510554;
extern dword g_510558;
extern bool g_510580;
extern long g_510584;
extern long g_5107dc;

bool network_link_initialize(c_class_93590 *link);
bool network_observer_initialize(s_network_observer *observer, void *unknown04,
	s_network_observer_configuration *configuration, void *link, void *unknown0c);
bool network_session_initialize(long owner_index, s_network_observer *observer, c_class_58d20 *session,
	c_class_58d20 **sessions, long value14, long value38, void *unknown04);
bool network_session_manager_initialize(long unknown40, long unknown44, void *unknown2c,
	c_class_58d20 *session_a, c_class_58d20 *session_c, c_class_58d20 *session_b);
bool network_session_interface_initialize(void *session_manager);
bool function_7f020(s_network_observer *observer, real *trim);
void __stdcall function_08e090(long unused);
void __stdcall function_08e0b0(long unused);
void __stdcall function_6cad0(long unused);
void transport_register_transition_functions(transport_transition_function startup, transport_transition_function shutdown,
	transport_transition_function reset, void *context);
void online_tasks_initialize(void);
bool function_7fa40(void);
void function_7f930(void);
void pending_messages_reset(void);
void function_53210(void);
void function_8dd70(void);

// @retail 0x8d9f0
void function_8d9f0(void)
{
	bool initialize = !g_4d8ba0;
	if (initialize)
	{
		network_configuration_initialize();
		g_510560 = (c_class_93590 *)&g_528000;
		g_510564 = &g_528588;
		g_510568 = (s_network_message_gateway *)g_528b28;
		g_51056c = (c_class_938e0 *)g_529188;
		g_510570 = (s_network_observer *)g_5291a0;
		g_510574 = (c_class_58d20 *)g_52e0f8;
		g_510578 = &g_52e0e8;
		long link_ready = network_link_initialize(g_510560);
		memset(g_510564, 0, 0x5a0);
		network_message_types_register_discovery(g_510564);
		network_message_types_register_connection(g_510564);
		function_adab0(g_510564);
		network_message_types_register_session_membership(g_510564);
		network_message_types_register_session_parameters(g_510564);
		network_message_types_register_view_establishment(g_510564);
		network_message_types_register_synchronous(g_510564);
		network_message_types_register_game_results(g_510564);
		network_message_types_register_test(g_510564);
		long observer_ready = false;
		if (link_ready)
		{
			s_08d9f0_gateway *gateway = (s_08d9f0_gateway *)g_510568;
			gateway->link = (s_08d9f0_link *)g_510560;
			gateway->link->consumer = gateway;
			gateway->messages = g_510564;
			gateway->handler = NULL;
			gateway->unknown14 = false;
			gateway->initialized = true;
			s_08d9f0_handler *handler = (s_08d9f0_handler *)g_51056c;
			handler->link = g_510560;
			handler->messages = g_510564;
			handler->gateway = g_510568;
			((s_08d9f0_gateway *)handler->gateway)->handler = g_51056c;
			handler->observer = NULL;
			handler->sessions = NULL;
			handler->initialized = true;
			if (network_observer_initialize(g_510570, g_510560,
				(s_network_observer_configuration *)((byte *)&g_network_configuration + 0x14a0), g_510568, g_510564))
				observer_ready = true;
		}
		((s_08d9f0_handler *)g_51056c)->observer = g_510570;
		long sessions_ready = false;
		if (observer_ready)
		{
			g_510578->sessions[0] = NULL;
			g_510578->sessions[1] = NULL;
			g_510578->sessions[2] = NULL;
			sessions_ready = true;
		}
		g_510550 = g_510578;
		((s_08d9f0_handler *)g_51056c)->sessions = g_510578;
		if (sessions_ready &&
			network_session_initialize(0, g_510570, g_510574, g_510578->sessions, 1, 0, g_510568) &&
			network_session_initialize(1, g_510570, g_510574 + 1, g_510578->sessions, 1, 1, g_510568) &&
			network_session_initialize(2, g_510570, g_510574 + 2, g_510578->sessions, 2, 2, g_510568))
		{
			g_4d8ea8.link = g_510560;
			memset(&g_4d8ecc, 0, 0x38);
			g_4d8f04 = 0;
			g_4d8f08 = false;
			g_4d8f0c = 0;
			g_4d8f10 = NULL;
			g_4d8f14 = 0;
			g_4d8f18 = NULL;
			g_4d8eb0 = g_510568;
			g_4d8ea8.initialized = true;
			g_4d8ecc.task_index = NONE;
			g_4d8ecc.qos_handle = NONE;
			if (network_session_manager_initialize((long)g_510568, (long)g_510570, g_510578,
				g_510574, g_510574 + 1, g_510574 + 2) &&
				network_session_interface_initialize(g_510578))
			{
				memset(&g_509450, 0, 0x70f8);
				*(bool *)&g_509450 = true;
				g_510530 = NONE;
				if (function_7f020(g_510570, &g_network_configuration.real10))
				{
					transport_register_transition_functions((transport_transition_function)function_08e090, (transport_transition_function)function_08e0b0, NULL, NULL);
					online_tasks_initialize();
					transport_register_transition_functions(NULL, (transport_transition_function)function_6cad0, NULL, NULL);
					if (!function_7fa40())
						function_7f930();
					g_510558 = GetTickCount();
					g_510554 = true;
					pending_messages_reset();
					function_53210();
					memset(&g_510580, 0, 0x268);
					g_510584 = NONE;
					g_5107dc = NONE;
					g_4d8ba0 = true;
					return;
				}
			}
		}
		function_8dd70();
	}
}
