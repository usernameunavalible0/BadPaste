//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: Shared util code between client and server.
//
//=============================================================================//
#ifndef UTIL_SHARED_H
#define UTIL_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#define CLIENT_DLL

#include "../client/c_baseplayer.h"
#include "../../public/gametrace.h"
#include "../../public/mathlib/vector.h"
#include "../../public/tier1/utlvector.h"
#include "../../public/engine/IEngineTrace.h"
#include "../../public/engine/IStaticPropMgr.h"
#include "../../public/engine/ivdebugoverlay.h"

inline bool g_bVisualizeTraces = false;

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class CGameTrace;
typedef CGameTrace trace_t;

//-----------------------------------------------------------------------------
// Language IDs.
//-----------------------------------------------------------------------------
#define LANGUAGE_ENGLISH				0
#define LANGUAGE_GERMAN					1
#define LANGUAGE_FRENCH					2
#define LANGUAGE_BRITISH				3 //Innit

bool FClassnameIs(IClientEntity* pEntity, const char* const szClassName);

//-----------------------------------------------------------------------------
// Pitch + yaw
//-----------------------------------------------------------------------------
float		UTIL_VecToYaw(const Vector& vec);
float		UTIL_VecToPitch(const Vector& vec);
float		UTIL_VecToYaw(const matrix3x4_t& matrix, const Vector& vec);
float		UTIL_VecToPitch(const matrix3x4_t& matrix, const Vector& vec);
Vector		UTIL_YawToVector(float yaw);

//-----------------------------------------------------------------------------
// Shared random number generators for shared/predicted code:
// whenever generating random numbers in shared/predicted code, these functions
// have to be used. Each call should specify a unique "sharedname" string that
// seeds the random number generator. In loops make sure the "additionalSeed"
// is increased with the loop counter, otherwise it will always return the
// same random number
//-----------------------------------------------------------------------------
/*
float	SharedRandomFloat(const char* sharedname, float flMinVal, float flMaxVal, int additionalSeed = 0);
int		SharedRandomInt(const char* sharedname, int iMinVal, int iMaxVal, int additionalSeed = 0);
Vector	SharedRandomVector(const char* sharedname, float minVal, float maxVal, int additionalSeed = 0);
QAngle	SharedRandomAngle(const char* sharedname, float minVal, float maxVal, int additionalSeed = 0);
*/

//-----------------------------------------------------------------------------
// Standard collision filters...
//-----------------------------------------------------------------------------
bool PassServerEntityFilter(const IHandleEntity* pTouch, const IHandleEntity* pPass);
bool StandardFilterRules(IHandleEntity* pHandleEntity, int fContentsMask);

//-----------------------------------------------------------------------------
// Converts an IHandleEntity to an C_BaseEntity
//-----------------------------------------------------------------------------
inline const C_BaseEntity* EntityFromEntityHandle(const IHandleEntity* pConstHandleEntity)
{
	IHandleEntity* pHandleEntity = const_cast<IHandleEntity*>(pConstHandleEntity);

#ifdef CLIENT_DLL
	IClientUnknown* pUnk = (IClientUnknown*)pHandleEntity;
	return pUnk->GetBaseEntity();
#else
	if (staticpropmgr->IsStaticProp(pHandleEntity))
		return NULL;

	IServerUnknown* pUnk = (IServerUnknown*)pHandleEntity;
	return pUnk->GetBaseEntity();
#endif
}

inline C_BaseEntity* EntityFromEntityHandle(IHandleEntity* pHandleEntity)
{
#ifdef CLIENT_DLL
	IClientUnknown* pUnk = (IClientUnknown*)pHandleEntity;
	return pUnk->GetBaseEntity();
#else
	if (staticpropmgr->IsStaticProp(pHandleEntity))
		return NULL;

	IServerUnknown* pUnk = (IServerUnknown*)pHandleEntity;
	return pUnk->GetBaseEntity();
#endif
}

typedef bool (*ShouldHitFunc_t)(IHandleEntity* pHandleEntity, int contentsMask);

//-----------------------------------------------------------------------------
// traceline methods
//-----------------------------------------------------------------------------
class CTraceFilterSimple : public CTraceFilter
{
public:
	// It does have a base, but we'll never network anything below here..
	//DECLARE_CLASS_NOBASE(CTraceFilterSimple);

