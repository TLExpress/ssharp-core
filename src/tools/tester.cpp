
#include "ssharp-hashfs.h"
using std::string;
using namespace ssharp::fs;
using namespace ssharp::dictionary;

int main()
{
	auto list(init_list);

	auto&& entries = hashfs::loadFile("C:\\Program Files (x86)\\Steam\\steamapps\\common\\Euro Truck Simulator 2\\base.scs");
	auto&& dirs = entries.dirList();
	//auto&& lst = dirs.parseBuff([](auto) {return true; });
	system("pause");
}

