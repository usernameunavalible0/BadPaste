#pragma once

template <class T>
class CVar
{
public:
	T m_Var;
	const wchar_t* m_szDisplayName;
};

namespace Vars
{
	namespace Menu
	{
		inline Rect_t Position = { 100, 100, 600, 420 };
		inline const wchar_t* Title = L"Team Fortress 2";
		inline int TitleBarH = 16;

		inline int SpacingX = 8;
		inline int SpacingY = 4;
		inline int SpacingText = 4;

		inline int AltSpacingX = 2;
		inline int AltSpacingY = 2;

		inline int CheckBoxW = 14;
		inline int CheckBoxH = 14;
		inline int CheckBoxFillSize = 3;

		inline int ButtonW = 90;
		inline int ButtonH = 20;

		inline int ButtonWSmall = 70;
		inline int ButtonHSmall = 16;

		inline int ComboBoxW = 90;
		inline int ComboBoxH = 14;

		inline int InputBoxW = 90;
		inline int InputBoxH = 14;
		inline int InputColorBoxW = 30;

		namespace Colors
		{
			inline Color WindowBackground = { 7, 7, 7, 255 };
			inline Color TitleBar = { 7, 7, 7, 255 };
			inline Color Text = { 255, 255, 255, 190 };
			inline Color Widget = { 214, 19, 58, 255 };
			inline Color WidgetActive = { 255, 26, 72, 255 };
			inline Color OutlineMenu = { 112, 10, 31, 255 };
		}
	}

	namespace ESP
	{
		inline CVar<bool> Enabled{ true, L"Enabled" };

		namespace Players
		{
			inline CVar<bool> Enabled{ true, L"Enabled" };
			inline CVar<bool> IgnoreTeam{ true, L"Ignore Team" };
			inline CVar<bool> ActiveWeapon{ false, L"Active Weapon" };
			inline CVar<bool> HealthText{ true, L"Health Text" };
			inline CVar<bool> Name{ true, L"Name" };
		}

		namespace Buildings
		{
			inline CVar<bool> Enabled{ true, L"Enabled" };
			inline CVar<bool> IgnoreTeam{ false, L"Ignore Team" };
			inline CVar<bool> HealthText{ true, L"Health Text" };
			inline CVar<bool> Name{ true, L"Type" };
			inline CVar<bool> Info{ true, L"Type" };
		}
	}

	namespace Visual
	{
		inline CVar<bool> RemoveVisualRecoil{ true, L"Remove Visual Recoil" };
		inline CVar<bool> Tracers{ false, L"Tracers" };
		inline CVar<bool> Snow{ false, L"Menu Snow" };
		inline CVar<bool> ToolTips{ false, L"Menu tooltips" };
	}

	namespace Misc
	{
		inline CVar<bool> Bunnyhop{ true, L"Bunnyhop" };
		inline CVar<bool> BypassPure{ true, L"Bypass Pure" };
	}
}
