#include "../Hooks.h"

DEFINE_HOOK(CInput_GetUserCmd, CUserCmd*, __fastcall, void* ecx, void* edx, int sequence_number)
{
	CUserCmd* m_pCommands = *reinterpret_cast<CUserCmd**>(reinterpret_cast<DWORD>(I::Input) + 0x0FC);
	CUserCmd* usercmd = &m_pCommands[sequence_number % MULTIPLAYER_BACKUP];

	return usercmd;
}