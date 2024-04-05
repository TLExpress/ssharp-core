#pragma once

#ifndef SSHARP_ELIST_H
#define SSHARP_ELIST_H

#ifdef _WINDLL
#ifdef SSHARP_ELIST
#define SSHARP_ELIST_DLL __declspec(dllexport)
#else
#define SSHARP_ELIST_DLL __declspec(dllimport)
#endif
#else
#define SSHARP_ELIST_DLL
#endif

#include <list>
#include "entry-objects.h"
#include "path-spliter.h"
#include "nonabsentable.h"

using namespace ssharp::entry_objects;
using namespace ssharp::dictionary;

namespace ssharp::entry_list
{
	typedef filter_ft<nabptr_obj<basic_obj>> list_filter_f;
	typedef sorter_ft<nabptr_obj<basic_obj>> list_sorter_f;
	class entry_list_t : public vector<nabptr_obj<basic_obj>>  //做成派生類，並把方法分發到各種格式
	{
		uint64_t salt=0;
	public:
		using vector<nabptr_obj<basic_obj>>::vector;

		parsed_paths_t parseBuff(list_filter_f filter);
		size_t findNameFromDic(dictionary_t dic);
		size_t findNameFromDic(list_filter_f filter, dictionary_t dic);
		void setHashFromName(list_filter_f filter);// {}//
		size_t identByName(list_filter_f filter);
		bool rmNameFromDir(const string& name);
		int addNameToDir(const string& name);

		size_t hashfsInit();
		size_t zipfsInit();
		size_t sysfsInit();
		size_t namelistInit();

		entry_list_t find_if(list_filter_f filter);
		entry_list_t& load(list_filter_f filter);
		entry_list_t& loadUnloaded();
		entry_list_t& loadBuff(list_filter_f filter);
		entry_list_t& unload(list_filter_f filter);
		entry_list_t& commit(list_filter_f filter);
		entry_list_t& rebuildDir();
		entry_list_t& rmEmptyDir();
		entry_list_t& rmUnesEntDir();
		entry_list_t& rmEntFromDir(list_filter_f filter);
		entry_list_t& remove_if(list_filter_f filter);
		entry_list_t& sort(list_sorter_f sorter);
		entry_list_t& sortByHash();
		entry_list_t& sortByName();
		uint16_t getSalt();
		entry_list_t& changeSalt();
	};
}

#endif