#include "../Hooks.h"
#include "../../Features/Aimbot/AimbotProjectile/MovementSimulation/MovementSimulation.h"

DEFINE_HOOK(CTFGameMovement_PlayerMove, void, __fastcall, void* ecx, void* edx)
{
	if (A::MovementSimulation.IsRunning())
	{
		reinterpret_cast<void(__thiscall*)(void*)>(U::Offsets.m_dwPlayerMove)(ecx);
		return;
	}

	Func.Original<FN>()(ecx, edx);
}