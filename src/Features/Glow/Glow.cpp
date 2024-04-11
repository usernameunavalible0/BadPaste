#include "Glow.h"
#include "../Vars.h"

bool CFeatures_Glow::Initialize()
{
    KeyValues* pMatWireframe = new KeyValues("UnlitGeneric");
    pMatWireframe->SetString("$basetexture", "white");
    pMatWireframe->SetString("$wireframe", "1");
    pMatWireframe->SetString("$color2", "[2 2 2]");
    m_pMatWireframe = I::MaterialSystem->CreateMaterial("GlowMaterialWireframe", pMatWireframe);

    KeyValues* pMatBlurX = new KeyValues("BlurFilterX");
    pMatBlurX->SetString("$basetexture", "GlowRTQuarterSize0");
    m_pMatBlurX = I::MaterialSystem->CreateMaterial("GlowMaterialBlurX", pMatBlurX);

    KeyValues* pMatBlurY = new KeyValues("BlurFilterY");
    pMatBlurY->SetString("$basetexture", "GlowRTQuarterSize1");
    m_pMatBlurY = I::MaterialSystem->CreateMaterial("GlowMaterialBlurY", pMatBlurY);

    KeyValues* pMatHaloAddToScreen = new KeyValues("UnlitGeneric");
    pMatHaloAddToScreen->SetString("$basetexture", "GlowRTQuarterSize0");
    pMatHaloAddToScreen->SetString("$additive", "1");
    m_pMatHaloAddToScreen = I::MaterialSystem->CreateMaterial("GlowMaterialHaloAddToScreen", pMatHaloAddToScreen);

    m_pMatGlowColor = I::MaterialSystem->FindMaterial("dev/glow_color", TEXTURE_GROUP_OTHER, true);
    m_pMatDownsample = I::MaterialSystem->FindMaterial("dev/glow_downsample", TEXTURE_GROUP_OTHER, true);

    m_pRtFullFrame = I::MaterialSystem->FindTexture("_rt_FullFrameFB", TEXTURE_GROUP_RENDER_TARGET);
    m_pRtQuarterSize0 = I::MaterialSystem->CreateNamedRenderTargetTextureEx("GlowRTQuarterSize0", m_pRtFullFrame->GetActualWidth(), m_pRtFullFrame->GetActualHeight(),
        RT_SIZE_LITERAL, IMAGE_FORMAT_RGB888, MATERIAL_RT_DEPTH_SHARED, TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT | TEXTUREFLAGS_EIGHTBITALPHA, CREATERENDERTARGETFLAGS_HDR);
    m_pRtQuarterSize0->IncrementReferenceCount();
    m_pRtQuarterSize1 = I::MaterialSystem->CreateNamedRenderTargetTextureEx("GlowRTQuarterSize1", m_pRtFullFrame->GetActualWidth(), m_pRtFullFrame->GetActualHeight(),
        RT_SIZE_LITERAL, IMAGE_FORMAT_RGB888, MATERIAL_RT_DEPTH_SHARED, TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT | TEXTUREFLAGS_EIGHTBITALPHA, CREATERENDERTARGETFLAGS_HDR);
    m_pRtQuarterSize1->IncrementReferenceCount();

    return true;
}

static void SetRenderTargetAndViewPort(ITexture* rt, int w, int h)
{
    IMatRenderContext* pRenderContext = I::MaterialSystem->GetRenderContext();
    pRenderContext->SetRenderTarget(rt);
    pRenderContext->Viewport(0, 0, w, h);
}

