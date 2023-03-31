#include "Menu.h"

void CMenu::DrawTooltip()
{
	if (m_szCurTip.empty() || !Vars::Visual::ToolTips.m_Var)
		return;

	int w, h;
	I::MatSystemSurface->GetTextSize(G::Draw.m_Fonts.Element(G::Draw.m_Fonts.Find(FONT_MENU)).m_hFont, m_szCurTip.data(), w, h);

	G::Draw.OutlinedRect(g_InputHelper.m_nMouseX + 13, g_InputHelper.m_nMouseY + 13, w + 4, h + 4, { 0, 0, 0, 255 });
	G::Draw.Rect(g_InputHelper.m_nMouseX + 14, g_InputHelper.m_nMouseY + 14, w + 2, h + 2, Vars::Menu::Colors::WindowBackground);
	G::Draw.String(FONT_MENU, g_InputHelper.m_nMouseX + 15, g_InputHelper.m_nMouseY + 15, Vars::Menu::Colors::Text, TXT_DEFAULT, m_szCurTip.data());
}

void CMenu::Separator()
{
	int x = m_LastWidget.x + m_LastWidget.width + Vars::Menu::SpacingX;
	int y = Vars::Menu::MenuPos.y;
	int w = 0;
	int h = 0;

	G::Draw.Line(x, y, x, (y + Vars::Menu::MenuPos.height - 1), Vars::Menu::Colors::OutlineMenu);

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

bool CMenu::Button(const wchar_t* Label, bool Active, int WidthOverride, int HeightOverride, int SpacingOverride)
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + (SpacingOverride ? SpacingOverride : Vars::Menu::SpacingY);
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

bool CMenu::TextureButton(const char* Texture, int x, int y, bool Active, int WidthOverride, int HeightOverride)
{
	static bool callback = false;

	int w = WidthOverride ? WidthOverride : Vars::Menu::ButtonW;
	int h = HeightOverride ? HeightOverride : Vars::Menu::ButtonH;

	if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
	{
		if (g_InputHelper.IsPressed(VK_LBUTTON))
			callback = !callback;

		G::Draw.Texture(x, y, x + w, y + h, Vars::Menu::Colors::WidgetActive, Texture);
	}
	else if (Active)
		G::Draw.Texture(x, y, x + w, y + h, Vars::Menu::Colors::WidgetActive, Texture);
	else
		G::Draw.Texture(x, y, x + w, y + h, COLOR_WHITE, Texture);

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);

	if (callback)
		m_bReopened = true;

	return callback;
}

bool CMenu::ComboBox(CVar<int>& Var, const std::vector<CVar<int>>& List)
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

bool CMenu::InputFloat(CVar<float>& Var, float Min, float Max, float Step, const wchar_t* Fmt)
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

bool CMenu::InputInt(CVar<int>& Var, int Min, int Max, int Step)
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

bool CMenu::InputColor(Color& Var, const wchar_t* Label)
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
					Var.SetColor(std::clamp<int>(Var.r(), static_cast<byte>(0), static_cast<byte>(255)), Var.g(), Var.b(), Var.a());
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
					Var.SetColor(std::clamp<int>(Var.r(), static_cast<byte>(0), static_cast<byte>(255)), Var.g(), Var.b(), Var.a());
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
					Var.SetColor(Var.r(), std::clamp<int>(Var.g(), static_cast<byte>(0), static_cast<byte>(255)), Var.b(), Var.a());
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
					Var.SetColor(Var.r(), std::clamp<int>(Var.g(), static_cast<byte>(0), static_cast<byte>(255)), Var.b(), Var.a());
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
					Var.SetColor(Var.r(), Var.g(), std::clamp<int>(Var.b(), static_cast<byte>(0), static_cast<byte>(255)), Var.a());
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
					Var.SetColor(Var.r(), Var.g(), std::clamp<int>(Var.b(), static_cast<byte>(0), static_cast<byte>(255)), Var.a());
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
					Var.SetColor(Var.r(), Var.g(), Var.b(), std::clamp<int>(Var.a(), static_cast<byte>(0), static_cast<byte>(255)));
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
					Var.SetColor(Var.r(), Var.g(), Var.b(), std::clamp<int>(Var.a(), static_cast<byte>(0), static_cast<byte>(255)));
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

