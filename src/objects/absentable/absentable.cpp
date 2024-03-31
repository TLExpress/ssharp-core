#include "absentable.h"

namespace ssharp::absentable
{
	template<typename T>
	abs_obj<T>::abs_obj() {}

	template<typename T>
	abs_obj<T>::abs_obj(T&& rhs) {value = std::make_shared<T>(std::forward<T>(rhs)); }

	template<typename T>
	abs_obj<T>::abs_obj(abs_obj& rhs) { *this = rhs; }

	template<typename T>
	abs_obj<T>::abs_obj(abs_obj&& rhs) { *this = std::move(rhs); }

	template<typename T>
	bool abs_obj<T>::attend() { return value != nullptr; }

	template<typename T>
	bool abs_obj<T>::absent() { return value == nullptr; }

	template<typename T>
	void abs_obj<T>::unload() { value = nullptr; }

	template<typename T>
	const T& abs_obj<T>::get() 
	{
		if (absent())
			throw ssharp::exceptions::value_absent("value absent");
		return *value.get();
	}

	template<typename T>
	void abs_obj<T>::set(T&& rhs) {	value = make_shared<T>(std::forward<T>(rhs)); }

	template<typename T>
	void abs_obj<T>::set(abs_obj& rhs) { value = make_shared<T>(rhs.get()); }

	template<typename T>
	void abs_obj<T>::set(abs_obj&& rhs)
	{
		value = make_shared<T>(std::move(rhs.get()));
		rhs.unload();
	}

	template<typename T>
	bool abs_obj<T>::operator==(const T& rhs) { return value == rhs; }

	template<typename T>
	bool abs_obj<T>::operator==(const abs_obj& rhs) { return value == rhs.get(); }

	template<typename T>
	T& abs_obj<T>::operator=(T&& rhs)
	{ 
		this->set(std::forward<T>(rhs)); 
		return *this->get();
	}

	template<typename T>
	abs_obj<T>& abs_obj<T>::operator=(abs_obj& rhs)
	{
		this->set(rhs);
		return *this;
	}

	template<typename T>
	abs_obj<T>& abs_obj<T>::operator=(abs_obj&& rhs)
	{
		this->set(std::forward<T>(rhs));
		return *this->get(); 
	}
}