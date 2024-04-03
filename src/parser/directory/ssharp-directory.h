#pragma once

#ifndef __SSHARP_DIRECTORY_H_
#define __SSHARP_DIRECTORY_H_

#ifdef _WINDLL
#ifdef __SSHARP_DIRECTORY
#define __SSHARP_DIRECTORY_DLL __declspec(dllexport)
#else
#define __SSHARP_DIRECTORY_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_DIRECTORY_DLL
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

using namespace ssharp::exceptions;
using namespace ssharp::types;
using namespace ssharp::loader;
using namespace ssharp::parser::utils;

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::bad_alloc;
using std::function;
using std::ifstream;
using std::ofstream;
using std::istream;
using std::ostream;
using std::vector;
using std::string;
using std::ios;
using std::set;

namespace ssharp::parser::directory
{
	parsed_paths_t __SSHARP_DIRECTORY_DLL parseBuff(const buff_pair_t& inpair);
}
#endif

#ifdef __cplusplus
#define __SSHARP_DIRECTORY_EXT extern "C"
#else
#define __SSHARP_DIRECTORY_EXT extern
#endif

#ifdef _WIN32
#define __SSHARP_DIRECTORY_CALLTYPE __stdcall
#else
#define __SSHARP_DIRECTORY_CALLTYPE
#endif

#define __SSHARP_DIRECTORY_CALL __SSHARP_DIRECTORY_DLL __SSHARP_DIRECTORY_CALLTYPE

#ifndef __cplusplus
#include <stdio.h>
#include <stdlib.h>
#include "parser-utils.h"
#endif

__SSHARP_DIRECTORY_EXT parser_result __SSHARP_DIRECTORY_CALLTYPE ssdirectory_parseBuff(char* buff, size_t size, char*** set, int* set_size);

#endif