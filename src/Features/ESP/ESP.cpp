#include "ESP.h"

#include "../Vars.h"

void CFeatures_ESP::Render(C_TFPlayer* pLocal)
{
	if (!Vars::ESP::Enabled.m_Var)
		return;

	int x, y, w, h;
	for (int n = 1; n < (g_Globals.m_nMaxEntities + 1); n++)
	{
		if (n == g_Globals.m_nLocalIndex)
			continue;

		IClientEntity* pEntity = UTIL_EntityByIndex(n);

		if (!pEntity || pEntity->IsDormant())
			continue;

		ClientClass* pCC = pEntity->GetClientClass();

		if (!pCC)
			continue;

		switch (pCC->GetTFClientClass())
		{
			case ETFClientClass::CTFPlayer:
			{
				if (!Vars::ESP::Players::Enabled.m_Var)
					break;

				C_TFPlayer* pPlayer = pEntity->As<C_TFPlayer*>();

				if (pPlayer->deadflag() || (Vars::ESP::Players::IgnoreTeam.m_Var && pPlayer->InLocalTeam()))
					break;

				if (!GetDynamicBounds(pPlayer, x, y, w, h))
					break;

				const int nHealth = pPlayer->GetHealth();
				const int nMaxHealth = pPlayer->GetMaxHealth();

				const Color clrHealth = G::Util.GetHealthColor(nHealth, nMaxHealth);
				const Color clrTeam = G::Util.GetTeamColor(pPlayer->GetTeamNumber());

				const int nDrawX = x + (w + 3);
				int nDrawY = y;

				player_info_t pi;
				if (Vars::ESP::Players::Name.m_Var && I::EngineClient->GetPlayerInfo(n, &pi))
				{
					uchar16 szName[MAX_PLAYER_NAME_LENGTH];
					Q_UTF8ToUTF16(UTIL_SafeName(pi.name), szName, MAX_PLAYER_NAME_LENGTH);

					G::Draw.String(EFonts::ESP_NAME, nDrawX, nDrawY, clrTeam, TXT_CENTERY, szName);
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP_NAME);
				}

				if (Vars::ESP::Players::HealthText.m_Var)
				{
					G::Draw.String(EFonts::ESP, nDrawX, nDrawY, clrHealth, TXT_CENTERY, L"%i / %ihp", nHealth, nMaxHealth);
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP);
				}

				if (Vars::ESP::Players::ActiveWeapon.m_Var)
				{
					C_BaseCombatWeapon* pWeapon = pPlayer->GetActiveWeapon();

					if (pWeapon)
						G::Draw.String(EFonts::ESP_WEAPON, x + (w / 2), (y + h), COLOR_GREY, TXT_CENTERXY, pWeapon->GetName() + 10); //tf_weapon_
				}

				if (Vars::ESP::Players::Box.m_Var)
				{
					G::Draw.OutlinedRect(x, y, w, h, clrTeam);

					if (Vars::ESP::Outlines.m_Var)
					{
						//Outline
						G::Draw.OutlinedRect(x - 1, y - 1, w + 2, h + 2, COLOR_BLACK);

						//Inline
						G::Draw.OutlinedRect(x + 1, y + 1, w - 2, h - 2, COLOR_BLACK);
					}
				}

				if (Vars::ESP::Players::HealthBar.m_Var)
				{
					x -= 1;

					const float flMaxHealth = static_cast<float>(nMaxHealth);
					const float flHealth = Clamp<float>(static_cast<float>(nHealth), 1.0f, flMaxHealth);

					static const int nWidth = 2;
					const int nHeight = (h + (flHealth < flMaxHealth ? 2 : 1));
					const int nHeight2 = (h + 1);

					const float flRatio = (flHealth / flMaxHealth);
					G::Draw.Rect(static_cast<int>(((x - nWidth) - 2)), static_cast<int>((y + nHeight - (nHeight * flRatio))), nWidth, static_cast<int>((nHeight * flRatio)), clrHealth);
					if (Vars::ESP::Outlines.m_Var)
						G::Draw.OutlinedRect(static_cast<int>(((x - nWidth) - 2) - 1), static_cast<int>((y + nHeight - (nHeight * flRatio)) - 1), nWidth + 2, static_cast<int>((nHeight * flRatio) + 1), COLOR_BLACK);

					x += 1;
				}

				break;
			}
			case ETFClientClass::CObjectSentrygun:
			{
				if (!Vars::ESP::Buildings::Enabled.m_Var)
					break;

				C_ObjectSentrygun* pSentry = pEntity->As<C_ObjectSentrygun*>();

				if (Vars::ESP::Buildings::IgnoreTeam.m_Var && pSentry->InLocalTeam())
					break;
				
				if (!GetDynamicBounds(pSentry, x, y, w, h))
					break;

				const int nHealth = pSentry->GetHealth();
				const int nMaxHealth = pSentry->GetMaxHealth();

				const Color clrHealth = G::Util.GetHealthColor(nHealth, nMaxHealth);
				const Color clrTeam = G::Util.GetTeamColor(pSentry->GetTeamNumber());

				const int nDrawX = x + (w / 2);
				int nDrawY = y + (h / 2);

				if (Vars::ESP::Buildings::Name.m_Var)
				{
					G::Draw.String(EFonts::ESP_NAME, nDrawX, nDrawY, clrTeam, TXT_CENTERXY, L"sentrygun");
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP_NAME);
				}

				if (Vars::ESP::Buildings::HealthText.m_Var)
				{
					G::Draw.String(EFonts::ESP, nDrawX, nDrawY, clrHealth, TXT_CENTERXY, L"%i / %ihp", nHealth, nMaxHealth);
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP);
				}

				if (Vars::ESP::Buildings::Info.m_Var)
				{
					int nShells, nMaxShells, nRockets, nMaxRockets;
					pSentry->GetAmmoCount(nShells, nMaxShells, nRockets, nMaxRockets);
 
					G::Draw.String(EFonts::ESP_WEAPON, nDrawX, nDrawY, COLOR_GREY, TXT_CENTERXY, L"shells: %i / %i", nShells, nMaxShells);
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP_WEAPON);

					if (nMaxRockets)
					{
						G::Draw.String(EFonts::ESP_WEAPON, nDrawX, nDrawY, COLOR_GREY, TXT_CENTERXY, L"rockets: %i / %i", nRockets, nMaxRockets);
						nDrawY += G::Draw.GetFontHeight(EFonts::ESP_WEAPON);
					}
				}

				break;
			}
			case ETFClientClass::CObjectDispenser:
			{
				if (!Vars::ESP::Buildings::Enabled.m_Var)
					break;

				C_ObjectDispenser* pDispenser = pEntity->As<C_ObjectDispenser*>();

				if (Vars::ESP::Buildings::IgnoreTeam.m_Var && pDispenser->InLocalTeam())
					break;

				if (!GetDynamicBounds(pDispenser, x, y, w, h))
					break;

				const int nHealth = pDispenser->GetHealth();
				const int nMaxHealth = pDispenser->GetMaxHealth();

				const Color clrHealth = G::Util.GetHealthColor(nHealth, nMaxHealth);
				const Color clrTeam = G::Util.GetTeamColor(pDispenser->GetTeamNumber());

				const int nDrawX = x + (w / 2);
				int nDrawY = y + (h / 2);

				if (Vars::ESP::Buildings::Name.m_Var)
				{
					G::Draw.String(EFonts::ESP_NAME, nDrawX, nDrawY, clrTeam, TXT_CENTERXY, L"dispenser");
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP_NAME);
				}

				if (Vars::ESP::Buildings::HealthText.m_Var)
				{
					G::Draw.String(EFonts::ESP, nDrawX, nDrawY, clrHealth, TXT_CENTERXY, L"%i / %ihp", nHealth, nMaxHealth);
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP);
				}

				if (Vars::ESP::Buildings::Info.m_Var)
				{
					G::Draw.String(EFonts::ESP_WEAPON, nDrawX, nDrawY, COLOR_GREY, TXT_CENTERXY, L"metal: %i / %i", pDispenser->m_iAmmoMetal(), pDispenser->GetMaxMetal());
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP_WEAPON);
				}

				break;
			}
			case ETFClientClass::CObjectTeleporter:
			{
				if (!Vars::ESP::Buildings::Enabled.m_Var)
					break;

				C_ObjectTeleporter* pTeleport = pEntity->As<C_ObjectTeleporter*>();

				if (Vars::ESP::Buildings::IgnoreTeam.m_Var && pTeleport->InLocalTeam())
					break;

				if (!GetDynamicBounds(pTeleport, x, y, w, h))
					break;

				const int nHealth = pTeleport->GetHealth();
				const int nMaxHealth = pTeleport->GetMaxHealth();

				const Color clrHealth = G::Util.GetHealthColor(nHealth, nMaxHealth);
				const Color clrTeam = G::Util.GetTeamColor(pTeleport->GetTeamNumber());

				const int nDrawX = x + (w / 2);
				int nDrawY = y + (h / 2);

				if (Vars::ESP::Buildings::Name.m_Var)
				{
					G::Draw.String(EFonts::ESP_NAME, nDrawX, nDrawY, clrTeam, TXT_CENTERXY, L"teleport");
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP_NAME);
				}

				if (Vars::ESP::Buildings::HealthText.m_Var)
				{
					G::Draw.String(EFonts::ESP, nDrawX, nDrawY, clrHealth, TXT_CENTERXY, L"%i / %ihp", nHealth, nMaxHealth);
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP);
				}

				if (Vars::ESP::Buildings::Info.m_Var)
				{
					const float flChargeTime = pTeleport->GetChargeTime();

					if (flChargeTime > 0.0f)
					{
						G::Draw.String(EFonts::ESP_WEAPON, nDrawX, nDrawY, COLOR_GREY, TXT_CENTERXY, L"charging: %.1f", flChargeTime);
						nDrawY += G::Draw.GetFontHeight(EFonts::ESP_WEAPON);
					}
				}

				break;
			}
			default:
				break;
		}
	}
}

