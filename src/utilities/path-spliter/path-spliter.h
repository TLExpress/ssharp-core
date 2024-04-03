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
	extern auto __SSHARP_PATHSPLITER_DLL splitPath(const string&) -> pair<string, string>;
}

#endif