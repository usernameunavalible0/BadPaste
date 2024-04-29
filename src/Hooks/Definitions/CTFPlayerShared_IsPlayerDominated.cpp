#include "../Hooks.h"

//Credit to Spook953
DEFINE_HOOK(CTFPlayerShared_IsPlayerDominated, bool, __fastcall, void* ecx, void* edx, int iPlayerIndex)
{
	bool bResult = Func.Original<FN>()(ecx, edx, iPlayerIndex);

	if (!bResult)
	{
		static const DWORD s_pfJumpFrom = g_Pattern.Find("client.dll", "E8 ? ? ? ? 84 C0 74 22 8B 45 FC") + 0x5;
		static const DWORD s_pfJumpTo = g_Pattern.Find("client.dll", "8B 0D ? ? ? ? 81 C1 50 05 00 00 57 8B 01 8B 40 10 FF D0 84 C0 0F 84 FD 00 00 00");

		if (reinterpret_cast<DWORD>(_ReturnAddress()) == s_pfJumpFrom)
			*reinterpret_cast<PDWORD>(_AddressOfReturnAddress()) = s_pfJumpTo;
	}

	return bResult;
}