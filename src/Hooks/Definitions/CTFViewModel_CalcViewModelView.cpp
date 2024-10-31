#include "../Hooks.h"

#include "../../Features/Vars.h"

DEFINE_HOOK(CTFViewModel_CalcViewModelView, void, __fastcall, void* ecx, void* edx, C_BasePlayer* owner, const Vector& eyePosition, const QAngle& eyeAngles)
{
	C_TFPlayer* pLocal = G::EntityCache.GetLocal();

	if (pLocal && !pLocal->deadflag())
	{
		Vector vEyePosition = eyePosition;

		if (Vars::Visual::ViewModelOffset.m_Var)
		{
			Vector vForward, vRight, vUp;
			AngleVectors(eyeAngles, &vForward, &vRight, &vUp);

			vEyePosition += (
				(vForward * Vars::Visual::ViewModel_Position_Offset_Forward.m_Var) +
				(vRight * Vars::Visual::ViewModel_Position_Offset_Right.m_Var) +
				(vUp * Vars::Visual::ViewModel_Position_Offset_Up.m_Var)
				);
		}

		QAngle vEyeAngles = eyeAngles;

		if (Vars::Visual::ViewModelAngle.m_Var)
		{
			vEyeAngles += QAngle(Vars::Visual::ViewModelAnglePitch.m_Var, Vars::Visual::ViewModelAngleYaw.m_Var, Vars::Visual::ViewModelAngleRoll.m_Var);
		}

		Func.Original<FN>()(ecx, edx, owner, vEyePosition, vEyeAngles);
	}
}