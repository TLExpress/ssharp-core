#pragma once

#ifndef __SSHARP_DEFLATE_H_
#define __SSHARP_DEFLATE_H_

#ifdef _WINDLL
#ifdef __SSHARP_DEFLATE
#define __SSHARP_DEFLATE_DLL __declspec(dllexport)
#else
#define __SSHARP_DEFLATE_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_DEFLATE_DLL
#endif

#include <iostream>
#include <memory>

#include "zlib.h"
#include "zutil.h"
#include "types.h"
#include "exceptions.h"

using namespace ssharp::types;
using namespace ssharp::exceptions;

using std::make_shared;
using std::shared_ptr;
using std::istream;

namespace ssharp::deflate
{
	extern buff_pair_t deflateBuff(const buff_pair_t& inpair);
	extern buff_pair_t inflateBuff(const buff_pair_t& inpair, uint32_t outsize);
	extern uint32_t crc32Buff(const buff_pair_t& inpair);
	extern uint32_t adler32Buff(const buff_pair_t& inpair);
}


#endif