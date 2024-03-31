#include "ssharp-deflate.h"
#include "stream-loader.h"
#include <sstream>
#include <iostream>
int main()
{
	std::stringstream ss("abcdefghijklmnopqrstuvwxyz");
	auto inpair = ssharp::loader::stream_loader::loadStream(ss);
	auto defpair = ssharp::deflate::deflateBuff(inpair, Z_DEFAULT_COMPRESSION);
	auto infpair = ssharp::deflate::inflateBuff(defpair, (uint32_t)inpair.second, Z_DEFAULT_COMPRESSION);
	std::string out(infpair.first.get(), infpair.second);
	std::cout << out << std::endl;
}