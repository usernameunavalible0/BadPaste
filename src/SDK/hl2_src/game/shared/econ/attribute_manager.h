//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Attributable entities contain one of these, which handles game specific handling:
//				- Save / Restore
//				- Networking
//				- Attribute providers
//				- Application of attribute effects
//
//=============================================================================

#ifndef ATTRIBUTE_MANAGER_H
#define ATTRIBUTE_MANAGER_H
#ifdef _WIN32
#pragma once
#endif

#define CALL_ATTRIB_HOOK(vartype, retval, hookName, who, itemlist) \
		retval = AttribHookValue<vartype>(retval, #hookName, static_cast<const C_BaseEntity*>(who), itemlist, true);

#define CALL_ATTRIB_HOOK_INT(retval, hookName) CALL_ATTRIB_HOOK(int, retval, hookName, this, NULL)
#define CALL_ATTRIB_HOOK_FLOAT(retval, hookName) CALL_ATTRIB_HOOK(float, retval, hookName, this, NULL)

template <class T> static T AttribHookValue(T TValue, const char* pszAttribHook, const C_BaseEntity* pEntity, CUtlVector<C_BaseEntity*>* pItemList = NULL, bool bIsGlobalConstString = false)
{
	return reinterpret_cast<T(__cdecl*)(T, const char*, const C_BaseEntity*, CUtlVector<C_BaseEntity*>*, bool)>(U::Offsets.m_dwAttribHookValue)(TValue, pszAttribHook, pEntity, pItemList, bIsGlobalConstString);
}

#endif // ATTRIBUTE_MANAGER_H