#include "../Hooks.h"

//Credit to Spook953
DEFINE_HOOK(CTFPlayerShared_IsPlayerDominated, bool, __fastcall, void* ecx, void* edx, int iPlayerIndex)
{
	bool bResult = Func.Original<FN>()(ecx, edx, iPlayerIndex);

	if (!bResult)
	{
		static const DWORD s_pfJumpFrom = g_Pattern.Find("client.dll", "84 C0 74 21 80 7D");
		static const DWORD s_pfJumpTo = g_Pattern.Find("client.dll", "89 45 BC E8 ? ? ? ? 3B");

		if (reinterpret_cast<DWORD>(_ReturnAddress()) == s_pfJumpFrom)
			*reinterpret_cast<PDWORD>(_AddressOfReturnAddress()) = s_pfJumpTo;

	}

	return bResult;
}