#pragma once

#ifndef __SSHARP_TYPES_H_
#define __SSHARP_TYPES_H_

#ifdef _WINDLL
#ifdef __SSHARP_TYPES
#define __SSHARP_TYPES_DLL __declspec(dllexport)
#else
#define __SSHARP_TYPES_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_TYPES_DLL
#endif

#include <functional>
#include <iostream>
#include <vector>
#include <set>
#include <map>

namespace ssharp::types
{
	typedef std::shared_ptr<char[]> buff_t;
	typedef std::vector<std::string> strings;
	typedef std::vector<std::wstring> wstrings;
	typedef std::set<std::string> parsed_paths_t;
	typedef std::pair<buff_t, size_t> buff_pair_t;
	typedef std::function<parsed_paths_t(const buff_pair_t&)> parse_buff_ft;
	typedef std::map<uint64_t, std::string> dictionary_t;
	typedef struct
	{
		uint8_t CMF;
		uint8_t DLG;
	}zlib_header_t;
}
#endif