#pragma once

#ifndef __SSHARP_STREAMLOADER_H_
#define __SSHARP_STREAMLOADER_H_

#ifdef _WINDLL
#ifdef __SSHARP_STREAMLOADER
#define __SSHARP_STREAMLOADER_DLL __declspec(dllexport)
#else
#define __SSHARP_STREAMLOADER_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_STREAMLOADER_DLL
#endif

#include <iostream>
#include "types.h"

using std::istream;
using std::string;
using std::pair;
using std::ios;
using namespace ssharp::types;

namespace ssharp::utils::stream_loader
{
	extern buff_pair_t __SSHARP_STREAMLOADER_DLL loadStream(istream& instream,size_t pos, size_t insize);
	extern buff_pair_t __SSHARP_STREAMLOADER_DLL loadStream(istream&& instream, size_t pos, size_t insize);
	extern buff_pair_t __SSHARP_STREAMLOADER_DLL loadStream(istream& instream);
	extern buff_pair_t __SSHARP_STREAMLOADER_DLL loadStream(istream&& instream);
}

#endif