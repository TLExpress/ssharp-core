#pragma once

#ifndef __SSHARP_CONTENTS_H_
#define __SSHARP_CONTENTS_H_

#ifdef _WINDLL
#ifdef __SSHARP_CONTENTS
#define __SSHARP_CONTENTS_DLL __declspec(dllexport)
#else
#define __SSHARP_CONTENTS_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_CONTENTS_DLL
#endif

#include <iostream>
#include <memory>
#include <string>
#include "types.h"
#include "exceptions.h"
#include "absentable.h"

using std::string;
using std::shared_ptr;

using namespace ssharp::exceptions;
using namespace ssharp::containers;

namespace ssharp::types
{
	enum source_type_t { memory, file };

	struct __SSHARP_CONTENTS_DLL content_t
	{
		content_t() {}
		content_t(const content_t& rhs) { *this = rhs; }
		content_t(content_t&& rhs) noexcept { *this = std::move(rhs); }
		source_type_t source_type = memory;
		abs_obj<string> filename;
		abs_obj<uint64_t> hash;
		abs_obj<uint16_t> salt;
		abs_obj<string> source_name;
		abs_obj<size_t> source_pos;
		abs_obj<size_t> source_size;
		abs_obj<uint32_t> crc32;
		uint32_t size = 0;
		uint32_t zsize = 0;
		bool is_directory = false;
		bool compressed = false;
		bool varify = false;
		bool encrypted = false;
		abs_obj<zlib_header_t> zlib_header;
		abs_obj<uint32_t> adler32;
		abs_obj<buff_pair_t> mbuff;
		content_t& operator=(const content_t& rhs)
		{
			this->source_type = rhs.source_type;
			this->filename = rhs.filename;
			this->hash = rhs.hash;
			this->salt = rhs.salt;
			this->source_name = rhs.source_name;
			this->source_pos = rhs.source_pos;
			this->source_size = rhs.source_size;
			this->crc32 = rhs.crc32;
			this->size = rhs.size;
			this->zsize = rhs.zsize;
			this->is_directory = rhs.is_directory;
			this->compressed = rhs.compressed;
			this->varify = rhs.varify;
			this->encrypted = rhs.encrypted;
			this->zlib_header = rhs.zlib_header;
			this->adler32 = rhs.adler32;
			this->mbuff = rhs.mbuff->copy();
			return *this;
		}

		content_t& operator=(content_t&& rhs) noexcept
		{
			this->source_type = std::move(rhs.source_type);
			this->filename = std::move(rhs.filename);
			this->hash = std::move(rhs.hash);
			this->salt = std::move(rhs.salt);
			this->source_name = std::move(rhs.source_name);
			this->source_pos = std::move(rhs.source_pos);
			this->source_size = std::move(rhs.source_size);
			this->crc32 = std::move(rhs.crc32);
			this->size = std::move(rhs.size);
			this->is_directory = std::move(rhs.is_directory);
			this->compressed = std::move(rhs.compressed);
			this->varify = std::move(rhs.varify);
			this->encrypted = std::move(rhs.encrypted);
			this->zlib_header = std::move(rhs.zlib_header);
			this->adler32 = std::move(rhs.adler32);
			this->mbuff = std::move(rhs.mbuff);
			return *this;
		}
		buff_pair_t getBuff(size_t size) {
			if (mbuff.attend())
				return *mbuff;
			return stream_loader::loadStream(ifstream(*filename, ios::in | ios::binary), *source_pos, size);
		}
		buff_pair_t loadBuff() {
			if (source_type == memory)
				return *mbuff;
			mbuff = stream_loader::loadStream(ifstream(*filename, ios::in | ios::binary), *source_pos, *source_size);
			source_type = memory;
			return *mbuff;
		}
	};
}
#endif