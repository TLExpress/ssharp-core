#pragma once

#ifndef __SSHARP_CITYHASH_H
#define __SSHARP_CITYHASH_H

#ifdef _WINDLL
#ifdef SSHARP_CITYHASH
#define SSHARP_CITYHASH_DLL __declspec(dllexport)
#else
#define SSHARP_CITYHASH_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_CITYHASH_DLL
#endif

#ifdef __cplusplus
#include <iostream>
#include <string>
#endif

#ifdef __cplusplus
extern "C"
{
#endif
#include "city.h"
#ifdef __cplusplus
}
#endif

using std::string;
using std::ostream;

namespace ssharp::utils::cityhash
{
	extern uint64_t __SSHARP_CITYHASH_DLL hash(const string& str);
	extern uint64_t __SSHARP_CITYHASH_DLL hashSalt(const string& str, uint16_t salt);
}

#ifdef _WIN32
#define __SSHARP_CITYHASH_CALLTYPE __stdcall
#else
#define __SSHARP_CITYHASH_CALLTYPE
#endif

#define __SSHARP_CITYHASH_CALL __SSHARP_CITYHASH_DLL __SSHARP_CITYHASH_CALLTYPE

#ifdef __cplusplus
#define __SSHARP_CITYHASH_EXT extern "C"
#else
#define __SSHARP_CITYHASH_EXT extern
#include <stdio.h>
#endif

__SSHARP_CITYHASH_EXT uint64_t __SSHARP_CITYHASH_CALL ssHash(const char* str, size_t len);
__SSHARP_CITYHASH_EXT uint64_t __SSHARP_CITYHASH_CALL ssHashSalt(const char* str, size_t len, uint16_t salt);

#endif