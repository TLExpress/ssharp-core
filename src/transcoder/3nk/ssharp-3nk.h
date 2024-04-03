﻿#pragma once

#ifndef __SSHARP_3NK_H_
#define __SSHARP_3NK_H_

#ifdef _WINDLL
#ifdef __SSHARP_3NK
#define __SSHARP_3NK_DLL __declspec(dllexport)
#else
#define __SSHARP_3NK_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_3NK_DLL
#endif
#ifdef __cplusplus

#include <functional>
#include <intrin.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

#include "types.h"
#include "exceptions.h"

using namespace ssharp::exceptions;
using namespace ssharp::types;
using std::stringstream;
using std::make_shared;
using std::shared_ptr;
using std::bad_alloc;
using std::function;
using std::ifstream;
using std::ofstream;
using std::istream;
using std::ostream;
using std::string;
using std::cerr;
using std::endl;
using std::ios;

namespace ssharp::_3nk
{
	struct Header
	{
		uint32_t sign;
		uint8_t unk;
		uint8_t seed;
	};
	const size_t buffer_size = 0x1000; // 4KiB
	const uint32_t _3nK_sign = 0x014b6e33;
	const size_t header_size = 0x6;
	const uint8_t key_table[] = {
		0xF8, 0xD1, 0xAA, 0x83, 0x5C, 0x75, 0x0E, 0x27, 0xB0, 0x99, 0xE2, 0xCB, 0x14, 0x3D, 0x46, 0x6F,
		0x68, 0x41, 0x3A, 0x13, 0xCC, 0xE5, 0x9E, 0xB7, 0x20, 0x09, 0x72, 0x5B, 0x84, 0xAD, 0xD6, 0xFF,
		0xD8, 0xF1, 0x8A, 0xA3, 0x7C, 0x55, 0x2E, 0x07, 0x90, 0xB9, 0xC2, 0xEB, 0x34, 0x1D, 0x66, 0x4F,
		0x48, 0x61, 0x1A, 0x33, 0xEC, 0xC5, 0xBE, 0x97, 0x00, 0x29, 0x52, 0x7B, 0xA4, 0x8D, 0xF6, 0xDF,
		0xB8, 0x91, 0xEA, 0xC3, 0x1C, 0x35, 0x4E, 0x67, 0xF0, 0xD9, 0xA2, 0x8B, 0x54, 0x7D, 0x06, 0x2F,
		0x28, 0x01, 0x7A, 0x53, 0x8C, 0xA5, 0xDE, 0xF7, 0x60, 0x49, 0x32, 0x1B, 0xC4, 0xED, 0x96, 0xBF,
		0x98, 0xB1, 0xCA, 0xE3, 0x3C, 0x15, 0x6E, 0x47, 0xD0, 0xF9, 0x82, 0xAB, 0x74, 0x5D, 0x26, 0x0F,
		0x08, 0x21, 0x5A, 0x73, 0xAC, 0x85, 0xFE, 0xD7, 0x40, 0x69, 0x12, 0x3B, 0xE4, 0xCD, 0xB6, 0x9F,
		0x78, 0x51, 0x2A, 0x03, 0xDC, 0xF5, 0x8E, 0xA7, 0x30, 0x19, 0x62, 0x4B, 0x94, 0xBD, 0xC6, 0xEF,
		0xE8, 0xC1, 0xBA, 0x93, 0x4C, 0x65, 0x1E, 0x37, 0xA0, 0x89, 0xF2, 0xDB, 0x04, 0x2D, 0x56, 0x7F,
		0x58, 0x71, 0x0A, 0x23, 0xFC, 0xD5, 0xAE, 0x87, 0x10, 0x39, 0x42, 0x6B, 0xB4, 0x9D, 0xE6, 0xCF,
		0xC8, 0xE1, 0x9A, 0xB3, 0x6C, 0x45, 0x3E, 0x17, 0x80, 0xA9, 0xD2, 0xFB, 0x24, 0x0D, 0x76, 0x5F,
		0x38, 0x11, 0x6A, 0x43, 0x9C, 0xB5, 0xCE, 0xE7, 0x70, 0x59, 0x22, 0x0B, 0xD4, 0xFD, 0x86, 0xAF,
		0xA8, 0x81, 0xFA, 0xD3, 0x0C, 0x25, 0x5E, 0x77, 0xE0, 0xC9, 0xB2, 0x9B, 0x44, 0x6D, 0x16, 0x3F,
		0x18, 0x31, 0x4A, 0x63, 0xBC, 0x95, 0xEE, 0xC7, 0x50, 0x79, 0x02, 0x2B, 0xF4, 0xDD, 0xA6, 0x8F,
		0x88, 0xA1, 0xDA, 0xF3, 0x2C, 0x05, 0x7E, 0x57, 0xC0, 0xE9, 0x92, 0xBB, 0x64, 0x4D, 0x36, 0x1F,
	};

