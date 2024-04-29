#include "../Hooks.h"

//Credit to bertti
DEFINE_HOOK(CTFPlayerShared_InCond, bool, __fastcall, void* ecx, void* edx, ETFCond eCond)
{
	if (eCond == ETFCond::TF_COND_ZOOMED)
	{
		static const DWORD s_pfWearableShouldDraw = g_Pattern.Find("client.dll", "E8 ? ? ? ? 84 C0 0F 85 A9 01 00 00 6A 03") + 0x5;
		static const DWORD s_pfPlayerShouldDraw = g_Pattern.Find("client.dll", "E8 ? ? ? ? 84 C0 74 05 5E") + 0x5;

		if (reinterpret_cast<DWORD>(_ReturnAddress()) == s_pfWearableShouldDraw ||
			reinterpret_cast<DWORD>(_ReturnAddress()) == s_pfPlayerShouldDraw)
			return false;
	}

	return Func.Original<FN>()(ecx, edx, eCond);
}