bool CMenu::InputString(const wchar_t* Label, std::wstring& output)
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputBoxW * 2;
	int h = Vars::Menu::ButtonH;

	static bool active = false;
	static std::wstring active_str = {};

	if (g_InputHelper.IsPressed(VK_LBUTTON))
	{
		if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h)
			active = !active;

		else active = false;
	}

	if (active)
	{
		m_bTyping = true;

		if (g_InputHelper.IsPressed(VK_INSERT)) {
			active = false;
			return false;
		}

		if (active_str.length() < 21)
		{
			if (g_InputHelper.IsPressed(VK_SPACE))
				active_str += char(VK_SPACE);

			for (int16_t key = L'A'; key < L'Z' + 1; key++)
			{
				if (g_InputHelper.IsPressed(key))
					active_str += wchar_t(key);
			}
		}

		if (g_InputHelper.IsPressedAndHeld(VK_BACK) && !active_str.empty())
			active_str.erase(active_str.end() - 1);

		if (g_InputHelper.IsPressed(VK_RETURN))
		{
			active = false;

			if (!active_str.empty())
			{
				output = active_str;
				callback = true;
			}
		}

		G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, "%ws", active_str.empty() ? L"Enter a Name" : active_str.c_str());
	}

	else
	{
		active_str = {};
		G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, Label);
	}

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
}

bool CMenu::InputKey(CVar<int>& output, bool bAllowNone)
{
	auto VK2STR = [&](const short key) -> std::wstring
	{
		switch (key) {
		case VK_LBUTTON: return L"LButton";
		case VK_RBUTTON: return L"RButton";
		case VK_MBUTTON: return L"MButton";
		case VK_XBUTTON1: return L"XButton1";
		case VK_XBUTTON2: return L"XButton2";
		case 0x0: return L"None";
		default: break;
		}

		WCHAR output[16] = { L"\0" };

		if (const int result = GetKeyNameTextW(MapVirtualKeyW(key, MAPVK_VK_TO_VSC) << 16, output, 16))
			return output;

		return L"VK2STR_FAILED";
	};

	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputBoxW;
	int h = Vars::Menu::InputBoxH;

	static bool active = false, old_active = active;
	static float time = I::EngineClient->Time();
	float elapsed = I::EngineClient->Time() - time;

	if (old_active != active) {
		time = I::EngineClient->Time();
		old_active = active;
	}

	if (!active && elapsed > 0.1f && g_InputHelper.IsPressed(VK_LBUTTON))
	{
		if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h) {
			active = true;
			g_InputHelper.NullKey(VK_LBUTTON);
		}
	}

	static float time_notactive = 0.0f;

	if (active)
	{
		m_bTyping = true;

		if (I::EngineClient->Time() - time_notactive > 0.1f)
		{
			for (short n = 0; n < 256; n++)
			{
				if ((n > 0x0 && n < 0x7)
					|| (n > L'A' - 1 && n < L'Z' + 1)
					|| n == VK_LSHIFT || n == VK_RSHIFT || n == VK_SHIFT
					|| n == VK_ESCAPE || n == VK_INSERT)
				{
					if (g_InputHelper.IsPressed(n))
					{
						if (n == VK_INSERT) {
							active = false;
							break;
						}

						if (n == VK_ESCAPE && bAllowNone) {
							output.m_Var = 0x0;
							active = false;
							break;
						}

						output.m_Var = n;
						active = false;
						break;
					}
				}
			}
		}

		G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, L"Press a Key");
	}

	else
	{
		time_notactive = I::EngineClient->Time();
		G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, VK2STR(output.m_Var).c_str());
	}

	G::Draw.String(FONT_MENU, x + w + Vars::Menu::SpacingText, y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERY, output.m_szDisplayName);
	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
}

