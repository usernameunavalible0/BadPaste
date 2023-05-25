#include "../Hooks.h"

//Credit to Spook953
DEFINE_HOOK(KeyValues_SetInt, void, __fastcall, void* ecx, void* edx, const char* keyName, int value)
{
	Func.Original<FN>()(ecx, edx, keyName, value);

	static const DWORD s_pfJumpFrom = g_Pattern.Find("client.dll", "8B 03 8B CB 56 57 ? 90 B8 03 ? ? 6A");
	static const DWORD s_pfJumpTo = g_Pattern.Find("client.dll", "89 45 BC E8 ? ? ? ? 3B");

	if (reinterpret_cast<DWORD>(_ReturnAddress()) == s_pfJumpFrom && !strcmp(keyName, "nemesis"))
		*reinterpret_cast<PDWORD>(_AddressOfReturnAddress()) = s_pfJumpTo;
}