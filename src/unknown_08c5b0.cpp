// @flags /O2 /Gr
/* UNKNOWN_08C5B0.CPP: the settings bungie.net keeps for a signed-in
   user; each handler applies one to the user's controller (lane D) */

#include "unknown_11c920.h"
#include "globals.h"
#include "pending_messages.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>

/* what a handler is given */
struct s_bungienet_user_request
{
	byte unknown00[0xe];
	short kind;
	long controller_index;
};

typedef bool (__stdcall *bungienet_user_handler)(s_bungienet_user_request *request);

struct s_bungienet_user_handler
{
	long unknown00;
	bungienet_user_handler handler;
};

/* the controllers' state (g_54e8e0, 0xc70 bytes each); this file reads only
   the voice setting at +0x204 */
struct s_controller_voice_view
{
	byte unknown000[0x204];
	long voice_setting;
	byte unknown208[0xc70 - 0x208];
};


/* The request header and its payload have separate lifetimes. */
extern s_pending_message_header g_479750;
struct s_user_settings_payload
{
 dword header[4];
 long voice_setting;
 byte field_14[0x300];
};
s_user_settings_payload g_4d8800;

void function_18fe9e(long index);

static inline s_controller_voice_view *controller_voice_view_get(long controller_index)
{
	return &((s_controller_voice_view *)g_54e8e0)[controller_index];
}

// @retail 0x8c5b0
bool __stdcall bungienet_user_voice_reset(s_bungienet_user_request *request)
{
	long controller_index = request->controller_index;
	s_controller_voice_view *controller = controller_voice_view_get(controller_index);
	long old_setting = controller->voice_setting;
	bool changed = false;
	if (old_setting != 1)
		changed = true;
	controller->voice_setting = 1;
	if (changed)
		function_18fe9e(controller_index);
	return true;
}

static inline bool bungienet_block_available(void)
{
	if (g_479750.kind != 0 && (g_479750.flags & 2))
		return true;
	return false;
}

static inline long bungienet_user_voice_setting(short kind)
{
	if (kind == 1 && bungienet_block_available())
		return g_4d8800.voice_setting;
	return 1;
}

// score 11 (was 11): retail keeps the request pointer in ECX and reads the kind with a
// 16-bit load; ours keeps the pointer in EAX, so the kind test becomes a memory compare.
// @retail 0x8c5f0
bool __stdcall bungienet_user_voice_apply(s_bungienet_user_request *request)
{
	s_bungienet_user_request *local_0 = request;
	short local_1 = *(volatile short *)&local_0->kind;
	long controller_index = request->controller_index;
	long setting = bungienet_user_voice_setting(local_1);
	s_controller_voice_view *controller = controller_voice_view_get(controller_index);
	long old_setting = controller->voice_setting;
	bool changed = false;
	if (setting != old_setting)
		changed = true;
	controller->voice_setting = setting;
	if (changed)
		function_18fe9e(controller_index);
	return true;
}

struct s_user_settings_definition
{
 long kind;
 const wchar_t *filename;
 long version;
 s_bungienet_user_handler handlers[2];
};

s_user_settings_definition g_46729c =
{
 1, L"bungienet_user.dat", 1,
 { {0, bungienet_user_voice_reset}, {0, bungienet_user_voice_apply} }
};
s_pending_message_header g_479750 =
{
 (dword)&g_46729c, 0, 0, 1, 0, 0, NONE,
 {NONE, NONE, 0}, NONE, &g_4d8800, sizeof(g_4d8800)
};

bool pending_message_request_receive(s_pending_message_header *header, long controller,
 s_pending_message_values values, long value06);

static inline XUID *user_settings_identity(XONLINE_USER *user)
{
 XUID *result = 0;
 if (user) result = &user->xuid;
 return result;
}

static __forceinline long user_settings_next_controller(long index)
{
 long result;
 switch (index + 1)
 {
 case 0: result = 0; break;
 case 1: result = 1; break;
 case 2: result = 2; break;
 case 3: result = 3; break;
 default: result = NONE; break;
 }
 return result;
}

static __forceinline void user_settings_request(long controller, XUID *identity)
{
 if (identity && g_479750.state == 0)
 {
  g_479750.flags &= ~2;
  pending_message_request_receive(&g_479750, controller,
   *(s_pending_message_values *)identity, 0);
 }
}

// @retail 0x8c660
void function_8c660(void)
{
 XONLINE_USER users[4];
 memset(users, 0, sizeof(users));
 XONLINE_USER *signed_in = XOnlineGetLogonUsers();
 if (signed_in) memcpy(users, signed_in, sizeof(users));
 for (long controller = 0; controller != NONE; controller = user_settings_next_controller(controller))
 {
  HRESULT status = users[controller].hr;
  XUID *identity = user_settings_identity(&users[controller]);
  if (identity && identity->qwUserID && SUCCEEDED(status))
  {
   if (!XOnlineIsUserGuest(users[controller].xuid.dwUserFlags))
   {
    if (controller_voice_view_get(controller)->voice_setting == 0)
     user_settings_request(controller, user_settings_identity(&users[controller]));
   }
   else
   {
    s_controller_voice_view *slot = controller_voice_view_get(controller);
    long old_setting = slot->voice_setting;
    bool changed = false;
    if (old_setting != 1) changed = true;
    slot->voice_setting = 1;
    if (changed) function_18fe9e(controller);
   }
  }

 }
}
