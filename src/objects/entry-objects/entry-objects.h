#pragma once

#ifndef __SSHARP_EOBJECT_H
#define __SSHARP_EOBJECT_H

#ifdef _WINDLL
#ifdef __SSHARP_EOBJECT
#define __SSHARP_EOBJECT_DLL __declspec(dllexport)
#else
#define __SSHARP_EOBJECT_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_EOBJECT_DLL
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
	class __SSHARP_EOBJECT_DLL basic_obj : protected nabs_obj<content_t>
	{
	private:
		basic_obj() {}
		using nabs_obj<content_t>::set;
		using nabs_obj<content_t>::operator=;
	protected:
		content_t source;
	public:
		parse_buff_ft parseBuff_f = [](const buff_pair_t&) {return parsed_paths_t(); };

		using nabs_obj<content_t>::get;
		using nabs_obj<content_t>::operator*;
		using nabs_obj<content_t>::operator->;
		using nabs_obj<content_t>::operator content_t&;
		using nabs_obj<content_t>::operator const content_t&;

		basic_obj(const basic_obj& rhs);
		basic_obj(basic_obj&& rhs)  noexcept;
		basic_obj(const content_t& source);
		basic_obj(content_t&& source) noexcept;

		virtual parsed_paths_t parseBuff();
		virtual bool isDirectory() { return false; };

		bool isSourceCompressed() const;
		bool isSourceEncrypted() const;
		bool isSourceVarifySet() const;
		bool isCompressed() const;
		bool isEncrypted() const;

		void compress();
		void uncompress();
		void reload();
		void commit();
		void hashName();
		void hashName(uint16_t salt);

		bool setNameFromDictionary(const dictionary_t& dictionary);
		bool hashfsInit();
		bool zipfsInit();
		bool namelistInit();
		bool sysfsInit();

		content_t& getSource();
		const content_t& getSource() const;

		basic_obj& operator=(const basic_obj& rhs);
		basic_obj& operator=(basic_obj&& rhs) noexcept;
		content_t& operator=(const content_t& rhs);
		content_t& operator=(content_t&& rhs) noexcept;

		const content_t* operator->() const;
		const content_t& operator*() const;
		content_t* operator->();
		content_t& operator*();
	};

	class __SSHARP_EOBJECT_DLL sii_obj : public basic_obj
	{
	protected:
		bool encoded = false;
		bool encrypted = false;
		bool collapsed = false;
	public:
		using basic_obj::basic_obj;
		sii_obj() = delete;
		sii_obj(const basic_obj& rhs) :basic_obj(rhs) {}
		sii_obj(basic_obj&& rhs)noexcept :basic_obj(std::move(rhs)) {}
		bool isEncoded() const;
		bool isSiiEncrypted() const;
		bool isCollapsed() const;
		void encrypt();
		void decrypt();
		void encode();
		void decode();
		void collapse();
		void expand();
	};

	class __SSHARP_EOBJECT_DLL directory_obj : public basic_obj
	{
	protected:
		parsed_paths_t content_list;
	public:
		using basic_obj::basic_obj;
		directory_obj() = delete;
		directory_obj(const basic_obj& rhs) : basic_obj(rhs) {}
		directory_obj(basic_obj&& rhs) : basic_obj(std::move(rhs)) {}
		directory_obj(const parsed_paths_t&);

		const parsed_paths_t& getList();
		parsed_paths_t parseBuff() override;
		bool isDirectory() override { return true; };
		void clearList();
		bool insertEntry(const string& entry);
		bool eraseEntry(const string& entry);
		bool hasEntry(const string& entry);
		void saveToBuff();
	};
}
#endif