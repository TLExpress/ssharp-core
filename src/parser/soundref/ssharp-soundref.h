#pragma once

#ifndef __SSHARP_SOUNDREF_H_
#define __SSHARP_SOUNDREF_H_

#ifdef _WINDLL
#ifdef __SSHARP_SOUNDREF
#define __SSHARP_SOUNDREF_DLL __declspec(dllexport)
#else
#define __SSHARP_SOUNDREF_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_SOUNDREF_DLL
#endif
#ifdef __cplusplus
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#include "types.h"
#include "exceptions.h"
#include "stream-loader.h"
#include "parser-utils.h"

using namespace ssharp::exceptions;
using namespace ssharp::types;
using namespace ssharp::utils;
using namespace ssharp::parser;

using std::make_unique;
using std::bad_alloc;
using std::function;
using std::ifstream;
using std::istream;
using std::vector;
using std::string;
using std::ios;

namespace ssharp::parser::soundref
{
	string __SSHARP_SOUNDREF_DLL parseBuff(const buff_pair_t& inpair);
}
#endif

#ifdef __cplusplus
#define __SSHARP_SOUNDREF_EXT extern "C"
#else
#define __SSHARP_SOUNDREF_EXT extern
#endif

#ifdef _WIN32
#define __SSHARP_SOUNDREF_CALLTYPE __stdcall
#else
#define __SSHARP_SOUNDREF_CALLTYPE
#endif

#define __SSHARP_SOUNDREF_CALL __SSHARP_SOUNDREF_DLL __SSHARP_SOUNDREF_CALLTYPE

#ifndef __cplusplus
#include <stdio.h>
#include <stdlib.h>
#include "parser-utils.h"
#endif

// export zone :D
__SSHARP_SOUNDREF_EXT parser_result_t __SSHARP_SOUNDREF_CALL sssoundref_parseBuff(char* buff, size_t size, char** str);

#endif