#include "entry-objects.h"

namespace ssharp::entry_objects
{
	content_t& content_t::operator=(const content_t& rhs)
	{
		this->source_type = rhs.source_type;
		this->filename = rhs.filename;
		this->hash = rhs.hash;
		this->salt = rhs.salt;
		this->source_name = rhs.source_name;
		this->source_pos = rhs.source_pos;
		this->source_size = rhs.source_size;
		this->crc32 = rhs.crc32;
		this->size = rhs.size;
		this->is_directory = rhs.is_directory;
		this->compressed = rhs.compressed;
		this->varify = rhs.varify;
		this->encrypted = rhs.encrypted;
		this->zlib_header = rhs.zlib_header;
		this->adler32 = rhs.adler32;
		this->mbuff = rhs.mbuff;
		return *this;
	}

	content_t& content_t::operator=(content_t&& rhs) noexcept
	{
		this->source_type = std::move(rhs.source_type);
		this->filename = std::move(rhs.filename);
		this->hash = std::move(rhs.hash);
		this->salt = std::move(rhs.salt);
		this->source_name = std::move(rhs.source_name);
		this->source_pos = std::move(rhs.source_pos);
		this->source_size = std::move(rhs.source_size);
		this->crc32 = std::move(rhs.crc32);
		this->size = std::move(rhs.size);
		this->is_directory = std::move(rhs.is_directory);
		this->compressed = std::move(rhs.compressed);
		this->varify = std::move(rhs.varify);
		this->encrypted = std::move(rhs.encrypted);
		this->zlib_header = std::move(rhs.zlib_header);
		this->adler32 = std::move(rhs.adler32);
		this->mbuff = std::move(rhs.mbuff);
		return *this;
	}

	buff_pair_t content_t::getBuff(size_t size)
	{
		if (mbuff.attend())
			return *mbuff;
		return stream_loader::loadStream(ifstream(*filename, ios::in | ios::binary),*source_pos,size);
	}

	buff_pair_t content_t::loadBuff()
	{
		if (source_type == memory)
			return *mbuff;
		mbuff = stream_loader::loadStream(ifstream(*filename, ios::in | ios::binary), *source_pos, *source_size);
		source_type = memory;
		return *mbuff;
	}

	basic_obj::basic_obj() {}

	basic_obj::basic_obj(const basic_obj& rhs)
	{
		*this = rhs;
	}

	basic_obj::basic_obj(basic_obj&& rhs) noexcept
	{
		*this = std::move(rhs);
	}

	basic_obj::basic_obj(const content_t& source)
	{
		this->source = source;
	}

	basic_obj::basic_obj(content_t&& source)
	{
		this->source = std::move(source);
	}

	parsed_paths_t basic_obj::parseBuff()
	{
		return parseBuff_f(*modified->mbuff);
	}

	bool basic_obj::isSourceCompressed() const
	{
		return source.compressed;
	}
	
	bool basic_obj::isSourceEncrypted() const
	{
		return source.encrypted;
	}

	bool basic_obj::isSourceVarifySet() const
	{
		return source.varify;
	}

	bool basic_obj::isCompressed() const
	{
		return modified->compressed;
	}

	bool basic_obj::isEncoded() const
	{
		return false;
	}

	bool basic_obj::isEncrypted() const
	{
		return modified->encrypted;
	}

	bool basic_obj::isSelfEncrypted() const
	{
		return false;
	}

	bool basic_obj::isCollapsed() const
	{
		return false;
	}

	bool basic_obj::isDirectory() const
	{
		return modified->is_directory;
	}
	
	void basic_obj::compress()
	{
		if (!isCompressed())
			modified->mbuff = deflateBuff(*modified->mbuff);
	}

	void basic_obj::uncompress()
	{
		if (isCompressed())
			modified->mbuff = inflateBuff(*modified->mbuff,modified->size);
	}
	
	void basic_obj::load()
	{
		if(modified.absent())
		modified = source;
	}

	bool basic_obj::loaded() const
	{
		return modified.attend();
	}

	void basic_obj::unload()
	{
		modified.unload();
	}

	void basic_obj::reload()
	{
		modified.unload();
		modified = source;
	}

	void basic_obj::commit()
	{
		source = *modified;
	}

	void basic_obj::encrypt() {}

	void basic_obj::decrypt() {}

	void basic_obj::encode() {}
	
	void basic_obj::decode() {}

	void basic_obj::collapse() {}

	void basic_obj::expand() {}

	void basic_obj::setHashFromName()
	{
		ssharp::cityhash::hashSalt(*modified->filename, *modified->salt);
	}

	bool basic_obj::setNameFromDictionary(dictionary_t dictionary)
	{
		auto const& it = dictionary.find(*modified->hash);
		if (it != dictionary.end())
		{
			*modified->filename = it->second;
			return true;
		}
		return false;
	}

	void basic_obj::changeHashSalt(uint16_t salt)
	{
		modified->salt = salt;
		setHashFromName();
	}
	
	basic_obj& basic_obj::operator=(const basic_obj& rhs)
	{
		source = rhs.source;
		modified = rhs.modified;
		return *this;
	}

	basic_obj& basic_obj::operator=(basic_obj&& rhs) noexcept
	{
		source = std::move(rhs.source);
		modified = std::move(rhs.modified);
		return *this;
	}

	bool sii_obj::isEncoded() const
	{
		return encoded;
	}

	bool sii_obj::isSelfEncrypted() const
	{
		return encrypted;
	}

	bool sii_obj::isCollapsed() const
	{
		return collapsed;
	}

	void sii_obj::encode()
	{
		if(!isEncoded())
			modified->mbuff = ssharp::_3nk::transcoder::encodeFileBuff(*modified->mbuff);
	}

	void sii_obj::decode()
	{
		if (isEncoded())
			modified->mbuff = ssharp::_3nk::transcoder::decodeFileBuff(*modified->mbuff);
	}

	content_t& basic_obj::getSource()
	{
		return source;
	}

	content_t& basic_obj::getModified()
	{
		return *modified;
	}

	const content_t& basic_obj::getSource() const
	{
		return source;
	}

	const content_t& basic_obj::getModified() const
	{
		return *modified;
	}

	content_t* basic_obj::operator->()
	{
		return &getModified();
	}

	content_t& basic_obj::operator*()
	{
		return getModified();
	}

	const content_t* basic_obj::operator->() const
	{
		return &*modified;
	}

	const content_t& basic_obj::operator*() const
	{
		return *modified;
	}

	parsed_paths_t directory_obj::parseBuff()
	{
		auto&& ret = parseBuff_f(*modified->mbuff);
		content_list = ret;
		return ret;
	}

	bool directory_obj::eraseEntry(const string& entry)
	{
		return content_list.erase(entry);
	}

	bool directory_obj::insertEntry(const string& entry)
	{
		return content_list.insert(entry).second;
	}

	void directory_obj::saveToBuff()
	{
		modified->mbuff.unload();
		size_t buff_size = 0;
		for (auto const& name : content_list)
			buff_size += name.size() + 1;
		buff_t buff = make_shared<char[]>(buff_size);
		size_t pos = 0;
		for (auto const& name : content_list)
		{
			sprintf_s(buff.get() + pos, buff_size-pos, "%s\x0a", name.c_str());
			pos += name.size() + 1;
		}
		modified->mbuff = std::make_pair(buff, buff_size);
	}
}