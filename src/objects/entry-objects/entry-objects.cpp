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
		reload();
	}

	basic_obj::basic_obj(content_t&& source) noexcept
	{
		this->source = std::move(source);
		reload();
	}

	parsed_paths_t basic_obj::parseBuff()
	{
		return parseBuff_f(value->mbuff);
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
		return value->compressed;
	}

	bool basic_obj::isEncrypted() const
	{
		return false;
	}
	
	void basic_obj::compress()
	{
		if (!isCompressed())
		{
			value->mbuff = deflateBuff(value->mbuff);
			value->zsize = value->mbuff->second;
		}
	}

	void basic_obj::uncompress()
	{
		if (isCompressed())
		{
			value->mbuff = inflateBuff(value->mbuff, value->size);
			value->zsize = value->mbuff->second;
		}
	}

	void basic_obj::reload()
	{
		*this = source;
	}

	void basic_obj::commit()
	{
		source = *this;
	}

	void basic_obj::hashName()
	{
		cityhash::hashSalt(value->filename->substr(1), value->salt);
	}

	void basic_obj::hashName(uint16_t salt)
	{
		value->salt = salt;
		hashName();
	}

	bool basic_obj::setNameFromDictionary(const dictionary_t& dictionary)
	{
		auto const& it = dictionary.find(value->hash);
		if (it != dictionary.end())
		{
			value->filename = it->second;
			return true;
		}
		return false;
	}
	
	basic_obj& basic_obj::operator=(const basic_obj& rhs)
	{
		source = rhs.source;
		*value = *rhs.value;
		return *this;
	}

	basic_obj& basic_obj::operator=(basic_obj&& rhs) noexcept
	{
		source = std::move(rhs.source);
		*value = std::move(*rhs.value);
		return *this;
	}

	content_t& basic_obj::getSource()
	{
		return source;
	}

	const content_t& basic_obj::getSource() const
	{
		return source;
	}

	bool sii_obj::isEncoded() const
	{
		return encoded;
	}

	bool sii_obj::isSiiEncrypted() const
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
			value->mbuff = ssharp::_3nk::transcoder::encodeFileBuff(value->mbuff);
			value->size = (uint32_t)value->mbuff->second;
			value->zsize.unload();
		}
	}

	void sii_obj::decode()
	{
		if (isEncoded())
		{
			value->mbuff = ssharp::_3nk::transcoder::decodeFileBuff(value->mbuff);
			value->size = (uint32_t)value->mbuff->second;
			value->zsize.unload();
		}
	}

	parsed_paths_t directory_obj::parseBuff()
	{
		auto&& ret = parseBuff_f(value->mbuff);
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
		value->mbuff.unload();
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
		value->mbuff = buff_pair_t(buff, buff_size);
		value->size = buff_size;
		value->zsize.unload();
	}
}