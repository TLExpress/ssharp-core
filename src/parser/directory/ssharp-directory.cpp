#include "ssharp-directory.h"

namespace ssharp::parser::directory
{

	parsed_paths_t parseBuff(const buff_pair_t& inpair)
	{
		string str;
		parsed_paths_t ret;
		auto& buff = inpair.first;
		for (size_t c = 0; c < inpair.second; c++)
		{
			if (buff[c] == '\x0a')
			{
				ret.insert(str);
				str = "";
			}
			else
				str += buff[c];
		}
		return ret;
	}
}

using namespace ssharp::parser::directory;

parser_result_t __SSHARP_DIRECTORY_CALLTYPE ssdirectory_parseBuff(char* buff, size_t size, char*** set, int* set_size)
{
	try
	{
		*set = nullptr;
		auto&& t_set = parseBuff(buff_pair_t(buff,size));
		if (!t_set.size())
			return parser_no_path_to_parse;
		*set = multi_paths_parser::setExporter(t_set);
	}
	catch (incorrect_format)
	{
		return parser_incorrect_format;
	}
	catch (bad_alloc)
	{
		return parser_buffer_error;
	}
	catch (...)
	{
		return parser_unknown_error;
	}
	return parser_ok;
}