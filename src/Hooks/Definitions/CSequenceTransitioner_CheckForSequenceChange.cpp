#include "../Hooks.h"

DEFINE_HOOK(CSequenceTransitioner_CheckForSequenceChange, void, __fastcall, void* ecx, void* edx, CStudioHdr* hdr, int nCurSequence, bool bForceNewSequence, bool bInterpolate)
{
	Func.Original<FN>()(ecx, edx, hdr, nCurSequence, bForceNewSequence, bInterpolate);
}