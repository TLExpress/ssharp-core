#include "exceptions.h"

namespace ssharp::exceptions
{
	basic_ssharp_exception::basic_ssharp_exception(const string& func, const string& str) : basic_ssharp_exception(str)
	{
		this->func = func;
	}

	string basic_ssharp_exception::getFunc()
	{
		return func;
	}
}