#pragma once
#include "../../SDK/SDK.h"

class CFeatures_Chams
{
private:
	IMaterial* m_pMatTest;
	IMaterial* m_pMatGlow;
	IMaterial* m_pMatFresnel;
	IMaterial* m_pMatShaded;
	IMaterial* m_pMatToxic;

public:
	bool Initialize();
	bool Render(void* ecx, void* edx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
};

namespace F { inline CFeatures_Chams Chams; }