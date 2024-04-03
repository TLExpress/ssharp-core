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
	class bptr_obj
	{
	protected:
		std::shared_ptr<T> value = nullptr;
		virtual bool attend() const;
		virtual bool absent() const;
		virtual void unload();
	public:
		bptr_obj();
		bptr_obj(const T& rhs);
		bptr_obj(T&& rhs);
		bptr_obj(const bptr_obj& rhs);
		bptr_obj(bptr_obj&& rhs);
		~bptr_obj();
		virtual T& get();
		virtual const T& get() const;
		virtual void set(const T& rhs);
		virtual void set(T&& rhs) noexcept;
		virtual void set(const bptr_obj& rhs);
		virtual void set(bptr_obj&& rhs) noexcept;
		virtual bool operator==(const T& rhs) const;
		virtual bool operator==(const bptr_obj& rhs) const;
		virtual T& operator=(const T& rhs);
		virtual T& operator=(T&& rhs) noexcept;
		virtual bptr_obj& operator=(const bptr_obj& rhs);
		virtual bptr_obj& operator=(bptr_obj&& rhs) noexcept;
		virtual const T* operator->() const;
		virtual const T& operator*() const;
		virtual T* operator->();
		virtual T& operator*();
	};

	template<typename T>
	class nabptr_obj : public bptr_obj<T>
	{
	public:
		using bptr_obj<T>::bptr_obj;
		~nabptr_obj() {}
		nabptr_obj(const nabptr_obj& rhs);
		nabptr_obj(nabptr_obj&& rhs) noexcept;
		nabptr_obj() = delete;
		void set(const nabptr_obj& rhs);
		void set(nabptr_obj&& rhs) noexcept;
		nabptr_obj& operator=(const nabptr_obj& rhs);
		nabptr_obj& operator=(nabptr_obj&& rhs) noexcept;
	};

	template<typename T>
	class abs_obj : public bptr_obj<T> 
	{
	public:
		using bptr_obj<T>::bptr_obj;
		~abs_obj(){}
		using bptr_obj<T>::absent;
		using bptr_obj<T>::attend;
		using bptr_obj<T>::unload;
		abs_obj(const abs_obj& rhs);
		abs_obj(abs_obj&& rhs) noexcept;
		void set(const T& rhs) override;

		void set(const abs_obj& rhs);
		void set(abs_obj&& rhs) noexcept;
		abs_obj& operator=(const abs_obj& rhs);
		abs_obj& operator=(abs_obj&& rhs) noexcept;
	};
}

#endif