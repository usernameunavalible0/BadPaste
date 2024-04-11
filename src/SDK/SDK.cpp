#include "SDK.h"

void CUtil::FixMovement(const QAngle& va, CUserCmd* cmd)
{
	const Vector vMove = { cmd->forwardmove, cmd->sidemove, cmd->upmove };

	QAngle vAng;
	VectorAngles(vMove, vAng);

	const float flSpeed = FastSqrt(vMove.x * vMove.x + vMove.y * vMove.y);

	float s, c;
	SinCos(DEG2RAD(va.y - cmd->viewangles.y + vAng.y), &s, &c);

	cmd->sidemove = (s * flSpeed);
	cmd->forwardmove = (c * flSpeed);
}

bool CUtil::IsVisible(const Vector& vStart, const Vector& vEnd)
{
	trace_t tr;
	CTraceFilterWorldAndPropsOnly filter;
	UTIL_TraceLine(vStart, vEnd, (MASK_SHOT | CONTENTS_GRATE), &filter, &tr);

	return !(tr.DidHit());
}

Color CUtil::GetTeamColor(const int nTeam)
{
	if (nTeam == 2)
		return COLOR_TF_RED;
	else if (nTeam == 3)
		return COLOR_TF_BLUE;
	else
		return COLOR_TF_SPECTATOR;
}

Color CUtil::GetHealthColor(const int nHealth, const int nMaxHealth)
{
	if (nHealth > nMaxHealth)
		return COLOR_BLUE;

	const int nHP = Max(0, Min(nHealth, nMaxHealth));

	const uint32 nR = static_cast<uint32>(Min((510 * (nMaxHealth - nHP)) / nMaxHealth, 200));
	const uint32 nG = static_cast<uint32>(Min((510 * nHP) / nMaxHealth, 200));

	return { static_cast<byte>(nR), static_cast<byte>(nG), 0u, 255u };
}

//https://github.com/lua9520/source-engine-2018-hl2_src/blob/master/engine/client.cpp#L624
float CUtil::GetClientInterpAmount()
{
	return max(G::ConVars.cl_interp_ratio->GetFloat() / G::ConVars.cl_updaterate->GetFloat(), G::ConVars.cl_interp->GetFloat());
}
