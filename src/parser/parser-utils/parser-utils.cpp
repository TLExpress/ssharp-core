#include "parser-utils.h"

namespace ssharp::parser::utils
{
	namespace multi_paths_parser
	{
		parsed_paths_t parseStream(istream& input, multipath_buff_parser parseBuff)
		{
			return parseBuff(stream_loader::loadStream(input));
		}

		parsed_paths_t parseStream(istream&& input, multipath_buff_parser parseBuff)
		{
			return parseStream(input, parseBuff);
		}

		parsed_paths_t parseFile(const string& filename, multipath_buff_parser parseBuff)
		{
			return parseStream(ifstream(filename, ios::in | ios::binary), parseBuff);
		}

		char** setExporter(const parsed_paths_t& set)
		{
			if (!set.size())
				return nullptr;
			char** t_set = new char* [set.size()];
			int i = 0;
#pragma warning(push)
#pragma warning(disable:4996)
			for (const auto& str : set) {
				*(t_set + i) = new char[str.size() + 1];
				std::strcpy(*(t_set + i), str.c_str());
				i++;
			}
#pragma warning(pop)
			return t_set;
		}
	}

	namespace single_path_parser
	{
		string parseStream(istream& input, singlepath_buff_parser parseBuff)
		{
			return parseBuff(stream_loader::loadStream(input));
		}

		string parseStream(istream&& input, singlepath_buff_parser parseBuff)
		{
			return parseStream(input, parseBuff);
		}

		string parseFile(const string& filename, singlepath_buff_parser parseBuff)
		{
			return parseStream(ifstream(filename, ios::in | ios::binary), parseBuff);
		}

		char* cstr_copy(const string& str)
		{
			auto ret = new char[str.size() + 1];
#pragma warning(push)
#pragma warning(disable:4996)
			std::strcpy(ret, str.c_str());
#pragma warning(pop)
			return ret;
		}
	}
}

parser_result __SSHARP_PARSERUTILS_CALLTYPE ssmulti_parseFilePtr(FILE* file, char*** set, int* set_size, multi_parseBuff_f parseBuff)
{
	try
	{
		if(!file)
			return parser_invalid_io;
		fseek(file, 0, SEEK_END);
		auto fsize = ftell(file);
		fseek(file, 0, SEEK_SET);
		if (!fsize)
			return parser_no_path_to_parse;
		buff_t buff = make_shared<char[]>(fsize);
		fread(buff.get(), sizeof(char), fsize, file);
		fseek(file, 0, SEEK_SET);
		return parseBuff(buff.get(), fsize, set, set_size);
	}
	catch (incorrect_format e)
	{
		return parser_incorrect_format;
	}
	catch (bad_alloc)
	{
		return parser_buffer_error;
	}
	catch (ios::failure)
	{
		return parser_invalid_io;
	}
	catch (...)
	{
		return parser_unknown_error;
	}
	return parser_ok;
}

parser_result __SSHARP_PARSERUTILS_CALLTYPE ssmulti_parseFile(char* filename, char*** set, int* set_size, multi_parseBuff_f parseBuff)
{
	try
	{
		FILE* file;
		if(fopen_s(&file, filename, "br"))
			return parser_invalid_io;
		return ssmulti_parseFilePtr(file, set, set_size, parseBuff);
	}
	catch (incorrect_format e)
	{
		return parser_incorrect_format;
	}
	catch (bad_alloc)
	{
		return parser_buffer_error;
	}
	catch (ios::failure)
	{
		return parser_invalid_io;
	}
	catch (...)
	{
		return parser_unknown_error;
	}
	return parser_ok;
}

parser_result __SSHARP_PARSERUTILS_CALLTYPE sssingle_parseFilePtr(FILE* file, char** str, single_parseBuff_f parseBuff)
{
	try
	{
		if (!file)
			return parser_invalid_io;
		fseek(file, 0, SEEK_END);
		auto fsize = ftell(file);
		fseek(file, 0, SEEK_SET);
		if (!fsize)
			return parser_no_path_to_parse;
		buff_t buff = make_shared<char[]>(fsize);
		fread(buff.get(), sizeof(char), fsize, file);
		fseek(file, 0, SEEK_SET);
		return parseBuff(buff.get(), fsize, str);
	}
	catch (incorrect_format e)
	{
		return parser_incorrect_format;
	}
	catch (bad_alloc)
	{
		return parser_buffer_error;
	}
	catch (ios::failure)
	{
		return parser_invalid_io;
	}
	catch (...)
	{
		return parser_unknown_error;
	}
	return parser_ok;
}

parser_result __SSHARP_PARSERUTILS_CALLTYPE ssmulti_parseFile(char* filename, char** str, single_parseBuff_f parseBuff)
{
	try
	{
		FILE* file;
		if (fopen_s(&file, filename, "br"))
			return parser_invalid_io;
		return sssingle_parseFilePtr(file, str, parseBuff);
	}
	catch (incorrect_format e)
	{
		return parser_incorrect_format;
	}
	catch (bad_alloc)
	{
		return parser_buffer_error;
	}
	catch (ios::failure)
	{
		return parser_invalid_io;
	}
	catch (...)
	{
		return parser_unknown_error;
	}
	return parser_ok;
}