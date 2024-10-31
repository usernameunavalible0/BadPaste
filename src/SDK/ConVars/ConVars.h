#pragma once

#include "../Interfaces/Interfaces.h"

class CConVars
{
public:
	bool Initialize();

public:
	ConVar* cl_interp = nullptr;
	ConVar* cl_interp_ratio = nullptr;
	ConVar* cl_updaterate = nullptr;
	ConVar* cl_wpn_sway_interp = nullptr;
	ConVar* sv_unlag = nullptr;
	ConVar* sv_lagcompensation_teleport_dist = nullptr;
	ConVar* sv_maxunlag = nullptr;
	ConVar* sv_unlag_fixstuck = nullptr;
	ConVar* host_limitlocal = nullptr;
	ConVar* cl_cmdrate = nullptr;
	ConVar* sv_gravity = nullptr;
	ConVar* cl_flipviewmodels = nullptr;
	ConVar* sv_clockcorrection_msecs = nullptr;
};

namespace G { inline CConVars ConVars; }