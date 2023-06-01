#include "../Hooks.h"

#include "../../Features/Vars.h"

DEFINE_HOOK(CTFViewModel_CalcViewModelView, void, __fastcall, void* ecx, void* edx, C_BasePlayer* owner, const Vector& eyePosition, const QAngle& eyeAngles)
{
	C_TFPlayer* pLocal = G::EntityCache.GetLocal();

	if (pLocal && !pLocal->deadflag() && Vars::Visual::ViewModel_Active.m_Var)
	{
		Vector vForward, vRight, vUp;
		AngleVectors(eyeAngles, &vForward, &vRight, &vUp);

		Vector vEyePosition = eyePosition + (
			(vForward * Vars::Visual::ViewModel_Position_Offset_Forward.m_Var) +
			(vRight * Vars::Visual::ViewModel_Position_Offset_Right.m_Var) +
			(vUp * Vars::Visual::ViewModel_Position_Offset_Up.m_Var)
			);

		Func.Original<FN>()(ecx, edx, owner, vEyePosition, eyeAngles);
	}
	else
	{
		Func.Original<FN>()(ecx, edx, owner, eyePosition, eyeAngles);
	}
}