#include "../Hooks.h"

#include "../../Features/Prediction/Prediction.h"
#include "../../Features/Misc/Misc.h"
#include "../../Features/Crithack/Crithack.h"

DEFINE_HOOK(ClientModeShared_CreateMove, bool, __fastcall, void* ecx, void* edx, float flInputSampleTime, CUserCmd* cmd)
{
	if (!cmd || cmd->command_number <= 0)
		return Func.Original<FN>()(ecx, edx, flInputSampleTime, cmd);

	uintptr_t _bp; __asm mov _bp, ebp;
	bool* pbSendPacket = (bool*)(***(uintptr_t***)_bp - 0x1);

	C_TFPlayer* pLocal = UTIL_TFPlayerByIndex(g_Globals.m_nLocalIndex);

	if (pLocal && pLocal->IsAlive())
	{
		C_TFWeaponBase* pWeapon = pLocal->GetActiveTFWeapon();

		if (pWeapon)
		{
			F::Crithack.Run(pWeapon, cmd);

			F::Prediction.Start(pLocal, cmd);
			{
				//Run aimbot, triggerbot etc. here
			}
			F::Prediction.Finish(pLocal);
		}

		F::Misc.Run(pLocal, cmd);
	}

	return false;
}