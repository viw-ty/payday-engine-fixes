#include "eos/eos_sessions_types.h"
#include "eos/eos_lobby_types.h"

#include "minhook.h"
#include "superblt_flat.h"

#include "string.h"
#include <stdint.h>
#include <ctype.h>

typedef EOS_EResult (__stdcall *copy_t)(EOS_HLobbyDetails handle, const EOS_LobbyDetails_CopyAttributeByIndexOptions* opts, EOS_Lobby_Attribute ** out);

copy_t orig = nullptr;

const char *values[] = {
  "JOB_CLASS", "JOB_CLASS_MAX", "JOB_ID", "OWNER_LEVEL", "DROP_IN", "ONE_DOWN",
  "MIN_LEVEL", "NUM_PLAYERS", "JOB_PLAN", "REGION", "DIFFICULTY", "LEVEL",
  "JOB_CLASS_MIN", "LOBBY_TYPE", "ALLOW_MODS", "CRIME_SPREE", "KICK_OPTION",
  "SKIRMISH", "PERMISSION", "STATE", 0
};

int compare_str(const char *s) {
  for (int i = 0; values[i]; i++)
    if (!strcmp(s, values[i]))
      return 1;

  return 0;
}

EOS_EResult __stdcall hcopy(EOS_HLobbyDetails handle, const EOS_LobbyDetails_CopyAttributeByIndexOptions *opts, EOS_Lobby_Attribute **out) {
  auto res = orig(handle, opts, out);

  if (!(int)res) {
    auto data = (*out)->Data;
    if (compare_str(data->Key) && ((int)data->ValueType == 3)) {
      // magic we compare later
      data->Value.AsInt64 = -2000;
      data->ValueType = EOS_ELobbyAttributeType::EOS_AT_INT64;
    }
  }

  return res;
}

void Plugin_Init() {
  auto h = LoadLibraryA("EOSSDK-Win32-Shipping.dll");
  auto fn = GetProcAddress(h,  "_EOS_LobbyDetails_CopyAttributeByIndex@12");
  orig = (copy_t)fn;

  MH_Initialize();
  MH_CreateHook(fn, (void*)&hcopy, (void**)&orig);
  MH_EnableHook(fn);
}

bool Plugin_Unload() { return false; }

void Plugin_Update() {}

void Plugin_Setup_Lua(lua_State *L) {}

int Plugin_PushLua(lua_State *L) {
  return 0;
}

SBLT_API_EXPORT const char *MODULE_LICENCE_DECLARATION =
    "This module is licenced under the GNU GPL version 2 or later, or another "
    "compatible licence";
SBLT_API_EXPORT const char *MODULE_SOURCE_CODE_LOCATION = "https://github.com/viw-ty/payday-engine-fixes";
SBLT_API_EXPORT const char *MODULE_SOURCE_CODE_REVISION = "1.2";
