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

using namespace ssharp::entry_objects;

namespace ssharp::entry_list
{
	typedef filter_ft<nabptr_obj<basic_obj>> list_filter_f;
	class entry_list : public vector<nabptr_obj<basic_obj>>  //做成派生類，並把方法分發到各種格式
	{
	public:
		using vector<nabptr_obj<basic_obj>>::vector;

		parsed_paths_t parseBuff(list_filter_f filter);
		size_t findNameFromDic(dictionary_t dic);
		size_t findNameFromDic(list_filter_f filter, dictionary_t dic);
		size_t identByName(list_filter_f filter);
		bool rmNameFromDir(const string& name);
		int addNameToDir(const string& name);
		entry_list find_if(list_filter_f filter);
		entry_list& load(list_filter_f filter);
		entry_list& loadBuff(list_filter_f filter);
		entry_list& unload(list_filter_f filter);
		entry_list& commit(list_filter_f filter);
		entry_list& rebuildDir();
		entry_list& rmEmptyDir();
		entry_list& rmUnesEntDir();
		entry_list& rmEntFromDir(list_filter_f filter);
		entry_list& remove_if(list_filter_f filter);
	};
}

#endif