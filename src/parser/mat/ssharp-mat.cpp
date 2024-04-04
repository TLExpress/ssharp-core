#include "ssharp-mat.h"

namespace ssharp::parser::mat
{
	parsed_paths_t parseBuff(const buff_pair_t& inpair)
	{
		auto& inbuff = inpair.first;
		auto& size = inpair.second;
		auto buff = make_unique<char[]>(size);
		std::memcpy(buff.get(), inbuff.get(), size);
		bool quote = false;
		bool comment = false;
		for (size_t c = 0; c < size; c++)
		{
			if (buff[c] == '\"')
				quote = !quote;
			else if (buff[c] == '\r' || buff[c] == '\n')
			{
				quote = false;
				comment = false;
			}
			else if (buff[c] == '#')
				if (!quote)
				{
					comment = true;
					buff[c] = ' ';
				}
			else
				if (!quote && comment)
					buff[c] = ' ';

		}
		vector<size_t> colons;
		for (size_t c = 1; c < size; c++)
			if (buff[c] == ':')
				colons.push_back(c);
		parsed_paths_t ret;
		for (auto const& c : colons)
		{
			auto d = c + 1;
			while (d <= size)
			{
				if (d == size)
				{
					if (quote)
						throw incorrect_format("corrupted");
					else 
						return ret;
				}
				if (buff[d] == '\r' || buff[d] == '\n' || buff[d] == '\t' || buff[d] == ' ')
				{
					d++;
					continue;
				}
				if (buff[d] == '\"')
				{
					string str;
					d++;
					while (d <= size)
					{
						if(d==size)
							throw incorrect_format("corrupted");
						if (buff[d] == '\"')
						{
							ret.insert(str);
							break;
						}
						str += buff[d];
						d++;
					}
				}
				d++;
			}
		}
		return ret;
	}
}

using namespace ssharp::parser::mat;

parser_result_t __SSHARP_MAT_CALLTYPE ssmat_parseBuff(char* buff, size_t size, char*** set, int* set_size)
{
	try
	{
		*set = nullptr;
		auto&& t_set = parseBuff(buff_pair_t(buff,size));
		if (!t_set.size())
			return parser_no_path_to_parse;
		*set = multi_paths_parser::setExporter(t_set);
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