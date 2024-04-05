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
#include <memory>
#include <vector>
#include <set>
#include <map>

namespace ssharp::types
{
	typedef std::shared_ptr<char[]> buff_t;
	typedef std::vector<std::string> strings;
	typedef std::vector<std::wstring> wstrings;
	typedef std::set<std::string> parsed_paths_t;
	template<typename T>
	using filter_ft = std::function<bool(const T&)>;
	template<typename T>
	using sorter_ft = std::function<bool(const T&, const T&)>;
	typedef struct
	{
		uint8_t cmf;
		uint8_t dlg;
	}zlib_header_t;

	struct __SSHARP_TYPES_DLL buff_pair_t
	{
		buff_t first;
		size_t second;
		buff_pair_t() = delete;
		buff_pair_t(size_t size) { first = std::make_shared<char[]>(size); second = size; }
		buff_pair_t(const buff_t& buff, size_t size) { first = buff; second = size; }
		buff_pair_t(buff_t&& buff, size_t size) noexcept { first = std::move(buff); second = size; }
		buff_pair_t(char* buff, size_t size) { first = std::shared_ptr<char[]>(buff, [](char*) {}); second = size; }
		buff_pair_t(const buff_pair_t& rhs) { *this = rhs; }
		buff_pair_t(buff_pair_t&& rhs) noexcept { *this = std::move(rhs); }
		buff_pair_t& operator=(const buff_pair_t& rhs) { first = rhs.first; second = rhs.second; return *this; }
		buff_pair_t& operator=(buff_pair_t&& rhs) noexcept { first = std::move(rhs.first); second = std::move(rhs.second); return *this; }
		buff_pair_t copy() const {
			buff_pair_t c(second);
			std::memcpy(c.first.get(), first.get(), second);
			return c;
		}
		buff_t& operator*() { return first; }
		const buff_t& operator*() const { return first; }
		operator const size_t() const { return second; }
		operator size_t() { return second; }
		operator const buff_t&() const { return first; }
		operator buff_t&() { return first; }
		operator const char*() const { return first.get(); }
		operator char*() { return first.get(); }
	};
	typedef std::function<parsed_paths_t(const buff_pair_t&)> parse_buff_ft;
}
#endif