	CTraceFilterSimple(const IHandleEntity* passentity, int collisionGroup, ShouldHitFunc_t pExtraShouldHitCheckFn = NULL);
	virtual bool ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask);
	virtual void SetPassEntity(const IHandleEntity* pPassEntity) { m_pPassEnt = pPassEntity; }
	virtual void SetCollisionGroup(int iCollisionGroup) { m_collisionGroup = iCollisionGroup; }

	const IHandleEntity* GetPassEntity(void) { return m_pPassEnt; }

private:
	const IHandleEntity* m_pPassEnt;
	int m_collisionGroup;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;

};

class CTraceFilterSkipTwoEntities : public CTraceFilterSimple
{
public:
	// It does have a base, but we'll never network anything below here..
	//DECLARE_CLASS(CTraceFilterSkipTwoEntities, CTraceFilterSimple);

	CTraceFilterSkipTwoEntities(const IHandleEntity* passentity, const IHandleEntity* passentity2, int collisionGroup);
	virtual bool ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask);
	virtual void SetPassEntity2(const IHandleEntity* pPassEntity2) { m_pPassEnt2 = pPassEntity2; }

private:
	const IHandleEntity* m_pPassEnt2;
};

class CTraceFilterSimpleList : public CTraceFilterSimple
{
public:
	CTraceFilterSimpleList(int collisionGroup);
	virtual bool ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask);

	void	AddEntityToIgnore(IHandleEntity* pEntity);
protected:
	CUtlVector<IHandleEntity*>	m_PassEntities;
};

class CTraceFilterOnlyNPCsAndPlayer : public CTraceFilterSimple
{
public:
	CTraceFilterOnlyNPCsAndPlayer(const IHandleEntity* passentity, int collisionGroup)
		: CTraceFilterSimple(passentity, collisionGroup)
	{
	}

	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_ENTITIES_ONLY;
	}

	virtual bool ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask);
};

class CTraceFilterNoNPCsOrPlayer : public CTraceFilterSimple
{
public:
	CTraceFilterNoNPCsOrPlayer(const IHandleEntity* passentity, int collisionGroup)
		: CTraceFilterSimple(passentity, collisionGroup)
	{
	}

	virtual bool ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask);
};

class CTraceFilterSkipClassname : public CTraceFilterSimple
{
public:
	CTraceFilterSkipClassname(const IHandleEntity* passentity, const char* pchClassname, int collisionGroup);
	virtual bool ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask);

private:

	const char* m_pchClassname;
};

class CTraceFilterSkipTwoClassnames : public CTraceFilterSkipClassname
{
public:
	// It does have a base, but we'll never network anything below here..
	//DECLARE_CLASS(CTraceFilterSkipTwoClassnames, CTraceFilterSkipClassname);

	CTraceFilterSkipTwoClassnames(const IHandleEntity* passentity, const char* pchClassname, const char* pchClassname2, int collisionGroup);
	virtual bool ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask);

private:
	const char* m_pchClassname2;
};

class CTraceFilterSimpleClassnameList : public CTraceFilterSimple
{
public:
	CTraceFilterSimpleClassnameList(const IHandleEntity* passentity, int collisionGroup);
	virtual bool ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask);

	void	AddClassnameToIgnore(const char* pchClassname);
private:
	CUtlVector<const char*>	m_PassClassnames;
};

class CTraceFilterChain : public CTraceFilter
{
public:
	CTraceFilterChain(ITraceFilter* pTraceFilter1, ITraceFilter* pTraceFilter2);
	virtual bool ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask);

private:
	ITraceFilter* m_pTraceFilter1;
	ITraceFilter* m_pTraceFilter2;
};

inline void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,
	const IHandleEntity* ignore, int collisionGroup, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	CTraceFilterSimple traceFilter(ignore, collisionGroup);

	I::EngineTrace->TraceRay(ray, mask, &traceFilter, ptr);

	if (g_bVisualizeTraces)
		I::DebugOverlay->AddLineOverlay(ptr->startpos, ptr->endpos, 255, 0, 0, true, 0.01f);
}

inline void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,
	ITraceFilter* pFilter, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);

	I::EngineTrace->TraceRay(ray, mask, pFilter, ptr);

	if (g_bVisualizeTraces)
		I::DebugOverlay->AddLineOverlay(ptr->startpos, ptr->endpos, 255, 0, 0, true, 0.01f);
}

