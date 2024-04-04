#include "ssharp-hashfs.h"

namespace ssharp::fs::hashfs
{

	entry_list::entry_list SSHARP_HASHFS_DLL ssharp::fs::hashfs::loadFile(const string& filename)
	{
		entry_list::entry_list list;
		std::ifstream file(filename, ios::in | ios::binary);
		header_t header = {};
		file.read((char*)&header, sizeof(header));
		file.seekg(header.offset);
		for (uint32_t c = 0; c < header.entries_count; c++)
		{
			entry_t entry = {};
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
			content.zsize = entry.zsize - sizeof(zlib_header) - sizeof(adler);
			content.is_directory = entry.flags & is_directory;
			content.compressed = entry.flags & is_compressed;
			content.varify = entry.flags & varify;
			content.encrypted = entry.flags & encrypted;
			content.zlib_header = zlib_header;
			content.adler32 = adler;
			list.push_back(is_directory ? directory_obj(std::move(content)) : basic_obj(std::move(content)));
		}
		return list;
	}
}
