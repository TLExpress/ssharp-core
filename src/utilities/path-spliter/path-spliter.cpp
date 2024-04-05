#include "path-spliter.h"

namespace ssharp::utils::path_spliter
{
	pair<string, string> __SSHARP_PATHSPLITER_DLL splitPath(const string& name)
	{
		auto&& slash = name.find_last_of('/');
		string d_name, e_name;
		if (slash != name.npos)
		{
			d_name = name.substr(0, slash);
			e_name = name.substr(slash + 1);
		}
		else
		{
			d_name = "";
			e_name = name;
		}
		return std::make_pair(d_name, e_name);
	}
}

using namespace ssharp::utils::path_spliter;

void __SSHARP_PATHSPLITER_CALLTYPE ssSplitPath(const char* in, char** left, char** right)
{
	auto ret = splitPath(string(in));
	*left = new char[ret.first.size() + 1];
	*right = new char[ret.second.size() + 1];
	std::strcpy(*left, ret.first.c_str());
	std::strcpy(*right, ret.second.c_str());
	return;
}