void CFeatures_ESP::LevelInitPostEntity()
{
	m_vecHealth.Purge();
	{
		//Normal
		m_vecHealth.AddToTail(I::ModelInfoClient->GetModelIndex("models/items/medkit_large.mdl"));
		m_vecHealth.AddToTail(I::ModelInfoClient->GetModelIndex("models/items/medkit_medium.mdl"));
		m_vecHealth.AddToTail(I::ModelInfoClient->GetModelIndex("models/items/medkit_small.mdl"));
		//Birthday
		m_vecHealth.AddToTail(I::ModelInfoClient->GetModelIndex("models/items/medkit_large_bday.mdl"));
		m_vecHealth.AddToTail(I::ModelInfoClient->GetModelIndex("models/items/medkit_medium_bday.mdl"));
		m_vecHealth.AddToTail(I::ModelInfoClient->GetModelIndex("models/items/medkit_small_bday.mdl"));
		//Halloween
		m_vecHealth.AddToTail(I::ModelInfoClient->GetModelIndex("models/props_halloween/halloween_medkit_large.mdl"));
		m_vecHealth.AddToTail(I::ModelInfoClient->GetModelIndex("models/props_halloween/halloween_medkit_medium.mdl"));
		m_vecHealth.AddToTail(I::ModelInfoClient->GetModelIndex("models/props_halloween/halloween_medkit_small.mdl"));
		//Medieval
		m_vecHealth.AddToTail(I::ModelInfoClient->GetModelIndex("models/props_medieval/medieval_meat.mdl"));
	}

	m_vecAmmo.Purge();
	{
		//Normal
		m_vecAmmo.AddToTail(I::ModelInfoClient->GetModelIndex("models/items/ammopack_large.mdl"));
		m_vecAmmo.AddToTail(I::ModelInfoClient->GetModelIndex("models/items/ammopack_medium.mdl"));
		m_vecAmmo.AddToTail(I::ModelInfoClient->GetModelIndex("models/items/ammopack_small.mdl"));
		//Birthday
		m_vecAmmo.AddToTail(I::ModelInfoClient->GetModelIndex("models/items/ammopack_large_bday.mdl"));
		m_vecAmmo.AddToTail(I::ModelInfoClient->GetModelIndex("models/items/ammopack_medium_bday.mdl"));
		m_vecAmmo.AddToTail(I::ModelInfoClient->GetModelIndex("models/items/ammopack_small_bday.mdl"));
	}
}

