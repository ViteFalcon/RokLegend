#include "RoException.h"

const char* RoException::what() const throw()
{
	return boost::diagnostic_information_what(*this);
}
