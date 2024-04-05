#include "entry-objects.h"

namespace ssharp::entry_objects
{
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

	bool basic_obj::isModifiedEncrypted() const
	{
		return false;
	}
	
	void basic_obj::compress()
	{
		if (!isCompressed())
		{
			modified->mbuff = deflateBuff(*modified->mbuff);
			modified->zsize = modified->mbuff->second;
		}
	}

	void basic_obj::uncompress()
	{
		if (isCompressed())
		{
			modified->mbuff = inflateBuff(*modified->mbuff, modified->size);
			modified->zsize = modified->mbuff->second;
		}
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

	void basic_obj::setHashFromName()
	{
		cityhash::hashSalt(modified->filename->substr(1), *modified->salt-1);
	}

	bool basic_obj::setNameFromDictionary(const dictionary_t& dictionary)
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

	bool sii_obj::isEncoded() const
	{
		return encoded;
	}

	bool sii_obj::isEncrypted() const
	{
		return encrypted;
	}

	bool sii_obj::isCollapsed() const
	{
		return collapsed;
	}

	void sii_obj::encode()
	{
		if (!isEncoded())
		{
			modified->mbuff = ssharp::_3nk::transcoder::encodeFileBuff(*modified->mbuff);
			modified->size = (uint32_t)modified->mbuff->second;
			modified->zsize.unload();
		}
	}

	void sii_obj::decode()
	{
		if (isEncoded())
		{
			modified->mbuff = ssharp::_3nk::transcoder::decodeFileBuff(*modified->mbuff);
			modified->size = (uint32_t)modified->mbuff->second;
			modified->zsize.unload();
		}
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

	bool directory_obj::hasEntry(const string& entry)
	{
		return content_list.find(entry)!=content_list.end();
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
		modified->mbuff = buff_pair_t(buff, buff_size);
	}
}