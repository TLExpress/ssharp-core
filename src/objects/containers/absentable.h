#ifndef __SSHARP_ABSENTABLE_H
#define __SSHARP_ABSENTABLE_H

#ifdef _WINDLL
#ifdef __SSHARP_ABSENTABLE
#define __SSHARP_ABSENTABLE_DLL __declspec(dllexport)
#else
#define __SSHARP_ABSENTABLE_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_ABSENTABLE_DLL
#endif

#include "types.h"
#include "exceptions.h"
#include "basic-ptr.h"

namespace ssharp::containers
{
	template<typename T>
	class __SSHARP_ABSENTABLE_DLL nabs_obj : public bptr_obj<T>
	{
	public:
		using bptr_obj<T>::bptr_obj;
		nabs_obj() {}

		nabs_obj(const nabs_obj& rhs) { this->set(rhs); }
		nabs_obj(nabs_obj&& rhs) { this->set(std::move(rhs)); }

		void set(const nabs_obj& rhs) { this->value = make_shared<T>(rhs); }
		void set(nabs_obj&& rhs) noexcept { this->value = std::move((T)rhs); }

		using bptr_obj<T>::operator=;
		nabs_obj& operator=(const nabs_obj& rhs) {
			this->set(rhs);
			return *this;
		}
		nabs_obj& operator=(nabs_obj&& rhs) noexcept {
			this->set(std::move(rhs));
			return *this;
		}
	};
	template<typename T>
	class __SSHARP_ABSENTABLE_DLL abs_obj : public nabs_obj<T>
	{
	public:
		using nabs_obj<T>::nabs_obj;
		bool attend() const { return this->value != nullptr; }
		bool absent() const { return this->value == nullptr; }
		void unload() { this->value.reset(); }
	};
}

#endif