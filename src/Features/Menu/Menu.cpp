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

bool CFeatures_Menu::InputColorNew(Color& Var, bool AllowAlpha)
{
	return false;
}

bool CFeatures_Menu::InputString(const wchar_t* Label, std::wstring& output)
{
	bool callback = false;

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::InputBoxW;
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

			for (int16_t key = L'0'; key < L'9' + 1; key++)
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

		G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, "%ws", active_str.empty() ? L"Enter a FriendID" : active_str.c_str());
	}

	else
	{
		active_str = {};
		G::Draw.String(FONT_MENU, x + (w / 2), y + (h / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, "%ws", Label);
	}

	G::Draw.OutlinedRect(x, y, w, h, Vars::Menu::Colors::OutlineMenu);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = h;

	return callback;
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

void CFeatures_Menu::MultiSelect(bool* Var, const wchar_t* Title, const std::vector<CVar<bool>>& List, bool& bOpened)
{
	int ListAmount = List.size();

	int x = m_LastWidget.x;
	int y = m_LastWidget.y + m_LastWidget.height + Vars::Menu::SpacingY;
	int w = Vars::Menu::ComboBoxW;
	int h1 = Vars::Menu::ComboBoxH;

	auto GetListTotalHeight = [&]() -> int
	{
		int nHeight = h1;

		for (size_t n = 0; n < List.size(); n++)
		{
			nHeight += G::Draw.GetFontHeight(FONT_MENU) + 4;
		}

		return nHeight;
	};

	auto GetActiveItemsStr = [&]() -> std::wstring
	{
		std::wstring result = L"";
		const wchar_t* seperator = L", ";

		int nCount = 0;

		for (size_t n = 0; n < List.size(); n++)
		{
			if (List[n].m_Var == true)
			{
				result += List[n].m_szDisplayName;
				result += seperator;
				nCount++;
			}
		}

		if (nCount == 1)
		{
			result.erase(result.length() - 2);
		}

		int txtWidth, txtHeight;
		I::MatSystemSurface->GetTextSize(G::Draw.m_Fonts.Element(G::Draw.m_Fonts.Find(FONT_MENU)).m_hFont, result.c_str(), txtWidth, txtHeight);

		while (txtWidth > w)
		{
			result.erase(result.length() - 1);
			I::MatSystemSurface->GetTextSize(G::Draw.m_Fonts.Element(G::Draw.m_Fonts.Find(FONT_MENU)).m_hFont, result.c_str(), txtWidth, txtHeight);
		}

		return result;
	};

	if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > y && g_InputHelper.m_nMouseY < y + h1)
	{
		if (!bOpened)
		{
			if (g_InputHelper.IsPressed(VK_LBUTTON))
				bOpened = true;

			G::Draw.GradientRect(x, y, x + w, y + h1, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, false);
		}
		else if (bOpened)
		{
			if (g_InputHelper.IsPressed(VK_LBUTTON))
				bOpened = false;
		}
	}
	if (bOpened)
	{
		int nDrawY = y + h1;
		static int FontHeight = G::Draw.GetFontHeight(FONT_MENU) + 4;

		for (size_t n = 0; n < List.size(); n++)
		{
			if (g_InputHelper.m_nMouseX > x && g_InputHelper.m_nMouseX < x + w && g_InputHelper.m_nMouseY > nDrawY && g_InputHelper.m_nMouseY < nDrawY + FontHeight)
			{
				if (g_InputHelper.IsPressed(VK_LBUTTON))
					Var[n] = !Var[n];

				G::Draw.GradientRect(x, nDrawY, x + w, nDrawY + FontHeight, Vars::Menu::Colors::WidgetActive, Vars::Menu::Colors::Widget, false);
			}

			G::Draw.OutlinedRect(x, nDrawY, w, FontHeight, Vars::Menu::Colors::OutlineMenu);
			std::wstring concatStr = Var[n] ? L"[x] " : L"[ ] ";
			concatStr.append(List[n].m_szDisplayName);
			G::Draw.String(FONT_MENU, x + (w / 2), nDrawY + (FontHeight / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, concatStr.c_str());
			nDrawY += FontHeight - 1;
		}

		nDrawY -= FontHeight;

		if (g_InputHelper.m_nMouseX < x && g_InputHelper.m_nMouseX > x + w && g_InputHelper.m_nMouseY < y && g_InputHelper.m_nMouseY > y + GetListTotalHeight())
			bOpened = false;
	}
	else
		bOpened = false;

	G::Draw.OutlinedRect(x, y, w, h1 + 1, Vars::Menu::Colors::OutlineMenu);
	if (GetActiveItemsStr() == L"")
		G::Draw.String(FONT_MENU, x + (w / 2), y + (h1 / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, L"None");
	else
		G::Draw.String(FONT_MENU, x + (w / 2), y + (h1 / 2), Vars::Menu::Colors::Text, TXT_CENTERXY, GetActiveItemsStr().c_str());

	G::Draw.String(FONT_MENU, x + w + Vars::Menu::SpacingText, y + (h1 / 2), Vars::Menu::Colors::Text, TXT_CENTERY, Title);

	m_LastWidget.x = x;
	m_LastWidget.y = y;
	m_LastWidget.width = w;
	m_LastWidget.height = bOpened ? GetListTotalHeight() : h1;
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
			case EMainTabs::TAB_AIM:
			{
				m_LastWidget.y += Vars::Menu::SpacingY;
				Rect_t checkpoint = m_LastWidget;
				GroupBoxStart();
				{
					CheckBox(Vars::Aimbot::Global::Enabled, L"Master aimbot switch");
					CheckBox(Vars::Aimbot::Global::AutoShoot, L"Should shoot while aimbotting");
					InputKey(Vars::Aimbot::Global::AimKey, L"Key that enabled the aimbot");
					static bool bOpened1 = false;
					MultiSelect(Vars::Aimbot::Global::Targets, L"Targets", {
						{Vars::Aimbot::Global::Targets[0], L"Players"},
						{Vars::Aimbot::Global::Targets[1], L"Buildings"}
						}, bOpened1);
					static bool bOpened2 = false;
					MultiSelect(Vars::Aimbot::Global::Ignores, L"Ignore", {
						{Vars::Aimbot::Global::Ignores[0], L"Invulnerable"},
						{Vars::Aimbot::Global::Ignores[1], L"Cloaked"},
						{Vars::Aimbot::Global::Ignores[2], L"Taunting"},
						{Vars::Aimbot::Global::Ignores[3], L"Friends"}
						}, bOpened2);
				}
				GroupBoxEnd(L"Global", 160);

				GroupBoxStart();
				{
					CheckBox(Vars::Aimbot::Melee::Enabled, L"Melee aimbot toggle");
					ComboBox(Vars::Aimbot::Melee::AimMethod, { {0, L"Plain"}, {1, L"Smooth"}, {2, L"Silent"} });
					ComboBox(Vars::Aimbot::Melee::SortMethod, { {0, L"FOV"}, {1, L"Distance"} });
					InputFloat(Vars::Aimbot::Melee::AimFOV, 1.0f, 180.0f, 1.0f, L"%.0f");
					InputFloat(Vars::Aimbot::Melee::SmoothingAmount, 1.0f, 10.0f, 1.0f, L"%.0f");
					CheckBox(Vars::Aimbot::Melee::RangeCheck, L"Should we check if target is in melee range");
					CheckBox(Vars::Aimbot::Melee::WhipTeam, L"Should we whip teammates with the Disciplinary Action");
				}
				GroupBoxEnd(L"Melee", 160);

				checkpoint.x += 160 + Vars::Menu::SpacingX;
				m_LastWidget = checkpoint;

				GroupBoxStart();
				{
					CheckBox(Vars::Aimbot::Hitscan::Enabled, L"Hitscan aimbot toggle");
					ComboBox(Vars::Aimbot::Hitscan::AimMethod, { {0, L"Plain"}, {1, L"Smooth"}, {2, L"Silent"} });
					ComboBox(Vars::Aimbot::Hitscan::PreferedHitbox, { {0, L"Head"}, {1, L"Body"}, {2, L"Auto"} });
					ComboBox(Vars::Aimbot::Hitscan::SortMethod, { {0, L"FOV"}, {1, L"Distance"} });
					static bool bOpened3 = false;
					MultiSelect(Vars::Aimbot::Hitscan::ScanHitboxes, L"Scan", {
						{Vars::Aimbot::Hitscan::ScanHitboxes[0], L"HEAD"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[1], L"NECK"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[2], L"LOWER_NECK"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[3], L"PELVIS"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[4], L"BODY"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[5], L"THORAX"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[6], L"CHEST"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[7], L"UPPER_CHEST"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[8], L"RIGHT_THIGH"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[9], L"LEFT_THIGH"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[10], L"RIGHT_CALF"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[11], L"LEFT_CALF"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[12], L"RIGHT_FOOT"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[13], L"LEFT_FOOT"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[14], L"RIGHT_HAND"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[15], L"LEFT_HAND"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[16], L"RIGHT_UPPER_ARM"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[17], L"LEFT_UPPER_ARM"},
						{Vars::Aimbot::Hitscan::ScanHitboxes[18], L"LEFT_FOREARM"}
						}, bOpened3);
					InputFloat(Vars::Aimbot::Hitscan::AimFOV, 1.0f, 180.0f, 1.0f, L"%.0f");
					InputFloat(Vars::Aimbot::Hitscan::SmoothingAmount, 1.0f, 10.0f, 1.0f, L"%.0f");
					CheckBox(Vars::Aimbot::Hitscan::SpectatedSmooth, L"Should we change aim type to smooth when being spectated");
					CheckBox(Vars::Aimbot::Hitscan::WaitForHeadshot, L"Should we wait until we can headshot the target");
					CheckBox(Vars::Aimbot::Hitscan::WaitForCharge, L"Should we wait until we are charged enough to kill the target");
				}
				GroupBoxEnd(L"Hitscan", 160);

				checkpoint.x += 160 + Vars::Menu::SpacingX;
				m_LastWidget = checkpoint;

				GroupBoxStart();
				{

				}
				GroupBoxEnd(L"Projectile", 160);

				break;
			}
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
						CheckBox(Vars::ESP::Outlines, L"ESP Outline Master Toggle");
					}
					GroupBoxEnd(L"Main", 160);

					GroupBoxStart();
					{
						CheckBox(Vars::ESP::Players::Enabled, L"Player ESP Toggle");
						CheckBox(Vars::ESP::Players::IgnoreTeam, L"Ignore the local team");
						CheckBox(Vars::ESP::Players::ActiveWeapon, L"Active Weapon text");
						CheckBox(Vars::ESP::Players::HealthText, L"Player Health Text");
						CheckBox(Vars::ESP::Players::HealthBar, L"Player Health Bar");
						CheckBox(Vars::ESP::Players::Box, L"2D Player Box");
						//CheckBox(Vars::ESP::Players::ThreeDBox, L"3D Player Box");
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
					Rect_t checkpoint = m_LastWidget;

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

					Rect_t checkpoint1 = m_LastWidget;
					checkpoint.x += 160 + Vars::Menu::SpacingX;
					m_LastWidget = checkpoint;

					GroupBoxStart();
					{
						CheckBox(Vars::Chams::ViewModel::Enabled, L"Player ViewModel Chams Toggle");
						ComboBox(Vars::Chams::ViewModel::Material, { { 0, L"Shaded" } });
					}
					GroupBoxEnd(L"ViewModel", 160);

					GroupBoxStart();
					{
						CheckBox(Vars::Chams::World::Healthpacks, L"World Healthpack Chams Toggle");
						CheckBox(Vars::Chams::World::Ammopacks, L"World Ammopack Chams Toggle");
					}
					GroupBoxEnd(L"World", 160);

					if (Vars::Chams::Players::Material.m_Var == 1)
					{
						m_LastWidget = checkpoint1;

						GroupBoxStart();
						{
							InputColor(Vars::Chams::Players::FresnelVars::Enemies::BaseColor, L"Base Color");
							InputColor(Vars::Chams::Players::FresnelVars::Enemies::GlowColor, L"Glow Color");
							InputFloat(Vars::Chams::Players::FresnelVars::Enemies::FresnelX, -10.f, 10.f, 0.1f, L"%.1f");
							InputFloat(Vars::Chams::Players::FresnelVars::Enemies::FresnelY, -10.f, 10.f, 0.1f, L"%.1f");
							InputFloat(Vars::Chams::Players::FresnelVars::Enemies::FresnelZ, -10.f, 10.f, 0.1f, L"%.1f");
						}
						GroupBoxEnd(L"Fresnel Vars (Enemies)", 210);

						checkpoint1.x += 210 + Vars::Menu::SpacingX;
						m_LastWidget = checkpoint1;

						GroupBoxStart();
						{
							InputColor(Vars::Chams::Players::FresnelVars::Teammates::BaseColor, L"Base Color");
							InputColor(Vars::Chams::Players::FresnelVars::Teammates::GlowColor, L"Glow Color");
							InputFloat(Vars::Chams::Players::FresnelVars::Teammates::FresnelX, -10.f, 10.f, 0.1f, L"%.1f");
							InputFloat(Vars::Chams::Players::FresnelVars::Teammates::FresnelY, -10.f, 10.f, 0.1f, L"%.1f");
							InputFloat(Vars::Chams::Players::FresnelVars::Teammates::FresnelZ, -10.f, 10.f, 0.1f, L"%.1f");
						}
						GroupBoxEnd(L"Fresnel Vars (Teammates)", 210);
					}

					break;
				}

				case EVisualsTabs::TAB_GLOW:
				{
					Rect_t checkpoint = m_LastWidget;

					GroupBoxStart();
					{
						CheckBox(Vars::Glow::Enabled, L"Glow Master Toggle");
					}
					GroupBoxEnd(L"Main", 185);

					GroupBoxStart();
					{
						CheckBox(Vars::Glow::Players::Enabled, L"Player Glow Toggle");
						CheckBox(Vars::Glow::Players::IgnoreTeam, L"Ignore the Local Team");
						ComboBox(Vars::Glow::Players::Style, { { 0, L"Blur" } });
						InputFloat(Vars::Glow::Players::BloomAmount, 1.0f, 10.0f);
						ComboBox(Vars::Glow::Players::GlowColorMode, { { 0, L"Custom" }, { 1, L"Rainbow" }, { 2, L"Health" } });
						if (!Vars::Glow::Players::GlowColorMode.m_Var)
							InputColor(Vars::Glow::Players::GlowColor, L"Color");
					}
					GroupBoxEnd(L"Players", 185);

					checkpoint.x += 185 + Vars::Menu::SpacingX;
					m_LastWidget = checkpoint;

					GroupBoxStart();
					{
						CheckBox(Vars::Glow::World::Healthpacks, L"Health pack glow toggle");
						CheckBox(Vars::Glow::World::Ammopacks, L"Ammo pack glow toggle");
					}
					GroupBoxEnd(L"World", 185);

					break;
				}

				case EVisualsTabs::TAB_OTHER:
				{
					Rect_t checkpoint = m_LastWidget;

					GroupBoxStart();
					{
						CheckBox(Vars::Visual::RemoveVisualRecoil, L"Removes visual weapon recoil");
						ComboBox(Vars::Visual::Tracers, { { 0, L"Default" }, { 1, L"Disabled" }, { 2, L"MERASMUS!!" } });
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

				Rect_t checkpoint = m_LastWidget;

				GroupBoxStart();
				{
					CheckBox(Vars::Misc::ForceMeleeCrits, L"Forces criticals on melee weapons");
					CheckBox(Vars::Misc::Bunnyhop, L"Automatic jumping");
					CheckBox(Vars::Misc::BypassPure, L"Bypass sv_pure");
				}
				GroupBoxEnd(L"Main", 160);

				GroupBoxStart();
				{
					CheckBox(Vars::AntiHack::AntiAim::Enabled, L"Anti-Aim master switch");
					ComboBox(Vars::AntiHack::AntiAim::Pitch, { { 0, L"None" }, { 1, L"Up" }, { 2, L"Down" }, { 3, L"Fake Up" }, { 4, L"Fake Down" } });
					ComboBox(Vars::AntiHack::AntiAim::YawReal, { { 0, L"None" }, { 1, L"Left" }, { 2, L"Right" }, { 3, L"Backwards" }, { 4, L"Emotion" }, { 5, L"Random" } });
					ComboBox(Vars::AntiHack::AntiAim::YawFake, { { 0, L"None" }, { 1, L"Left" }, { 2, L"Right" }, { 3, L"Backwards" }, { 4, L"Random" } });
					CheckBox(Vars::AntiHack::AntiAim::DrawFakeAngles, L"Toggle Anti-Aim Fake Angle Chams");
					CheckBox(Vars::AntiHack::Fakelag, L"Fakelag master switch");
				}
				GroupBoxEnd(L"Anti-Aim", 160);

				GroupBoxStart();
				{
					CheckBox(Vars::AntiHack::Resolver::Enabled, L"Resolver master switch");
					ComboBox(Vars::AntiHack::Resolver::Mode, { { 0, L"Auto" }, { 1, L"Manual" } });
					InputFloat(Vars::AntiHack::Resolver::ManualPitch, -89.f, 89.f);
					InputFloat(Vars::AntiHack::Resolver::ManualYaw, -180.f, 180.f);
				}
				GroupBoxEnd(L"Resolver", 160);

				checkpoint.x += 160 + Vars::Menu::SpacingX;
				m_LastWidget = checkpoint;

				GroupBoxStart();
				{
					ComboBox(Vars::Auto::AutoBackstab, { {0, L"Disabled"}, {1, L"Legit"}, {2, L"Rage"} });
					ComboBox(Vars::Auto::AutoHeal, { {0, L"Disabled"}, {1, L"All"}, {2, L"Friends Only"} });
					std::wstring friendid = std::to_wstring(Vars::Misc::FollowFriendID.m_Var);
					if (InputString(std::to_wstring(Vars::Misc::FollowFriendID.m_Var).c_str(), friendid))
					{
						wchar_t* endPtr;
						uint32 value = std::wcstoul(friendid.c_str(), &endPtr, 10);
						Vars::Misc::FollowFriendID.m_Var = value;
					}
				}
				GroupBoxEnd(L"Auto", 180);

				break;
			}

			default: break;
			}
		}
	}

    I::MatSystemSurface->DrawSetAlphaMultiplier(1.0f);
}
