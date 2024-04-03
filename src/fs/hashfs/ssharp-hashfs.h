#pragma once

#ifndef SSHARP_HASHFS_H
#define SSHARP_HASHFS_H

#ifdef _WINDLL
#ifdef SSHARP_HASHFS
#define SSHARP_HASHFS_DLL __declspec(dllexport)
#else
#define SSHARP_HASHFS_DLL __declspec(dllimport)
#endif
#else
#define SSHARP_HASHFS_DLL
#endif

#include <list>
#include "entry-list.h"

using namespace ssharp::entry_list;

namespace ssharp::fs::hashfs
{
	typedef struct
	{
		uint32_t sign;
		uint16_t version;
		uint16_t salt;
		uint32_t method;
		uint32_t entries_count;
		uint64_t offset;
		uint64_t v_offset;
	}header_t;

	enum hashfs_entry_flags_t : uint32_t
	{
		is_directory = 0x1,
		is_compressed = 0x2,
		varify = 0x4,
		encrypted = 0x8
	};
	
	typedef struct
	{
		uint64_t hash;
		uint64_t offset;
		hashfs_entry_flags_t flags;
		uint32_t crc;
		uint32_t size;
		uint32_t zsize;
	}entry_t;

	extern entry_list::entry_list SSHARP_HASHFS_DLL loadFile(const string& filename);
}

#endif