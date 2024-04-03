#pragma once

#ifndef __SSHARP_PMD_H_
#define __SSHARP_PMD_H_

#ifdef _WINDLL
#ifdef __SSHARP_PMD
#define __SSHARP_PMD_DLL __declspec(dllexport)
#else
#define __SSHARP_PMD_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_PMD_DLL
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

namespace ssharp::parser::pmd
{
	const uint32_t expected_version = 0x04;
	typedef struct
	{
		int32_t from;						// +0x0
		int32_t to;							// +0x4
	} attrib_link_t;

	typedef struct {
		uint64_t name;						// +0x0
		int32_t type;						// +0x8
		int32_t offset;						// +0xC
	} attrib_def_t;

	typedef struct {
		union
		{
			int int_value;
			float float_value;
		};
	} attrib_value_t;

	typedef struct {
		uint32_t version;						// 0x0

		uint32_t material_count;				// 0x4
		uint32_t look_count;					// 0x8
		uint32_t piece_count;					// 0xC
		uint32_t variant_count;					// 0x10
		uint32_t part_count;					// 0x14
		uint32_t attribs_count;					// 0x18

		uint32_t attribs_values_size;			// 0x1C
		uint32_t material_block_size;			// 0x20

		uint32_t look_offset;					// 0x24
		uint32_t variant_offset;				// 0x28
		uint32_t part_attribs_offset;			// 0x2C
		uint32_t attribs_value_offset;			// 0x30
		uint32_t attribs_offset;				// 0x34
		uint32_t material_offset;				// 0x38
		uint32_t material_data_offset;			// 0x3C
		/*
			uint64_t looks[look_count];
			uint64_t variants[variant_count];
			attrib_link_t attribs_link[part_count];
			attrib_def_t attribs_def[attribs_count];
			uint8_t attribs_value_block[variant_count * attribs_values_size];
			uint32_t materials_offset[look_count * material_count];
			uint8_t materials_data[material_block_size];
		*/
	} header_t;

	header_t __SSHARP_PMD_DLL getHeaderFromBuff(const buff_pair_t& inpair);
	parsed_paths_t __SSHARP_PMD_DLL parseBuff(const buff_pair_t& inpair);
}
#endif

#ifdef __cplusplus
#define __SSHARP_PMD_EXT extern "C"
#else
#define __SSHARP_PMD_EXT extern
#endif

#ifdef _WIN32
#define __SSHARP_PMD_CALLTYPE __stdcall
#else
#define __SSHARP_PMD_CALLTYPE
#endif

#define __SSHARP_PMD_CALL __SSHARP_PMD_DLL __SSHARP_PMD_CALLTYPE

#ifndef __cplusplus
#include <stdio.h>
#include <stdlib.h>
#include "parser-utils.h"
#endif

__SSHARP_PMD_EXT parser_result __SSHARP_PMD_CALLTYPE sspmd_parseBuff(char* buff, size_t size, char*** set, int* set_size);

#endif