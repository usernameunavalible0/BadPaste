#include "Menu.h"

void CMenu::Separator()
{
	int x = m_LastWidget.x + m_LastWidget.width + Vars::Menu::AltSpacingX;
	int y = Vars::Menu::Position.y;
	int w = 0;
	int h = 0;

	G::Draw.Line(x, y, x, (y + Vars::Menu::Position.height - 1), Vars::Menu::Colors::OutlineMenu);

	m_LastWidget.x = x + Vars::Menu::SpacingX;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;
}

bool CMenu::CheckBox(CVar<bool>& Var, const wchar_t* const szToolTip)
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::CheckBoxW;
	int h = Vars::Menu::CheckBoxH;

	if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
	{
		if (g_InputHelper.IsPressed(VK_LBUTTON)) {
			callback = true;
			Var.m_Var = !Var.m_Var;
		}

		G::Draw.Rect(x, y, w, h, Vars::Menu::Colors::WidgetActive);
		m_szCurTip += szToolTip;
	}

	if (Var.m_Var)
	{
		G::Draw.GradientRect(
			x,
			y,
			x + w,
			y + h,
			Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive,
			false);
	}

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.String(FONT_MENU, x + w + Vars::Menu::SpacingText, y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERY, Var.m_szDisplayName);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
}

bool CMenu::Button(const wchar_t* Label, bool Active, int WidthOverride, int HeightOverride)
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::AltSpacingY;
	int w = WidthOverride ? WidthOverride : Vars::Menu::ButtonW;
	int h = HeightOverride ? HeightOverride : Vars::Menu::ButtonH;

	if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
	{
		if (g_InputHelper.IsPressed(VK_LBUTTON))
			callback = true;

		G::Draw.Rect(x, y, w, h, Vars::Menu::Colors::WidgetActive);
	}

	if (Active)
		G::Draw.GradientRect(x, y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, false);

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, Label);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	if (callback)
		m_bReopened = true;

	return callback;
}

bool CMenu::ComboBox(CVar<int>& Var, const std::vector<CVar<int>>& List)
{
    return false;
}

bool CMenu::InputFloat(CVar<float>& Var, float Min, float Max, float Step, const wchar_t* Fmt)
{
    return false;
}

bool CMenu::InputInt(CVar<int>& Var, int Min, int Max, int Step)
{
    return false;
}

bool CMenu::InputColor(Color& Var, const wchar_t* Label)
{
    return false;
}

bool CMenu::InputString(const wchar_t* Label, std::wstring& output)
{
    return false;
}

bool CMenu::InputKey(CVar<int>& output, bool bAllowNone)
{
    return false;
}

void CMenu::GroupBoxStart()
{
	m_LastGroupBox.x = m_LastWidget.x;
	m_LastGroupBox.y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;

	m_LastWidget.x += Vars::Menu::SpacingX;
	m_LastWidget.y += Vars::Menu::SpacingY + 1;
}

