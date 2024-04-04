#include "ssharp-pmd.h"

namespace ssharp::parser::pmd
{
	header_t getHeaderFromBuff(const buff_pair_t& inpair)
	{
		auto& buff = inpair.first;
		auto& size = inpair.second;
#define header (*((header_t*)buff.get()))
		if (size < sizeof(header)|| header.version != expected_version)
			throw incorrect_format("not a valid pmd");
		return header;
#undef header
	}

	parsed_paths_t parseBuff(const buff_pair_t& inpair)
	{
		parsed_paths_t set;
		auto&& header = getHeaderFromBuff(inpair);
		auto& buff = inpair.first;
		auto& size = inpair.second;
		auto b = buff.get();
		uint32_t path_count = header.material_count * header.look_count;
		for (uint32_t c = 0; c < path_count; c++)
		{
			auto offset = header.material_offset + c * sizeof(uint32_t);
			if (offset+ sizeof(uint32_t) >= size)
				throw incorrect_format("not a valid pmd");
			auto d_offset = *(uint32_t*)(b + offset);
			if (d_offset >= size)
				throw incorrect_format("not a valid pmd");
			string path;
			for (auto cc = d_offset;; cc++)
			{
				if (cc >= size)
					throw incorrect_format("not a valid pmd");
				if (b[cc] == '\0')
					break;
				path += b[cc];
			}
			set.insert(std::move(path));
		}
		return set;
	}
}

using namespace ssharp::parser::pmd;

parser_result_t __SSHARP_PMD_CALLTYPE sspmd_parseBuff(char* buff, size_t size, char*** set, int* set_size)
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