inline void UTIL_TraceLineFilterEntity(C_BaseEntity* pEntity, const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, trace_t* ptr)
{
	CTraceFilterHitAll traceFilter(pEntity);
	UTIL_TraceLine(vecAbsStart, vecAbsEnd, mask, &traceFilter, ptr);
}

inline void UTIL_TraceHull(const Vector& vecAbsStart, const Vector& vecAbsEnd, const Vector& hullMin,
	const Vector& hullMax, unsigned int mask, const IHandleEntity* ignore,
	int collisionGroup, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd, hullMin, hullMax);
	CTraceFilterSimple traceFilter(ignore, collisionGroup);

	I::EngineTrace->TraceRay(ray, mask, &traceFilter, ptr);

	if (g_bVisualizeTraces)
		I::DebugOverlay->AddLineOverlay(ptr->startpos, ptr->endpos, 255, 0, 0, true, 0.01f);
}

inline void UTIL_TraceHull(const Vector& vecAbsStart, const Vector& vecAbsEnd, const Vector& hullMin,
	const Vector& hullMax, unsigned int mask, ITraceFilter* pFilter, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd, hullMin, hullMax);

	I::EngineTrace->TraceRay(ray, mask, pFilter, ptr);

	if (g_bVisualizeTraces)
		I::DebugOverlay->AddLineOverlay(ptr->startpos, ptr->endpos, 255, 0, 0, true, 0.01f);
}

inline void UTIL_TraceRay(const Ray_t& ray, unsigned int mask,
	const IHandleEntity* ignore, int collisionGroup, trace_t* ptr, ShouldHitFunc_t pExtraShouldHitCheckFn = NULL)
{
	CTraceFilterSimple traceFilter(ignore, collisionGroup, pExtraShouldHitCheckFn);

	I::EngineTrace->TraceRay(ray, mask, &traceFilter, ptr);

	if (g_bVisualizeTraces)
		I::DebugOverlay->AddLineOverlay(ptr->startpos, ptr->endpos, 255, 0, 0, true, 0.01f);
}

// Sweeps a particular entity through the world
void UTIL_TraceEntity(C_BaseEntity* pEntity, const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, trace_t* ptr);
void UTIL_TraceEntity(C_BaseEntity* pEntity, const Vector& vecAbsStart, const Vector& vecAbsEnd,
	unsigned int mask, ITraceFilter* pFilter, trace_t* ptr);
void UTIL_TraceEntity(C_BaseEntity* pEntity, const Vector& vecAbsStart, const Vector& vecAbsEnd,
	unsigned int mask, const IHandleEntity* ignore, int collisionGroup, trace_t* ptr);

bool UTIL_EntityHasMatchingRootParent(C_BaseEntity* pRootParent, C_BaseEntity* pEntity);

inline int UTIL_PointContents(const Vector& vec)
{
	return I::EngineTrace->GetPointContents(vec);
}

// Sweeps against a particular model, using collision rules 
void UTIL_TraceModel(const Vector& vecStart, const Vector& vecEnd, const Vector& hullMin,
	const Vector& hullMax, C_BaseEntity* pentModel, int collisionGroup, trace_t* ptr);

void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr);

// Particle effect tracer
//void		UTIL_ParticleTracer(const char* pszTracerEffectName, const Vector& vecStart, const Vector& vecEnd, int iEntIndex = 0, int iAttachment = 0, bool bWhiz = false);

// Old style, non-particle system, tracers
//void		UTIL_Tracer(const Vector& vecStart, const Vector& vecEnd, int iEntIndex = 0, int iAttachment = TRACER_DONT_USE_ATTACHMENT, float flVelocity = 0, bool bWhiz = false, const char* pCustomTracerName = NULL, int iParticleID = 0);

bool		UTIL_IsLowViolence(void);
bool		UTIL_ShouldShowBlood(int bloodColor);
//void		UTIL_BloodDrips(const Vector& origin, const Vector& direction, int color, int amount);

//void		UTIL_BloodImpact(const Vector& pos, const Vector& dir, int color, int amount);
//void		UTIL_BloodDecalTrace(trace_t* pTrace, int bloodColor);
//void		UTIL_DecalTrace(trace_t* pTrace, char const* decalName);
bool		UTIL_IsSpaceEmpty(C_BaseEntity* pMainEnt, const Vector& vMin, const Vector& vMax);

