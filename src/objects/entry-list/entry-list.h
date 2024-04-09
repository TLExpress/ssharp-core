#pragma once

#ifndef __SSHARP_ELIST_H
#define __SSHARP_ELIST_H

#ifdef _WINDLL
#ifdef __SSHARP_ELIST
#define __SSHARP_ELIST_DLL __declspec(dllexport)
#else
#define __SSHARP_ELIST_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_ELIST_DLL
#endif

#include <list>
#include <concepts>
#include <algorithm>
#include "entry-objects.h"
#include "path-spliter.h"

using namespace ssharp::pointers;
using namespace ssharp::entry_objects;
using namespace ssharp::dictionary;
using namespace ssharp::utils::path_spliter;

namespace ssharp::entry_list
{
	template<typename T>
	concept Basic_obj = std::derived_from<T, basic_obj>;

	//template<Basic_obj T>
	using list_filter_f = filter_ft<ss_ptr<basic_obj>>;
	//template<Basic_obj T>
	using list_sorter_f = sorter_ft<ss_ptr<basic_obj>>;

	const list_sorter_f hash_sorter = [](const ss_ptr <basic_obj>& left, const ss_ptr<basic_obj>& right) {return *left->get()->hash <= *right->get()->hash; };
	const list_sorter_f name_sorter = [](const ss_ptr<basic_obj>& left, const ss_ptr <basic_obj>& right) {return *left->get()->filename <= *right->get()->filename; };

	//template<Basic_obj T>
	class entry_list_t : public vector<ss_ptr<basic_obj>> 
	{
		uint16_t salt=0;
	public:
		using vector<ss_ptr<basic_obj>>::vector;

		uint16_t getSalt() const { return salt; };

		parsed_paths_t parseBuff(list_filter_f filter); //done
		entry_list_t find_if(list_filter_f filter); //done
		entry_list_t dirList(); //done
		iterator find(const ss_ptr<basic_obj>& obj) { return std::find(begin(), end(), obj); }
		iterator find(const string& filename);
		iterator find(const uint64_t hash);
		size_t identByName(list_filter_f filter); //done
		int addNameToDir(const string& name); //done

		entry_list_t hashfsInit();
		entry_list_t zipfsInit();
		entry_list_t sysfsInit();
		entry_list_t namelistInit();

		using vector<ss_ptr<basic_obj>>::erase;
		entry_list_t& erase(const iterator& it);
		entry_list_t& erase(size_t at){ erase(begin() + at); }
		entry_list_t& erase(const ss_ptr<basic_obj>& obj);
		entry_list_t& erase_if(list_filter_f filter);
		entry_list_t& eraseEmptyDir();
		entry_list_t& eraseIndex(const string& name);
		entry_list_t& eraseIndex(const iterator& it);
		entry_list_t& eraseIndex(size_t at){ return eraseIndex(begin() + at); }
		entry_list_t& eraseIndex(const ss_ptr<basic_obj>& obj) { return eraseIndex(find(obj)); }
		entry_list_t& eraseIndex_if(list_filter_f filter);
		entry_list_t& eraseUnessEntIdx();
		entry_list_t& rebuildIdx();
		entry_list_t& changeSalt(uint16_t salt);
		entry_list_t& sort(list_sorter_f sorter){ std::sort(begin(), end(), sorter); }
	};
}

#endif