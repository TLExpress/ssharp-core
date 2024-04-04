#include "entry-list.h"

namespace ssharp::entry_list
{
	size_t entry_list::findNameFromDic(dictionary_t dic)
	{
		return findNameFromDic([](const nabptr_obj<basic_obj>&) {return true; }, dic);
	}

	size_t entry_list::findNameFromDic(list_filter_f filter, dictionary_t dic)
	{
		size_t ret=0;
		for (auto & entry : find_if(filter))
			ret+=entry->setNameFromDictionary(dic);
		return ret;
	}

	size_t entry_list::identByName(list_filter_f filter)
	{
		size_t ret=0;
		for (auto& entry : find_if(filter))
		{
			if (entry->isDirectory())
				continue;
			auto& filename = *entry->getModified().filename;
			auto find = filename.find('.');
			if (find == filename.npos || find == filename.size() - 1)
				continue;
			auto&& ext = filename.substr(filename.find('.')+1);

			if (ext == "sii")
				entry = sii_obj(std::move(*entry));
			else if (ext == "sui")
				entry->parseBuff_f = mat::parseBuff;
			else if (ext == "pmd")
				entry->parseBuff_f = mat::parseBuff;
			else if (ext == "mat")
				entry->parseBuff_f = mat::parseBuff;
			else if (ext == "tobj")
				entry->parseBuff_f = [](const buff_pair_t& b) {return parsed_paths_t({ parser::tobj::parseBuff(b) });};
			else if (ext == "soundref")
				entry->parseBuff_f = [](const buff_pair_t& b) {return parsed_paths_t({ parser::soundref::parseBuff(b) }); };
		}
		return ret;
	}

	entry_list& entry_list::load(list_filter_f filter)
	{
		for (auto& entry : find_if(filter))
			entry->load();
		return *this;
	}

	entry_list& entry_list::loadBuff(list_filter_f filter)
	{
		for (auto& entry : find_if(filter))
			entry->getModified().loadBuff();
		return *this;
	}

	entry_list& entry_list::unload(list_filter_f filter)
	{
		for (auto& entry : find_if(filter))
			entry->unload();
		return *this;
	}

	entry_list& entry_list::commit(list_filter_f filter)
	{
		for (auto& entry : find_if(filter))
			entry->commit();
		return *this;
	}

	parsed_paths_t entry_list::parseBuff(list_filter_f filter)
	{
		parsed_paths_t ret;
		for (auto& entry : find_if(filter))
		{
			auto&& t_set = entry->parseBuff();
			for (auto const& s : t_set)
			{
				if(s[0]!='/')
				if (entry->getModified().filename.attend())
				{
					const auto& filename = *entry->getModified().filename;
					if (s[0] == '*')
					{
						auto&& out = (string)"*" + filename.substr(0, filename.find_last_of('/') + 1) + s.substr(1);
						ret.insert(std::move(out));
					}
					else
					{
						auto&& out = filename.substr(0, filename.find_last_of('/') + 1) + s;
						ret.insert(std::move(out));
					}
				}
				else
				{
					char hash[17];
					sprintf_s(hash, "%016llx", *entry->getModified().hash);
					if (s[0] == '*')
					{
						auto&& out = (string)hash + s;
						ret.insert(std::move(out));
					}
					else
					{
						auto&& out = (string)hash +'|' + s;
						ret.insert(std::move(out));
					}

				}
			}
		}
		return ret;
	}

	entry_list entry_list::find_if(list_filter_f filter)
	{
		entry_list sub;
		for (auto & entry : *this)
			if (filter(*entry))
				sub.push_back(entry);
		return sub;
	}

	entry_list& entry_list::remove_if(list_filter_f filter)
	{
		for (auto const& e : find_if(filter))
			if (e->getModified().filename.attend())
				rmNameFromDir(*e->getModified().filename);
		std::erase_if(*this, filter);
		return *this;
	}

	entry_list& entry_list::rmEmptyDir()
	{
		erase(std::remove_if(this->begin(), this->end(), [](const nabptr_obj<basic_obj>& obj) {
			auto pdir = dynamic_cast< const directory_obj*>(&(*obj));
			return pdir && pdir->content_list.empty();
			}));
		return *this;
	}

	entry_list& entry_list::rebuildDir()
	{
		size_t add = 0;
		do
		{
			add = 0;
			for (auto const& e : *this)
			{
				auto& name = e->getModified().filename;
				if (name.absent())
					continue;
				if(addNameToDir(*name)<0)
					this->push_back(nabptr_obj<basic_obj>(directory_obj(set<string>({ std::move(utils::path_spliter::splitPath(*name).second) }))));
			}
		} while (add);
		return *this;
	}

	bool entry_list::rmNameFromDir(const string& name)
	{
		auto&& name_pair = ssharp::utils::path_spliter::splitPath(name);
		auto&& dir_list = find_if([&name_pair](const nabptr_obj<basic_obj>& obj) {
			return 
				obj->getModified().filename.attend() && 
				*obj->getModified().filename == name_pair.first &&
				dynamic_cast<const directory_obj*>(&(*obj));
			});
		bool ret = false;
		for (auto& dir : dir_list)
			ret = ret || dynamic_cast<directory_obj*>(&(*dir))->content_list.erase(name_pair.second);
		return ret;
	}

	int entry_list::addNameToDir(const string& name)
	{
		auto&& name_pair = ssharp::utils::path_spliter::splitPath(name);
		auto&& dir_list = find_if([&name_pair](const nabptr_obj<basic_obj>& obj) {
			return
				obj->getModified().filename.attend() && 
				*obj->getModified().filename == name_pair.first &&
				dynamic_cast<const directory_obj*>(&(*obj));
			});
		int ret = (int)(bool)dir_list.size() - 1;
		for (auto& dir : dir_list)
			ret = (bool)ret | dynamic_cast<directory_obj*>(&(*dir))->content_list.insert(name_pair.second).second;
		return ret;
	}

	entry_list& entry_list::rmEntFromDir(list_filter_f filter)
	{
		for (auto const& e : find_if(filter))
			if(e->getModified().filename.attend())
				rmNameFromDir(*e->getModified().filename);
		return *this;
	}

	entry_list& entry_list::rmUnesEntDir() //¨S°µ
	{
		return *this;
	}
}