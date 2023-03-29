#include "VGuiSurface.h"

#include "../../Features/Menu/Menu.h"

using namespace Hooks;

void __fastcall VGuiSurface::OnScreenSizeChanged::Detour(void* ecx, void* edx, int oldWidth, int oldHeight)
{
	Table.Original<FN>(Index)(ecx, edx, oldWidth, oldHeight);

	G::Draw.ReloadScreenSize();
	G::Draw.ReloadFonts();
}

void __fastcall VGuiSurface::LockCursor::Detour(void* ecx, void* edx)
{
	F::Menu.m_bOpen ? I::VGuiSurface->UnlockCursor() : Table.Original<FN>(Index)(ecx, edx);
}

void VGuiSurface::Initialize()
{
	XASSERT(Table.Initialize(I::VGuiSurface) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&OnScreenSizeChanged::Detour, OnScreenSizeChanged::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&LockCursor::Detour, LockCursor::Index) == FAILED_TO_HOOK);
}
