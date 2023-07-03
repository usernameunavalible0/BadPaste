#include "../Hooks.h"

#include "../../Features/Prediction/Prediction.h"
#include "../../Features/Misc/Misc.h"
#include "../../Features/Crithack/Crithack.h"
#include "../../Features/Aimbot/Aimbot.h"
#include "../../Features/Auto/Auto.h"

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
			F::Prediction.Start(pLocal, cmd);
			{
				//Run aimbot, triggerbot etc. here
				F::Aimbot.Run(pLocal, pWeapon, cmd);
				F::Auto.Run(pLocal, pWeapon, cmd);
			}
			F::Prediction.Finish(pLocal);

			F::Crithack.Run(pWeapon, cmd);
		}

		F::Misc.Run(pLocal, cmd);
	}

	return false;
}