void		UTIL_StringToVector(float* pVector, const char* pString);
void		UTIL_StringToIntArray(int* pVector, int count, const char* pString);
void		UTIL_StringToFloatArray(float* pVector, int count, const char* pString);
void		UTIL_StringToColor32(color32* color, const char* pString);

class C_TFPlayer;

IClientEntity* UTIL_EntityByIndex(int entindex);
C_BasePlayer*  UTIL_PlayerByIndex(int entindex);
C_TFPlayer*    UTIL_TFPlayerByIndex(int entindex);

// Helper for use with console commands and the like.
// Returns NULL if not found or if the provided arg would match multiple players.
// Currently accepts, in descending priority:
//  - Formatted SteamID ([U:1:1234])
//  - SteamID64 (76561197989728462)
//  - Legacy SteamID (STEAM_0:1:1234)
//  - UserID preceded by a pound (#4)
//  - Partial name match (if unique)
//  - UserID not preceded by a pound*
//
// *Does not count as ambiguous with higher priority items
//C_BasePlayer* UTIL_PlayerByCommandArg(const char* arg);

C_BasePlayer* UTIL_PlayerByUserId(int userID);
C_BasePlayer* UTIL_PlayerByName(const char* name); // not case sensitive
// Finds a player who has this non-ambiguous substring.  Also not case sensitive.
C_BasePlayer* UTIL_PlayerByPartialName(const char* name);

// decodes a buffer using a 64bit ICE key (inplace)
void		UTIL_DecodeICE(unsigned char* buffer, int size, const unsigned char* key);

//--------------------------------------------------------------------------------------------------------------
/**
 * Given a position and a ray, return the shortest distance between the two.
 * If 'pos' is beyond either end of the ray, the returned distance is negated.
 */
inline float DistanceToRay(const Vector& pos, const Vector& rayStart, const Vector& rayEnd, float* along = NULL, Vector* pointOnRay = NULL)
{
	Vector to = pos - rayStart;
	Vector dir = rayEnd - rayStart;
	float length = dir.NormalizeInPlace();

	float rangeAlong = DotProduct(dir, to);
	if (along)
	{
		*along = rangeAlong;
	}

	float range;

	if (rangeAlong < 0.0f)
	{
		// off start point
		range = -(pos - rayStart).Length();

		if (pointOnRay)
		{
			*pointOnRay = rayStart;
		}
	}
	else if (rangeAlong > length)
	{
		// off end point
		range = -(pos - rayEnd).Length();

		if (pointOnRay)
		{
			*pointOnRay = rayEnd;
		}
	}
	else // within ray bounds
	{
		Vector onRay = rayStart + rangeAlong * dir;
		range = (pos - onRay).Length();

		if (pointOnRay)
		{
			*pointOnRay = onRay;
		}
	}

	return range;
}

//--------------------------------------------------------------------------------------------------------------
/**
* Macro for creating an interface that when inherited from automatically maintains a list of instances
* that inherit from that interface.
*/

