#pragma once

#define INVALID_KEY_SYMBOL (-1)

class KeyValues
{
public:
	KeyValues(const char* keyName);

	enum types_t
	{
		TYPE_NONE = 0,
		TYPE_STRING,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_PTR,
		TYPE_WSTRING,
		TYPE_COLOR,
		TYPE_UINT64,
		TYPE_NUMTYPES,
	};

	void SetString(const char* keyName, const char* value);

public:
	int m_iKeyName;
	char* m_sValue;
	wchar_t* m_wsValue;

	union
	{
		int m_iValue;
		float m_flValue;
		void* m_pValue;
		unsigned char m_Color[4];
	};

	char m_iDataType;
	char m_bHasEscapeSequences;
	char m_bEvaluateConditionals;
	char unused[1];

	KeyValues* m_pPeer;
	KeyValues* m_pSub;
	KeyValues* m_pChain;
};

class CKeyValUtils
{
public:
	bool LoadFromBuffer(KeyValues* key_value, char const* resource_name, const char* buffer, void* file_system = 0, const char* path_id = 0);
	KeyValues* FindKey(KeyValues* key_value, const char* keyName, bool bCreate = false);
	KeyValues* Initialize(KeyValues* key_value, char* name);
};

inline CKeyValUtils g_KeyValues;