	class __SSHARP_3NK_DLL transcoder
	{
	protected:
		typedef function<buff_pair_t(const buff_pair_t&)> streamRoutine;
		typedef function<void(istream&&, ostream&)> fileRoutine;
		static void transcodeBuffer(const buff_t inbuff, buff_t outbuff, const size_t size, const int64_t seed);
		static void transcodeBuffer(const buff_t buff, const size_t size, const int64_t seed);
		static void processStream(istream& input, ostream& output, streamRoutine routine);
		static void processFile(const string& inFileName, const string& outFileName, fileRoutine routine);

	public:
		static bool is3nKFileBuff(const buff_pair_t& inpair);
		template<typename IStream>
		static bool is3nKStream(IStream&& stream);
		static bool is3nKFile(const string& FileName);
		static buff_pair_t encodeFileBuff(const buff_pair_t& inpair);
		static buff_pair_t decodeFileBuff(const buff_pair_t& inpair);
		static buff_pair_t transcodeFileBuff(const buff_pair_t& inpair);
		static buff_pair_t encodeFileBuff(const buff_pair_t& inpair, bool nodelete);
		static buff_pair_t decodeFileBuff(const buff_pair_t& inpair, bool nodelete);
		static buff_pair_t transcodeFileBuff(const buff_pair_t& inpair, bool nodelete);
		template<typename Istream, typename Ostream>
		static void encodeStream(Istream&& input, Ostream&& output);
		template<typename Istream, typename Ostream>
		static void decodeStream(Istream&& input, Ostream&& output);
		template<typename Istream, typename Ostream>
		static void transcodeStream(Istream&& input, Ostream&& output);
		static void encodeFile(const string& FileName, const string& outFileName);
		static void decodeFile(const string& inFileName, const string& outFileName);
		static void transcodeFile(const string& inFileName, const string& outFileName);
		static void encodeFile(const string& FileName);
		static void decodeFile(const string& FileName);
		static void transcodeFile(const string& FileName);
	};
}
#endif

#ifdef __cplusplus
#define __SSHARP_3NK_EXT extern "C"
#else
#define __SSHARP_3NK_EXT extern
#endif

#ifndef __cplusplus
#include <stdio.h>
#include <stdlib.h>
#endif

// export zone :D
typedef enum { s3_ok, s3_invalid_io, s3_buffer_error, s3_incorrect_format, s3_unknown_error } s3_result;
__SSHARP_3NK_EXT bool __stdcall ss3nk_is3nKFileBuff(char* buff, size_t size);
__SSHARP_3NK_EXT bool __stdcall ss3nk_is3nkFilePtr(FILE* file);
__SSHARP_3NK_EXT bool __stdcall ss3nk_is3nKFile(char* FileName);
__SSHARP_3NK_EXT int __stdcall ss3nk_encodeFileBuff(char* inbuff, size_t insize, char** outbuff, size_t* outsize);
__SSHARP_3NK_EXT int __stdcall ss3nk_decodeFileBuff(char* inbuff, size_t insize, char** outbuff, size_t* outsize);
__SSHARP_3NK_EXT int __stdcall ss3nk_transcodeFileBuff(char* inbuff, size_t insize, char** outbuff, size_t* outsize);
__SSHARP_3NK_EXT int __stdcall ss3nk_encodeFilePtr(FILE* infile, FILE* outfile);
__SSHARP_3NK_EXT int __stdcall ss3nk_decodeFilePtr(FILE* infile, FILE* outfile);
__SSHARP_3NK_EXT int __stdcall ss3nk_transcodeFilePtr(FILE* infile, FILE* outfile);
__SSHARP_3NK_EXT int __stdcall ss3nk_encodeFile(char* inFileName, char* outFileName);
__SSHARP_3NK_EXT int __stdcall ss3nk_decodeFile(char* inFileName, char* outFileName);
__SSHARP_3NK_EXT int __stdcall ss3nk_transcodeFile(char* inFileName, char* outFileName);
#endif