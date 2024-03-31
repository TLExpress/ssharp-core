#pragma once

#ifndef SSHARP_ELIST_H
#define SSHARP_ELIST_H

#ifdef _WINDLL
#ifdef SSHARP_ELIST
#define SSHARP_ELIST_DLL __declspec(dllexport)
#else
#define SSHARP_ELIST_DLL __declspec(dllimport)
#endif
#else
#define SSHARP_ELIST_DLL
#endif

#include <list>

namespace ssharp::entry_list
{
	class list_node
	{

	};
	class entry_list
	{

	};
}

#endif