void CMenu::GroupBoxStart()
{
	m_LastGroupBox.x = m_LastWidget.x;
	m_LastGroupBox.y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;

	m_LastWidget.x += Vars::Menu::SpacingX;
	m_LastWidget.y += Vars::Menu::SpacingY * 2;
}

void CMenu::GroupBoxEnd(const wchar_t* Label, int Width)
{
	int h = m_LastWidget.y - m_LastGroupBox.y + m_LastWidget.height + Vars::Menu::SpacingY;

	int label_w, label_h;
	I::MatSystemSurface->GetTextSize(G::Draw.m_Fonts.Element(G::Draw.m_Fonts.Find(FONT_MENU)).m_hFont, Label, label_w, label_h);

	int label_x = m_LastGroupBox.x + Vars::Menu::SpacingText;
	int label_y = m_LastGroupBox.y - (label_h / 2);

	G::Draw.Line(m_LastGroupBox.x, m_LastGroupBox.y, label_x, m_LastGroupBox.y, Vars::Menu::Colors::OutlineMenu);
	G::Draw.Line(label_x + label_w, m_LastGroupBox.y, m_LastGroupBox.x + Width, m_LastGroupBox.y, Vars::Menu::Colors::OutlineMenu);
	G::Draw.Line(m_LastGroupBox.x + Width, m_LastGroupBox.y, m_LastGroupBox.x + Width, m_LastGroupBox.y + h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.Line(m_LastGroupBox.x + Width, m_LastGroupBox.y + h, m_LastGroupBox.x, m_LastGroupBox.y + h, Vars::Menu::Colors::OutlineMenu);
	G::Draw.Line(m_LastGroupBox.x, m_LastGroupBox.y + h, m_LastGroupBox.x, m_LastGroupBox.y, Vars::Menu::Colors::OutlineMenu);

	G::Draw.String(FONT_MENU, label_x, label_y, Vars::Menu::Colors::Text, TXT_DEFAULT, Label);

	m_LastWidget.x -= Vars::Menu::SpacingX;
	m_LastWidget.y += Vars::Menu::SpacingY * 2;
	m_LastGroupBox.height = h;
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

		//Do the About Window
		{
			//Why cant I just use input helper :((((
			{
				static POINT delta;
				static bool drag = false;
				static bool move = false;
				bool held = g_InputHelper.IsHeld(VK_LBUTTON);

				if ((g_InputHelper.m_nMouseX > Vars::Menu::AboutPos.x && g_InputHelper.m_nMouseX < Vars::Menu::AboutPos.x + Vars::Menu::AboutPos.width && g_InputHelper.m_nMouseY > Vars::Menu::AboutPos.y - Vars::Menu::TitleBarH && g_InputHelper.m_nMouseY < Vars::Menu::AboutPos.y - Vars::Menu::TitleBarH + Vars::Menu::TitleBarH) && held)
				{
					drag = true;

					if (!move) {
						delta.x = g_InputHelper.m_nMouseX - Vars::Menu::AboutPos.x;
						delta.y = g_InputHelper.m_nMouseY - Vars::Menu::AboutPos.y;
						move = true;
					}
				}

				if (drag) {
					Vars::Menu::AboutPos.x = g_InputHelper.m_nMouseX - delta.x;
					Vars::Menu::AboutPos.y = g_InputHelper.m_nMouseY - delta.y;
				}

				if (!held) {
					drag = false;
					move = false;
				}
			}

			G::Draw.Rect(
				Vars::Menu::AboutPos.x,
				Vars::Menu::AboutPos.y,
				Vars::Menu::AboutPos.width,
				Vars::Menu::AboutPos.height,
				Vars::Menu::Colors::WindowBackground);

			G::Draw.Rect(
				Vars::Menu::AboutPos.x,
				Vars::Menu::AboutPos.y - Vars::Menu::TitleBarH,
				Vars::Menu::AboutPos.width,
				Vars::Menu::TitleBarH,
				Vars::Menu::Colors::TitleBar);

			G::Draw.String(FONT_MENU,
				Vars::Menu::AboutPos.x + (Vars::Menu::AboutPos.width / 2),
				Vars::Menu::AboutPos.y - (Vars::Menu::TitleBarH / 2),
				Vars::Menu::Colors::Text,
				TXT_CENTERXY,
				L"About");

			//must change with new texture
			static const int texSize = 64;

			G::Draw.Texture(
				Vars::Menu::AboutPos.x,
				Vars::Menu::AboutPos.y - Vars::Menu::TitleBarH,
				Vars::Menu::AboutPos.x + Vars::Menu::AboutPos.width,
				Vars::Menu::AboutPos.y + texSize,
				COLOR_WHITE,
				"logo/inferno_logo_anim"
			);

			G::Draw.String(FONT_MENU,
				Vars::Menu::AboutPos.x + (Vars::Menu::AboutPos.width / 2),
				Vars::Menu::AboutPos.y + texSize + G::Draw.GetFontHeight(FONT_MENU),
				Vars::Menu::Colors::Text,
				TXT_CENTERXY,
				"Registered to: "
				REGISTERED_USER
			);

			G::Draw.String(FONT_MENU,
				Vars::Menu::AboutPos.x + (Vars::Menu::AboutPos.width / 2),
				Vars::Menu::AboutPos.y + texSize + (G::Draw.GetFontHeight(FONT_MENU) * 2),
				Vars::Menu::Colors::Text,
				TXT_CENTERXY,
				"Build: "
				__DATE__
			);

			G::Draw.String(FONT_MENU,
				Vars::Menu::AboutPos.x + (Vars::Menu::AboutPos.width / 2),
				Vars::Menu::AboutPos.y + texSize + (G::Draw.GetFontHeight(FONT_MENU) * 3),
				Vars::Menu::Colors::Text,
				TXT_CENTERXY,
				"Developers: whydoihax"
				);

			G::Draw.String(FONT_MENU,
				Vars::Menu::AboutPos.x + (Vars::Menu::AboutPos.width / 2),
				Vars::Menu::AboutPos.y + Vars::Menu::AboutPos.height - (G::Draw.GetFontHeight(FONT_MENU) * 2),
				{ 65, 65, 65, 255 },
				TXT_CENTERXY,
				L"Source Engine Games Training Software"
			);

			G::Draw.String(FONT_MENU,
				Vars::Menu::AboutPos.x + (Vars::Menu::AboutPos.width / 2),
				Vars::Menu::AboutPos.y + Vars::Menu::AboutPos.height - G::Draw.GetFontHeight(FONT_MENU),
				{ 65, 65, 65, 255 },
				TXT_CENTERXY,
				L"Avalible for free @ unknowncheats.me");
		}

		//Do the Window
		{
			g_InputHelper.Drag(
				g_InputHelper.m_nMouseX,
				g_InputHelper.m_nMouseY,
				Vars::Menu::MenuPos.x,
				Vars::Menu::MenuPos.y,
				Vars::Menu::MenuPos.width,
				Vars::Menu::TitleBarH,
				Vars::Menu::TitleBarH);

			G::Draw.Rect(
				Vars::Menu::MenuPos.x,
				Vars::Menu::MenuPos.y,
				Vars::Menu::MenuPos.width,
				Vars::Menu::MenuPos.height,
				Vars::Menu::Colors::WindowBackground);

			G::Draw.Rect(
				Vars::Menu::MenuPos.x,
				Vars::Menu::MenuPos.y - Vars::Menu::TitleBarH,
				Vars::Menu::MenuPos.width,
				Vars::Menu::TitleBarH,
				Vars::Menu::Colors::TitleBar);

			G::Draw.String(FONT_MENU,
				Vars::Menu::MenuPos.x + (Vars::Menu::MenuPos.width / 2),
				Vars::Menu::MenuPos.y - (Vars::Menu::TitleBarH / 2),
				Vars::Menu::Colors::Text,
				TXT_CENTERXY,
				L"Team Fortress 2");
		}

		//Do the task bar
		{
			G::Draw.Rect(
				0,
				g_Globals.m_nScreenHeight - 40,
				g_Globals.m_nScreenWidht,
				40,
				Vars::Menu::Colors::WindowBackground
			);

			G::Draw.OutlinedRect(
				0,
				g_Globals.m_nScreenHeight - 40,
				g_Globals.m_nScreenWidht,
				40,
				Vars::Menu::Colors::OutlineMenu
			);

			static bool m_bMainButton = false;
			if (TextureButton("vgui/info", 0, g_Globals.m_nScreenHeight - 40, m_bMainButton, 40, 40))
				m_bMainButton = true;
			else
				m_bMainButton = false;

			if (m_bMainButton)
			{
				G::Draw.Rect(
					0,
					g_Globals.m_nScreenHeight - 240,
					300,
					200,
					Vars::Menu::Colors::WindowBackground
				);

				G::Draw.OutlinedRect(
					0,
					g_Globals.m_nScreenHeight - 240,
					300,
					200,
					Vars::Menu::Colors::OutlineMenu
				);

				G::Draw.Texture(
					0,
					g_Globals.m_nScreenHeight - 240,
					80,
					g_Globals.m_nScreenHeight - 160,
					COLOR_WHITE,
					"hud/afterburn"
				);

				G::Draw.OutlinedRect(
					0,
					g_Globals.m_nScreenHeight - 240,
					80,
					80,
					Vars::Menu::Colors::OutlineMenu
				);

				G::Draw.OutlinedRect(
					82,
					g_Globals.m_nScreenHeight - 236,
					216,
					G::Draw.GetFontHeight(EFonts::MENU_CONSOLAS) * 2,
					Vars::Menu::Colors::OutlineMenu
				);

				G::Draw.OutlinedRect(
					82,
					g_Globals.m_nScreenHeight - 210,
					216,
					G::Draw.GetFontHeight(EFonts::MENU_CONSOLAS) * 2,
					Vars::Menu::Colors::OutlineMenu
				);

				G::Draw.OutlinedRect(
					82,
					g_Globals.m_nScreenHeight - 184,
					216,
					G::Draw.GetFontHeight(EFonts::MENU_CONSOLAS) * 2,
					Vars::Menu::Colors::OutlineMenu
				);

				G::Draw.String(EFonts::MENU_CONSOLAS,
					86,
					g_Globals.m_nScreenHeight - 236 + G::Draw.GetFontHeight(EFonts::MENU_CONSOLAS),
					Vars::Menu::Colors::Text,
					TXT_CENTERY,
					L"Unknown User"
				);

				G::Draw.String(EFonts::MENU_CONSOLAS,
					86,
					g_Globals.m_nScreenHeight - 210 + G::Draw.GetFontHeight(EFonts::MENU_CONSOLAS),
					{ 70, 70, 70, 255 },
					TXT_CENTERY,
					L"<Custom Name>"
				);

				G::Draw.String(EFonts::MENU_CONSOLAS,
					86,
					g_Globals.m_nScreenHeight - 184 + G::Draw.GetFontHeight(EFonts::MENU_CONSOLAS),
					{ 70, 70, 70, 255 },
					TXT_CENTERY,
					L"<Disconnect Reason>"
				);

				m_LastWidget = { 0, g_Globals.m_nScreenHeight - 160 };

				//TODO: Add functionality to these
				Button(L"CL_FullUpdate", false, 0, 0, 2);
				Button(L"SND_Restart", false, 0, 0, 2);
				Button(L"StopSound", false, 0, 0, 2);
				Button(L"Status", false, 0, 0, 2);
				Button(L"Retry", false, 0, 0, 2);
			}
		}
	}

	I::MatSystemSurface->DrawSetAlphaMultiplier(1.0f);
}