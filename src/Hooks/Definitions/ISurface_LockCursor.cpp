#include "../Hooks.h"

#include "../../Features/Menu/Menu.h"

DEFINE_HOOK(ISurface_LockCursor, void, __fastcall, void* ecx, void* edx)
{
	F::Menu.m_bOpen ? I::VGuiSurface->UnlockCursor() : Func.Original<FN>()(ecx, edx);
}