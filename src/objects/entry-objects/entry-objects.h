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
#include "ssharp-dictionary.h"
#include "ssharp-mat.h"
#include "ssharp-tobj.h"
#include "ssharp-soundref.h"
#include "absentable.h"
#include "parser-utils.h"
#include "ssharp-deflate.h"
#include "ssharp-cityhash.h"
#include "ssharp-3nk.h"
#include "ssharp-contents.h"


using std::string;
using std::shared_ptr;
using std::ifstream;
using namespace ssharp::types;
using namespace ssharp::exceptions;
using namespace ssharp::parser;
using namespace ssharp::containers;
using namespace ssharp::utils::deflate;
using namespace ssharp::utils;
using namespace ssharp::dictionary;

namespace ssharp::entry_objects
{
	class basic_obj
	{
	protected:
		content_t source;
		abs_obj<content_t> modified;
	public:
		basic_obj() = delete;
		basic_obj(const basic_obj& rhs);
		basic_obj(basic_obj&& rhs)  noexcept;
		basic_obj(const content_t& source);
		basic_obj(content_t&& source);
		parse_buff_ft parseBuff_f = [](const buff_pair_t&) {return parsed_paths_t(); };
		virtual parsed_paths_t parseBuff();
		virtual bool isSourceCompressed() const;
		virtual bool isSourceEncrypted() const;
		virtual bool isSourceVarifySet() const;
		virtual bool isCompressed() const;
		virtual bool isModifiedEncrypted() const;
		virtual bool isDirectory() { return false; };
		virtual void compress();
		virtual void uncompress();
		virtual bool loaded() const;
		virtual void load();
		virtual void unload();
		virtual void reload();
		virtual void commit();
		virtual void setHashFromName();
		virtual bool setNameFromDictionary(const dictionary_t& dictionary);
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

	class sii_obj : public basic_obj
	{
		bool encoded = false;
		bool encrypted = false;
		bool collapsed = false;
	public:
		using basic_obj::basic_obj;
		sii_obj(const basic_obj& rhs) :basic_obj(rhs) {}
		sii_obj(basic_obj&& rhs)noexcept :basic_obj(std::move(rhs)) {}
		bool isEncoded() const;
		bool isEncrypted() const;
		bool isCollapsed() const;
		void encrypt();
		void decrypt();
		void encode();
		void decode();
		void collapse();
		void expand();
	};

	class directory_obj : public basic_obj
	{
	public:
		using basic_obj::basic_obj;
		parsed_paths_t content_list;
		virtual parsed_paths_t parseBuff() override;
		virtual bool isDirectory()override { return false; };
		directory_obj(parsed_paths_t);
		directory_obj(const basic_obj& rhs) : basic_obj(rhs) {}
		directory_obj(basic_obj&& rhs) : basic_obj(std::move(rhs)) {}
		bool insertEntry(const string& entry);
		bool eraseEntry(const string& entry);
		bool hasEntry(const string& entry);
		void saveToBuff();
	};
}
#endif