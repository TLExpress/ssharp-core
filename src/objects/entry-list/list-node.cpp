#include "entry-list.h"

namespace ssharp::entry_list
{
	list_node::list_node(const basic_obj& rhs)
	{
		entry = rhs;
	}

	list_node::list_node(basic_obj&& rhs) noexcept
	{
		entry = std::move(rhs);
	}

	list_node::list_node(const list_node& rhs)
	{
		entry = rhs.entry;
	}

	list_node::list_node(list_node&& rhs) noexcept
	{
		entry = std::move(rhs.entry);
	}

	basic_obj& list_node::get()
	{
		return *entry;
	}

	const basic_obj& list_node::get() const
	{
		return *entry;
	}

	void list_node::set(const basic_obj& rhs)
	{
		entry = rhs;
	}

	void list_node::set(basic_obj&& rhs) noexcept
	{
		entry = std::move(rhs);
	}

	basic_obj& list_node::operator=(const basic_obj& rhs)
	{
		set(rhs);
		return *entry;
	}

	basic_obj& list_node::operator=(basic_obj&& rhs)noexcept
	{
		set(std::move(rhs));
		return *entry;
	}

	list_node& list_node::operator=(const list_node& rhs)
	{
		entry = rhs.entry;
		return *this;
	}

	list_node& list_node::operator=(list_node&& rhs) noexcept
	{
		entry = std::move(rhs.entry);
		return *this;
	}

	const basic_obj& list_node::operator*() const
	{
		return get();
	}

	const basic_obj* list_node::operator->() const
	{
		return &get();
	}

	basic_obj& list_node::operator*()
	{
		return get();
	}

	basic_obj* list_node::operator->()
	{
		return &get();
	}
}