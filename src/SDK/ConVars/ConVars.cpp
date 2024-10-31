#include "ConVars.h"

#define _CHECKVAR(x) if (x) { Error(#x); return false; }
#define CHECKVAR(x) _CHECKVAR(x == nullptr)

bool CConVars::Initialize()
{
	cl_interp = I::Cvar->FindVar("cl_interp");
	CHECKVAR(cl_interp);

	cl_interp_ratio = I::Cvar->FindVar("cl_interp_ratio");
	CHECKVAR(cl_interp_ratio);

	cl_updaterate = I::Cvar->FindVar("cl_updaterate");
	CHECKVAR(cl_updaterate);

	cl_wpn_sway_interp = I::Cvar->FindVar("cl_wpn_sway_interp");
	CHECKVAR(cl_wpn_sway_interp);

	sv_unlag = I::Cvar->FindVar("sv_unlag");
	CHECKVAR(sv_unlag);

	sv_lagcompensation_teleport_dist = I::Cvar->FindVar("sv_lagcompensation_teleport_dist");
	CHECKVAR(sv_lagcompensation_teleport_dist);

	sv_maxunlag = I::Cvar->FindVar("sv_maxunlag");
	CHECKVAR(sv_maxunlag);

	sv_unlag_fixstuck = I::Cvar->FindVar("sv_unlag_fixstuck");
	CHECKVAR(sv_unlag_fixstuck);

	host_limitlocal = I::Cvar->FindVar("host_limitlocal");
	CHECKVAR(host_limitlocal);

	cl_cmdrate = I::Cvar->FindVar("cl_cmdrate");
	CHECKVAR(cl_cmdrate);

	sv_gravity = I::Cvar->FindVar("sv_gravity");
	CHECKVAR(sv_gravity);

	cl_flipviewmodels = I::Cvar->FindVar("cl_flipviewmodels");
	CHECKVAR(cl_flipviewmodels);

	sv_clockcorrection_msecs = I::Cvar->FindVar("sv_clockcorrection_msecs");
	CHECKVAR(sv_clockcorrection_msecs);

	//If we arrive here, return success.
	return true;
}