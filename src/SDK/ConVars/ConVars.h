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
	ConVar* tf_weapon_criticals_bucket_default = nullptr;
	ConVar* tf_weapon_criticals_bucket_bottom = nullptr;
};

namespace G { inline CConVars ConVars; }