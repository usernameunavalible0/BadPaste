#include "../Hooks.h"

#include "../../Features/Vars.h"

DEFINE_HOOK(IPanel_PaintTraverse, void, __fastcall, void* ecx, void* edx, vgui::VPANEL vguiPanel, bool forceRepaint, bool allowForce)
{
	if (Vars::Visual::RemoveScope.m_Var && !strcmp(I::VGuiPanel->GetName(vguiPanel), "HudScope"))
		return;

	Func.Original<FN>()(ecx, edx, vguiPanel, forceRepaint, allowForce);
}