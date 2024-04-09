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
#include "ssharp-ptr.h"

using std::string;
using std::shared_ptr;

using namespace ssharp::exceptions;
using namespace ssharp::pointers;

namespace ssharp::types
{
	enum source_type_t { memory, file };

	struct __SSHARP_CONTENTS_DLL content_t
	{
		content_t() {}
		content_t(const content_t& rhs) { *this = rhs; }
		content_t(content_t&& rhs) noexcept { *this = std::move(rhs); }
		source_type_t source_type = memory;
		ss_cptr<string> filename;
		ss_cptr<uint64_t> hash;
		ss_cptr<uint16_t> salt;
		ss_cptr<string> source_name;
		ss_cptr<size_t> source_pos;
		ss_cptr<size_t> source_size;
		ss_cptr<uint32_t> crc32;
		uint32_t size = 0;
		ss_cptr<uint32_t> zsize;
		bool is_directory = false;
		bool compressed = false;
		bool varify = false;
		bool encrypted = false;
		ss_cptr<zlib_header_t> zlib_header;
		ss_cptr<uint32_t> adler32;
		ss_cptr<buff_pair_t> mbuff;
		ss_cptr<uint64_t>offset;
		content_t& operator=(const content_t& rhs)
		{
			source_type = rhs.source_type;
			filename = rhs.filename;
			hash = rhs.hash;
			salt = rhs.salt;
			source_name = rhs.source_name;
			source_pos = rhs.source_pos;
			source_size = rhs.source_size;
			crc32 = rhs.crc32;
			size = rhs.size;
			zsize = rhs.zsize;
			is_directory = rhs.is_directory;
			compressed = rhs.compressed;
			varify = rhs.varify;
			encrypted = rhs.encrypted;
			zlib_header = rhs.zlib_header;
			adler32 = rhs.adler32;
			if(rhs.mbuff)
				*this->mbuff = rhs.mbuff->copy();
			return *this;
		}

		content_t& operator=(content_t&& rhs) noexcept
		{
			source_type = std::move(rhs.source_type);
			filename = std::move(rhs.filename);
			hash = std::move(rhs.hash);
			salt = std::move(rhs.salt);
			source_name = std::move(rhs.source_name);
			source_pos = std::move(rhs.source_pos);
			source_size = std::move(rhs.source_size);
			crc32 = std::move(rhs.crc32);
			size = std::move(rhs.size);
			is_directory = std::move(rhs.is_directory);
			compressed = std::move(rhs.compressed);
			varify = std::move(rhs.varify);
			encrypted = std::move(rhs.encrypted);
			zlib_header = std::move(rhs.zlib_header);
			adler32 = std::move(rhs.adler32);
			mbuff = std::move(rhs.mbuff);
			return *this;
		}
		buff_pair_t getBuff() {
			return getBuff(source_size);
		}
		buff_pair_t getBuff(size_t size) {
			if (mbuff)
				return mbuff;
			return stream_loader::loadStream(ifstream(source_name, ios::in | ios::binary), source_pos, size);
		}
		buff_pair_t loadBuff() {
			if (source_type == memory)
				return mbuff;
			mbuff = stream_loader::loadStream(ifstream(source_name, ios::in | ios::binary), source_pos, source_size);
			source_type = memory;
			return mbuff;
		}
	};
}
#endif