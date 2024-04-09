#include "entry-objects.h"

namespace ssharp::entry_objects
{
	//basic_obj

	bool basic_obj::makeParseReady()
	{
		if (!buffLoaded())
			loadBuff();
		/*if (isEncrypted())
			decrypt();*/
		if (isCompressed())
			uncompress();
		return parseReady();
	}

	void basic_obj::compress()
	{
		if (!isCompressed())
		{
			ptr->adler32 = adler32Buff(ptr->mbuff);
			ptr->mbuff = deflateBuff(ptr->mbuff);
			ptr->zsize = (uint32_t)ptr->mbuff->second;
			ptr->compressed = true;
		}
	}

	void basic_obj::uncompress()
	{
		if (isCompressed())
		{
			ptr->adler32.reset();
			ptr->mbuff = inflateBuff(ptr->mbuff, ptr->size);
			ptr->zsize = (uint32_t)ptr->mbuff->second;
			ptr->compressed = false;
		}
	}

	bool basic_obj::findName(const dictionary_t& dictionary)
	{
		auto const& it = dictionary.find(ptr->hash);
		if (it != dictionary.end())
		{
			ptr->filename = it->second;
			return true;
		}
		return false;
	}

	bool basic_obj::hashfsInit()
	{
		if(ptr->hash)
		{
			if (!hasFilename())
				return false;
			hashName();
			ptr->zlib_header = zlib_header_t({ 0x78,0x9c });
		}
		return true;
	}

	bool basic_obj::sysfsInit()
	{
		if (!hasFilename())
			return false;
		if (isCompressed())
			uncompress();
		return !isCompressed();
	}
	
	basic_obj& basic_obj::operator=(const basic_obj& rhs)
	{
		source = rhs.source;
		ptr = make_shared<content_t>(*rhs.ptr);
		return *this;
	}

	basic_obj& basic_obj::operator=(basic_obj&& rhs) noexcept
	{
		source = std::move(rhs.source);
		ptr = std::move(rhs.ptr);
		return *this;
	}

	//sii_obj

	bool sii_obj::makeParseReady()
	{
		if (!buffLoaded())
			loadBuff();
		/*if (isEncrypted())
			decrypt();*/
		if (!isCompressed())
			uncompress();
		while (!siiReady())
		{
			if (isSiiEncoded())
				siiDecode();
			/*if (isSiiEncrypted())
				siiDecrypt();
			if (isSiiCollapsed())
				siiExpand();*/
			if(!siiReady())
				return false;
		}
		return parseReady();
	}

	void sii_obj::siiEncode()
	{
		if (!isSiiEncoded())
		{
			ptr->mbuff = _3nk::transcoder::encodeFileBuff(ptr->mbuff);
			ptr->size = (uint32_t)ptr->mbuff->second;
			ptr->zsize.reset();
		}
	}

	void sii_obj::siiDecode()
	{
		if (isSiiEncoded())
		{
			ptr->mbuff = _3nk::transcoder::decodeFileBuff(ptr->mbuff);
			ptr->size = (uint32_t)ptr->mbuff->second;
			ptr->zsize.reset();
		}
	}

	parsed_paths_t directory_obj::parseBuff()
	{
		if (!makeParseReady())
			return parsed_paths_t();
		auto&& ret = parseBuff_f(ptr->mbuff);
		content_list = ret;
		return ret;
	}

	//directory_obj

	directory_obj::directory_obj(const string& name, const parsed_paths_t& list)
	{
		ptr->source_type = memory;
		ptr->filename = name;
		content_list = list;
		saveToBuff();
		ptr->crc32 = crc32Buff(ptr->mbuff);
		ptr->size = ptr->zsize = (uint32_t)ptr->mbuff->second;
		commit();
	}

	void directory_obj::saveToBuff()
	{
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
		ptr->mbuff = buff_pair_t(buff, buff_size);
		ptr->size = (uint32_t)buff_size;
		ptr->zsize.reset();
	}
}