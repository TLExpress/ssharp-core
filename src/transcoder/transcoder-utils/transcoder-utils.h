#pragma once

#ifndef __SSHARP_TRANSCUTILS_H_
#define __SSHARP_TRANSCUTILS_H_

#ifdef _WINDLL
#ifdef __SSHARP_TRANSCUTILS
#define __SSHARP_TRANSCUTILS_DLL __declspec(dllexport)
#else
#define __SSHARP_TRANSCUTILS_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_TRANSCUTILS_DLL
#endif

#ifdef __cplusplus

#include <functional>
#include <intrin.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

#include "types.h"
#include "exceptions.h"
#include "stream-loader.h"

using namespace ssharp::exceptions;
using namespace ssharp::utils;
using namespace ssharp::types;
using std::stringstream;
using std::make_shared;
using std::shared_ptr;
using std::bad_alloc;
using std::function;
using std::ifstream;
using std::ofstream;
using std::istream;
using std::ostream;
using std::string;
using std::cerr;
using std::endl;
using std::ios;

namespace ssharp::transcoder
{
	typedef function<buff_pair_t(const buff_pair_t&)> buff_transcoder_ft;

	template<typename Istream, typename Ostream>
	extern void __SSHARP_TRANSCUTILS_DLL transcodeStream(Istream&& input, Ostream&& output, buff_transcoder_ft transcoder)
	{
		try
		{
			output.exceptions(ios::failbit | ios::badbit);
		}
		catch (ios::failure e)
		{
			throw ios::failure((string)"at function \"" + __func__ + "\", input/output stream is invalid");
		}
		auto insize = (size_t)input.seekg(0, ios::end).tellg();
		buff_t inbuff = stream_loader::loadStream(input);
		auto outpair = transcoder(buff_pair_t(inbuff, insize));
		output.seekp(0).write(outpair, outpair);
	}
	extern void __SSHARP_TRANSCUTILS_DLL transcodeFile(const string& infilename, const string& outfilename, buff_transcoder_ft transcoder);
	extern void __SSHARP_TRANSCUTILS_DLL transcodeFile(const string& infilename, buff_transcoder_ft transcoder);
}
#endif

#ifdef __cplusplus
#define __SSHARP_TRANSCUTILS_EXT extern "C"
#else
#define __SSHARP_TRANSCUTILS_EXT extern 
#endif

#ifndef __cplusplus
#include <stdio.h>
#include <stdlib.h>
#endif

#ifdef _WIN32
#define __SSHARP_TRANSCUTILS_CALLTYPE __stdcall
#else
#define __SSHARP_TRANSCUTILS_CALLTYPE
#endif

#define __SSHARP_TRANSCUTILS_CALL __SSHARP_TRANSCUTILS_DLL __SSHARP_TRANSCUTILS_CALLTYPE

typedef enum { transcoder_ok, transcoder_invalid_io, transcoder_buffer_error, transcoder_incorrect_format, transcoder_unknown_error } transcoder_result_t;
typedef transcoder_result_t(*transcoder_ft) (char* inbuff, size_t insize, char** outbuff, size_t* outsize);

__SSHARP_TRANSCUTILS_EXT transcoder_result_t __SSHARP_TRANSCUTILS_CALL sstrans_transcodeFilePtr(FILE* in, FILE* out, transcoder_ft transcoder);
__SSHARP_TRANSCUTILS_EXT transcoder_result_t __SSHARP_TRANSCUTILS_CALL sstrans_transcodeFile(char* inname, char* outname, transcoder_ft transcoder);
__SSHARP_TRANSCUTILS_EXT transcoder_result_t __SSHARP_TRANSCUTILS_CALL sstrans_transcodeFileSingle(char* inname, transcoder_ft transcoder);

#endif