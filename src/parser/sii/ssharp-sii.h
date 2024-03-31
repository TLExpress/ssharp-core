#pragma once

#ifndef __SSHARP_SII_H_
#define __SSHARP_SII_H_

#ifdef _WINDLL
#ifdef __SSHARP_SII
#define __SSHARP_SII_DLL __declspec(dllexport)
#else
#define __SSHARP_SII_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_SII_DLL
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
#include "parser-utils.h"

using namespace ssharp::types;
using namespace ssharp::exceptions;
using namespace ssharp::loader;
using namespace ssharp::parser::utils;

using std::make_unique;
using std::bad_alloc;
using std::function;
using std::ifstream;
using std::iostream;
using std::istream;
using std::vector;
using std::string;
using std::ios;

namespace ssharp::parser::sii
{
	extern parsed_paths_t __SSHARP_SII_DLL parseBuff(const buff_pair_t& inpair);
}
#endif

#ifdef __cplusplus
#define __SSHARP_SII_EXT extern "C"
#else
#define __SSHARP_SII_EXT extern
#endif

#ifdef _WIN32
#define __SSHARP_SII_CALLTYPE __stdcall
#else
#define __SSHARP_SII_CALLTYPE
#endif

#define __SSHARP_SII_CALL __SSHARP_SII_DLL __SSHARP_SII_CALLTYPE

#ifndef __cplusplus
#include <stdio.h>
#include <stdlib.h>
#include "parser-utils.h"
#endif

// export zone :D
__SSHARP_SII_EXT parser_result __SSHARP_SII_CALL sssii_parseBuff(char* buff, size_t size, char*** set, int* set_size);

#endif