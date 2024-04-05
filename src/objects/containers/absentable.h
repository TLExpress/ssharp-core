#ifndef SSHARP_ABSENTABLE_H
#define SSHARP_ABSENTABLE_H

#ifdef _WINDLL
#ifdef SSHARP_ABSENTABLE
#define SSHARP_ABSENTABLE_DLL __declspec(dllexport)
#elseabs_obj
#define SSHARP_ABSENTABLE_DLL __declspec(dllimport)
#endif
#else
#define SSHARP_ABSENTABLE_DLL
#endif

#include "types.h"
#include "exceptions.h"
#include "basic-ptr.h"

namespace ssharp::containers
{
	template<typename T>
	class abs_obj : public bptr_obj<T>
	{
	public:
		using bptr_obj<T>::bptr_obj;
		abs_obj() {}

		abs_obj(const abs_obj& rhs) { this->set(rhs); }
		abs_obj(abs_obj&& rhs) { this->set(std::move(rhs)); }

		void set(const abs_obj& rhs) { this->value = make_shared<T>(*rhs.value.get()); }
		void set(abs_obj&& rhs) noexcept { this->value = std::move(rhs.value); }

		bool attend() const { return this->value != nullptr; }
		bool absent() const { return this->value == nullptr; }
		void unload() { this->value.reset(); }
		using bptr_obj<T>::operator=;
		abs_obj& operator=(const abs_obj& rhs) {
			this->set(rhs);
			return *this;
		}
		abs_obj& operator=(abs_obj&& rhs) noexcept {
			this->set(std::move(rhs));
			return *this;
		}
	};
}

#endif