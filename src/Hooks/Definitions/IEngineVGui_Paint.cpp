#include "../Hooks.h"

#include "../../Features/ESP/ESP.h"
#include "../../Features/Menu/Menu.h"
#include "../../Features/Visual/Visual.h"
#include "../../Features/Backtrack/Backtrack.h"

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

					F::Backtrack.DrawLagRecordsBasic();
				}
			}

			F::Menu.Run();
		}
		pfFinishDrawing(I::MatSystemSurface);
	}
}