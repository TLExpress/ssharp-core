#pragma once

#ifndef __SSHARP_DICTIONARY_H_
#define __SSHARP_DICTIONARY_H_

#ifdef _WINDLL
#ifdef __SSHARP_DICTIONARY
#define __SSHARP_DICTIONARY_DLL __declspec(dllexport)
#else
#define __SSHARP_DICTIONARY_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_DICTIONARY_DLL
#endif

#include <map>
#include <string>
#include <iostream>

#include "types.h"
#include "ssharp-cityhash.h"

using namespace ssharp::types;
using namespace ssharp::utils::cityhash;
using std::string;
using std::map;

namespace ssharp::dictionary
{
	const parsed_paths_t init_list({
		"/",
		"/asset",
		"/automat",
		"/autoexec.cfg",
		"/contentbrowser",
		"/custom",
		"/def",
		"/defpaintjobonly.vbs",
		"/dlc",
		"/effect",
		"/font",
		"/locale",
		"/map"
		"/material",
		"/matlib",
		"/mod3l",
		"/model",
		"/model2",
		"/prefab",
		"/prefab2",
		"/road_template",
		"/sound",
		"/system",
		"/ui",
		"/unit",
		"/vehicle",
		"/video",
		"/version.txt",
		});
	struct dictionary_t : public map<uint64_t, string>
	{
		using map<uint64_t, string>::map;
		dictionary_t(uint16_t salt) : map<uint64_t, string>() { this->salt=salt; }
		dictionary_t(const parsed_paths_t& list, uint16_t salt) :map<uint64_t, string>() {
			this->salt = salt;
			insert(list);
		}
		using map<uint64_t, string>::insert;
		void insert(string val) { insert(std::make_pair(hashSalt(val.substr(1), salt), val)); }
		void insert(const parsed_paths_t& list) {
			size_t ret = 0;
			for (auto const& val : list)
				if (val.size())
				{
					insert(val);
					ret++;
				}
		}
		void changeSalt(uint16_t salt) {
			dictionary_t newdic(salt);
			for (auto const& rec : *this)
				newdic.insert(rec.second);
			*this = std::move(newdic);
		}
		parsed_paths_t list(){
			parsed_paths_t list;
			for (auto const& rec : *this)
				list.insert(rec.second);
			return list;
		}
		using map<uint64_t, string>::operator=;
		const parsed_paths_t& operator=(const parsed_paths_t& rhs)
		{
			*this = dictionary_t(rhs,salt);
			return rhs;
		}
		operator parsed_paths_t() { return list(); }
	private:
		uint16_t salt=0;
	};
}

#endif
