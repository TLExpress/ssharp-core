#ifndef SSHARP_BPTR_H
#define SSHARP_BPTR_H

#ifdef _WINDLL
#ifdef SSHARP_BPTR
#define SSHARP_BPTR_DLL __declspec(dllexport)
#elseabs_obj
#define SSHARP_BPTR_DLL __declspec(dllimport)
#endif
#else
#define SSHARP_BPTR_DLL
#endif

#include "types.h"
#include "exceptions.h"

using namespace ssharp;
using std::make_shared;

namespace ssharp::containers
{
	template<typename T>
	class bptr_obj
	{
	protected:
		std::shared_ptr<T> value = nullptr;
		bptr_obj() {}
	public:
		bptr_obj(const T& rhs) { set(rhs); }
		bptr_obj(T&& rhs) { set(std::move(rhs)); }

		~bptr_obj() {}

		T& get() { return *value.get(); }
		const T& get() const { return *value.get(); }

		void set(const T& rhs) { value = make_shared<T>(rhs); }
		void set(T&& rhs) noexcept { value = make_shared<T>(std::move(rhs)); }

		const T* operator->() const { return value.get(); }
		const T& operator*() const { return *value.get(); }

		T* operator->() { return value.get(); }
		T& operator*() { return *value.get(); }

		T& operator=(const T& rhs) {
			this->set(rhs);
			return this->get();
		}
		T& operator=(T&& rhs) noexcept {
			this->set(std::move(rhs));
			return this->get();
		}
	};
}

#endif
