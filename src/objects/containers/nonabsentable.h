#ifndef SSHARP_NONABS_H
#define SSHARP_NONABS_H

#ifdef _WINDLL
#ifdef SSHARP_NONABS
#define SSHARP_NONABS_DLL __declspec(dllexport)
#elseabs_obj
#define SSHARP_NONABS_DLL __declspec(dllimport)
#endif
#else
#define SSHARP_NONABS_DLL
#endif

#include "types.h"
#include "exceptions.h"
#include "basic-ptr.h"

namespace ssharp::containers
{
	template<typename T>
	class nabptr_obj : public bptr_obj<T>
	{
	public:
		using bptr_obj<T>::bptr_obj;
		nabptr_obj() = delete;

		nabptr_obj(const nabptr_obj& rhs) { this->set(rhs); }
		nabptr_obj(nabptr_obj&& rhs) noexcept { this->set(std::move(rhs)); }

		void set(const nabptr_obj& rhs) { this->value = rhs.value; }
		void set(nabptr_obj&& rhs) noexcept { this->value = std::move(rhs.value); }

		nabptr_obj copy() { return nabptr_obj<T>(this->get()); }

		nabptr_obj& operator=(const nabptr_obj& rhs) {
			this->set(rhs);
			return *this;
		}
		nabptr_obj& operator=(nabptr_obj&& rhs) noexcept {
			this->set(std::move(rhs));
			return *this;
		}
	};
}

#endif