// interface for entities that want to a auto maintained global list
#define DECLARE_AUTO_LIST( interfaceName ) \
	class interfaceName; \
	abstract_class interfaceName \
	{ \
	public: \
		interfaceName( bool bAutoAdd = true ); \
		virtual ~interfaceName(); \
		static void AddToAutoList( interfaceName *pElement ) { m_##interfaceName##AutoList.AddToTail( pElement ); } \
		static void RemoveFromAutoList( interfaceName *pElement ) { m_##interfaceName##AutoList.FindAndFastRemove( pElement ); } \
		static const CUtlVector< interfaceName* >& AutoList( void ) { return m_##interfaceName##AutoList; } \
	private: \
		static CUtlVector< interfaceName* > m_##interfaceName##AutoList; \
	};

// Creates the auto add/remove constructor/destructor...
// Pass false to the constructor to not auto add
#define IMPLEMENT_AUTO_LIST( interfaceName ) \
	CUtlVector< class interfaceName* > interfaceName::m_##interfaceName##AutoList; \
	interfaceName::interfaceName( bool bAutoAdd ) \
	{ \
		if ( bAutoAdd ) \
		{ \
			AddToAutoList( this ); \
		} \
	} \
	interfaceName::~interfaceName() \
	{ \
		RemoveFromAutoList( this ); \
	}

//--------------------------------------------------------------------------------------------------------------
// You can use this if you need an autolist without an extra interface type involved.
// To use this, just inherit (class Mine : public TAutoList<Mine> {)
template< class T >
class TAutoList
{
public:
	typedef CUtlVector< T* > AutoListType;

	static AutoListType& GetAutoList()
	{
		return m_autolist;
	}

protected:
	TAutoList()
	{
		m_autolist.AddToTail(static_cast<T*>(this));
	}

	virtual ~TAutoList()
	{
		m_autolist.FindAndFastRemove(static_cast<T*>(this));
	}

private:
	static AutoListType m_autolist;
};

template< class T >
CUtlVector< T* > TAutoList< T >::m_autolist;

//--------------------------------------------------------------------------------------------------------------
/**
 * Simple class for tracking intervals of game time.
 * Upon creation, the timer is invalidated.  To measure time intervals, start the timer via Start().
 */
class IntervalTimer
{
public:
	IntervalTimer(void)
	{
		m_timestamp = -1.0f;
	}

	void Reset(void)
	{
		m_timestamp = Now();
	}

	void Start(void)
	{
		m_timestamp = Now();
	}

	void Invalidate(void)
	{
		m_timestamp = -1.0f;
	}

	bool HasStarted(void) const
	{
		return (m_timestamp > 0.0f);
	}

	/// if not started, elapsed time is very large
	float GetElapsedTime(void) const
	{
		return (HasStarted()) ? (Now() - m_timestamp) : 99999.9f;
	}

	bool IsLessThen(float duration) const
	{
		return (Now() - m_timestamp < duration) ? true : false;
	}

	bool IsGreaterThen(float duration) const
	{
		return (Now() - m_timestamp > duration) ? true : false;
	}

private:
	float m_timestamp;
	float Now(void) const;		// work-around since client header doesn't like inlined gpGlobals->curtime
};


//--------------------------------------------------------------------------------------------------------------
/**
 * Simple class for counting down a short interval of time.
 * Upon creation, the timer is invalidated.  Invalidated countdown timers are considered to have elapsed.
 */
class CountdownTimer
{
public:
	CountdownTimer(void)
	{
		m_timestamp = -1.0f;
		m_duration = 0.0f;
	}

	void Reset(void)
	{
		m_timestamp = Now() + m_duration;
	}

	void Start(float duration)
	{
		m_timestamp = Now() + duration;
		m_duration = duration;
	}

	void Invalidate(void)
	{
		m_timestamp = -1.0f;
	}

	bool HasStarted(void) const
	{
		return (m_timestamp > 0.0f);
	}

	bool IsElapsed(void) const
	{
		return (Now() > m_timestamp);
	}

	float GetElapsedTime(void) const
	{
		return Now() - m_timestamp + m_duration;
	}

	float GetRemainingTime(void) const
	{
		return (m_timestamp - Now());
	}

	/// return original countdown time
	float GetCountdownDuration(void) const
	{
		return (m_timestamp > 0.0f) ? m_duration : 0.0f;
	}

private:
	float m_duration;
	float m_timestamp;
	virtual float Now(void) const;		// work-around since client header doesn't like inlined gpGlobals->curtime
};

class RealTimeCountdownTimer : public CountdownTimer
{
	virtual float Now(void) const OVERRIDE
	{
		return Plat_FloatTime();
	}
};

//char* ReadAndAllocStringValue(KeyValues* pSub, const char* pName, const char* pFilename = NULL);

int UTIL_StringFieldToInt(const char* szValue, const char** pValueStrings, int iNumStrings);

//-----------------------------------------------------------------------------
// Holidays
//-----------------------------------------------------------------------------

// Used at level change and round start to re-calculate which holiday is active
void				UTIL_CalculateHolidays();

bool				UTIL_IsHolidayActive( /*EHoliday*/ int eHoliday);
/*EHoliday*/ int	UTIL_GetHolidayForString(const char* pszHolidayName);

// This will return the first active holiday string it can find. In the case of multiple
// holidays overlapping, the list order will act as priority.
const char* UTIL_GetActiveHolidayString();

//const char* UTIL_GetRandomSoundFromEntry(const char* pszEntryName);

/// Clamp and round float vals to int.  The values are in the 0...255 range.
Color FloatRGBAToColor(float r, float g, float b, float a);
float LerpFloat(float x0, float x1, float t);
Color LerpColor(const Color& c0, const Color& c1, float t);

// Global econ-level helper functionality.
//EUniverse GetUniverse();

#endif // UTIL_SHARED_H