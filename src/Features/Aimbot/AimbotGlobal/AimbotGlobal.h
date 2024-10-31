#pragma once
#include "../../../SDK/SDK.h"

enum struct ETargetType {
	UNKNOWN, PLAYER, BUILDING
};

enum struct ESortMethod {
	UNKNOWN, FOV, DISTANCE
};

struct Target_t
{
	C_BaseEntity* m_pEntity = nullptr;
	ETargetType m_TargetType = ETargetType::UNKNOWN;
	Vector m_vPos = {};
	QAngle m_vAngleTo = {};
	float m_flFOVTo = FLT_MAX;
	float m_flDistTo = FLT_MAX;
	int m_nAimedHitbox = HITBOX_MAX;
};

class CAimbot_Global
{
public:
	bool IsKeyDown();
	void SortTargets(const ESortMethod& Method);
	const Target_t& GetBestTarget(const ESortMethod& Method);

public:
	CUtlVector<Target_t> m_vecTargets;
};

namespace A { inline CAimbot_Global Global; }