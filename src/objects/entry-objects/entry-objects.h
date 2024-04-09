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

#include "types.h"
#include "exceptions.h"
#include "ssharp-sii.h"
#include "ssharp-pmd.h"
#include "ssharp-directory.h"
#include "ssharp-dictionary.h"
#include "ssharp-mat.h"
#include "ssharp-tobj.h"
#include "ssharp-soundref.h"
#include "ssharp-ptr.h"
#include "parser-utils.h"
#include "ssharp-deflate.h"
#include "ssharp-cityhash.h"
#include "ssharp-3nk.h"
#include "ssharp-contents.h"

using std::string;
using std::shared_ptr;
using std::ifstream;

using namespace ssharp::pointers;
using namespace ssharp::dictionary;
using namespace ssharp::parser;
using namespace ssharp::transcoder;
using namespace ssharp::exceptions;
using namespace ssharp::types;
using namespace ssharp::utils;
using namespace ssharp::utils::deflate;
using namespace ssharp::utils::cityhash;

namespace ssharp::entry_objects
{
	class __SSHARP_EOBJECT_DLL basic_obj : protected ss_cptr<content_t>
	{
	private:
		using ss_cptr<content_t>::set;
		using ss_cptr<content_t>::operator=;
	protected:
		basic_obj() {}
		content_t source;
	public:
		parse_buff_ft parseBuff_f = [](const buff_pair_t&) {return parsed_paths_t(); };

		using ss_cptr<content_t>::get;
		using ss_cptr<content_t>::operator*;
		using ss_cptr<content_t>::operator->;

		basic_obj(const basic_obj& rhs) { *this = rhs; }
		basic_obj(basic_obj&& rhs)  noexcept { *this = std::move(rhs); }
		basic_obj(const content_t& source) { *this = source; }
		basic_obj(content_t&& source) noexcept { *this = std::move(source); }

		virtual parsed_paths_t parseBuff() { return makeParseReady()? parseBuff_f(ptr->mbuff):parsed_paths_t(); }
		virtual bool isDirectory() const { return false; };
		virtual bool isSii() const { return false; }
		virtual bool parseReady() const { return !isCompressed() && !isEncrypted() && buffLoaded(); }

		bool isSourceCompressed() const { return source.compressed; }
		bool isSourceEncrypted() const { return source.encrypted; }
		bool isSourceVarifySet() const { return source.varify; }
		bool isCompressed() const { return ptr->compressed; }
		bool isEncrypted() const { return ptr->encrypted; }
		bool isVarifySet() const { return ptr->varify; }
		bool buffLoaded() const { return ptr->mbuff; }
		bool hasFilename() const { return ptr->filename; }
		bool hasHash() const { return ptr->hash; }
		uint16_t getSalt() const { return *ptr->salt; }

		virtual bool makeParseReady();//cpp
		void loadBuff() { ptr->loadBuff(); }
		void compress(); //cpp
		void uncompress();//cpp
		void reload() { *ptr = source; }
		void commit() { source = *ptr; }
		void hashName(){ hashSalt(ptr->filename->substr(1), *ptr->salt); }
		void hashName(uint16_t salt) {
			ptr->salt = salt;
			hashName();
		}

		bool findName(const dictionary_t& dictionary);

		virtual bool hashfsInit(); //cpp
		bool sysfsInit(); //cpp

		bool hashfsReady()const { return ptr->hash && ptr->zlib_header; }
		bool sysfsReady()const { return !isCompressed()&& hasFilename(); }
		bool zipfsReady()const {return !isEncrypted();}  
		bool namelistReady()const { return hasFilename(); }

		content_t& getSource(){ return source; }
		const content_t& getSource() const{ return source; }

		basic_obj& operator=(const basic_obj& rhs); //cpp
		basic_obj& operator=(basic_obj&& rhs) noexcept; //cpp
		content_t& operator=(const content_t& rhs) {
			source = rhs;
			ptr = make_shared<content_t>(source);
			return *ptr;
		}
		content_t& operator=(content_t&& rhs) noexcept {
			source = std::move(rhs);
			ptr = make_shared<content_t>(source);
			return *ptr;
		}
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
		bool isSii()const override { return true; }
		bool isSiiEncoded() const{ return encoded; }
		bool isSiiEncrypted() const{ return encrypted; }
		bool isSiiCollapsed() const{ return collapsed; }
		bool siiReady() const { return !isSiiEncoded() && !isSiiEncrypted() && !isSiiCollapsed(); }
		bool parseReady() const override { return !isCompressed() && !isEncrypted() && buffLoaded() && siiReady(); }
		bool makeParseReady()override; //cpp
		//void siiEncrypt();
		//void siiDecrypt();
		void siiEncode();//cpp
		void siiDecode();//cpp
		//void siiCollapse();
		//void siiExpand();
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
		directory_obj(const string& name, const parsed_paths_t& list);//not done

		const parsed_paths_t& getList() const{ return content_list; }
		parsed_paths_t parseBuff() override;//cpp
		bool isDirectory() const override { return true; };
		void clearList() { content_list.clear(); }
		bool insertEntry(const string& entry){ return content_list.insert(entry).second; }
		bool eraseEntry(const string& entry){ return content_list.erase(entry); }
		bool hasEntry(const string& entry){ return content_list.find(entry) != content_list.end(); }
		void saveToBuff(); //cpp
		bool hashfsInit() override {
			saveToBuff();
			return basic_obj::hashfsInit();

		}
	};
}
#endif