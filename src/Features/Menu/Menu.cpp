#include "Menu.h"

void CFeatures_Menu::Separator()
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

bool CFeatures_Menu::CheckBox(CVar<bool>& Var, const wchar_t* const szToolTip)
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

bool CFeatures_Menu::Button(const wchar_t* Label, bool Active, int WidthOverride, int HeightOverride)
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

bool CFeatures_Menu::ComboBox(CVar<int>& Var, const std::vector<CVar<int>>& List)
{
	auto FindCurItemName = [&]() -> const wchar_t*
	{
		for (const auto& Item : List)
		{
			if (Item.m_Var == Var.m_Var)
				return Item.m_szDisplayName;
		}

		return L"UNKNOWN_ITEM";
	};

	auto FindCurItemIndex = [&]() -> int
	{
		for (size_t n = 0; n < List.size(); n++)
		{
			if (List[n].m_Var == Var.m_Var)
				return n;
		}

		return 0;
	};

	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::ComboBoxW;
	int h = Vars::Menu::ComboBoxH;

	static std::map<CVar<int>*, int> indexes = {};
	static std::map<CVar<int>*, bool> inits = {};

	if (!inits[&Var] || m_bReopened) {
		indexes[&Var] = FindCurItemIndex();
		inits[&Var] = true;
	}

	if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + (w / 2) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
	{
		if (indexes[&Var] > 0)
		{
			if (g_InputHelper.IsPressed(VK_LBUTTON)) {
				Var.m_Var = List[--indexes[&Var]].m_Var;
				callback = true;
			}

			G::Draw.GradientRect(x, y, x + (w / 2), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true);
		}
	}

	else if (g_InputHelper.m_nMouseX > x + (w / 2) && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
	{
		if (indexes[&Var] < int(List.size() - 1))
		{
			if (g_InputHelper.IsPressed(VK_LBUTTON)) {
				Var.m_Var = List[++indexes[&Var]].m_Var;
				callback = true;
			}

			G::Draw.GradientRect(x + (w / 2), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true);
		}
	}

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, FindCurItemName());
	G::Draw.String(FONT_MENU, x + w + Vars::Menu::SpacingText, y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERY, Var.m_szDisplayName);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
}

bool CFeatures_Menu::InputFloat(CVar<float>& Var, float Min, float Max, float Step, const wchar_t* Fmt)
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputBoxW;
	int h = Vars::Menu::InputBoxH;

	if (Var.m_Var != Min)
	{
		if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + (w / 2) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
		{
			if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
				Var.m_Var -= Step;
				Var.m_Var = std::clamp(Var.m_Var, Min, Max);
				callback = true;
			}

			G::Draw.GradientRect(x, y, x + (w / 2), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true);
		}
	}

	if (Var.m_Var != Max)
	{
		if (g_InputHelper.m_nMouseX > x + (w / 2) && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
		{
			if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
				Var.m_Var += Step;
				Var.m_Var = std::clamp(Var.m_Var, Min, Max);
				callback = true;
			}

			G::Draw.GradientRect(x + (w / 2), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true);
		}
	}

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, Fmt, Var.m_Var);
	G::Draw.String(FONT_MENU, x + w + Vars::Menu::SpacingText, y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERY, Var.m_szDisplayName);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
}

bool CFeatures_Menu::InputInt(CVar<int>& Var, int Min, int Max, int Step)
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputBoxW;
	int h = Vars::Menu::InputBoxH;

	if (Var.m_Var != Min)
	{
		if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + (w / 2) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
		{
			if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
				Var.m_Var -= Step;
				Var.m_Var = std::clamp(Var.m_Var, Min, Max);
				callback = true;
			}

			G::Draw.GradientRect(x, y, x + (w / 2), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true);
		}
	}

	if (Var.m_Var != Max)
	{
		if (g_InputHelper.m_nMouseX > x + (w / 2) && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
		{
			if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
				Var.m_Var += Step;
				Var.m_Var = std::clamp(Var.m_Var, Min, Max);
				callback = true;
			}

			G::Draw.GradientRect(x + (w / 2), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true);
		}
	}

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, "%d", Var.m_Var);
	G::Draw.String(FONT_MENU, x + w + Vars::Menu::SpacingText, y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERY, Var.m_szDisplayName);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
}

