#include "stream-loader.h"

namespace ssharp::loader::stream_loader
{
	buff_pair_t loadStream(istream& input)
	{
		try
		{
			input.exceptions(ios::failbit | ios::badbit);
		}
		catch (ios::failure e)
		{
			throw ios::failure((string)"at function \"" + __func__ + "\", input/output stream is invalid");
		}
		auto size = input.seekg(0, ios::end).tellg();
		auto buff = make_shared<char[]>(size);
		input.seekg(0).read(buff.get(), size);
		return std::make_pair(buff,size);
	}

	buff_pair_t loadStream(istream&& input)
	{
		return loadStream(input);
	}
}