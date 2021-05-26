#include "Timed.h"

Timed& Timed::operator=(const Timed& rhs)
{
	if (this != &rhs)
	{
		time_handler = rhs.time_handler;
	}
	return *this;
}
