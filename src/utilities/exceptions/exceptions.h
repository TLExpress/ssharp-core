#pragma once

#ifndef __SSHARP_EXCEPTIONS_H_
#define __SSHARP_EXCEPTIONS_H_

#ifdef _WINDLL
#ifdef __SSHARP_EXCEPTIONS
#define __SSHARP_EXCEPTIONS_DLL __declspec(dllexport)
#else
#define __SSHARP_EXCEPTIONS_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_EXCEPTIONS_DLL
#endif

#include <iostream>

using std::string;

namespace ssharp::exceptions
{
	class __SSHARP_EXCEPTIONS_DLL basic_ssharp_exception : public std::exception
	{
		string func;
	public:
		string getFunc();
		basic_ssharp_exception(const string& str) : std::exception(str.c_str()) {}
		basic_ssharp_exception(const string& func, const string& str);
	};

	class __SSHARP_EXCEPTIONS_DLL incorrect_format : public basic_ssharp_exception 
	{ 
		using basic_ssharp_exception::basic_ssharp_exception; 
	};

	class __SSHARP_EXCEPTIONS_DLL unsuccess_zlib_operation : public basic_ssharp_exception 
	{ 
		using basic_ssharp_exception::basic_ssharp_exception; 
	};

	class __SSHARP_EXCEPTIONS_DLL value_absent : public basic_ssharp_exception 
	{ 
		using basic_ssharp_exception::basic_ssharp_exception; 
	};

	class __SSHARP_EXCEPTIONS_DLL file_absent : public basic_ssharp_exception
	{
		using basic_ssharp_exception::basic_ssharp_exception;
	};
}

#endif

