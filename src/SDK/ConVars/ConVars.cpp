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

	tf_weapon_criticals_bucket_default = I::Cvar->FindVar("tf_weapon_criticals_bucket_default");
	CHECKVAR(tf_weapon_criticals_bucket_default);

	tf_weapon_criticals_bucket_bottom = I::Cvar->FindVar("tf_weapon_criticals_bucket_bottom");
	CHECKVAR(tf_weapon_criticals_bucket_bottom);

	//If we arrive here, return success.
	return true;
}