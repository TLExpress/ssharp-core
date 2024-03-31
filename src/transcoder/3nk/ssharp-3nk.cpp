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

	void transcoder::processStream(istream& input, ostream& output, streamRoutine routine)
	{
		try
		{
			input.exceptions(ios::failbit | ios::badbit);
			output.exceptions(ios::failbit | ios::badbit);
		}
		catch(ios::failure e)
		{
			throw ios::failure((string)"at function \"" + __func__ + "\", input/output stream is invalid");
		}
		auto insize = input.seekg(0, ios::end).tellg();
		size_t outsize;
		buff_t inbuff(make_shared<char[]>((size_t)insize));
		input.seekg(0).read(inbuff.get(), insize);
		buff_t outbuff;
		routine(inbuff, (size_t)insize, outbuff, outsize);
		output.seekp(0).write(outbuff.get(), outsize);
	}

	void transcoder::processFile(const string& infilename, const string& outfilename, fileRoutine routine)
	{
		stringstream ss(ios::in | ios::out | ios::binary);
		routine(ifstream(infilename, ios::in | ios::binary), ss);
		ofstream(outfilename, ios::out | ios::binary) << ss.seekg(0).rdbuf();
	}

// public zone

	bool transcoder::is3nKFileBuff(const buff_t buff, const size_t size)
	{
		return size >= header_size && *(uint32_t*)(buff.get()) == _3nK_sign;
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

	void transcoder::encodeFileBuff(const buff_t& inbuff, const size_t insize, buff_t& outbuff, size_t& outsize)
	{
		srand((uint32_t)time(NULL));
		outsize = insize + header_size;
		Header header = { _3nK_sign ,0,(uint8_t)(rand() % 255 + 1) };
		outbuff = make_shared<char[]>(outsize);
		*(Header*)(outbuff.get()) = header;
		transcodeBuffer(inbuff, buff_t(outbuff, outbuff.get() + header_size), insize, header.seed);
	}

	void transcoder::decodeFileBuff(const buff_t& inbuff , const size_t insize, buff_t& outbuff,size_t& outsize)
	{
		if (!is3nKFileBuff(inbuff, insize))
			throw incorrect_format(__func__, "Not a 3nK buff");
		outsize = insize - header_size;
		outbuff = make_shared<char[]>(outsize);
		transcodeBuffer(buff_t(inbuff, inbuff.get() + header_size), outbuff, outsize, ((Header*)inbuff.get())->seed);
	}

	void transcoder::transcodeFileBuff(const buff_t& inbuff, size_t insize, buff_t& outbuff, size_t& outsize)
	{
		is3nKFileBuff(inbuff, insize) ? 
			decodeFileBuff(inbuff, insize, outbuff, outsize) : 
			encodeFileBuff(inbuff, insize, outbuff, outsize);
	}

	template<typename Istream, typename Ostream>
	void transcoder::encodeStream(Istream&& input, Ostream&& output)
	{
		is3nKStream(input)?
			throw incorrect_format(__func__, "Already 3nK encoded."):
			processStream(input, output, encodeFileBuff);
	}

	template<typename Istream, typename Ostream>
	void transcoder::decodeStream(Istream&& input, Ostream&& output)
	{
		processStream(input, output, decodeFileBuff);
	}

	template<typename Istream, typename Ostream>
	void transcoder::transcodeStream(Istream&& input, Ostream&& output)
	{
		is3nKStream(input) ?
			decodeStream(input, output) :
			encodeStream(input, output);
	}
#define frType static_cast<void(*)(istream&&, ostream&)>
	void transcoder::encodeFile(const string& infilename, const string& outfilename)
	{
		infilename == outfilename ?
			processFile(infilename, outfilename, frType(&encodeStream)) :
			encodeStream(ifstream(infilename, ios::in | ios::binary), ofstream(outfilename, ios::out | ios::binary));
	}

	void transcoder::decodeFile(const string& infilename, const string& outfilename)
	{
		infilename == outfilename ?
			processFile(infilename, outfilename, frType(&decodeStream)) :
			decodeStream(ifstream(infilename, ios::in | ios::binary), ofstream(outfilename, ios::out | ios::binary));
	}

	void transcoder::transcodeFile(const string& infilename, const string& outfilename)
	{
		infilename == outfilename ?
			processFile(infilename, outfilename, frType(&transcodeStream)) :
			transcodeStream(ifstream(infilename, ios::in | ios::binary), ofstream(outfilename, ios::out | ios::binary));
	}
#undef frType
	void transcoder::encodeFile(const string& filename)
	{
		encodeFile(filename, filename);
	}

	void transcoder::decodeFile(const string& filename)
	{
		decodeFile(filename, filename);
	}

	void transcoder::transcodeFile(const string& filename)
	{
		transcodeFile(filename, filename);
	}

}

// export zone

using namespace ssharp::_3nk;

