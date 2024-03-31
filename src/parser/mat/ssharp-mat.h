#pragma once

#ifndef __SSHARP_MAT_H_
#define __SSHARP_MAT_H_

#ifdef _WINDLL
#ifdef __SSHARP_MAT
#define __SSHARP_MAT_DLL __declspec(dllexport)
#else
#define __SSHARP_MAT_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_MAT_DLL
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

using namespace ssharp::parser::utils;
using namespace ssharp::exceptions;
using namespace ssharp::types;
using namespace ssharp::loader;

using std::make_shared;
using std::make_unique;
using std::bad_alloc;
using std::function;
using std::ifstream;
using std::iostream;
using std::istream;
using std::vector;
using std::string;
using std::ios;

namespace ssharp::parser::mat
{
	extern parsed_paths_t __SSHARP_MAT_DLL parseBuff(const buff_pair_t& inpair);
}
#endif

#ifdef __cplusplus
#define __SSHARP_MAT_EXT extern "C"
#else
#define __SSHARP_MAT_EXT extern
#endif

#ifdef _WIN32
#define __SSHARP_MAT_CALLTYPE __stdcall
#else
#define __SSHARP_MAT_CALLTYPE
#endif

#define __SSHARP_MAT_CALL __SSHARP_MAT_DLL __SSHARP_MAT_CALLTYPE

#ifndef __cplusplus
#include <stdio.h>
#include <stdlib.h>
#include "parser-utils.h"
#endif

__SSHARP_MAT_EXT parser_result __SSHARP_MAT_CALL ssmat_parseBuff(char* buff, size_t size, char*** set, int* set_size);

#endif