#pragma once

#include "../../SDK/SDK.h"

class CFeatures_ESP
{
public:
	void Render(C_TFPlayer* pLocal);

public:
	void LevelInitPostEntity();

	bool IsHealth(const int nModelIndex);
	bool IsAmmo(const int nModelIndex);

private:
	CUtlVector<int> m_vecHealth;
	CUtlVector<int> m_vecAmmo;

private:
	bool GetDynamicBounds(C_BaseEntity* pEntity, int& x, int& y, int& w, int& h);
};

namespace F { inline CFeatures_ESP ESP; }