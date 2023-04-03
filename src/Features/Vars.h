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

	namespace Aimbot
	{
		namespace Global
		{
			inline CVar<bool> Active{ true, L"Active" };
			inline CVar<int> AimKey{ VK_LSHIFT, L"Aim Key" };
			inline CVar<bool> AutoShoot{ true, L"Auto Shoot" };
			inline CVar<bool> AimPlayers{ true, L"Aim Players" };
			inline CVar<bool> AimBuildings{ true, L"Aim Buildings" };
			inline CVar<bool> IgnoreInvlunerable{ true, L"Ignore Invulnerable" };
			inline CVar<bool> IgnoreCloaked{ false, L"Ignore Cloaked" };
			inline CVar<bool> IgnoreFriends{ true, L"Ignore Friends" };
			inline CVar<bool> IgnoreTaunting{ true, L"Ignore Taunting" };
		}

		namespace Melee
		{
			inline CVar<bool> Active{ true, L"Active" };
			inline CVar<int> SortMethod{ 1, L"Sort Method" };	//0 - FOV,		1 - Distance
			inline CVar<int> AimMethod{ 1, L"Aim Method" };	//0 - Normal,	1 - Smooth, 2 - Silent
			inline CVar<float> AimFOV{ 45.0f, L"Aim FOV" };
			inline CVar<float> SmoothingAmount{ 8.0f, L"Smoothing Amount" };
			inline CVar<bool> RangeCheck{ true, L"Range Check" };
			inline CVar<bool> PredictSwing{ true, L"Predict Swing" };
			inline CVar<bool> WhipTeam{ true, L"Whip Teammates" };
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