void CFeatures_Glow::Render()
{
    m_bIsDrawing = false;

    if (!Vars::Glow::Enabled.m_Var || g_Globals.m_bIsGameUIVisible)
        return;

    IMatRenderContext* pRenderContext = I::MaterialSystem->GetRenderContext();

    if (!pRenderContext)
        return;

    // Setup
    {
        ShaderStencilState_t stencilState;
        stencilState.m_bEnable = true;
        stencilState.m_nReferenceValue = 1;
        stencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
        stencilState.m_PassOp = STENCILOPERATION_REPLACE;
        stencilState.m_FailOp = STENCILOPERATION_KEEP;
        stencilState.m_ZFailOp = STENCILOPERATION_REPLACE;
        stencilState.SetStencilState(pRenderContext);
    }

    ShaderStencilState_t stencilStateDisable;
    stencilStateDisable.m_bEnable = false;

    // Draw!!!
    {
        m_bIsDrawing = true;

        I::ModelRender->ForcedMaterialOverride(m_pMatGlowColor);
        float flSavedBlend = I::RenderView->GetBlend();
        I::RenderView->SetBlend(0.0f);
        pRenderContext->OverrideDepthEnable(true, false);

        for (IClientEntity* pEntity : G::EntityCache.GetGroup(Vars::Glow::Players::IgnoreTeam.m_Var ? EEntGroup::PLAYERS_ENEMIES : EEntGroup::PLAYERS_ALL))
            DrawModel(pEntity);

        pRenderContext->OverrideDepthEnable(false, false);
        I::RenderView->SetBlend(flSavedBlend);
        stencilStateDisable.SetStencilState(pRenderContext);
        I::ModelRender->ForcedMaterialOverride(nullptr);

        pRenderContext->PushRenderTargetAndViewport();
        {
            Vector vOrigColor;
            I::RenderView->GetColorModulation(vOrigColor.Base());
            float flOrigBlend = I::RenderView->GetBlend();

            pRenderContext->SetRenderTarget(m_pRtQuarterSize0);
            pRenderContext->Viewport(0, 0, g_Globals.m_nScreenWidht, g_Globals.m_nScreenHeight);
            pRenderContext->ClearColor3ub(0, 0, 0);
            pRenderContext->ClearBuffers(true, false, false);

            I::ModelRender->ForcedMaterialOverride(m_pMatGlowColor);

            ShaderStencilState_t stencilState;
            stencilState.m_bEnable = false;
            stencilState.m_nReferenceValue = 0;
            stencilState.m_nTestMask = 0xFF;
            stencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
            stencilState.m_PassOp = STENCILOPERATION_KEEP;
            stencilState.m_FailOp = STENCILOPERATION_KEEP;
            stencilState.m_ZFailOp = STENCILOPERATION_KEEP;
            stencilState.SetStencilState(pRenderContext);

            if (Vars::Glow::Players::GlowColorMode.m_Var == 1)
            {
                I::RenderView->SetBlend(1.0f);
                // FIXME: Just use floats instead of calculating between 0-255 and turning into float
                Vector Rainbow = Vector(
                    static_cast<byte>(floor(sin(I::GlobalVars->curtime) * 127.0f + 128.0f)) / 255.f,
                    static_cast<byte>(floor(sin(I::GlobalVars->curtime + 2.0f) * 127.0f + 128.0f)) / 255.f,
                    static_cast<byte>(floor(sin(I::GlobalVars->curtime + 4.0f) * 127.0f + 128.0f)) / 255.f);
                I::RenderView->SetColorModulation(&Rainbow[0]);
            }
            else if (Vars::Glow::Players::GlowColorMode.m_Var == 0)
            {
                I::RenderView->SetBlend(Vars::Glow::Players::GlowColor.a() / 255.f);
                Vector vGlowColor = Vector(Vars::Glow::Players::GlowColor.r() / 255.f, Vars::Glow::Players::GlowColor.g() / 255.f, Vars::Glow::Players::GlowColor.b() / 255.f) * (Vars::Glow::Players::GlowColor.a() / 255.f);
                I::RenderView->SetColorModulation(&vGlowColor[0]);
            }

            for (IClientEntity* pEntity : G::EntityCache.GetGroup(Vars::Glow::Players::IgnoreTeam.m_Var ? EEntGroup::PLAYERS_ENEMIES : EEntGroup::PLAYERS_ALL))
            {
                C_BaseEntity* pBaseEntity = pEntity->As<C_BaseEntity*>();
                if (Vars::Glow::Players::GlowColorMode.m_Var == 2 && pBaseEntity)
                {
                    I::RenderView->SetBlend(1.0f);
                    Color clrHealth = G::Util.GetHealthColor(pBaseEntity->GetHealth(), pBaseEntity->GetMaxHealth());
                    Vector vColorHealth = Vector(clrHealth.r() / 255.f, clrHealth.g() / 255.f, clrHealth.b() / 255.f);
                    I::RenderView->SetColorModulation(&vColorHealth[0]);
                }

                DrawModel(pEntity);
            }

            I::ModelRender->ForcedMaterialOverride(nullptr);
            I::RenderView->SetColorModulation(vOrigColor.Base());
            I::RenderView->SetBlend(flOrigBlend);

            stencilStateDisable.SetStencilState(pRenderContext);
        }
        pRenderContext->PopRenderTargetAndViewport();

        pRenderContext->PushRenderTargetAndViewport();
        {
            static bool s_bFirstPass = true;

            // Get viewport
            int nSrcWidth = g_Globals.m_nScreenWidht;
            int nSrcHeight = g_Globals.m_nScreenHeight;
            int nViewportX, nViewportY, nViewportWidth, nViewportHeight;
            pRenderContext->GetViewport(nViewportX, nViewportY, nViewportWidth, nViewportHeight);

            //============================//
            // Guassian blur x rt0 to rt1 //
            //============================//

            // First clear the full target to black if we're not going to touch every pixel
            if (s_bFirstPass || (m_pRtQuarterSize1->GetActualWidth() != (g_Globals.m_nScreenWidht)) || (m_pRtQuarterSize1->GetActualHeight() != (g_Globals.m_nScreenHeight)))
            {
                // On the first render, this viewport may require clearing
                s_bFirstPass = false;
                SetRenderTargetAndViewPort(m_pRtQuarterSize1, m_pRtQuarterSize1->GetActualWidth(), m_pRtQuarterSize1->GetActualHeight());
                pRenderContext->ClearColor3ub(0, 0, 0);
                pRenderContext->ClearBuffers(true, false, false);
            }

            // Set the viewport
            SetRenderTargetAndViewPort(m_pRtQuarterSize1, g_Globals.m_nScreenWidht, g_Globals.m_nScreenHeight);

            pRenderContext->DrawScreenSpaceRectangle(m_pMatBlurX, 0, 0, nSrcWidth, nSrcHeight,
                0, 0, nSrcWidth - 1, nSrcHeight - 1,
                m_pRtQuarterSize0->GetActualWidth(), m_pRtQuarterSize0->GetActualHeight());

            //============================//
            // Gaussian blur y rt1 to rt0 //
            //============================//
            SetRenderTargetAndViewPort(m_pRtQuarterSize0, g_Globals.m_nScreenWidht, g_Globals.m_nScreenHeight);
            IMaterialVar* pBloomAmountVar = m_pMatBlurY->FindVar("$bloomamount", NULL);
            pBloomAmountVar->SetFloatValue(Vars::Glow::Players::BloomAmount.m_Var);
            pRenderContext->DrawScreenSpaceRectangle(m_pMatBlurY, 0, 0, nSrcWidth, nSrcHeight,
                0, 0, nSrcWidth - 1, nSrcHeight - 1,
                m_pRtQuarterSize1->GetActualWidth(), m_pRtQuarterSize1->GetActualHeight());
        }
        pRenderContext->PopRenderTargetAndViewport();

        // Set stencil state
        ShaderStencilState_t stencilState;
        stencilState.m_bEnable = true;
        stencilState.m_nWriteMask = 0x0; // We're not changing stencil
        stencilState.m_nTestMask = 0xFF;
        stencilState.m_nReferenceValue = 0x0;
        stencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_EQUAL;
        stencilState.m_PassOp = STENCILOPERATION_KEEP;
        stencilState.m_FailOp = STENCILOPERATION_KEEP;
        stencilState.m_ZFailOp = STENCILOPERATION_KEEP;
        stencilState.SetStencilState(pRenderContext);

        int nViewportX, nViewportY, nViewportWidth, nViewportHeight;
        pRenderContext->GetViewport(nViewportX, nViewportY, nViewportWidth, nViewportHeight);

        pRenderContext->DrawScreenSpaceRectangle(m_pMatHaloAddToScreen, 0, 0, nViewportWidth, nViewportHeight,
            0.0f, -0.5f, g_Globals.m_nScreenWidht - 1, g_Globals.m_nScreenHeight - 1,
            m_pRtQuarterSize1->GetActualWidth(), m_pRtQuarterSize1->GetActualHeight());

        m_bIsDrawing = false;
    }

    // Finish
    {
        stencilStateDisable.SetStencilState(pRenderContext);
    }
}

void CFeatures_Glow::DrawModel(IClientEntity* pClientEntity)
{
    C_BaseEntity* pEntity = pClientEntity->As<C_BaseEntity*>();

    if (pEntity)
    {
        pEntity->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
        C_BaseEntity* pAttachment = pEntity->FirstMoveChild();

        while ( pAttachment != NULL )
        {
            if ( pAttachment->ShouldDraw() )
            {
                pAttachment->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
            }
            pAttachment = pAttachment->NextMovePeer();
        }
    }
}
