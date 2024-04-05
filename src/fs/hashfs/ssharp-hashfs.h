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
	struct header_t
	{
		uint32_t sign = 0x23534353U;
		uint16_t version = 1;
		uint16_t salt = 0;
		uint32_t method = 0x59544943;
		uint32_t entries_count=0;
		uint64_t offset=0;
		uint64_t v_offset=0;
	};

	/*enum hashfs_entry_flags_t : uint32_t
	{
		is_directory = 0x1,
		is_compressed = 0x2,
		varify = 0x4,
		encrypted = 0x8
	};*/

	struct entry_t
	{
		uint64_t hash=0;
		uint64_t offset=0;
		uint32_t flags=0;
		uint32_t crc=0;
		uint32_t size=0;
		uint32_t zsize=0;
		entry_t() {}
		entry_t(const basic_obj& entry) {
			auto const& obj = entry.getModified();
			hash = obj.hash;
			offset = obj.offset;
			flags=(obj.is_directory)|(obj.compressed<<1)|(obj.varify<<2)|(obj.encrypted<<3);
			crc = obj.crc32;
			size = obj.size;
			zsize = obj.zsize+sizeof(zlib_header_t)+sizeof(uint32_t);
		}
	};

	extern entry_list_t SSHARP_HASHFS_DLL loadFile(const string& filename);
	extern void SSHARP_HASHFS_DLL storeFile(const string& filename,const entry_list_t& list);
}

#endif