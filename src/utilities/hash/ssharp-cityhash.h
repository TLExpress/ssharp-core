#pragma once

#ifndef SSHARP_CITYHASH_H
#define SSHARP_CITYHASH_H

#ifdef _WINDLL
#ifdef SSHARP_CITYHASH
#define SSHARP_CITYHASH_DLL __declspec(dllexport)
#else
#define SSHARP_CITYHASH_DLL __declspec(dllimport)
#endif
#else
#define SSHARP_CITYHASH_DLL
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
	extern uint64_t __stdcall SSHARP_CITYHASH_DLL hash(const string& str);
	extern uint64_t __stdcall SSHARP_CITYHASH_DLL hashSalt(const string& str, uint16_t salt);
}

extern
#ifdef __cplusplus
"C"
#endif
{
	uint64_t __stdcall SSHARP_CITYHASH_DLL sSharpHash(const char* str, size_t len);
	uint64_t __stdcall SSHARP_CITYHASH_DLL sSharpHashSalt(const char* str, size_t len, uint16_t salt);
}
#endif