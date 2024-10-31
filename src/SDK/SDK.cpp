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

bool CUtil::IsVisible(const Vector& vStart, const Vector& vEnd, const Vector& vMins, const Vector& vMaxs)
{
	return false;
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

static bool IgnoreActorsTraceFilterFunction(IHandleEntity* pServerEntity, int contentsMask)
{
	C_BaseEntity* entity = EntityFromEntityHandle(pServerEntity);
	return (entity->MyCombatCharacterPointer() == NULL);	// includes all bots, npcs, players, and TF2 buildings
}

//--------------------------------------------------------------------------------------------
/**
 * Trace filter that skips all players and NextBots
 */
class NextBotTraceFilterIgnoreActors : public CTraceFilterSimple
{
public:
	NextBotTraceFilterIgnoreActors(const IHandleEntity* passentity, int collisionGroup) : CTraceFilterSimple(passentity, collisionGroup, IgnoreActorsTraceFilterFunction)
	{
	}
};

Vector CUtil::EstimateProjectileImpactPosition(C_TFPlayer* pPlayer, float pitch, float yaw, float initVel)
{
	// copied from CTFWeaponBaseGun::FirePipeBomb()
	Vector vecForward, vecRight, vecUp;
	QAngle angles(pitch, yaw, 0.0f);
	AngleVectors(angles, &vecForward, &vecRight, &vecUp);

	// we will assume bots never flip viewmodels
	float fRight = 8.f;
	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	vecSrc += vecForward * 16.0f + vecRight * fRight + vecUp * -6.0f;

	const float initVelScale = 0.9f;
	Vector vecVelocity = initVelScale * ((vecForward * initVel) + (vecUp * 200.0f));

	const float timeStep = 0.01f;
	const float maxTime = 5.0f;

	Vector pos = vecSrc;
	Vector lastPos = pos;
	const float g = G::ConVars.sv_gravity->GetFloat(); //GetCurrentGravity();

	// compute forward facing unit vector in horiz plane
	Vector alongDir = vecForward;
	alongDir.z = 0.0f;
	alongDir.NormalizeInPlace();

	float alongVel = FastSqrt(vecVelocity.x * vecVelocity.x + vecVelocity.y * vecVelocity.y);

	trace_t trace;
	NextBotTraceFilterIgnoreActors traceFilter(NULL, COLLISION_GROUP_NONE);

	float t;
	for (t = 0.0f; t < maxTime; t += timeStep)
	{
		float along = alongVel * t;
		float height = vecVelocity.z * t - 0.5f * g * t * t;

		pos.x = vecSrc.x + alongDir.x * along;
		pos.y = vecSrc.y + alongDir.y * along;
		pos.z = vecSrc.z + height;

		UTIL_TraceHull(lastPos, pos, -Vector(8, 8, 8), Vector(8, 8, 8), MASK_SOLID_BRUSHONLY, &traceFilter, &trace);

		if (trace.DidHit())
		{
//#ifdef STAGING_ONLY
			//if (tf_debug_ballistics.GetBool())
			{
				//NDebugOverlay::Cross3D(trace.endpos, 10.0f, 100, 255, 0, true, NDEBUG_PERSIST_TILL_NEXT_SERVER);
				I::DebugOverlay->AddBoxOverlay(trace.endpos, Vector(10.f, 10.f, 10.f), Vector(-10.f, -10.f, -10.f), QAngle(0.f, 0.f, 0.f), 100, 255, 0, 255, NDEBUG_PERSIST_TILL_NEXT_SERVER);
			}
//
			break;
		}

//#ifdef STAGING_ONLY
		//if (tf_debug_ballistics.GetBool())
		{
			//NDebugOverlay::Line(lastPos, pos, 0, 255, 0, false, NDEBUG_PERSIST_TILL_NEXT_SERVER);
			I::DebugOverlay->AddLineOverlay(lastPos, pos, 0, 255, 0, false, NDEBUG_PERSIST_TILL_NEXT_SERVER);
		}
//#endif

		lastPos = pos;
	}

	return trace.endpos;
}

Vector CUtil::VelocityToAngles(const Vector direction)
{
	auto Magnitude = [&](const Vector& v) -> float {
		return sqrtf(v.Dot(v));
		};

	float yaw, pitch;

	if (direction.y == 0.0f && direction.x == 0.0f)
	{
		yaw = 0.0f;

		if (direction.z > 0.0f)
			pitch = 270.0f;
		else
			pitch = 90.0f;
	}
	else
	{
		yaw = RAD2DEG(::atan2f(direction.y, direction.x));
		pitch = RAD2DEG(::atan2f(-direction.z, Magnitude(Vector(direction))));

		if (yaw < 0.0f)
			yaw += 360.0f;

		if (pitch < 0.0f)
			pitch += 360.0f;
	}

	return { pitch, yaw, 0.0f };
}
