#include "../Hooks.h"

DEFINE_HOOK(IPanel_PaintTraverse, void, __fastcall, void* ecx, void* edx, vgui::VPANEL vguiPanel, bool forceRepaint, bool allowForce)
{
	Func.Original<FN>()(ecx, edx, vguiPanel, forceRepaint, allowForce);
}