#pragma once
#include "../../SDK/SDK.h"

class CFeatures_Glow
{
private:
	IMaterial* m_pMatWireframe;

public:
	bool Initialize();
	void Render();

	bool m_bIsDrawing;

private:
	void DrawModel(IClientEntity* pEntity);

	void SetDrawInfo(IMaterial* pMaterial, Color clr, float flAlpha, bool bDepth)
	{
		I::ModelRender->ForcedMaterialOverride(pMaterial);
		float fColor[3]; clr.AsFloat(fColor);
		I::RenderView->SetColorModulation(fColor);
		I::RenderView->SetBlend(flAlpha);

		if (IMatRenderContext* pRenderContext = I::MaterialSystem->GetRenderContext())
			pRenderContext->DepthRange(0.0f, bDepth ? 1.0f : 0.2f);
	}

	void ResetDrawInfo()
	{
		I::ModelRender->ForcedMaterialOverride(nullptr);
		float fClear[3] = { 1.f, 1.f, 1.f };
		I::RenderView->SetColorModulation(fClear);
		I::RenderView->SetBlend(1.0f);

		if (IMatRenderContext* pRenderContext = I::MaterialSystem->GetRenderContext())
			pRenderContext->DepthRange(0.0f, 1.0f);
	}
};

namespace F { inline CFeatures_Glow Glow; }