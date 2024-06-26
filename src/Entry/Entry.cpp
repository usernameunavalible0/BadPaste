#include "Entry.h"
#include "../Features/Chams/Chams.h"
#include "../Features/Glow/Glow.h"
#include "../Features/ESP/ESP.h"

#define _VALIDATE(x) if (!x) { Error(#x); m_bShouldUnload = true; return; }

void CModuleEntry::Load()
{
	while (!GetModuleHandleW(L"GameUI.dll"))
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	MathLib_Init();

	_VALIDATE(U::Offsets.Initialize());
	_VALIDATE(g_Interfaces.Initialize());

	_VALIDATE(G::ConVars.Initialize());
	_VALIDATE(G::Draw.Initialize());

	G::Draw.ReloadFonts();
	G::Draw.ReloadScreenSize();

	if (I::EngineClient->IsInGame())
	{
		g_Globals.m_nLocalIndex = I::EngineClient->GetLocalPlayer();
		F::ESP.LevelInitPostEntity();
	}

	_VALIDATE(F::Chams.Initialize());
	_VALIDATE(F::Glow.Initialize());

	_VALIDATE(G::Hooks.Initialize());

	//I::EngineClient->ClientCmd_Unrestricted("r_drawtracers_firstperson 1");
}

void CModuleEntry::Unload()
{
	_VALIDATE(G::Hooks.UnInitialize());

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	G::Draw.UnInitialize();

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

bool CModuleEntry::ShouldUnload()
{
	if (GetAsyncKeyState(VK_F11))
		m_bShouldUnload = true;

	return m_bShouldUnload;
}