#include "eos/eos_lobby_types.h"
#include "eos/eos_lobby.h"

#include "minhook.h"
#include "superblt_flat.h"

#include "string.h"
#include <stdint.h>
#include <ctype.h>
#include <unordered_set>
#include <string>

typedef EOS_EResult (__stdcall *copy_t)(EOS_HLobbyDetails handle, const EOS_LobbyDetails_CopyAttributeByIndexOptions* opts, EOS_Lobby_Attribute ** out);

copy_t copy_attribute_orig = nullptr;

std::unordered_set<std::string> values = {
    "JOB_CLASS",
    "JOB_CLASS_MAX",
    "OWNER_LEVEL",
    "DROP_IN",
    "ONE_DOWN",
    "MIN_LEVEL",
    "JOB_PLAN",
    "REGION",
    "DIFFICULTY",
    "LEVEL",
    "JOB_CLASS_MIN",
    "LOBBY_TYPE",
    "ALLOW_MODS",
    "CRIME_SPREE",
    "KICK_OPTION",
    "SKIRMISH",
    "PERMISSION",
    "STATE"
};

EOS_EResult __stdcall copy_attribute_hk(EOS_HLobbyDetails handle, const EOS_LobbyDetails_CopyAttributeByIndexOptions *opts, EOS_Lobby_Attribute **out) {
  auto res = copy_attribute_orig(handle, opts, out);

  if (res == EOS_EResult::EOS_Success) {
    auto data = (*out)->Data;
    std::string key = data->Key;

    if (values.contains(key) && data->ValueType != EOS_ELobbyAttributeType::EOS_AT_INT64) {
      // magic we compare later
      data->Value.AsInt64 = -2000;
      data->ValueType = EOS_ELobbyAttributeType::EOS_AT_INT64;
    } else if (key == "NUM_PLAYERS") {
      // i can only assume that this gets set to 1 due to a race condition with the game loading the player list.
      // as i cannot get everyone to download this piece of shit you're getting a client-side fix instead of a proper one.
      EOS_LobbyDetails_GetMemberCountOptions options = { EOS_LOBBYDETAILS_GETMEMBERCOUNT_API_LATEST };
      auto count = EOS_LobbyDetails_GetMemberCount(handle, &options);
      // fuck you, starbreeze
      data->Value.AsInt64 = count;
      data->ValueType = EOS_ELobbyAttributeType::EOS_AT_INT64;
    }
  }

  return res;
}

void Plugin_Init() {
  auto h = LoadLibraryA("EOSSDK-Win32-Shipping.dll");
  auto fn = GetProcAddress(h, "_EOS_LobbyDetails_CopyAttributeByIndex@12");

  MH_Initialize();
  MH_CreateHook(fn, (void*)&copy_attribute_hk, (void**)&copy_attribute_orig);
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
SBLT_API_EXPORT const char *MODULE_SOURCE_CODE_REVISION = "1.3";
