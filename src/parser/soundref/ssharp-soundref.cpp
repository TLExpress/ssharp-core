#include "ssharp-soundref.h"

namespace ssharp::parser::soundref
{
	string parseBuff(const buff_pair_t& inpair)
	{
		auto& buff = inpair.first;
		auto& size = inpair.second;
		char* b = nullptr;
		size_t b_size=0;
		if (size >= 3 && buff.get()[0] == '\xef' && buff.get()[1] == '\xbb' && buff.get()[2] == '\xbf')
		{
			b = buff.get() + 3;
			b_size = size - 3;
		}
		else
		{
			b = buff.get();
			b_size = size;
		}
		string ts(b, 8);
		if (b_size < 9 || string(b,8) != "source=\"")
			throw incorrect_format("not a soundref");
		size_t c = 8;
		for (; c <= b_size; c++)
		{
			if(c==size)
				throw incorrect_format("not a soundref");
			if (b[c] == '\"')
				break;
		}
		string str(b + 8, c - 9);
		return str.substr(0,str.find('#'));
	}
}

using namespace ssharp::parser::soundref;

parser_result __SSHARP_SOUNDREF_CALLTYPE sssoundref_parseBuff(char* buff, size_t size, char** str)
{
	try
	{
		*str = single_path_parser::cstr_copy(parseBuff(std::make_pair(buff_t(buff), size)));
	}
	catch (incorrect_format e)
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