bool __stdcall ss3nk_is3nKFileBuff(char* buff, size_t size)
{
	return transcoder::is3nKFileBuff(buff_t(buff), size);
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

int __stdcall ss3nk_encodeFileBuff(char* inbuff, size_t insize, char** outbuff, size_t* outsize)
{
	buff_t out(*outbuff, [](char*) {});
	try
	{
		transcoder::encodeFileBuff(buff_t(inbuff), insize, out, *outsize);
	}
	catch (incorrect_format e)
	{
		return s3_incorrect_format;
	}
	catch (bad_alloc e)
	{
		return s3_buffer_error;
	}
	catch (...)
	{
		return s3_unknown_error;
	}
	return s3_ok;
}

int __stdcall ss3nk_decodeFileBuff(char* inbuff, size_t insize, char** outbuff, size_t* outsize)
{
	try
	{
		buff_t out(*outbuff, [](char*) {});
		transcoder::decodeFileBuff(buff_t(inbuff), insize, out, *outsize);
	}
	catch (incorrect_format e)
	{
		return s3_incorrect_format;
	}
	catch (bad_alloc e)
	{
		return s3_buffer_error;
	}
	catch (...)
	{
		return s3_unknown_error;
	}
	return s3_ok;
}

int __stdcall ss3nk_transcodeFileBuff(char* inbuff, size_t insize, char** outbuff, size_t* outsize)
{
	try
	{
		buff_t out(*outbuff, [](char*) {});
		transcoder::transcodeFileBuff(buff_t(inbuff), insize, out, *outsize);
	}
	catch (incorrect_format e)
	{
		return s3_incorrect_format;
	}
	catch (bad_alloc e)
	{
		return s3_buffer_error;
	}
	catch (...)
	{
		return s3_unknown_error;
	}
	return s3_ok;
}

int __stdcall ss3nk_encodeFilePtr(FILE* infile, FILE* outfile)
{
	if (!infile || !outfile)
		return s3_invalid_io;
	try
	{
		transcoder::encodeStream(ifstream(infile), ofstream(outfile));
	}
	catch (incorrect_format e)
	{
		return s3_incorrect_format;
	}
	catch (bad_alloc e)
	{
		return s3_buffer_error;
	}
	catch (ios::failure e)
	{
		return s3_invalid_io;
	}
	catch (...)
	{
		return s3_unknown_error;
	}
	return s3_ok;
}

int __stdcall ss3nk_decodeFilePtr(FILE* infile, FILE* outfile)
{
	if (!infile || !outfile)
		return s3_invalid_io;
	try
	{
		transcoder::decodeStream(ifstream(infile), ofstream(outfile));
	}
	catch (incorrect_format e)
	{
		return s3_incorrect_format;
	}
	catch (bad_alloc e)
	{
		return s3_buffer_error;
	}
	catch (ios::failure e)
	{
		return s3_invalid_io;
	}
	catch (...)
	{
		return s3_unknown_error;
	}
	return s3_ok;
}

int __stdcall ss3nk_transcodeFilePtr(FILE* infile, FILE* outfile)
{
	if (!infile || !outfile)
		return s3_invalid_io;
	try
	{
		transcoder::transcodeStream(ifstream(infile), ofstream(outfile));
	}
	catch (incorrect_format)
	{
		return s3_incorrect_format;
	}
	catch (bad_alloc)
	{
		return s3_buffer_error;
	}
	catch (ios::failure)
	{
		return s3_invalid_io;
	}
	catch (...)
	{
		return s3_unknown_error;
	}
	return s3_ok;
}

int __stdcall ss3nk_encodeFile(char* infilename, char* outfilename)
{
	try
	{
		transcoder::encodeFile(infilename, outfilename);
	}
	catch (incorrect_format)
	{
		return s3_incorrect_format;
	}
	catch (bad_alloc)
	{
		return s3_buffer_error;
	}
	catch (ios::failure)
	{
		return s3_invalid_io;
	}
	catch (...)
	{
		return s3_unknown_error;
	}
	return s3_ok;
}

int __stdcall ss3nk_decodeFile(char* infilename, char* outfilename)
{
	try
	{
		transcoder::decodeFile(infilename, outfilename);
	}
	catch (incorrect_format)
	{
		return s3_incorrect_format;
	}
	catch (bad_alloc)
	{
		return s3_buffer_error;
	}
	catch (ios::failure)
	{
		return s3_invalid_io;
	}
	catch (...)
	{
		return s3_unknown_error;
	}
	return s3_ok;
}

int __stdcall ss3nk_transcodeFile(char* infilename, char* outfilename)
{
	try
	{
		transcoder::transcodeFile(infilename, outfilename);
	}
	catch (incorrect_format)
	{
		return s3_incorrect_format;
	}
	catch (bad_alloc)
	{
		return s3_buffer_error;
	}
	catch (ios::failure)
	{
		return s3_buffer_error;
	}
	catch (...)
	{
		return s3_unknown_error;
	}
	return s3_ok;
}