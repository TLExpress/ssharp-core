#include "ssharp-tobj.h"

namespace ssharp::parser::tobj
{
	header_t getHeaderFromBuff(const buff_pair_t& inpair)
	{
		auto& buff = inpair.first;
		auto& size = inpair.second;
#define header (*((header_t*)buff.get()))
		if (size < sizeof(header_t)|| header.sign != expected_sign)
			throw incorrect_format("not a valid tobj");
		return header;
#undef header
	}

	string parseBuff(const buff_pair_t& inpair)
	{
#define h_t header_t
#define t_t texture_t
#define ssum(x,y) sizeof(x)+sizeof(y)
#define tlen ((t_t*)(buff.get() + sizeof(h_t)))->length
		getHeaderFromBuff(inpair);
		auto& buff = inpair.first;
		auto& size = inpair.second;
		if (size < ssum(h_t, t_t) || size < ssum(h_t, t_t) + tlen)
			throw incorrect_format("not a valid tobj");
		return string(buff.get() + ssum(h_t,t_t),tlen);
#undef sizesum
#undef tlen
#undef h_t
#undef t_t
	}
}

using namespace ssharp::parser::tobj;

parser_result_t __SSHARP_TOBJ_CALLTYPE sstobj_parseBuff(char* buff, size_t size, char** str)
{
	try
	{
		*str = single_path_parser::cstr_copy(parseBuff(buff_pair_t(buff,size)));
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