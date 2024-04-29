#include "../Hooks.h"
#include "../../Features/Glow/Glow.h"

DEFINE_HOOK(ISurface_OnScreenSizeChanged, void, __fastcall, void* ecx, void* edx, int oldWidth, int oldHeight)
{
	Func.Original<FN>()(ecx, edx, oldWidth, oldHeight);

	G::Draw.ReloadFonts();
	G::Draw.ReloadScreenSize();

	F::Glow.OnScreenSizeChanged();
}