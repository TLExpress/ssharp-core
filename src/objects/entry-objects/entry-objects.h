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
#include "ssharp-directory.h"
#include "ssharp-mat.h"
#include "ssharp-tobj.h"
#include "ssharp-soundref.h"
#include "absentable.h"
#include "parser-utils.h"
#include "ssharp-deflate.h"
#include "ssharp-cityhash.h"
#include "ssharp-3nk.h"


using std::string;
using std::shared_ptr;
using std::ifstream;
using namespace ssharp::types;
using namespace ssharp::exceptions;
using namespace ssharp::parser;
using namespace ssharp::absentable;
using namespace ssharp::deflate;

namespace ssharp::entry_objects
{
	enum source_type_t { memory, file, hashfs, zipfs };

	struct content_t
	{
		source_type_t source_type = memory;
		abs_obj<string> filename;
		abs_obj<uint64_t> hash;
		abs_obj<uint16_t> salt;
		abs_obj<string> source_name;
		abs_obj<size_t> source_pos;
		abs_obj<size_t> source_size;
		abs_obj<uint32_t> crc32;
		uint32_t size=0;
		uint32_t zsize=0;
		bool is_directory = false;
		bool compressed = false;
		bool varify = false;
		bool encrypted = false;
		abs_obj<zlib_header_t> zlib_header;
		abs_obj<uint32_t> adler32;
		abs_obj<buff_pair_t> mbuff;
		content_t& operator=(const content_t& rhs);
		content_t& operator=(content_t&& rhs) noexcept;
		buff_pair_t getBuff(size_t size);
		buff_pair_t loadBuff();
	};

	class basic_obj
	{
	protected:
		content_t source;
		abs_obj<content_t> modified;
		parse_buff_ft parseBuff_f = [](const buff_pair_t&) {return parsed_paths_t(); };
		basic_obj();
	public:
		basic_obj(const basic_obj& rhs);
		basic_obj(basic_obj&& rhs)  noexcept;
		basic_obj(const content_t& source);
		basic_obj(content_t&& source);
		virtual parsed_paths_t parseBuff();
		virtual bool isSourceCompressed() const;
		virtual bool isSourceEncrypted() const;
		virtual bool isSourceVarifySet() const;
		virtual bool isCompressed() const;
		virtual bool isEncoded() const;
		virtual bool isEncrypted() const;
		virtual bool isSelfEncrypted() const;
		virtual bool isCollapsed() const;
		virtual bool isDirectory() const;
		virtual void compress();
		virtual void uncompress();
		virtual bool loaded() const;
		virtual void load();
		virtual void unload();
		virtual void reload();
		virtual void commit();
		virtual void encrypt();
		virtual void decrypt();
		virtual void encode();
		virtual void decode();
		virtual void collapse();
		virtual void expand();
		virtual void setHashFromName();
		virtual bool setNameFromDictionary(dictionary_t dictionary);
		virtual void changeHashSalt(uint16_t salt);
		virtual content_t& getSource();
		virtual content_t& getModified();
		virtual const content_t& getSource() const;
		virtual const content_t& getModified() const;
		virtual basic_obj& operator=(const basic_obj& rhs);
		virtual basic_obj& operator=(basic_obj&& rhs) noexcept;
		virtual const content_t* operator->() const;
		virtual const content_t& operator*() const;
		virtual content_t* operator->();
		virtual content_t& operator*();
	};

	class unknown_obj : public basic_obj
	{
		using basic_obj::basic_obj;
	};

	class sii_obj : public basic_obj
	{
		using basic_obj::basic_obj;
		bool encoded;
		bool encrypted;
		bool collapsed;
		sii_obj() : basic_obj() { parseBuff_f = sii::parseBuff; }
	public:
		sii_obj(const basic_obj& rhs);
		sii_obj(basic_obj&& rhs):sii_obj(){}
		bool isEncoded() const override;
		bool isSelfEncrypted() const override;
		bool isCollapsed() const override;
		void encode() override;
		void decode() override;
	};

	class sui_obj : public basic_obj
	{
		using basic_obj::basic_obj;
		sui_obj() : basic_obj() { parseBuff_f = sii::parseBuff; }
	public:
		sui_obj(const basic_obj& rhs) : sui_obj() { source = rhs.getSource(); modified = rhs.getModified(); }
		sui_obj(basic_obj&& rhs) : sui_obj(rhs) {}
	};

	class pmd_obj : public basic_obj
	{
		using basic_obj::basic_obj;
		pmd_obj() : basic_obj() { parseBuff_f = pmd::parseBuff; }
	public:
		pmd_obj(const basic_obj& rhs) : pmd_obj() { source = rhs.getSource(); modified = rhs.getModified(); }
		pmd_obj(basic_obj&& rhs) : pmd_obj(rhs) {}
	};

	class mat_obj : public basic_obj
	{
		using basic_obj::basic_obj;
		mat_obj() : basic_obj() { parseBuff_f = pmd::parseBuff; }
	public:
		mat_obj(const basic_obj& rhs) : mat_obj() { source = rhs.getSource(); modified = rhs.getModified(); }
		mat_obj(basic_obj&& rhs) : mat_obj(rhs) {}
	};

	class tobj_obj : public basic_obj
	{
		using basic_obj::basic_obj;
		tobj_obj() : basic_obj() { parseBuff_f = pmd::parseBuff; }
	public:
		tobj_obj(const basic_obj& rhs) : tobj_obj() { source = rhs.getSource(); modified = rhs.getModified(); }
		tobj_obj(basic_obj&& rhs) : tobj_obj(rhs) {}
	};

	class soundref_obj : public basic_obj
	{
		using basic_obj::basic_obj;
		soundref_obj() : basic_obj() { parseBuff_f = pmd::parseBuff; }
	public:
		soundref_obj(const basic_obj& rhs) : soundref_obj() { source = rhs.getSource(); modified = rhs.getModified(); }
		soundref_obj(basic_obj&& rhs) : soundref_obj(rhs) {}
	};

	class directory_obj : public basic_obj
	{
		using basic_obj::basic_obj;
		directory_obj() : basic_obj() { parseBuff_f = directory::parseBuff; }
	public:
		parsed_paths_t content_list;
		virtual parsed_paths_t parseBuff() override;
		directory_obj(parsed_paths_t);
		directory_obj(const basic_obj& rhs) : directory_obj() { source = rhs.getSource(); modified = rhs.getModified(); }
		directory_obj(basic_obj&& rhs) : directory_obj(rhs) {}
		bool insertEntry(const string& entry);
		bool eraseEntry(const string& entry);
		void saveToBuff();
	};
}
#endif