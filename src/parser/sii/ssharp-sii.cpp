#include "ssharp-sii.h"

namespace ssharp::parser::sii
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
		quote = false;
		vector<size_t> colons;
		for (size_t c = 0; c < size; c++)
		{
			if (buff[c] == '\"')
				quote = !quote;
			if (buff[c] == ':'&&!quote)
				colons.push_back(c);
			else if (c + 8 <= size && string(buff.get() + c, 8) == "@include")
				colons.push_back(c + 8);
		}
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
						if (d == size)
							throw incorrect_format("corrupted");
						if (buff[d] == '\"')
						{
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
							int64_t e = c - 1;
#else
							int32_t e = c - 1;
#endif
							bool is_uimat = false;
							while (e>=0&&(buff[e] == '\r' || buff[e] == '\n' || buff[e] == '\t' || buff[e] == ' '))
							{
								e--;
								continue;
							}

							string str2;
							while (e >= 0)
							{
#define e(x) buff[e]==x
								if (e('\r') || e('\n') || e('\t') || e(' ') || e('{') || e('}') || e('\"'))
#undef e
								{
									if (str2 == "icon")
									{
										str = str[0] == '/' ? str + ".mat" : (string)"/material/ui/accessory/" + str + ".mat";
										is_uimat = true;
									}
									break;
								}
								if (is_uimat)
									break;
								str2 = (string)"" + buff[e] + str2;
								e--;
							} 
							while (str.find("|") != str.npos)
							{
								if (str.find("|") + 1 < str.size())
									str = str.substr(str.find("|") + 1);
								else
									break;
							}
#define nfind(x) str.find(x) == str.npos
#define found(x) str.find(x) != str.npos
							if (nfind('@') && nfind('<') && nfind('>') && nfind('*') &&
								nfind(':') && nfind('\\') && nfind('|') && found('.') &&
								str[0] != ' ' && nfind("https://")&&str!="."&&str!="..")
#undef nfind
								ret.insert(str);
							break;
						}
						str += buff[d];
						d++;
					}
				}
				break;
			}
		}
		return ret;
	}
}

using namespace ssharp::parser::sii;

parser_result __SSHARP_SII_CALLTYPE sssii_parseBuff(char* buff, size_t size, char*** set, int* set_size)
{
	try
	{
		*set = nullptr;
		auto&& t_set = parseBuff(std::make_pair(buff_t(buff), size));
		if (!t_set.size())
			return parser_no_path_to_parse;
		*set = multi_paths_parser::setExporter(t_set);
	}
	catch (incorrect_format)
	{
		return  parser_incorrect_format;
	}
	catch (bad_alloc)
	{
		return  parser_buffer_error;
	}
	catch (...)
	{
		return  parser_unknown_error;
	}
	return  parser_ok;
}