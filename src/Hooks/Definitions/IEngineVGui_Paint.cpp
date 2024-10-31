#include "../Hooks.h"

#include "../../Features/ESP/ESP.h"
#include "../../Features/Menu/Menu.h"
#include "../../Features/Visual/Visual.h"
#include "../../Features/Backtrack/Backtrack.h"
#include "../../Features/AntiHack/AntiHack.h"

DEFINE_HOOK(IEngineVGui_Paint, void, __fastcall, void* ecx, void* edx, int mode)
{
	static const auto pfStartDrawing = reinterpret_cast<void(__thiscall*)(IMatSystemSurface*)>(U::Offsets.m_dwStartDrawing);
	static const auto pfFinishDrawing = reinterpret_cast<void(__thiscall*)(IMatSystemSurface*)>(U::Offsets.m_dwFinishDrawing);

	Func.Original<FN>()(ecx, edx, mode);

	if (mode & PaintMode_t::PAINT_UIPANELS)
	{
		G::Draw.ReloadMatrix();

		pfStartDrawing(I::MatSystemSurface);
		{
			if (g_Globals.m_bIsInGame && !g_Globals.m_bIsGameUIVisible)
			{
				C_TFPlayer* pLocal = UTIL_TFPlayerByIndex(g_Globals.m_nLocalIndex);

				if (pLocal)
				{
					F::ESP.Render(pLocal);

					F::Visual.WeaponSway();

					//F::Backtrack.DrawLagRecordsBasic();

					F::AntiHack.ResolverSpew();

					//Tickbase info
					if (Vars::Misc::CL_Move::Enabled.m_Var)
					{
						C_TFWeaponBase* pWeapon = pLocal->GetActiveTFWeapon();;

						if (pWeapon)
						{
							if (pLocal->IsAlive())
							{
								const int nY = (g_Globals.m_nScreenHeight / 2) + 20;
								const int nX = (g_Globals.m_nScreenWidht / 2);

								if (g_Globals.m_nShifted)
									G::Draw.String(FONT_MENU, nX, nY, { 255, 64, 64, 255 }, TXT_CENTERX, L"Recharge! (%i / %i)", g_Globals.m_nShifted, MAX_NEW_COMMANDS);
								else if (!g_Globals.m_nShifted && g_Globals.m_nWaitForShift)
									G::Draw.String(FONT_MENU, nX, nY, { 255, 178, 0, 255 }, TXT_CENTERX, L"Wait! (%i / %i)", g_Globals.m_nWaitForShift, DT_WAIT_CALLS);
								else if (!g_Globals.m_nShifted && !g_Globals.m_nWaitForShift && pLocal->IsClass(TF_CLASS_HEAVYWEAPONS)  && pWeapon->GetSlot() == 0 && !pLocal->m_vecVelocity().IsZero())
									G::Draw.String(FONT_MENU, nX, nY, { 255, 178, 0, 255 }, TXT_CENTERX, L"Stop!");
								else
									G::Draw.String(FONT_MENU, nX, nY, { 153, 255, 153, 255 }, TXT_CENTERX, L"Shift ready!");
							}
						}
					}

					if (Vars::Visual::AimFOVAlpha.m_Var && g_Globals.m_flCurAimFOV)
					{
						float flFOV = static_cast<float>(Vars::Visual::FOV.m_Var);
						float flRadius = tanf(DEG2RAD(g_Globals.m_flCurAimFOV) / 2.f)
							/ tanf(DEG2RAD((pLocal->IsZoomed() && !Vars::Visual::RemoveZoom.m_Var) ? 30.0f : flFOV) / 2.f) * g_Globals.m_nScreenWidht;
						G::Draw.OutlinedCircle(g_Globals.m_nScreenWidht / 2, g_Globals.m_nScreenHeight / 2, flRadius, 68, { 204, 204, 204, Vars::Visual::AimFOVAlpha.m_Var });
					}
				}
			}

			F::Menu.Run();
		}
		pfFinishDrawing(I::MatSystemSurface);
	}
}