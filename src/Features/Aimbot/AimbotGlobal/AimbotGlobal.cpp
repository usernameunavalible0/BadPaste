#include "AimbotGlobal.h"
#include "../../Vars.h"

bool CAimbot_Global::IsKeyDown()
{
	return !Vars::Aimbot::Global::AimKey.m_Var ? true : (GetAsyncKeyState(Vars::Aimbot::Global::AimKey.m_Var) & 0x8000);
}

void CAimbot_Global::SortTargets(const ESortMethod& Method)
{
	m_vecTargets.SortPredicate([&](const Target_t& a, const Target_t& b) -> bool
		{
			switch (Method)
			{
			case ESortMethod::FOV: return (a.m_flFOVTo < b.m_flFOVTo);
			case ESortMethod::DISTANCE: return (a.m_flDistTo < b.m_flDistTo);
			default: return false;
			}
		});
}

const Target_t& CAimbot_Global::GetBestTarget(const ESortMethod& Method)
{
	return *std::min_element(m_vecTargets.begin(), m_vecTargets.end(), [&](const Target_t& a, const Target_t& b) -> bool
		{
			switch (Method)
			{
			case ESortMethod::FOV: return (a.m_flFOVTo < b.m_flFOVTo);
			case ESortMethod::DISTANCE: return (a.m_flDistTo < b.m_flDistTo);
			default: return false;
			}
		});
}
