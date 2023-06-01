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
};

namespace G { inline CConVars ConVars; }