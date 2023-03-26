//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//=============================================================================//
// net.h -- Half-Life's interface to the networking layer
// For banning IP addresses (or allowing private games)
#ifndef NET_H
#define NET_H
#ifdef _WIN32
#pragma once
#endif

#include "../public/tier1/bitbuf.h"

// Flow control bytes per second limits
#define MAX_RATE		(1024*1024)				
#define MIN_RATE		1000
#define DEFAULT_RATE	80000

#define SIGNON_TIME_OUT				300.0f  // signon disconnect timeout

#define FRAGMENT_BITS		8
#define FRAGMENT_SIZE		(1<<FRAGMENT_BITS)
#define MAX_FILE_SIZE_BITS	26
#define MAX_FILE_SIZE		((1<<MAX_FILE_SIZE_BITS)-1)	// maximum transferable size is	64MB

// 0 == regular, 1 == file stream
#define MAX_STREAMS			2    

#define	FRAG_NORMAL_STREAM	0
#define FRAG_FILE_STREAM	1

#define TCP_CONNECT_TIMEOUT		4.0f
#define	PORT_ANY				-1
#define PORT_TRY_MAX			32
#define TCP_MAX_ACCEPTS			8

#define LOOPBACK_SOCKETS	2

#define STREAM_CMD_NONE		0	// waiting for next blob
#define STREAM_CMD_AUTH		1	// first command, send back challengenr
#define STREAM_CMD_DATA		2	// receiving a data blob
#define STREAM_CMD_FILE		3	// receiving a file blob
#define STREAM_CMD_ACKN		4	// acknowledged a recveived blob

// NETWORKING INFO

// This is the packet payload without any header bytes (which are attached for actual sending)
#define	NET_MAX_PAYLOAD				288000	// largest message we can send in bytes
#define	NET_MAX_PAYLOAD_V23			96000	// largest message we can send in bytes
#define NET_MAX_PAYLOAD_BITS_V23	17		// 2^NET_MAX_PAYLOAD_BITS > NET_MAX_PAYLOAD
// This is just the client_t->netchan.datagram buffer size (shouldn't ever need to be huge)
#define NET_MAX_DATAGRAM_PAYLOAD	4000	// = maximum unreliable payload size

// UDP has 28 byte headers
#define UDP_HEADER_SIZE				(20+8)	// IP = 20, UDP = 8


#define MAX_ROUTABLE_PAYLOAD		1260	// Matches x360 size

#if (MAX_ROUTABLE_PAYLOAD & 3) != 0
#error Bit buffers must be a multiple of 4 bytes
#endif

#define MIN_ROUTABLE_PAYLOAD		16		// minimum playload size

#define NETMSG_TYPE_BITS	6	// must be 2^NETMSG_TYPE_BITS > SVC_LASTMSG

#define NETMSG_LENGTH_BITS	11	// 256 bytes 

// This is the payload plus any header info (excluding UDP header)

#define HEADER_BYTES	9	// 2*4 bytes seqnr, 1 byte flags

// Pad this to next higher 16 byte boundary
// This is the largest packet that can come in/out over the wire, before processing the header
//  bytes will be stripped by the networking channel layer
#define	NET_MAX_MESSAGE	PAD_NUMBER( ( NET_MAX_PAYLOAD + HEADER_BYTES ), 16 )

// Even connectionless packets require int32 value (-1) + 1 byte content
#define NET_MIN_MESSAGE 5

#define NET_HEADER_FLAG_SPLITPACKET				-2
#define NET_HEADER_FLAG_COMPRESSEDPACKET		-3

class INetChannel;

enum
{
	NS_CLIENT = 0,	// client socket
	NS_SERVER,	// server socket
	NS_HLTV,
	NS_MATCHMAKING,
	NS_SYSTEMLINK,
#ifdef LINUX
	NS_SVLAN,	// LAN udp port for Linux. See NET_OpenSockets for info.
#endif
	MAX_SOCKETS
};

typedef struct netpacket_s
{
	char		        from[10];		// sender IP
	int				    source;		// received source 
	double			    received;	// received time
	unsigned char*      data;		// pointer to raw packet data
	bf_read			    message;	// easy bitbuf data access
	int				    size;		// size in bytes
	int				    wiresize;   // size in bytes before decompression
	bool			    stream;		// was send as stream
	struct netpacket_s* pNext;	// for internal use, should be NULL in public
} netpacket_t;

class INetChannelHandler;
class IConnectionlessPacketHandler;

// Message data
typedef struct
{
	// Size of message sent/received
	int		size;
	// Time that message was sent/received
	float	time;
} flowstats_t;

#endif // !NET_H