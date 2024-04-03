#include "absentable.h"

namespace ssharp::absentable
{
	template<typename T>
	bool bptr_obj<T>::attend() const { return value != nullptr; }

	template<typename T>
	bool bptr_obj<T>::absent() const { return value == nullptr; }

	template<typename T>
	void bptr_obj<T>::unload() { this->value = nullptr; }

//	template<typename T>
//	bptr_obj<T>::bptr_obj() {}

	bptr_obj<uint64_t>::bptr_obj() {}

	template<typename T>
	bptr_obj<T>::bptr_obj(const T& rhs) { set(rhs); }

	template<typename T>
	bptr_obj<T>::bptr_obj(T&& rhs) {set(std::move(rhs)); }

	template<typename T>
	bptr_obj<T>::bptr_obj(const bptr_obj& rhs) { set(rhs); }

	template<typename T>
	bptr_obj<T>::bptr_obj(bptr_obj&& rhs) { set(std::move(rhs)); }
	template<typename T>
	bptr_obj<T>::~bptr_obj() { value = nullptr; }

	template<typename T>
	T& bptr_obj<T>::get()
	{
		if (absent())
			throw ssharp::exceptions::value_absent("value absent");
		return *value.get();
	}

	template<typename T>
	const T& bptr_obj<T>::get() const
	{
		if (absent())
			throw ssharp::exceptions::value_absent("value absent");
		return *value.get();
	}

	template<typename T>
	void bptr_obj<T>::set(const T& rhs) { *value = rhs; }

	template<typename T>
	void bptr_obj<T>::set(T&& rhs) noexcept { *value = std::move(rhs); }

	template<typename T>
	void bptr_obj<T>::set(const bptr_obj& rhs) { set(rhs.get()); }

	template<typename T>
	void bptr_obj<T>::set(bptr_obj&& rhs) noexcept
	{
		set(std::move(rhs.get()));
		rhs.unload();
	}

	template<typename T>
	bool bptr_obj<T>::operator==(const T& rhs) const { return *value == rhs; }

	template<typename T>
	bool bptr_obj<T>::operator==(const bptr_obj& rhs) const { return *value == rhs.get(); }

	template<typename T>
	T& bptr_obj<T>::operator=(const T& rhs)
	{
		this->set(rhs);
		return this->get();
	}

	template<typename T>
	T& bptr_obj<T>::operator=(T&& rhs) noexcept
	{ 
		this->set(std::move(rhs)); 
		return this->get();
	}

	template<typename T>
	bptr_obj<T>& bptr_obj<T>::operator=(const bptr_obj& rhs)
	{
		this->set(rhs);
		return *this;
	}

	template<typename T>
	bptr_obj<T>& bptr_obj<T>::operator=(bptr_obj&& rhs) noexcept
	{
		this->set(std::move(rhs));
		return *this; 
	}

	template<typename T>
	const T* bptr_obj<T>::operator->() const
	{
		if (absent())
			throw ssharp::exceptions::value_absent("value absent");
		return value.get();
	}

	template<typename T>
	T* bptr_obj<T>::operator->()
	{
		if (absent())
			throw ssharp::exceptions::value_absent("value absent");
		return value.get();
	}

	template<typename T>
	const T& bptr_obj<T>::operator*() const
	{
		return get();
	}

	template<typename T>
	T& bptr_obj<T>::operator*()
	{
		return get();
	}

	template<typename T>
	void abs_obj<T>::set(const T& rhs) { this->value = std::make_shared<T>(rhs); }

	template<typename T>
	void nabptr_obj<T>::set(const nabptr_obj& rhs) { set(rhs.get()); }

	template<typename T>
	void nabptr_obj<T>::set(nabptr_obj&& rhs) noexcept
	{
		set(std::move(rhs.get()));
		rhs.unload();
	}

	template<typename T>
	nabptr_obj<T>& nabptr_obj<T>::operator=(const nabptr_obj& rhs)
	{
		this->set(rhs);
		return *this;
	}

	template<typename T>
	nabptr_obj<T>& nabptr_obj<T>::operator=(nabptr_obj&& rhs) noexcept
	{
		this->set(std::move(rhs));
		return *this;
	}

	template<typename T>
	void abs_obj<T>::set(const abs_obj& rhs) { set(rhs.get()); }

	template<typename T>
	void abs_obj<T>::set(abs_obj&& rhs) noexcept
	{
		set(std::move(rhs.get()));
		rhs.unload();
	}

	template<typename T>
	abs_obj<T>& abs_obj<T>::operator=(const abs_obj& rhs)
	{
		this->set(rhs);
		return *this;
	}

	template<typename T>
	abs_obj<T>& abs_obj<T>::operator=(abs_obj&& rhs) noexcept
	{
		this->set(std::move(rhs));
		return *this;
	}
}