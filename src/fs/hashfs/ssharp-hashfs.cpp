#include "ssharp-hashfs.h"

namespace ssharp::fs::hashfs
{

	entry_list_t loadFile(const string& filename)
	{
		entry_list_t list;
		std::ifstream file(filename, ios::in | ios::binary);
		header_t header = {};
		file.read((char*)&header, sizeof(header));
		file.seekg(header.offset);
		for (uint32_t c = 0; c < header.entries_count; c++)
		{
			entry_t entry;
			file.read((char*)&entry, sizeof(entry));
			buff_t buff = make_shared<char[]>(entry.zsize);
			auto pos = file.tellg();
			zlib_header_t zlib_header = {};
			file.seekg(entry.offset);
			file.read((char*)&zlib_header, sizeof(zlib_header));
			uint32_t adler = 0;
			size_t source_pos = (size_t)file.tellg();
			file.seekg(entry.zsize - sizeof(zlib_header) - sizeof(adler), ios::cur);
			file.read((char*)&adler, sizeof(adler));
			file.seekg(pos);
			content_t content = {};
			content.source_type = source_type_t::file;
			content.hash = entry.hash;
			content.salt = header.salt;
			content.source_name = filename;
			content.source_pos = source_pos;
			content.crc32 = entry.crc;
			content.size = entry.size;
			content.source_size = content.zsize = entry.zsize - sizeof(zlib_header) - sizeof(adler);
			content.is_directory = entry.flags & 1;
			content.compressed = entry.flags & (1<<1);
			content.varify = entry.flags & (1<<2);
			content.encrypted = entry.flags & (1 << 3);
			content.zlib_header = zlib_header;
			content.adler32 = adler;
			if(content.is_directory)
				list.push_back(ss_ptr<basic_obj>(directory_obj(std::move(content))));
			else
				list.push_back(basic_obj(std::move(content)));
		}
		file.close();
		return list;
	}
	void storeFile(const string& filename, const entry_list_t& list)
	{
		auto tlist(list);
		tlist.hashfsInit();
		header_t header;
		header.salt = tlist.getSalt();
		header.entries_count = (uint32_t)tlist.size();
		header.offset = 0x20;//¼È®É
		ofstream file(filename, ios::out | ios::binary);
		file.write((char*)&header, sizeof(header));
		for (auto const& tentry : tlist)
		{
			entry_t entry(tentry);
			file.write((char*)&entry, sizeof(entry));
		}
		for (auto const& tentry : tlist)
		{
			file.write((char*)tentry->get()->zlib_header.get(), sizeof(zlib_header_t));
			if (tentry->get()->source_type == memory)
			{
				auto& buff = *tentry->get()->mbuff;
				file.write(buff, buff);
			}
			else
			{
				auto const& obj = *tentry;
				buff_pair_t buff(*obj->zsize);
				ifstream sfile(tentry->get()->source_name, ios::in | ios::binary);
				sfile.seekg(*obj->source_pos).read(buff,buff);
				file.write(buff, buff);
			}
			file.write((char*)tentry->get()->adler32.get(), sizeof(uint32_t));
		}
	}
}
