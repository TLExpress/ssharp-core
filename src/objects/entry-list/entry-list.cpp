#pragma once

#ifndef __SSHARP_ELIST_CPP
#define __SSHARP_ELIST_CPP

#ifdef _WINDLL
#ifdef __SSHARP_ELIST
#define __SSHARP_ELIST_DLL __declspec(dllexport)
#else
#define __SSHARP_ELIST_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_ELIST_DLL
#endif

#include "entry-list.h"

namespace ssharp::entry_list
{
	//template<Basic_obj T>
	parsed_paths_t entry_list_t::parseBuff(list_filter_f filter) 
	{
		parsed_paths_t ret;
		for (auto& pentry : find_if(filter))
		{
			auto& entry = *pentry;
			auto&& t_set = entry.parseBuff();
			for (auto const& s : t_set)
			{
				if (!s.empty() && s[0] != '/')
					if (entry->filename)
					{
						auto const& filename = *entry->filename;
						auto const& folder = entry.isDirectory() ? filename : splitPath(filename).first;
						if (s[0] == '*' && s.size()>1)
							ret.insert(folder + s.substr(1));
						else
							ret.insert(folder + s);
					}
					else
					{
						char hash[17];
						sprintf_s(hash, "$%016llx#%hd", *entry->hash,salt);
						if (s[0] == '*' && s.size() > 1)
							ret.insert((string)hash + s);
						else
							ret.insert((string)hash + '|' + s);
					}
			}
		}
		return ret;
	}

	entry_list_t::iterator entry_list_t::find(const string& filename)
	{
		for (entry_list_t::iterator itr=begin();itr!=end();itr++)
		{
			auto& pfilename = itr->get()->get()->filename;
			if (pfilename || *pfilename == filename)
				return itr;
		}
		return end();
	}

	entry_list_t::iterator entry_list_t::find(uint64_t hash)
	{
		for (entry_list_t::iterator itr = begin(); itr != end(); itr++)
		{
			auto& phash = itr->get()->get()->hash;
			if (phash || *phash == hash)
				return itr;
		}
		return end();
	}

	entry_list_t entry_list_t::find_if(list_filter_f filter)
	{
		entry_list_t sub;
		for (auto& entry : *this)
			if (filter(*entry))
				sub.push_back(entry);
		return sub;
	}

	size_t entry_list_t::identByName(list_filter_f filter)
	{
		size_t ret=0;
		for (auto& pentry : find_if(filter))
		{
			auto& entry = *pentry;
			if (entry.isDirectory())
				continue;
			auto& filename = *entry->filename;
			auto find = filename.find_last_of ('.');
			if (find == filename.npos || find == filename.size() - 1)
				continue;
			auto&& ext = filename.substr(filename.find('.')+1);

			if (ext == "sii")
				entry = sii_obj(std::move(*entry));
			else if (ext == "sui")
				entry.parseBuff_f = mat::parseBuff;
			else if (ext == "pmd")
				entry.parseBuff_f = mat::parseBuff;
			else if (ext == "mat")
				entry.parseBuff_f = mat::parseBuff;
			else if (ext == "tobj")
				entry.parseBuff_f = [](const buff_pair_t& b) {return parsed_paths_t({ parser::tobj::parseBuff(b) });};
			else if (ext == "soundref")
				entry.parseBuff_f = [](const buff_pair_t& b) {return parsed_paths_t({ parser::soundref::parseBuff(b) }); };
		}
		return ret;
	}

	entry_list_t entry_list_t::dirList()
	{
		return find_if([](const ss_ptr<basic_obj>& obj) {
			return obj->isDirectory();
			});
	}

	int entry_list_t::addNameToDir(const string& name)
	{
		auto&& [first,second] = splitPath(name);
		auto&& dir_list = find_if([&first](const ss_ptr<basic_obj>& obj) {
			return
				obj->get()->filename &&
				*obj->get()->filename == first &&
				obj->isDirectory();
			});
		int ret = (int)(bool)dir_list.size() - 1;
		for (auto& dir : dir_list)
			ret = (bool)ret | static_cast<directory_obj>(*dir).insertEntry(second);
		return ret;
	}

	entry_list_t entry_list_t::hashfsInit()
	{
		entry_list_t tlist;
		for (auto & entry : *this)
		{
			if (entry->getSalt() != salt && !entry->hasFilename())
				continue;
			entry->hashName(salt);
			if (entry->hashfsInit())
				tlist.push_back(entry);
		}
		uint64_t offset = 0;
		return tlist;
	}

