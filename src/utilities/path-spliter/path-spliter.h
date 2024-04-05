#pragma once

#ifndef __SSHARP_PATHSPLITER_H_
#define __SSHARP_PATHSPLITER_H_

#ifdef _WINDLL
#ifdef __SSHARP_PATHSPLITER
#define __SSHARP_PATHSPLITER_DLL __declspec(dllexport)
#else
#define __SSHARP_PATHSPLITER_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_PATHSPLITER_DLL
#endif

#include <iostream>
#include "types.h"

using std::istream;
using std::string;
using std::pair;
using std::ios;
using namespace ssharp::types;

namespace ssharp::utils::path_spliter
{
	extern pair<string, string> __SSHARP_PATHSPLITER_DLL splitPath(const string&);
}

#ifdef _WIN32
#define __SSHARP_PATHSPLITER_CALLTYPE __stdcall
#else
#define __SSHARP_PATHSPLITER_CALLTYPE
#endif

#define __SSHARP_PATHSPLITER_CALL __SSHARP_PATHSPLITER_DLL __SSHARP_PATHSPLITER_CALLTYPE

#ifdef __cplusplus
#define __SSHARP_PATHSPLITER_EXT extern "C"
#else
#define __SSHARP_PATHSPLUTER_EXT extern
#include <stdio.h>
#endif

__SSHARP_PATHSPLITER_EXT void __SSHARP_PATHSPLITER_CALL ssSplitPath(const char* in, char** left, char** right);

#endif