bool CFeatures_ESP::IsHealth(const int nModelIndex)
{
	for (int i = 0; i < m_vecHealth.Count(); ++i)
	{
		if (m_vecHealth.Element(i) == nModelIndex)
			return true;
	}

	return false;
}

bool CFeatures_ESP::IsAmmo(const int nModelIndex)
{
	for (int i = 0; i < m_vecAmmo.Count(); ++i)
	{
		if (m_vecAmmo.Element(i) == nModelIndex)
			return true;
	}

	return false;
}

bool CFeatures_ESP::GetDynamicBounds(C_BaseEntity* pEntity, int& x, int& y, int& w, int& h)
{
	Vector vPoints[8];
	BuildTransformedBox(vPoints, pEntity->m_vecMins(), pEntity->m_vecMaxs(), pEntity->RenderableToWorldTransform());

	Vector2D flb, brt, blb, frt, frb, brb, blt, flt;

	if (GetVectorInScreenSpace(vPoints[3], flb) && GetVectorInScreenSpace(vPoints[5], brt)
		&& GetVectorInScreenSpace(vPoints[0], blb) && GetVectorInScreenSpace(vPoints[4], frt)
		&& GetVectorInScreenSpace(vPoints[2], frb) && GetVectorInScreenSpace(vPoints[1], brb)
		&& GetVectorInScreenSpace(vPoints[6], blt) && GetVectorInScreenSpace(vPoints[7], flt)
		&& GetVectorInScreenSpace(vPoints[6], blt) && GetVectorInScreenSpace(vPoints[7], flt))
	{
		const Vector2D arr[8] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float left = flb.x;
		float top = flb.y;
		float righ = flb.x;
		float bottom = flb.y;

		for (int n = 1; n < 8; n++)
		{
			if (left > arr[n].x)
				left = arr[n].x;

			if (top < arr[n].y)
				top = arr[n].y;

			if (righ < arr[n].x)
				righ = arr[n].x;

			if (bottom > arr[n].y)
				bottom = arr[n].y;
		}

		x = static_cast<int>(left);
		y = static_cast<int>(bottom);
		w = static_cast<int>((righ - left));
		h = static_cast<int>((top - bottom));

		return !(x > g_Globals.m_nScreenWidht || (x + w) < 0 || y > g_Globals.m_nScreenHeight || (y + h) < 0);
	}

	return false;
}