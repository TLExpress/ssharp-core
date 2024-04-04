#include "ssharp-3nk.h"

namespace ssharp::_3nk
{
// protected zone
	void transcoder::transcodeBuffer(const buff_t inbuff, buff_t outbuff, const size_t size, const int64_t seed)
	{
		auto out = outbuff.get();
		auto in = inbuff.get();
		size_t i = 0;
#ifdef __AVX2__
		size_t skip = 0;
		skip = sizeof(__m256i) / sizeof(char);
#define k(x) key_table[(uint8_t)(seed+i+x)]
		for (; i <= size - skip; i += skip)
		{
			auto in_vec = _mm256_loadu_si256((__m256i*)(in + i));
			auto key_vec = _mm256_set_epi8(
				k(31), k(30), k(29), k(28), k(27), k(26), k(25), k(24),
				k(23), k(22), k(21), k(20), k(19), k(18), k(17), k(16),
				k(15), k(14), k(13), k(12), k(11), k(10), k( 9), k( 8),
				k( 7), k( 6), k( 5), k( 4), k( 3), k( 2), k( 1), k( 0)
			);
			auto result_vec = _mm256_xor_si256(in_vec, key_vec);
			_mm256_storeu_si256((__m256i*)(out + i), result_vec);
		}
		skip = sizeof(__m128i) / sizeof(char);
		for (; i <= size - skip; i += skip)
		{
			auto in_vec = _mm_loadu_si128((__m128i*)(in + i));
			auto key_vec = _mm_set_epi8(
				k(15), k(14), k(13), k(12), k(11), k(10), k( 9), k( 8),
				k( 7), k( 6), k( 5), k( 4), k( 3), k( 2), k( 1), k( 0)
			);
			auto result_vec = _mm_xor_si128(in_vec, key_vec);
			_mm_storeu_si128((__m128i*)(out + i), result_vec);
		}
#undef k
#endif
		for (; i < size; i++)
			out[i] = in[i] ^ key_table[(uint8_t)(seed + i)];
	}

	void transcoder::transcodeBuffer(const buff_t buff, const size_t size, const int64_t seed)
	{
		transcodeBuffer(buff, buff, size, seed);
	}

// public zone

	bool transcoder::is3nKFileBuff(const buff_pair_t& inpair)
	{
		return inpair.second >= header_size && *(uint32_t*)(inpair.first.get()) == _3nK_sign;
	}

	template<typename IStream>
	bool transcoder::is3nKStream(IStream&& stream)
	{
		try
		{
			stream.exceptions(ios::failbit | ios::badbit);
		}
		catch (ios::failure e)
		{
			throw ios::failure((string)"at function \"" + __func__ + "\", input stream is invalid");
		}
		Header header = {};
		size_t pos = (size_t)stream.tellg();
		size_t size = (size_t)stream.seekg(0, ios::end).tellg();
		if (size - pos < header_size)
			return false;
		stream.seekg(pos).read((char*)&header, header_size).seekg(pos);
		return header.sign == _3nK_sign;
	}

	bool transcoder::is3nKFile(const string& filename)
	{
		return is3nKStream(ifstream(filename, ios::in | ios::binary));
	}

	buff_pair_t transcoder::encodeFileBuff(const buff_pair_t& inpair, bool nodelete)
	{
		auto& inbuff = inpair.first;
		auto& insize = inpair.second;
		srand((uint32_t)time(NULL));
		auto outsize = insize + header_size;
		Header header = { _3nK_sign ,0,(uint8_t)(rand() % 255 + 1) };
		shared_ptr<char[]> outbuff(new char[outsize], [&nodelete](char* s) {if (!nodelete)delete[] s; });
		*(Header*)(outbuff.get()) = header;
		transcodeBuffer(inbuff, buff_t(outbuff, outbuff.get() + header_size), insize, header.seed);
		return buff_pair_t(std::move(outbuff),outsize);
	}

	buff_pair_t transcoder::decodeFileBuff(const buff_pair_t& inpair, bool nodelete)
	{
		auto& inbuff = inpair.first;
		auto& insize = inpair.second;
		if (!is3nKFileBuff(inpair))
			throw incorrect_format(__func__, "Not a 3nK buff");
		auto outsize = insize - header_size;
		shared_ptr<char[]> outbuff(new char[outsize], [&nodelete](char* s) {if (!nodelete)delete[] s; });
		transcodeBuffer(buff_t(inbuff, inbuff.get() + header_size), outbuff, outsize, ((Header*)inbuff.get())->seed);
		return buff_pair_t(std::move(outbuff), outsize);
	}

	buff_pair_t transcoder::transcodeFileBuff(const buff_pair_t& inpair, bool nodelete)
	{
		return is3nKFileBuff(inpair) ? 
			decodeFileBuff(inpair,nodelete) :
			encodeFileBuff(inpair, nodelete);
	}

	buff_pair_t transcoder::encodeFileBuff(const buff_pair_t& inpair)
	{
		return encodeFileBuff(inpair, false);
	}

	buff_pair_t transcoder::decodeFileBuff(const buff_pair_t& inpair)
	{
		return decodeFileBuff(inpair, false);
	}

	buff_pair_t transcoder::transcodeFileBuff(const buff_pair_t& inpair)
	{
		return transcodeFileBuff(inpair, false);
	}

}

// export zone

using namespace ssharp::_3nk;

bool __stdcall ss3nk_is3nKFileBuff(char* buff, size_t size)
{
	return transcoder::is3nKFileBuff(buff_pair_t(buff,size));
}

bool __stdcall ss3nk_is3nkFilePtr(FILE* file)
{
	if (!file)
		return false;
	return transcoder::is3nKStream(ifstream(file));
}

bool __stdcall ss3nk_is3nKFile(char* filename)
{
	return transcoder::is3nKFile(filename);
}

transcoder_result_t __stdcall ss3nk_encodeFileBuff(char* inbuff, size_t insize, char** outbuff, size_t* outsize)
{
	try
	{
		auto outpair = transcoder::encodeFileBuff( buff_pair_t(inbuff ,insize) , true);
		*outsize = outpair.second;
		*outbuff = outpair.first.get();
	}
	catch (incorrect_format e)
	{
		return transcoder_incorrect_format;
	}
	catch (bad_alloc e)
	{
		return transcoder_buffer_error;
	}
	catch (...)
	{
		return transcoder_unknown_error;
	}
	return transcoder_ok;
}

transcoder_result_t __stdcall ss3nk_decodeFileBuff(char* inbuff, size_t insize, char** outbuff, size_t* outsize)
{
	try
	{
		auto outpair = transcoder::decodeFileBuff(buff_pair_t(inbuff,insize), true);
		*outsize = outpair.second;
		*outbuff = outpair.first.get();
	}
	catch (incorrect_format e)
	{
		return transcoder_incorrect_format;
	}
	catch (bad_alloc e)
	{
		return transcoder_buffer_error;
	}
	catch (...)
	{
		return transcoder_unknown_error;
	}
	return transcoder_ok;
}

transcoder_result_t __stdcall ss3nk_transcodeFileBuff(char* inbuff, size_t insize, char** outbuff, size_t* outsize)
{
	try
	{
		auto outpair = transcoder::transcodeFileBuff(buff_pair_t(inbuff,insize), true);
		*outsize = outpair.second;
		*outbuff = outpair.first.get();
	}
	catch (incorrect_format e)
	{
		return transcoder_incorrect_format;
	}
	catch (bad_alloc e)
	{
		return transcoder_buffer_error;
	}
	catch (...)
	{
		return transcoder_unknown_error;
	}
	return transcoder_ok;
}