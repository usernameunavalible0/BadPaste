#include "../Hooks.h"

#include "../../Features/Prediction/Prediction.h"
#include "../../Features/Misc/Misc.h"
#include "../../Features/Crithack/Crithack.h"
#include "../../Features/Aimbot/Aimbot.h"
#include "../../Features/Auto/Auto.h"
#include "../../Features/AntiHack/AntiHack.h"
#include "../../Features/Vars.h"

DEFINE_HOOK(ClientModeShared_CreateMove, bool, __fastcall, void* ecx, void* edx, float flInputSampleTime, CUserCmd* cmd)
{
	g_Globals.m_bChoking = false;

	if (!cmd || cmd->command_number <= 0)
		return Func.Original<FN>()(ecx, edx, flInputSampleTime, cmd);

	uintptr_t _bp; __asm mov _bp, ebp;
	bool* pbSendPacket = (bool*)(***(uintptr_t***)_bp - 0x1);

	QAngle vOldAngles = cmd->viewangles;
	float fOldSide = cmd->sidemove;
	float fOldForward = cmd->forwardmove;

	C_TFPlayer* pLocal = UTIL_TFPlayerByIndex(g_Globals.m_nLocalIndex);

	if (pLocal && pLocal->IsAlive())
	{
		C_TFWeaponBase* pWeapon = pLocal->GetActiveTFWeapon();

		F::Misc.Run(pLocal, cmd);

		if (pWeapon)
		{
			F::Prediction.Start(pLocal, cmd);
			{
				//Run aimbot, triggerbot etc. here
				F::Aimbot.Run(pLocal, pWeapon, cmd);
				F::Auto.Run(pLocal, pWeapon, cmd);
				F::AntiHack.Run(cmd, pbSendPacket);
			}
			F::Prediction.Finish(pLocal);

			F::Crithack.Run(pWeapon, cmd);
		}

		g_Globals.m_vViewAngles = cmd->viewangles;

		if (Vars::AntiHack::Fakelag.m_Var)
			*pbSendPacket = I::EngineClient->GetNetChannelInfo()->m_nChokedPackets >= Vars::AntiHack::FakelagAmount.m_Var;

		//Credit to Spook953
		if (pLocal->IsTaunting() && pLocal->m_bAllowMoveDuringTaunt())
		{
			static float flYaw = cmd->viewangles.y;

			if (GetAsyncKeyState(VK_SHIFT))
			{
				float yaw{ 12.f };

				flYaw -= yaw;
				NormalizeAngle(flYaw);

				cmd->viewangles.y = flYaw;
			}
			else flYaw = cmd->viewangles.y;
		}
	}

	static bool bWasSet = false;

	if (g_Globals.m_bChoking) {
		*pbSendPacket = false;
		bWasSet = true;
	}

	else
	{
		if (bWasSet)
		{
			*pbSendPacket = true;
			cmd->viewangles = vOldAngles;
			cmd->sidemove = fOldSide;
			cmd->forwardmove = fOldForward;
			bWasSet = false;
		}
	}

	//failsafe
	{
		static int nChoked = 0;

		if (!*pbSendPacket)
			nChoked++;

		else nChoked = 0;

		if (nChoked > 14)
			*pbSendPacket = true;
	}

	return false;
}