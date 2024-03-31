#ifndef SSHARP_ABSENTABLE_H
#define SSHARP_ABSENTABLE_H

#ifdef _WINDLL
#ifdef SSHARP_ABSENTABLE
#define SSHARP_ABSENTABLE_DLL __declspec(dllexport)
#else
#define SSHARP_ABSENTABLE_DLL __declspec(dllimport)
#endif
#else
#define SSHARP_ABSENTABLE_DLL
#endif

#include "types.h"
#include "exceptions.h"

namespace ssharp::absentable
{

	template<typename T>
	class abs_obj
	{
	private:
		std::shared_ptr<T> value = nullptr;
	public:
		abs_obj();
		abs_obj(T&& rhs);
		abs_obj(abs_obj& rhs);
		abs_obj(abs_obj&& rhs);
		bool attend();
		bool absent();
		void unload();
		const T& get();
		void set(T&& rhs);
		void set(abs_obj& rhs);
		void set(abs_obj&& rhs);
		bool operator==(const T& rhs);
		bool operator==(const abs_obj& rhs);
		T& operator=(T&& rhs);
		abs_obj& operator=(abs_obj& rhs);
		abs_obj& operator=(abs_obj&& rhs);
	};
}

#endif