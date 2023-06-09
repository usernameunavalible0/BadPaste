#pragma once

#include "common/GameUI/IGameConsole.h"
#include "common/GameUI/IGameUI.h"
#include "common/engine_launcher_api.h"
#include "common/igameserverdata.h"
#include "common/info_key.h"
#include "common/IRunGameEngine.h"
#include "common/IVGuiModuleLoader.h"
#include "common/qlimits.h"
#include "common/ValveCDKeyGameAndTerritoryCodes.h"
#include "common/protocol.h"
#include "common/netmessages.h"

#include "engine/client.h"

#include "game/client/input.h"
#include "game/client/glow_outline_effect.h"
#include "game/client/cdll_util.h"
#include "game/client/clientmode_shared.h"
#include "game/client/iviewrender.h"
#include "game/client/prediction.h"
#include "game/client/c_team.h"
#include "game/client/tf/bot_npc/c_bot_npc.h"
#include "game/client/tf/bot_npc/c_bot_npc_minion.h"
#include "game/client/tf/bot_npc/map_entities/c_tf_bot_hint_engineer_nest.h"
#include "game/client/tf/c_entity_currencypack.h"
#include "game/client/tf/c_func_capture_zone.h"
#include "game/client/tf/c_obj_dispenser.h"
#include "game/client/tf/c_obj_sentrygun.h"
#include "game/client/tf/c_obj_teleporter.h"
#include "game/client/tf/c_tf_ammo_pack.h"
#include "game/client/tf/c_tf_player.h"
#include "game/client/tf/halloween/c_eyeball_boss.h"
#include "game/client/tf/halloween/c_headless_hatman.h"
#include "game/client/tf/halloween/c_merasmus.h"
#include "game/client/tf/halloween/c_merasmus_dancer.h"
#include "game/client/tf/halloween/c_zombie.h"
#include "game/client/tf/player_vs_environment/c_boss_alpha.h"
#include "game/client/tf/player_vs_environment/c_tf_tank_boss.h"
#include "game/client/tf/player_vs_environment/c_tf_upgrades.h"
#include "game/client/tf/workshop/item_import.h"

#include "game/shared/igamemovement.h"
#include "game/shared/in_buttons.h"
#include "game/shared/IVehicle.h"
#include "game/shared/mapdata_shared.h"
#include "game/shared/movetype_push.h"
#include "game/shared/npcevent.h"
#include "game/shared/triggers_shared.h"
#include "game/shared/tf/tf_weaponbase_gun.h"
#include "game/shared/tf/tf_weapon_compound_bow.h"
#include "game/shared/tf/tf_weapon_knife.h"
#include "game/shared/tf/tf_weapon_medigun.h"
#include "game/shared/tf/tf_weapon_sniperrifle.h"
#include "game/shared/tf/tf_item_wearable.h"

#include "public/tier1/checksum_md5.h"
#include "public/tier1/KeyValues.h"
#include "public/datacache/idatacache.h"
#include "public/datacache/imdlcache.h"
#include "public/demofile/demoformat.h"
#include "public/engine/IClientLeafSystem.h"
#include "public/engine/IEngineSound.h"
#include "public/engine/imatchmaking.h"
#include "public/engine/ishadowmgr.h"
#include "public/engine/ivmodelinfo.h"
#include "public/engine/ivmodelrender.h"
#include "public/input/mousecursors.h"
#include "public/inputsystem/iinputsystem.h"
#include "public/materialsystem/imaterial.h"
#include "public/materialsystem/imaterialvar.h"
#include "public/materialsystem/itexture.h"
#include "public/materialsystem/MaterialSystemUtil.h"
#include "public/toolframework/iclientenginetools.h"
#include "public/toolframework/ienginetool.h"
#include "public/toolframework/itoolentity.h"
#include "public/tools/bonelist.h"
#include "public/unicode/unicode.h"
#include "public/vgui/Dar.h"
#include "public/vgui/IBorder.h"
#include "public/vgui/IClientPanel.h"
#include "public/vgui/IInputInternal.h"
#include "public/vgui/IScheme.h"
#include "public/vgui/IVGui.h"
#include "public/vgui/Point.h"
#include "public/VGuiMatSurface/IMatSystemSurface.h"
#include "public/bone_setup.h"
#include "public/bone_accessor.h"
#include "public/dlight.h"
#include "public/ienginevgui.h"
#include "public/IGameUIFuncs.h"
#include "public/inetchannel.h"
#include "public/mouthinfo.h"
#include "public/OfflineMode.h"
#include "public/r_efx.h"
#include "public/renderparm.h"
#include "public/shattersurfacetypes.h"
#include "public/simple_physics.h"
#include "public/soundchars.h"
#include "public/surfinfo.h"
#include "public/UnicodeFileHelpers.h"
#include "public/view_shared.h"
#include "public/vphysics_interface.h"
#include "public/model_types.h"
#include "public/unicode/unicode.h"

template<typename T>
inline T UTIL_EntityAs(int entindex)
{
	IClientEntity* pEntity = UTIL_EntityByIndex(entindex);

	return pEntity ? pEntity->As<T>() : nullptr;
}