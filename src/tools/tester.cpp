
#include "ssharp-hashfs.h"
using std::string;
using namespace ssharp::fs;

int main()
{
	auto&& s = hashfs::loadFile("C:\\Program Files (x86)\\Steam\\steamapps\\common\\Euro Truck Simulator 2\\base.scs");
	system("pause");
}

