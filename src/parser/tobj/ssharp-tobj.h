#pragma once

#ifndef __SSHARP_TOBJ_H_
#define __SSHARP_TOBJ_H_

#ifdef _WINDLL
#ifdef __SSHARP_TOBJ
#define __SSHARP_TOBJ_DLL __declspec(dllexport)
#else
#define __SSHARP_TOBJ_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_TOBJ_DLL
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

using std::make_unique;
using std::bad_alloc;
using std::function;
using std::ifstream;
using std::istream;
using std::vector;
using std::string;
using std::ios;
using std::set;

namespace ssharp::parser::tobj
{
	const uint32_t expected_sign = 0x7b10a01;
	typedef struct
	{
		uint32_t sign;//拉出來
		uint32_t skip0;
		uint32_t skip1;
		uint32_t skip2;
		uint32_t skip3;
		uint16_t skip4;
		uint8_t bias;
		uint8_t skip5;
		uint8_t type;			// 0x2 - generic, 0x5 - cubic
		uint8_t skip6;			// 2 or 0
		uint8_t mag_filter;	// { nearest = 0, linear = 1, default = 3 }
		uint8_t min_filter;	// { nearest = 0, linear = 1, default = 3 }
		uint8_t mip_filter;	// { trilinear = 1, nomips = 2, default = 3 }
		uint8_t skip7;			// always 0?
		uint8_t addr_u;		// { repeat = 0, clamp = 1, clamp_to_edge = 2, clamp_to_border = 3,
		uint8_t addr_v;		//   mirror = 4, mirror_clamp = 5, mirror_clamp_to_edge = 6
		uint8_t addr_w;		// }
		uint8_t nocompress;
		uint8_t skip8;
		uint8_t noanisotropic;
		uint8_t skip9;
		uint8_t skip10;
		uint8_t custom_color_space;	// color_space srgb(0[default]), tsnormal(1), color_space linear(1)
		uint8_t skip11;
	} header_t;	

	typedef struct
	{
		uint32_t length;
		uint32_t skip;
	} texture_t;

	header_t __SSHARP_TOBJ_DLL getHeaderFromBuff(const buff_pair_t& inpair);
	string __SSHARP_TOBJ_DLL parseBuff(const buff_pair_t& inpair);
}
#endif

#ifdef __cplusplus
#define __SSHARP_TOBJ_EXT extern "C"
#else
#define __SSHARP_TOBJ_EXT extern
#endif

#ifdef _WIN32
#define __SSHARP_TOBJ_CALLTYPE __stdcall
#else
#define __SSHARP_TOBJ_CALLTYPE
#endif

#define __SSHARP_TOBJ_CALL __SSHARP_TOBJ_DLL __SSHARP_TOBJ_CALLTYPE

#ifndef __cplusplus
#include <stdio.h>
#include <stdlib.h>
#include "parser-utils.h"
#endif

// export zone :D
__SSHARP_TOBJ_EXT parser_result __SSHARP_TOBJ_CALL sstobj_parseBuff(char* buff, size_t size, char** str);

#endif