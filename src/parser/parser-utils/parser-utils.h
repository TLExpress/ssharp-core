#pragma once

#ifndef __SSHARP_PARSERUTILS_H_
#define __SSHARP_PARSERUTILS_H_

#ifdef _WINDLL
#ifdef __SSHARP_PARSERUTILS
#define __SSHARP_PARSERUTILS_DLL __declspec(dllexport)
#else
#define __SSHARP_PARSERUTILS_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_PARSERUTILS_DLL
#endif

#ifdef __cplusplus

#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <set>

#include "types.h"
#include "exceptions.h"
#include "stream-loader.h"

using namespace ssharp::exceptions;
using namespace ssharp::types;
using namespace ssharp::utils;

using std::make_shared;
using std::bad_alloc;
using std::function;
using std::ifstream;
using std::istream;
using std::string;
using std::ios;

namespace ssharp::parser
{
	namespace multi_paths_parser
	{
		typedef function<parsed_paths_t(const buff_pair_t&)> multipath_buff_parser_ft;
		extern parsed_paths_t __SSHARP_PARSERUTILS_DLL parseStream(istream& instream, multipath_buff_parser_ft parseBuff);
		extern parsed_paths_t __SSHARP_PARSERUTILS_DLL parseStream(istream&& instream, multipath_buff_parser_ft parseBuff);
		extern parsed_paths_t __SSHARP_PARSERUTILS_DLL parseFile(const string& filename, multipath_buff_parser_ft parseBuff);
		extern char** __SSHARP_PARSERUTILS_DLL setExporter(const parsed_paths_t& set);
	}

	namespace single_path_parser
	{
		typedef function<string(const buff_pair_t&)> singlepath_buff_parser_ft;
		extern string __SSHARP_PARSERUTILS_DLL parseStream(istream& input, singlepath_buff_parser_ft parseBuff);
		extern string __SSHARP_PARSERUTILS_DLL parseStream(istream&& input, singlepath_buff_parser_ft parseBuff);
		extern string __SSHARP_PARSERUTILS_DLL parseFile(const string& filename, singlepath_buff_parser_ft parseBuff);
		extern char* __SSHARP_PARSERUTILS_DLL cstr_copy(const string& str);
	}
}

#endif

#ifdef __cplusplus
#define __SSHARP_PARSERUTILS_EXT extern "C"
#else
#define __SSHARP_PARSERUTILS_EXT extern
#endif

#ifndef __cplusplus
#include <stdio.h>
#include <stdlib.h>
#endif

#ifdef _WIN32
#define __SSHARP_PARSERUTILS_CALLTYPE __stdcall
#else
#define __SSHARP_PARSERUTILS_CALLTYPE
#endif

#define __SSHARP_PARSERUTILS_CALL __SSHARP_PARSERUTILS_DLL __SSHARP_PARSERUTILS_CALLTYPE

typedef enum { parser_ok, parser_invalid_io, parser_buffer_error, parser_incorrect_format, parser_no_path_to_parse, parser_unknown_error } parser_result_t;

typedef parser_result_t(*multi_parseBuff_ft)(char* buff, size_t size, char*** set, int* set_size);
typedef parser_result_t(*single_parseBuff_ft)(char* buff, size_t size, char** str);

__SSHARP_PARSERUTILS_EXT parser_result_t __SSHARP_PARSERUTILS_CALL ssmulti_parseFilePtr(FILE* file, char*** set, int* set_size, multi_parseBuff_ft parseBuff);
__SSHARP_PARSERUTILS_EXT parser_result_t __SSHARP_PARSERUTILS_CALL ssmulti_parseFile(char* filename, char*** set, int* set_size, multi_parseBuff_ft parseBuff);
__SSHARP_PARSERUTILS_EXT parser_result_t __SSHARP_PARSERUTILS_CALL sssingle_parseFilePtr(FILE* file, char** str, single_parseBuff_ft parseBuff);
__SSHARP_PARSERUTILS_EXT parser_result_t __SSHARP_PARSERUTILS_CALL sssingle_parseFile(char* filename, char** str, single_parseBuff_ft parseBuff);
#endif