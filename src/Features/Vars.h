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
			inline Color WindowBackground = { 13, 13, 13, 255 };
			inline Color TitleBar = { 13, 13, 13, 255 };
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
			inline CVar<bool> Enabled{ true, L"Enabled" };
			inline CVar<int> AimKey { VK_XBUTTON2, L"Aim Key"};
			inline CVar<bool> AutoShoot{ true, L"Auto Shoot"};
			inline CVar<bool> AimPlayers{ true, L"Aim Players"};
			inline CVar<bool> AimBuildings{ true, L"Aim Buildings"};
			inline CVar<bool> IgnoreInvulnerable{ true, L"Ignore Invulnerable"};
			inline CVar<bool> IgnoreCloaked{ true, L"Ignore Cloaked"};
			inline CVar<bool> IgnoreTaunting{true, L"Ignore Taunting"};
			inline CVar<bool> IgnoreFriends{ true, L"Ignore Friends"};
		}

		namespace Hitscan
		{
			inline CVar<bool> Enabled{ true, L"Enabled"};
			inline CVar<int> SortMethod{ 0, L"Sort"};
			inline CVar<float> AimFOV{ 15.f, L"FOV"};
			inline CVar<int> AimHitbox{ 2, L"Hitbox"};
			inline CVar<int> AimMethod{ 2, L"Aim Type"};
			inline CVar<bool> ScopedOnly{false, L"Scoped Only"};
			inline CVar<bool> SpectatedSmooth{ true, L"Spectated Smooth"};
			inline CVar<int> SmoothingAmount{ 4, L"Smoothing"};
			inline CVar<bool> WaitForHeadshot{ true, L"Wait For Headshot"};
			inline CVar<bool> WaitForCharge{ true, L"Wait For Charge"};
		}
	}

	namespace Auto
	{
		inline CVar<int> AutoBackstab{2, L"Auto Backstab"}; //0 = Disabled, 1 = Legit, 2 = Rage
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
			inline CVar<bool> Info{ true, L"Info" };
		}
	}

	namespace Visual
	{
		inline CVar<bool> RemoveVisualRecoil{ true, L"Remove Visual Recoil" };
		inline CVar<bool> Tracers{ false, L"Tracers" };
		inline CVar<bool> Snow{ false, L"Menu Snow" };
		inline CVar<bool> ToolTips{ false, L"Menu tooltips" };
		inline CVar<bool> RemoveScope{ true, L"Remove Scope"};
		inline CVar<bool> RemoveZoom{ true, L"Remove Zoom"};
		inline CVar<int> FOV{ 90, L"FOV"};
		inline CVar<bool> Thirdperson{ true, L"Enabled"};
		inline CVar<int> ThirdpersonKey{ 'B', L"Toggle Key"};
		inline CVar<bool> ViewModelSway{ true, L"View Model Sway"};
		inline CVar<bool> ViewModel_Active{ true, L"ViewModel Offset"};
		inline CVar<int> ViewModel_Position_Offset_Forward{ 0, L"Forward/Backward Offset"};
		inline CVar<int> ViewModel_Position_Offset_Right{ 0, L"Left/Right Offset"};
		inline CVar<int> ViewModel_Position_Offset_Up{ 0, L"Up/Down Offset"};
		inline CVar<bool> ModulateWorld{ true, L"ModulateWorld"};
	}

	namespace Chams
	{
		inline CVar<bool> Enabled{ true, L"Enabled" };
		
		namespace Players
		{
			inline CVar<bool> Enabled{ true, L"Enabled" };
			inline CVar<int> Material{ 1, L"Material" }; // 0=Off, 1=Fresnel, 2=Glow, 3=Test, 4=Shaded, 5=Toxic
			inline CVar<bool> IgnoreTeam{ true, L"Ignore Team" };

			namespace FresnelVars
			{
				inline CVar<float> FresnelX{ 0.0, L"Fresnel X" };
				inline CVar<float> FresnelY{ 1.0, L"Fresnel Y" };
				inline CVar<float> FresnelZ{ 6.0, L"Fresnel Z" };
				inline Color BaseColor = COLOR_WHITE;
				inline Color GlowColor = COLOR_WHITE;
			}
		}
	}

	namespace Misc
	{
		inline CVar<bool> Bunnyhop{ true, L"Bunnyhop" };
		inline CVar<bool> BypassPure{ true, L"Bypass Pure" };
		inline CVar<bool> ForceMeleeCrits{ true, L"Force Melee Crits" };
	}
}