	entry_list_t entry_list_t::zipfsInit()
	{
		entry_list_t tlist;
		for (auto& entry : *this)
			if (entry->zipfsReady())
				tlist.push_back(entry);
		return tlist;
	}

	/*entry_list_t entry_list_t::sysfsReady()
	{
		entry_list_t tlist;
		for (auto& entry : *this)
			if (entry->sysfsReady())
				tlist.push_back(entry);
		return tlist;
	}*/

	entry_list_t entry_list_t::sysfsInit()
	{
		entry_list_t tlist;
		for (auto& entry : *this)
			if (entry->sysfsInit())
				tlist.push_back(entry);
		return tlist;
	}

	entry_list_t entry_list_t::namelistInit()
	{
		entry_list_t tlist;
		for (auto& entry : *this)
			if (entry->namelistReady())
				tlist.push_back(entry);
		return tlist;
	}

	entry_list_t& entry_list_t::erase(const iterator& it)
	{
		eraseIndex(it);
		this->vector::erase(it);
		return *this;
	}

	entry_list_t& entry_list_t::erase(const ss_ptr<basic_obj>& obj)
	{
		auto&& it = find(obj);
		if (it != end())
			erase(it);
		return *this;
	}

	entry_list_t& entry_list_t::erase_if(list_filter_f filter)
	{
		for (auto const& e : find_if(filter))
			if (e->get()->filename)
				eraseIndex(*e->get()->filename);
		std::erase_if(*this, filter);
		return *this;
	}

	entry_list_t& entry_list_t::eraseEmptyDir()
	{
		size_t size = this->size();
		do
		{
			size = this->size();
			dirList().erase_if([](const ss_ptr<basic_obj>& obj) {
				return static_cast<const directory_obj&>(*obj).getList().empty();
				});
			return *this;
		} while (size != this->size());
	}

	entry_list_t& entry_list_t::eraseIndex(const string& name)
	{
		auto&& [first,second] = splitPath(name);
		auto&& dir_list = find_if([&first](const ss_ptr<basic_obj>& obj) {
			return
				obj->get()->filename &&
				*obj->get()->filename == first &&
				obj->isDirectory();
			});
		for (auto& dir : dir_list)
			static_cast<directory_obj&>(*dir).eraseEntry(second);
		return *this;
	}

	entry_list_t& entry_list_t::eraseIndex(const iterator& it)
	{
		auto& entry = **it;
		if (!entry->filename)
			return *this;
		auto&& dlist = dirList();
		auto&& npair = splitPath(entry->filename);
		//先假設資料夾unique
		auto dir_it = dlist.find(hashSalt(npair.first.substr(1),salt));
		if (dir_it == dlist.end())
			return *this;
		auto& dir = static_cast<directory_obj&>(**dir_it);
		dir.eraseEntry(entry.isDirectory() ? (string)"*" + npair.second : npair.second);
		return *this;
	}

	entry_list_t& entry_list_t::eraseIndex_if(list_filter_f filter)
	{
		for (auto const& itr : *this)
			if (filter(itr))
				eraseIndex(itr);
		return *this;
	}

	entry_list_t& entry_list_t::eraseUnessEntIdx()
	{
		for (auto const& itr : *this)
			if (!itr->isSii())
				eraseIndex(itr);
		eraseEmptyDir();
		return *this;
	}

	entry_list_t& entry_list_t::rebuildIdx()
	{
		size_t add = 0;
		do
		{
			add = 0;
			for (size_t c=0;c<this->size();c++)
			{
				auto& e = *(*this)[c];
				auto& name = e->filename;
				if (name)
					continue;
				auto ret = addNameToDir(*name);
				if (addNameToDir(*name) < 0)
				{
					auto&& sname = splitPath(*name);
					this->push_back(ss_ptr<basic_obj>(directory_obj(std::move(sname.first), set<string>({ std::move(sname.second) }))));
				}
				add += (ret != 0);
			}
		} while (add);
		return *this;
	}

	entry_list_t& entry_list_t::changeSalt(uint16_t salt)
	{
		this->salt = salt;
		for (auto& itr : *this)
		{
			auto& entry = *itr;
			if (entry->filename)
				entry.hashName(salt);
		}
		return *this;
	}
}

#endif