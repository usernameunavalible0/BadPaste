#include "../public/tier1/KeyValues.h"
#include "../public/tier0/platform.h"
#include "../public/tier1/strtools.h"

#pragma warning (disable : 6031)

bool CKeyValUtils::LoadFromBuffer(KeyValues* key_value, char const* resource_name, const char* buffer, void* file_system, const char* path_id)
{
	return reinterpret_cast<int(__thiscall*)(KeyValues*, char const*, const char*, void*, const char*)>(U::Offsets.m_dwLoadFromBuffer)(key_value, resource_name, buffer, file_system, path_id);
}

KeyValues* CKeyValUtils::FindKey(KeyValues* key_value, const char* keyName, bool bCreate)
{
	return reinterpret_cast<KeyValues* (__thiscall*)(KeyValues*, const char*, bool)>(U::Offsets.m_dwFindKey)(key_value, keyName, bCreate);
}

KeyValues* CKeyValUtils::Initialize(KeyValues* key_value, char* name)
{
	return reinterpret_cast<KeyValues* (__thiscall*)(KeyValues*, char*)>(U::Offsets.m_dwInitializeKeyValue)(key_value, name);
}

KeyValues::KeyValues(const char* keyName)
{
	char szName[512];
	sprintf_s(szName, sizeof(szName), keyName);
	g_KeyValues.Initialize(this, szName);
}

void KeyValues::SetString(const char* keyName, const char* value)
{
	KeyValues* dat = g_KeyValues.FindKey(this, keyName, true);

	if (dat)
	{
		if (dat->m_iDataType == TYPE_STRING && dat->m_sValue == value)
		{
			return;
		}

		// delete the old value
		delete[] dat->m_sValue;
		// make sure we're not storing the WSTRING  - as we're converting over to STRING
		delete[] dat->m_wsValue;
		dat->m_wsValue = NULL;

		if (!value)
		{
			// ensure a valid value
			value = "";
		}

		// allocate memory for the new value and copy it in
		int len = Q_strlen(value);
		dat->m_sValue = new char[len + 1];
		Q_memcpy(dat->m_sValue, value, len + 1);

		dat->m_iDataType = TYPE_STRING;
	}
}