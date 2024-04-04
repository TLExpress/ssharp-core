#include "transcoder-utils.h"

namespace ssharp::transcoder
{
	void transcodeFile(const string& infilename, const string& outfilename, buff_transcoder_ft transcoder)
	{
		if (infilename == outfilename)
		{
			stringstream ss(ios::in | ios::out | ios::binary);
			transcodeStream(ifstream(infilename, ios::in | ios::binary), ss, transcoder);
			ofstream(outfilename, ios::out | ios::binary) << ss.seekg(0).rdbuf();
		}
		else
			transcodeStream(ifstream(infilename, ios::in | ios::binary), ofstream(outfilename, ios::out | ios::binary), transcoder);
	}

	void transcodeFile(const string& infilename, buff_transcoder_ft transcoder)
	{
		transcodeFile(infilename, infilename, transcoder);
	}
}

#define copen(x) if(!(x)) return transcoder_invalid_io
transcoder_result_t __SSHARP_TRANSCUTILS_CALLTYPE sstrans_transcodeFilePtr(FILE* in, FILE* out, transcoder_ft transcoder)
{
	try
	{
		copen(in && out);
		fseek(in, 0, SEEK_END);
		auto insize = ftell(in);
		fseek(in, 0, SEEK_SET);
		buff_pair_t inbuff(insize);
		fread(inbuff, sizeof(char), inbuff, in);
		char* outbuff = nullptr;
		size_t outsize;
		transcoder(inbuff, inbuff, &outbuff, &outsize);
		fwrite(outbuff, sizeof(char), outsize, out);
	}
	catch (incorrect_format e)
	{
		return transcoder_incorrect_format;
	}
	catch (bad_alloc)
	{
		return transcoder_buffer_error;
	}
	catch (ios::failure)
	{
		return transcoder_invalid_io;
	}
	catch (...)
	{
		return transcoder_unknown_error;
	}
	return transcoder_ok;
}

transcoder_result_t __SSHARP_TRANSCUTILS_CALLTYPE sstrans_transcodeFile(char* inname, char* outname, transcoder_ft transcoder)
{
	FILE* in; FILE* out;
	fopen_s(&in, inname, "rb");
	fopen_s(&out, outname, "wb");
	copen(in && out);
	auto ret = sstrans_transcodeFilePtr(in, out, transcoder);
	fclose(in);
	fclose(out);
	return ret;
}

transcoder_result_t __SSHARP_TRANSCUTILS_CALLTYPE sstrans_transcodeFileSingle(char* inname, transcoder_ft transcoder)
{
	try
	{
		FILE* in;
		fopen_s(&in, inname, "rb");
		copen(in);
		fseek(in, 0, SEEK_END);
		auto insize = ftell(in);
		fseek(in, 0, SEEK_SET);
		buff_pair_t inbuff(insize);
		fread(inbuff, sizeof(char), inbuff, in);
		fclose(in);
		char* outbuff = nullptr;
		size_t outsize;
		transcoder(inbuff, inbuff, &outbuff, &outsize);
		fopen_s(&in, inname, "wb");
		copen(in);
		fwrite(outbuff, sizeof(char), outsize, in);
		fclose(in);
	}
	catch (incorrect_format e)
	{
		return transcoder_incorrect_format;
	}
	catch (bad_alloc)
	{
		return transcoder_buffer_error;
	}
	catch (ios::failure)
	{
		return transcoder_invalid_io;
	}
	catch (...)
	{
		return transcoder_unknown_error;
	}
	return transcoder_ok;
}
#undef copen