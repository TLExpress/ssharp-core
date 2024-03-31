#pragma once

#ifndef SSHARP_EOBJECT_H
#define SSHARP_EOBJECT_H

#ifdef _WINDLL
#ifdef SSHARP_EOBJECT
#define SSHARP_EOBJECT_DLL __declspec(dllexport)
#else
#define SSHARP_EOBJECT_DLL __declspec(dllimport)
#endif
#else
#define SSHARP_EOBJECT_DLL
#endif

#include <sstream>
#include <iostream>
#include "types.h"
#include "exceptions.h"
#include "ssharp-sii.h"
#include "ssharp-pmd.h"
#include "ssharp-mat.h"
#include "ssharp-tobj.h"
#include "ssharp-soundref.h"
#include "absentable.h"
#include "parser-utils.h"


using std::string;
using std::shared_ptr;
using namespace ssharp::types;
using namespace ssharp::exceptions;
using namespace ssharp::parser;
using namespace ssharp::absentable;

namespace ssharp::entry_objects
{
	enum source_type_t { memory, file, hashfs, zipfs };

	struct content_t
	{
		source_type_t source_type;
		abs_obj<string> filename;
		abs_obj<uint64_t> hash;
		abs_obj<string> source_name;
		abs_obj<size_t> source_pos;
		abs_obj<size_t> source_size;
		abs_obj<uint32_t> crc32;
		bool is_directory;
		bool compressed;
		bool varify;
		bool encrypted;
		abs_obj<zlib_header_t> zlib_header;
		abs_obj<uint32_t> adler32;
		abs_obj<buff_pair_t> mbuff;
		content_t& operator=(const content_t& rhs);
		content_t& operator=(content_t&& rhs);
	};

	class basic_obj
	{
	protected:
		content_t source;
		abs_obj<content_t> load;
		parse_buff_ft parseBuff_f = [](const buff_pair_t&) {return parsed_paths_t(); };
		basic_obj();
	public:
		basic_obj(const basic_obj& rhs);
		basic_obj(basic_obj&& rhs);
		basic_obj(const content_t& source);
		basic_obj(content_t&& source);
		virtual parsed_paths_t parseBuff();
		virtual bool isSourceConpressed();
		virtual bool isSourceEncrypted();
		virtual bool isSourceVarifySet();
		virtual bool isCompressed();
		virtual bool isEncoded();
		virtual bool isEncrypted();
		virtual bool isCollapsed();
		virtual bool compress();
		virtual bool uncompress();
		virtual void unload();
		virtual void encrypt();
		virtual void decrypt();
		virtual void encode();
		virtual void decode();
		virtual void collapse();
		virtual void expand();
		virtual void setHashFromName();
		virtual bool setNameFromDictionary();
		virtual basic_obj& operator=(const basic_obj& rhs);
		virtual basic_obj& operator=(basic_obj&& rhs);
	};

	class unknown_obj : public basic_obj
	{
		using basic_obj::basic_obj;
	};

	class sii_obj : public basic_obj
	{
	private:
		bool encoded;
		bool encrypted;
		bool collapsed;
		sii_obj() : basic_obj() { parseBuff_f = sii::parseBuff; }
	};

	class sui_obj : public basic_obj
	{
	private:
		sui_obj() : basic_obj() { parseBuff_f = sii::parseBuff; }
	};

	class pmd_obj : public basic_obj
	{
	private:
		pmd_obj() : basic_obj() { parseBuff_f = pmd::parseBuff; }
	};

	class mat_obj : public basic_obj
	{
	private:
		mat_obj() : basic_obj() { parseBuff_f = pmd::parseBuff; }
	};

	class tobj_obj : public basic_obj
	{
	private:
		tobj_obj() : basic_obj() { parseBuff_f = pmd::parseBuff; }
	};

	class soundref_obj : public basic_obj
	{
	private:
		soundref_obj() : basic_obj() { parseBuff_f = pmd::parseBuff; }
	};

}
#endif