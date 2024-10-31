#include "../Hooks.h"
#include "../../Features/Glow/Glow.h"

DEFINE_HOOK(CViewRender_DrawViewModels, void, __fastcall, IViewRender* ecx, void* edx, const CViewSetup& viewRender, bool drawViewmodel)
{
	g_Globals.m_bIsDrawingViewmodels = true;

	Func.Original<FN>()(ecx, edx, viewRender, drawViewmodel);

	F::Glow.SetView(viewRender);
	F::Glow.SetFrustum(ecx->GetFrustum());
	F::Glow.Render();

	g_Globals.m_bIsDrawingViewmodels = false;
}