bool CFeatures_Menu::InputColor(Color& Var, const wchar_t* Label)
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputColorBoxW;
	int h = Vars::Menu::InputBoxH;
	int _x = x;

	int oldspacing = Vars::Menu::SpacingX;
	Vars::Menu::SpacingX /= 2;

	//red
	{
		if (Var.r() != 0)
		{
			if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + (w / 2) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
			{
				if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
					Var.SetColor(Var.r() - 1, Var.g(), Var.b(), Var.a());
					Var.SetColor(Clamp<int>(Var.r(), static_cast<byte>(0), static_cast<byte>(255)), Var.g(), Var.b(), Var.a());
					callback = true;
				}

				G::Draw.GradientRect(x, y, x + (w / 2), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true);
			}
		}

		if (Var.r() != 255)
		{
			if (g_InputHelper.m_nMouseX > x + (w / 2) && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
			{
				if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
					Var.SetColor(Var.r() + 1, Var.g(), Var.b(), Var.a());
					Var.SetColor(Clamp<int>(Var.r(), static_cast<byte>(0), static_cast<byte>(255)), Var.g(), Var.b(), Var.a());
					callback = true;
				}

				G::Draw.GradientRect(x + (w / 2), y, x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true);
			}
		}

		G::Draw.OutlinedRect(_x, y, w, h, Vars::Menu::Colors::OutlineMenu);
		G::Draw.String(FONT_MENU, _x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, "%d", Var.r());
		_x += w + Vars::Menu::SpacingX;
	}

	//green
	{
		if (Var.g() != 0)
		{
			if (g_InputHelper.m_nMouseX > _x && g_InputHelper.m_nMouseX < _x + (w / 2) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
			{
				if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
					Var.SetColor(Var.r(), Var.g() - 1, Var.b(), Var.a());
					Var.SetColor(Var.r(), Clamp<int>(Var.g(), static_cast<byte>(0), static_cast<byte>(255)), Var.b(), Var.a());
					callback = true;
				}

				G::Draw.GradientRect(_x, y, _x + (w / 2), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true);
			}
		}

		if (Var.g() != 255)
		{
			if (g_InputHelper.m_nMouseX > _x + (w / 2) && g_InputHelper.m_nMouseX < _x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
			{
				if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
					Var.SetColor(Var.r(), Var.g() + 1, Var.b(), Var.a());
					Var.SetColor(Var.r(), Clamp<int>(Var.g(), static_cast<byte>(0), static_cast<byte>(255)), Var.b(), Var.a());
					callback = true;
				}

				G::Draw.GradientRect(_x + (w / 2), y, _x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true);
			}
		}

		G::Draw.OutlinedRect(_x, y, w, h, Vars::Menu::Colors::OutlineMenu);
		G::Draw.String(FONT_MENU, _x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, "%d", Var.g());
		_x += w + Vars::Menu::SpacingX;
	}

	//blue
	{
		if (Var.b() != 0)
		{
			if (g_InputHelper.m_nMouseX > _x && g_InputHelper.m_nMouseX < _x + (w / 2) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
			{
				if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
					Var.SetColor(Var.r(), Var.g(), Var.b() - 1, Var.a());
					Var.SetColor(Var.r(), Var.g(), Clamp<int>(Var.b(), static_cast<byte>(0), static_cast<byte>(255)), Var.a());
					callback = true;
				}

				G::Draw.GradientRect(_x, y, _x + (w / 2), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true);
			}
		}

		if (Var.b() != 255)
		{
			if (g_InputHelper.m_nMouseX > _x + (w / 2) && g_InputHelper.m_nMouseX < _x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
			{
				if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
					Var.SetColor(Var.r(), Var.g(), Var.b() + 1, Var.a());
					Var.SetColor(Var.r(), Var.g(), Clamp<int>(Var.b(), static_cast<byte>(0), static_cast<byte>(255)), Var.a());
					callback = true;
				}

				G::Draw.GradientRect(_x + (w / 2), y, _x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true);
			}
		}

		G::Draw.OutlinedRect(_x, y, w, h, Vars::Menu::Colors::OutlineMenu);
		G::Draw.String(FONT_MENU, _x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, "%d", Var.b());
		_x += w + Vars::Menu::SpacingX;
	}

	//alpha
	{
		if (Var.a() != 0)
		{
			if (g_InputHelper.m_nMouseX > _x && g_InputHelper.m_nMouseX < _x + (w / 2) && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
			{
				if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
					Var.SetColor(Var.r(), Var.g(), Var.b(), Var.a() - 1);
					Var.SetColor(Var.r(), Var.g(), Var.b(), Clamp<int>(Var.a(), static_cast<byte>(0), static_cast<byte>(255)));
					callback = true;
				}

				G::Draw.GradientRect(_x, y, _x + (w / 2), y + h, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, true);
			}
		}

		if (Var.a() != 255)
		{
			if (g_InputHelper.m_nMouseX > _x + (w / 2) && g_InputHelper.m_nMouseX < _x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
			{
				if (g_InputHelper.IsPressedAndHeld(VK_LBUTTON)) {
					Var.SetColor(Var.r(), Var.g(), Var.b(), Var.a() + 1);
					Var.SetColor(Var.r(), Var.g(), Var.b(), Clamp<int>(Var.a(), static_cast<byte>(0), static_cast<byte>(255)));
					callback = true;
				}

				G::Draw.GradientRect(_x + (w / 2), y, _x + w, y + h, Vars::Menu::Colors::Widget, Vars::Menu::Colors::WidgetActive, true);
			}
		}

		G::Draw.OutlinedRect(_x, y, w, h, Vars::Menu::Colors::OutlineMenu);
		G::Draw.String(FONT_MENU, _x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, "%d", Var.a());
	}

	G::Draw.String(FONT_MENU, _x + w + Vars::Menu::SpacingText, y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERY, Label);

	Vars::Menu::SpacingX = oldspacing;

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
}