void CMenu::GroupBoxEnd(const wchar_t* Label, int Width)
{
	int h = m_LastWidget.y - m_LastGroupBox.y + m_LastWidget.height + Vars::Menu::SpacingY;

	int label_w, label_h;
	I::MatSystemSurface->GetTextSize(G::Draw.m_Fonts.Element(G::Draw.m_Fonts.Find(FONT_MENU)).m_hFont, Label, label_w, label_h);

	int label_x = m_LastGroupBox.x + (Width / 2);
	int label_y = m_LastGroupBox.y - (label_h / 2);

	G::Draw.Line(m_LastGroupBox.x, m_LastGroupBox.y, label_x - (label_w / 2) - Vars::Menu::SpacingText, m_LastGroupBox.y, Vars::Menu::Colors::OutlineMenu);
	G::Draw.Line(label_x + (label_w / 2) + Vars::Menu::SpacingText, m_LastGroupBox.y, m_LastGroupBox.x + Width, m_LastGroupBox.y, Vars::Menu::Colors::OutlineMenu);
	G::Draw.Line(m_LastGroupBox.x + Width, m_LastGroupBox.y, m_LastGroupBox.x + Width, m_LastGroupBox.y + h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.Line(m_LastGroupBox.x + Width, m_LastGroupBox.y + h, m_LastGroupBox.x, m_LastGroupBox.y + h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.Line(m_LastGroupBox.x, m_LastGroupBox.y + h, m_LastGroupBox.x, m_LastGroupBox.y, Vars::Menu::Colors::OutlineMenu);

	G::Draw.String(FONT_MENU, label_x, label_y, Vars::Menu::Colors::Text, TXT_CENTERX, Label);

	m_LastWidget.x -= Vars::Menu::SpacingX;
	m_LastWidget.y += Vars::Menu::SpacingY * 2;
	m_LastGroupBox.height = h;
}

void CMenu::DrawTooltip()
{
}

void CMenu::Run()
{
	m_bReopened = false;
	m_bTyping = false;

	static bool bOldOpen = m_bOpen;

	if (bOldOpen != m_bOpen)
	{
		bOldOpen = m_bOpen;

		if (m_bOpen)
			m_bReopened = true;
	}

	if (I::EngineClient->IsDrawingLoadingImage()) {
		m_bOpen = false;
		return;
	}

	static float flTimeOnChange = 0.0f;

	if (GetAsyncKeyState(VK_INSERT) & 1) {
		I::MatSystemSurface->SetCursorAlwaysVisible(m_bOpen = !m_bOpen);
		flTimeOnChange = I::EngineClient->Time();
	}

	m_flFadeElapsed = I::EngineClient->Time() - flTimeOnChange;

	if (m_flFadeElapsed < m_flFadeDuration) {
		m_flFadeAlpha = RemapValClamped(m_flFadeElapsed, 0.0f, m_flFadeDuration, !m_bOpen ? 1.0f : 0.0f, m_bOpen ? 1.0f : 0.0f);
		I::MatSystemSurface->DrawSetAlphaMultiplier(m_flFadeAlpha);
	}

	if (m_bOpen || m_flFadeElapsed < m_flFadeDuration)
	{
		m_szCurTip = L"";
		g_InputHelper.Update();

		//Do the Window
		{
			g_InputHelper.Drag(
				g_InputHelper.m_nMouseX,
				g_InputHelper.m_nMouseY,
				Vars::Menu::Position.x,
				Vars::Menu::Position.y,
				Vars::Menu::Position.width,
				Vars::Menu::TitleBarH,
				Vars::Menu::TitleBarH);

			G::Draw.Rect(
				Vars::Menu::Position.x,
				Vars::Menu::Position.y,
				Vars::Menu::Position.width,
				Vars::Menu::Position.height,
				Vars::Menu::Colors::WindowBackground);

			G::Draw.Rect(
				Vars::Menu::Position.x,
				Vars::Menu::Position.y - Vars::Menu::TitleBarH,
				Vars::Menu::Position.width,
				Vars::Menu::TitleBarH,
				Vars::Menu::Colors::TitleBar);

			G::Draw.OutlinedRect(
				Vars::Menu::Position.x - 1,
				Vars::Menu::Position.y - Vars::Menu::TitleBarH - 1,
				Vars::Menu::Position.width + 1,
				Vars::Menu::Position.height + Vars::Menu::TitleBarH + 1,
				Vars::Menu::Colors::OutlineMenu
			);

			G::Draw.Line(
				Vars::Menu::Position.x,
				Vars::Menu::Position.y - 1,
				Vars::Menu::Position.x + Vars::Menu::Position.width - 1,
				Vars::Menu::Position.y - 1,
				Vars::Menu::Colors::OutlineMenu
			);

			G::Draw.String(FONT_MENU,
				Vars::Menu::Position.x + (Vars::Menu::Position.width / 2),
				Vars::Menu::Position.y - (Vars::Menu::TitleBarH / 2),
				Vars::Menu::Colors::Text,
				TXT_CENTERXY,
				L"Team Fortress 2");
		}

		//Do the Widgets
		{
			enum struct EMainTabs { TAB_AIM, TAB_VISUALS, TAB_MISC, TAB_CONFIGS };
			enum struct EVisualsTabs { TAB_ESP, TAB_CHAMS, TAB_GLOW, TAB_OTHER, TAB_SKINS, TAB_COLORS };

			m_LastWidget = { Vars::Menu::Position.x + Vars::Menu::AltSpacingX, Vars::Menu::Position.y, 0, 0 };

			static EMainTabs Tab = EMainTabs::TAB_AIM;
			{
				if (Button(L"Aim", Tab == EMainTabs::TAB_AIM, Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall))
					Tab = EMainTabs::TAB_AIM;

				if (Button(L"Visuals", Tab == EMainTabs::TAB_VISUALS, Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall))
					Tab = EMainTabs::TAB_VISUALS;

				if (Button(L"Misc", Tab == EMainTabs::TAB_MISC, Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall))
					Tab = EMainTabs::TAB_MISC;

				if (Button(L"Configs", Tab == EMainTabs::TAB_CONFIGS, Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall))
					Tab = EMainTabs::TAB_CONFIGS;
			}

			Separator();

			switch (Tab)
			{
			case EMainTabs::TAB_VISUALS:
			{
				static EVisualsTabs Tab = EVisualsTabs::TAB_ESP;
				{
					Rect_t checkpoint_line = m_LastWidget;
					checkpoint_line.x -= Vars::Menu::SpacingX;
					checkpoint_line.y += Vars::Menu::ButtonHSmall + (Vars::Menu::AltSpacingY * 2);
					Rect_t checkpoint_move = m_LastWidget;
					checkpoint_move.x -= 5;
					m_LastWidget = checkpoint_move;

					if (Button(L"ESP", Tab == EVisualsTabs::TAB_ESP, Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall))
						Tab = EVisualsTabs::TAB_ESP;

					checkpoint_move.x += Vars::Menu::ButtonWSmall + Vars::Menu::AltSpacingX;
					m_LastWidget = checkpoint_move;

					if (Button(L"Chams", Tab == EVisualsTabs::TAB_CHAMS, Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall))
						Tab = EVisualsTabs::TAB_CHAMS;

					checkpoint_move.x += Vars::Menu::ButtonWSmall + Vars::Menu::AltSpacingX;
					m_LastWidget = checkpoint_move;

					if (Button(L"Glow", Tab == EVisualsTabs::TAB_GLOW, Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall))
						Tab = EVisualsTabs::TAB_GLOW;

					checkpoint_move.x += Vars::Menu::ButtonWSmall + Vars::Menu::AltSpacingX;
					m_LastWidget = checkpoint_move;

					if (Button(L"Other", Tab == EVisualsTabs::TAB_OTHER, Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall))
						Tab = EVisualsTabs::TAB_OTHER;

					checkpoint_move.x += Vars::Menu::ButtonWSmall + Vars::Menu::AltSpacingX;
					m_LastWidget = checkpoint_move;

					if (Button(L"Colors", Tab == EVisualsTabs::TAB_COLORS, Vars::Menu::ButtonWSmall, Vars::Menu::ButtonHSmall))
						Tab = EVisualsTabs::TAB_COLORS;

					m_LastWidget = checkpoint_line;
					G::Draw.Line(checkpoint_line.x, checkpoint_line.y, Vars::Menu::Position.x + Vars::Menu::Position.width - 1, checkpoint_line.y, Vars::Menu::Colors::OutlineMenu);
					checkpoint_line.x += Vars::Menu::SpacingX;
					checkpoint_line.y += Vars::Menu::SpacingY;
					m_LastWidget = checkpoint_line;
				}

				switch (Tab)
				{
				case EVisualsTabs::TAB_ESP:
				{
					Rect_t checkpoint = m_LastWidget;

					GroupBoxStart();
					{
						CheckBox(Vars::ESP::Enabled, L"ESP Master Toggle");
					}
					GroupBoxEnd(L"Main", 160);

					GroupBoxStart();
					{
						CheckBox(Vars::ESP::Players::Enabled, L"Player ESP Toggle");
						CheckBox(Vars::ESP::Players::IgnoreTeam, L"Ignore the local team");
						CheckBox(Vars::ESP::Players::ActiveWeapon, L"Active Weapon text");
						CheckBox(Vars::ESP::Players::HealthText, L"Player Health Text");
						CheckBox(Vars::ESP::Players::Name, L"Name Text");
					}
					GroupBoxEnd(L"Players", 160);

					checkpoint.x += 160 + Vars::Menu::SpacingX;
					m_LastWidget = checkpoint;

					GroupBoxStart();
					{
						CheckBox(Vars::ESP::Buildings::Enabled, L"Building ESP Toggle");
						CheckBox(Vars::ESP::Buildings::IgnoreTeam, L"Ignore the local team");
						CheckBox(Vars::ESP::Buildings::HealthText, L"Building Health Text");
						CheckBox(Vars::ESP::Buildings::Name, L"Building Type");
						CheckBox(Vars::ESP::Buildings::Info, L"Misc building info");
					}
					GroupBoxEnd(L"Buildings", 160);

					break;
				}

				case EVisualsTabs::TAB_OTHER:
				{
					GroupBoxStart();
					{
						CheckBox(Vars::Visual::RemoveVisualRecoil, L"Removes visual weapon recoil");
						CheckBox(Vars::Visual::Tracers, L"Bullet Tracers");
						CheckBox(Vars::Visual::Snow, L"Snow on the Menu");
						CheckBox(Vars::Visual::ToolTips, L"Informational tooltips for the Menu");
					}
					GroupBoxEnd(L"Main", 190);

					break;
				}

				default: break;
				}

				break;
			}

			case EMainTabs::TAB_MISC:
			{
				GroupBoxStart();
				{
					CheckBox(Vars::Misc::Bunnyhop, L"Automatic jumping");
					CheckBox(Vars::Misc::BypassPure, L"Bypass sv_pure");
				}
				GroupBoxEnd(L"Main", 160);

				break;
			}

			default: break;
			}
		}
	}

    I::MatSystemSurface->DrawSetAlphaMultiplier(1.0f);
}
