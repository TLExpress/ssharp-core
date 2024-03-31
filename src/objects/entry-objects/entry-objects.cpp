#include "entry-objects.h"

namespace ssharp::entry_objects
{
	basic_obj::basic_obj() {}

	basic_obj::basic_obj(const basic_obj& rhs)
	{
		*this = rhs;
	}

	basic_obj::basic_obj(basic_obj&& rhs)
	{
		*this = std::move(rhs);
	}

	basic_obj::basic_obj(const content_t& source)
	{
		this->source = source;
	}

	basic_obj::basic_obj(content_t&& source)
	{
		this->source = std::move(source);
	}


}