#ifndef __SSHARP_PTR_H
#define __SSHARP_PTR_H

#ifdef _WINDLL
#ifdef __SSHARP_PTR
#define __SSHARP_PTR_DLL __declspec(dllexport)
#else
#define __SSHARP_PTR_DLL __declspec(dllimport)
#endif
#else
#define __SSHARP_PTR_DLL
#endif

#include "types.h"
#include "exceptions.h"

using namespace ssharp;
using namespace ssharp::types;
using std::make_shared;

namespace ssharp::pointers
{

	template<typename U,typename T>
	concept DerivedType = std::derived_from<U, T> || std::is_same_v<U, T> || 
		(std::is_integral_v<U> && std::is_integral_v<T>);


	template<typename T>
	class __SSHARP_PTR_DLL ss_cptr
	{
	protected:
		std::shared_ptr<T> ptr = nullptr;
	public:
		constexpr ss_cptr() noexcept {}

		template<DerivedType<T> U>
		ss_cptr(const U& rhs) { set(rhs); }
		template<DerivedType<T> U>
		ss_cptr(T&& rhs) noexcept { set(std::move(rhs)); }

		ss_cptr(const ss_cptr& rhs) { if (rhs) *this = rhs; }
		ss_cptr(ss_cptr&& rhs) noexcept { if (rhs) *this = std::move(rhs); }

		~ss_cptr() {}

		T* get() { return ptr.get(); }
		const T* get() const { return ptr.get(); }

		template<DerivedType<T> U>
		void set(const U& rhs) { ptr = make_shared<U>(rhs); }
		template<DerivedType<T> U>
		void set(U&& rhs) noexcept { ptr = make_shared<U>(std::move(rhs)); }

		void reset() { ptr.reset(); }

		const T* operator->() const { return ptr.get(); }
		const T& operator*() const { return *ptr; }

		T* operator->() { return ptr.get(); }
		T& operator*() { return *ptr; }
		
		//template<DerivedType<T> U>
		T& operator=(const T& rhs) {
			this->set(rhs);
			return **this;
		}
		
		//template<DerivedType<T> U>
		T& operator=(T&& rhs) noexcept {
			this->set(std::move(rhs));
			return **this;
		}
		
		ss_cptr& operator=(const ss_cptr& rhs) { 
			if (rhs)
				ptr = make_shared<T>(*rhs); 
			return *this;
		}
		ss_cptr& operator=(ss_cptr&& rhs) noexcept {
			if (rhs)
				ptr = make_shared<T>(std::move(*rhs));
			return *this;
		}

		operator bool() { return (bool)ptr; }
		operator const bool() const { return (bool)ptr; }
		operator T& () { return *ptr; }
		operator const T& () const { return *ptr; }

	};

	template<typename T>
	class __SSHARP_PTR_DLL ss_ptr : public ss_cptr<T>
	{
	public:
		using ss_cptr<T>::ss_cptr;
		using ss_cptr<T>::operator=;
		ss_ptr(const ss_ptr& rhs) { if (rhs) *this = rhs; }
		ss_ptr(ss_ptr&& rhs) noexcept { if (rhs) *this = std::move(rhs); }

		ss_ptr& operator=(const ss_ptr& rhs) {
			if (rhs)
				this->ptr = rhs.ptr;
			return *this;
		}
		ss_ptr& operator=(ss_ptr&& rhs) noexcept {
			if (rhs)
				this->ptr = std::move(rhs.ptr);
			return *this;
		}
	};
}

#endif