bool CFeatures_Menu::InputString(const wchar_t* Label, std::wstring& output)
{
    return false;
}

bool CFeatures_Menu::InputKey(CVar<int>& output, bool bAllowNone)
{
    return false;
}

void CFeatures_Menu::GroupBoxStart()
{
	m_LastGroupBox.x = m_LastWidget.x;
	m_LastGroupBox.y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;

	m_LastWidget.x += Vars::Menu::SpacingX;
	m_LastWidget.y += Vars::Menu::SpacingY + 1;
}

void CFeatures_Menu::GroupBoxEnd(const wchar_t* Label, int Width)
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

void CFeatures_Menu::DrawTooltip()
{
}

void CFeatures_Menu::Run()
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

				case EVisualsTabs::TAB_CHAMS:
				{
					GroupBoxStart();
					{
						CheckBox(Vars::Chams::Enabled, L"Chams Master Toggle");
					}
					GroupBoxEnd(L"Main", 160);

					GroupBoxStart();
					{
						CheckBox(Vars::Chams::Players::Enabled, L"Player Chams Toggle");
						CheckBox(Vars::Chams::Players::IgnoreTeam, L"Ignore the local team");
						ComboBox(Vars::Chams::Players::Material, { { 1, L"Fresnel" }, {2, L"Glow"}, {3, L"Test"}, {4, L"Shaded"}, {5, L"Toxic"} });
					}
					GroupBoxEnd(L"Players", 160);

					if (Vars::Chams::Players::Material.m_Var == 1)
					{
						GroupBoxStart();
						{
							InputColor(Vars::Chams::Players::FresnelVars::BaseColor, L"Base Color");
							InputColor(Vars::Chams::Players::FresnelVars::GlowColor, L"Glow Color");
							InputFloat(Vars::Chams::Players::FresnelVars::FresnelX, -10.f, 10.f, 0.1f, L"%.1f");
							InputFloat(Vars::Chams::Players::FresnelVars::FresnelY, -10.f, 10.f, 0.1f, L"%.1f");
							InputFloat(Vars::Chams::Players::FresnelVars::FresnelZ, -10.f, 10.f, 0.1f, L"%.1f");
						}
						GroupBoxEnd(L"Fresnel Vars", 210);
					}

					break;
				}

				case EVisualsTabs::TAB_OTHER:
				{
					Rect_t checkpoint = m_LastWidget;

					GroupBoxStart();
					{
						CheckBox(Vars::Visual::RemoveVisualRecoil, L"Removes visual weapon recoil");
						CheckBox(Vars::Visual::Tracers, L"Bullet Tracers");
						CheckBox(Vars::Visual::RemoveScope, L"Removes sniper scope overlay");
						CheckBox(Vars::Visual::RemoveZoom, L"Removes sniper scope zoom");
						InputInt(Vars::Visual::FOV, 40, 140);
					}
					GroupBoxEnd(L"Main", 190);
					
					GroupBoxStart();
					{
						CheckBox(Vars::Visual::Thirdperson, L"Enables thirdperson");
						InputKey(Vars::Visual::ThirdpersonKey);
					}
					GroupBoxEnd(L"Thirdperson", 180);

					GroupBoxStart();
					{
						CheckBox(Vars::Visual::Snow, L"Snow on the Menu");
						CheckBox(Vars::Visual::ToolTips, L"Informational tooltips for the Menu");
					}
					GroupBoxEnd(L"Menu", 190);

					checkpoint.x += 190 + Vars::Menu::SpacingX;
					m_LastWidget = checkpoint;

					GroupBoxStart();
					{
						CheckBox(Vars::Visual::ViewModelSway, L"Slight weapon sway when moving camera");
						CheckBox(Vars::Visual::ViewModel_Active, L"Toggle to enable viewmodel offsets");
						InputInt(Vars::Visual::ViewModel_Position_Offset_Forward, -30, 30);
						InputInt(Vars::Visual::ViewModel_Position_Offset_Right, -30, 30);
						InputInt(Vars::Visual::ViewModel_Position_Offset_Up, -30, 30);
					}

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
					CheckBox(Vars::Misc::ForceMeleeCrits, L"Forces criticals on melee weapons");
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
