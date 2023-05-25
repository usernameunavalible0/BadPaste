#include "Crithack.h"
#include "../Vars.h"

//NOTE: This crithack is nowhere near perfect and it does not take the crit bucket system into account.
//This means that it does not crit 100% of the time but for my purposes this is fine
void CFeatures_Crithack::Run(C_TFWeaponBase* pWeapon, CUserCmd* cmd)
{
	if (!Vars::Misc::ForceMeleeCrits.m_Var)
		return;

	//This crithack is going to be melee only (for now!)
	C_TFWeaponBaseMelee* pMelee = pWeapon->As<C_TFWeaponBaseMelee*>();

	if (!pMelee)
		return;

	static int previousWeaponIdx = 0;
	//Lets check if we have a new melee weapon and if we do clear the crit ticks and scan again
	if (previousWeaponIdx != pMelee->entindex())
	{
		previousWeaponIdx = pMelee->entindex();
		m_CritTicks.Purge();
	}

	//Lets check the next 60 command numbers for seeds that result in critical hits
	for (int n = 0; n < 60; n++)
	{
		//Lets save some resources
		if (m_CritTicks.Size() >= 256)
			break;

		//Variable to represent the command number we are checking
		const int cmdNum = cmd->command_number + n;
		//Then lets set the seed used by the crit system to run our own check to see if this command number grants a critical hit
		C_BaseEntity::SetPredictionRandomSeed(MD5_PseudoRandom(cmdNum) & INT_MAX);
		//And finally lets run the check to see if we can get a crit and then if we can add this command number to our list
		if (pMelee->CalcIsAttackCriticalHelper())
			m_CritTicks.AddToTail(cmdNum);
	}
	
	//Finally lets set the seed back to prevent errors
	C_BaseEntity::SetPredictionRandomSeed(MD5_PseudoRandom(cmd->command_number) & INT_MAX);

	//If we are attacking lets give ourselves a crit
	if (cmd->buttons & IN_ATTACK)
	{
		//Lets avoid crashes due to m_Size < 0
		if (m_CritTicks.IsEmpty())
			return;

		//To give ourselves that juicy crit we set our command number and random seed based on the ones we collected earlier.
		//NOTE: This random is not nessisary I just thought it might be fun
		const int critTick = m_CritTicks.Random();
		cmd->command_number = critTick;
		cmd->random_seed = MD5_PseudoRandom(critTick) & INT_MAX;
	}
}