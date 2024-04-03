#include "path-spliter.h"

namespace ssharp::utils::path_spliter
{
	auto __SSHARP_PATHSPLITER_DLL splitPath(const string& name)->pair<string, string>
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