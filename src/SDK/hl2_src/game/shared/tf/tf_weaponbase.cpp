#include "../../client/tf/c_tf_player.h"
#include "tf_weaponbase.h"

void FindHullIntersection(const Vector& vecSrc, trace_t& tr, const Vector& mins, const Vector& maxs, C_BaseEntity* pEntity)
{
	int	i, j, k;
	trace_t tmpTrace;
	Vector vecEnd;
	float distance = 1e6f;
	Vector minmaxs[2] = { mins, maxs };
	Vector vecHullEnd = tr.endpos;

	vecHullEnd = vecSrc + ((vecHullEnd - vecSrc) * 2);
	UTIL_TraceLine(vecSrc, vecHullEnd, MASK_SOLID, pEntity, COLLISION_GROUP_NONE, &tmpTrace);
	if (tmpTrace.fraction < 1.0)
	{
		tr = tmpTrace;
		return;
	}

	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < 2; k++)
			{
				vecEnd.x = vecHullEnd.x + minmaxs[i][0];
				vecEnd.y = vecHullEnd.y + minmaxs[j][1];
				vecEnd.z = vecHullEnd.z + minmaxs[k][2];

				UTIL_TraceLine(vecSrc, vecEnd, MASK_SOLID, pEntity, COLLISION_GROUP_NONE, &tmpTrace);
				if (tmpTrace.fraction < 1.0)
				{
					float thisDistance = (tmpTrace.endpos - vecSrc).Length();
					if (thisDistance < distance)
					{
						tr = tmpTrace;
						distance = thisDistance;
					}
				}
			}
		}
	}
}

bool C_TFWeaponBase::CanFireCiriticalShotHelper(const bool bHeadShot)
{
	bool bRet = false;

	C_BasePlayer* pOwner = m_hOwner().Get()->As<C_BasePlayer*>();

	if (pOwner)
	{
		const int nOldFov = pOwner->m_iFOV();

		pOwner->m_iFOV() = 70;
		bRet = U::VFunc.Get<bool(__thiscall*)(void*, bool, C_BaseEntity*)>(this, 425u)(this, bHeadShot, nullptr);
		pOwner->m_iFOV() = nOldFov;
	}

	return bRet;
}
