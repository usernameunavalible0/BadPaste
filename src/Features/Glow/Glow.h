#pragma once
#include "../../SDK/SDK.h"

class CFeatures_Glow
{
private:
	IMaterial* m_pMatWireframe;
	IMaterial* m_pMatGlowColor;
	IMaterial* m_pMatDownsample;
	IMaterial* m_pMatBlurX;
	IMaterial* m_pMatBlurY;
	IMaterial* m_pMatHaloAddToScreen;

	ITexture* m_pRtFullFrame;
	ITexture* m_pRtQuarterSize0;
	ITexture* m_pRtQuarterSize1;

public:
	bool Initialize();
	void OnScreenSizeChanged();
	void Render();

	bool IsDrawing() const { return m_bIsDrawing; }
	void SetView(const CViewSetup& viewRender) { m_viewRender = viewRender; }
	void SetFrustum(VPlane* Frustum) { m_Frustum = Frustum; }

private:
	bool m_bIsDrawing;
	CViewSetup m_viewRender;
	VPlane* m_Frustum;

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
		static const float fClear[3] = { 1.f, 1.f, 1.f };
		I::RenderView->SetColorModulation(fClear);
		I::RenderView->SetBlend(1.0f);

		if (IMatRenderContext* pRenderContext = I::MaterialSystem->GetRenderContext())
			pRenderContext->DepthRange(0.0f, 1.0f);
	}
};

namespace F { inline CFeatures_Glow Glow; }