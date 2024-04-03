#include "stream-loader.h"

namespace ssharp::loader::stream_loader
{
	buff_pair_t loadStream(istream& input, size_t pos, size_t size)
	{
		try
		{
			input.exceptions(ios::failbit | ios::badbit);
		}
		catch (ios::failure e)
		{
			throw ios::failure((string)"at function \"" + __func__ + "\", input/output stream is invalid");
		}
		auto buff = std::make_shared<char[]>(size);
		input.seekg(pos).read(buff.get(), size);
		return std::make_pair(buff,size);
	}

	buff_pair_t loadStream(istream&& input, size_t pos, size_t size)
	{
		return loadStream(input,pos,size);
	}

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
		auto size = (size_t)input.seekg(0, ios::end).tellg();
		input.seekg(0).clear();
		return loadStream(input, 0, size);
	}

	buff_pair_t loadStream(istream&& input)
	{
		return loadStream(input);
	}
}