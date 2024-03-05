#include "Glow.h"
#include "../Vars.h"

bool CFeatures_Glow::Initialize()
{
    KeyValues* pMatWireframe = new KeyValues("UnlitGeneric");
    pMatWireframe->SetString("$basetexture", "white");
    pMatWireframe->SetString("$wireframe", "1");
    pMatWireframe->SetString("$color2", "[2 2 2]");
    m_pMatWireframe = I::MaterialSystem->CreateMaterial("GlowMaterialWireframe", pMatWireframe);

    return true;
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
        if (Vars::Glow::Players::Material.m_Var == 1) // Wireframe requires extra steps
        {
            stencilState.m_nTestMask = 0x0;
            stencilState.m_nWriteMask = 0xFF;
        }
        stencilState.SetStencilState(pRenderContext);
    }

    // Draw!!!
    {
        m_bIsDrawing = true;

        if (Vars::Glow::Players::Enabled.m_Var)
        {
            if (Vars::Glow::Players::Material.m_Var == 1) // Wireframe
            {
                SetDrawInfo(nullptr, {}, 0.0f, true);
                for (IClientEntity* pEntity : G::EntityCache.GetGroup(Vars::Glow::Players::IgnoreTeam.m_Var ? EEntGroup::PLAYERS_ENEMIES : EEntGroup::PLAYERS_ALL))
                    DrawModel(pEntity);
                ResetDrawInfo();

                ShaderStencilState_t stencilState;
                stencilState.m_bEnable = true;
                stencilState.m_nWriteMask = 0x0;
                stencilState.m_nTestMask = 0xFF;
                stencilState.m_nReferenceValue = 0x0;
                stencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_EQUAL;
                stencilState.m_PassOp = STENCILOPERATION_KEEP;
                stencilState.m_FailOp = STENCILOPERATION_KEEP;
                stencilState.m_ZFailOp = STENCILOPERATION_KEEP;
                stencilState.SetStencilState(pRenderContext);
            }

            SetDrawInfo(m_pMatWireframe, Color(255, 255, 255, 255), 1.0f, false);
            for (IClientEntity* pEntity : G::EntityCache.GetGroup(Vars::Glow::Players::IgnoreTeam.m_Var ? EEntGroup::PLAYERS_ENEMIES : EEntGroup::PLAYERS_ALL))
                DrawModel(pEntity);
            ResetDrawInfo();
        }

        m_bIsDrawing = false;
    }

    // Finish
    {
        ShaderStencilState_t stencilStateDisable;
        stencilStateDisable.m_bEnable = false;
        stencilStateDisable.SetStencilState(pRenderContext);
    }
}

void CFeatures_Glow::DrawModel(IClientEntity* pClientEntity)
{
    C_BaseEntity* pEntity = pClientEntity->As<C_BaseEntity*>();

    if (pEntity)
    {
        pEntity->DrawModel(STUDIO_RENDER);
        C_BaseEntity* pAttachment = pEntity->FirstMoveChild();

        while ( pAttachment != NULL )
        {
            if ( pAttachment->ShouldDraw() )
            {
                pAttachment->DrawModel(STUDIO_RENDER);
            }
            pAttachment = pAttachment->NextMovePeer();
        }
    }
}
