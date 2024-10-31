//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#ifndef NETMESSAGES_H
#define NETMESSAGES_H
#ifdef _WIN32
#pragma once
#pragma warning(disable : 4100)	// unreferenced formal parameter
#endif

#include "qlimits.h"
#include "protocol.h"
#include "../engine/net.h"
#include "../public/inetmessage.h"
#include "../public/inetchannelinfo.h"
#include "../public/inetmsghandler.h"

#if !defined( _X360 )
#include "xbox/xboxstubs.h"
#endif

class SendTable;
class KeyValue;
class KeyValues;
class INetMessageHandler;
class IServerMessageHandler;

//TODO: this is gay
class CNetMessageRatelimitPolicyBase
{
public:
	//no virtual destructor here
	virtual bool BIncomingMessageForProcessing(double dValue, int iValue) = 0;
};

class CNetMessageRatelimitPolicyNone : public CNetMessageRatelimitPolicyBase
{
public:
	CNetMessageRatelimitPolicyNone()
	{
		m_iUnk1 = 0;
		m_iUnk2 = 0;
		m_dUnk0 = 0.0;
		m_iUnk3 = 0;
		m_iUnk4 = 0;
		m_dUnk1 = 0.0;
		m_iUnk5 = 0;
		m_iUnk6 = 0;
	}

	//no virtual destructor here
	virtual bool BIncomingMessageForProcessing(double dValue, int iValue);

public:
	int m_iUnk1; //4
	int m_iUnk2; //8
	double m_dUnk0; //12
	int m_iUnk3; //20
	int m_iUnk4; //24
	double m_dUnk1; //28
	int m_iUnk5; //36
	int m_iUnk6; //40
};

//idk why but this inherits from base, not from none
class CNetMessageRatelimitPolicy : public CNetMessageRatelimitPolicyBase
{
public:
	CNetMessageRatelimitPolicy()
	{
		m_iUnk1 = 0;
		m_iUnk2 = 0;
		m_dUnk0 = 0.0;
		m_iUnk3 = 0;
		m_iUnk4 = 0;
		m_dUnk1 = 0.0;
		m_iUnk5 = 0;
		m_iUnk6 = 0;
	}

	//no virtual destructor here
	virtual bool BIncomingMessageForProcessing(double dValue, int iValue);

public:
	int m_iUnk1; //4
	int m_iUnk2; //8
	double m_dUnk0; //12
	int m_iUnk3; //20
	int m_iUnk4; //24
	double m_dUnk1; //28
	int m_iUnk5; //36
	int m_iUnk6; //40

	//additional here
	double m_dUnk2; //44 (set to 0.001 by default)
	int m_iUnk7; //52
	int m_iUnk8; //56
};

#define DECLARE_BASE_MESSAGE( msgtype )						\
	public:													\
		bool			ReadFromBuffer( bf_read &buffer );	\
		bool			WriteToBuffer( bf_write &buffer );	\
		const char		*ToString() const;					\
		int				GetType() const { return msgtype; } \
		const char		*GetName() const { return #msgtype;}\

#define DECLARE_NET_MESSAGE( name )			\
	DECLARE_BASE_MESSAGE( net_##name );		\
	INetMessageHandler *m_pMessageHandler;	\
	bool Process() { return m_pMessageHandler->Process##name( this ); }\

#define DECLARE_SVC_MESSAGE( name )		\
	DECLARE_BASE_MESSAGE( svc_##name );	\
	IServerMessageHandler *m_pMessageHandler;\
	bool Process() { return m_pMessageHandler->Process##name( this ); }\

#define DECLARE_CLC_MESSAGE( name )		\
	DECLARE_BASE_MESSAGE( clc_##name );	\
	IClientMessageHandler *m_pMessageHandler;\
	bool Process() { return m_pMessageHandler->Process##name( this ); }\

#define DECLARE_MM_MESSAGE( name )		\
	DECLARE_BASE_MESSAGE( mm_##name );	\
	IMatchmakingMessageHandler *m_pMessageHandler;\
	bool Process() { return m_pMessageHandler->Process##name( this ); }\

class CNetMessage : public INetMessage
{
public:
	CNetMessage() {
		m_bReliable = true;
		m_NetChannel = NULL;
	}

	virtual ~CNetMessage() {};

	virtual int		GetGroup() const { return INetChannelInfo::GENERIC; }
	INetChannel* GetNetChannel() const { return m_NetChannel; }

	virtual void	SetReliable(bool state) { m_bReliable = state; };
	virtual bool	IsReliable() const { return m_bReliable; };
	virtual void    SetNetChannel(INetChannel* netchan) { m_NetChannel = netchan; }
	virtual bool	Process() { return false; };	// no handler set

protected:
	bool				m_bReliable;	// true if message should be send reliable
	INetChannel*		m_NetChannel;	// netchannel this message is from/for
};

#define NET_TICK_SCALEUP	100000.0f
class NET_Tick : public CNetMessage
{
public:
	bool WriteToBuffer(bf_write& buffer)
	{
		buffer.WriteUBitLong(GetType(), NETMSG_TYPE_BITS);
		buffer.WriteLong(m_nTick);
		buffer.WriteUBitLong(clamp((int)(NET_TICK_SCALEUP * m_flHostFrameTime), 0, 65535), 16);
		buffer.WriteUBitLong(clamp((int)(NET_TICK_SCALEUP * m_flHostFrameTimeStdDeviation), 0, 65535), 16);
		return !buffer.IsOverflowed();
	}

	bool ReadFromBuffer(bf_read& buffer)
	{
		m_nTick = buffer.ReadLong();
		m_flHostFrameTime = (float)buffer.ReadUBitLong(16) / NET_TICK_SCALEUP;
		m_flHostFrameTimeStdDeviation = (float)buffer.ReadUBitLong(16) / NET_TICK_SCALEUP;
		return !buffer.IsOverflowed();
	}

	const char* ToString(void) const { return "NET_Tick"; }
	int GetType() const { return net_Tick; }
	const char* GetName() const { return "net_Tick"; }

	NET_Tick()
	{
		m_bReliable = false;
		m_flHostFrameTime = 0;
		m_flHostFrameTimeStdDeviation = 0;
	};

	NET_Tick(int tick, float hostFrametime, float hostFrametime_stddeviation)
	{
		m_bReliable = false;
		m_nTick = tick;
		m_flHostFrameTime = hostFrametime;
		m_flHostFrameTimeStdDeviation = hostFrametime_stddeviation;
	};

public:
	int			m_nTick;
	float		m_flHostFrameTime;
	float		m_flHostFrameTimeStdDeviation;
};

class CLC_Move : public CNetMessage
{
	DECLARE_CLC_MESSAGE(Move);

	int	GetGroup() const { return INetChannelInfo::MOVE; }

	CLC_Move() { m_bReliable = false; }

public:
	int				m_nBackupCommands;
	int				m_nNewCommands;
	int				m_nLength;
	bf_read			m_DataIn;
	bf_write		m_DataOut;
};

#endif //NETMESSAGES_H