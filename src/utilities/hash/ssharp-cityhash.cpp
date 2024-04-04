#include "ssharp-cityhash.h"

namespace ssharp::utils::cityhash
{
	uint64_t __stdcall SSHARP_CITYHASH_DLL hash(const string& str)
	{
		return CityHash64(str.c_str(), str.size());
	}

	uint64_t __stdcall SSHARP_CITYHASH_DLL hashSalt(const string& str, uint16_t salt)
	{
		return salt ? hash(str + std::to_string(salt)) : CityHash64(str.c_str(), str.size());
	}
}

uint64_t __stdcall SSHARP_CITYHASH_DLL sSharpHash(const char* str, size_t len)
{
	return CityHash64(str, len);
}

uint64_t __stdcall SSHARP_CITYHASH_DLL sSharpHashSalt(char* str, const size_t len, const uint16_t salt)
{
	return salt ? ssharp::utils::cityhash::hash(string(str, len) + std::to_string(salt)) : CityHash64(str, len);
}