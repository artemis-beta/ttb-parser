#include "Utilities.hxx"

namespace TTBParser
{
	std::string ptimeToString(boost::posix_time::ptime time, const char* form)
	{
		std::stringstream strs;
		boost::posix_time::time_facet* facet = new boost::posix_time::time_facet;
		facet->format(form);
		strs.imbue(std::locale(std::locale::classic(), facet));
		strs << time;
		return strs.str();
	}
    	boost::posix_time::ptime getTimeFromString(const std::string& time_str)
    	{
    	    const std::locale loc = std::locale(std::locale::classic(),
    	                                         new boost::posix_time::time_input_facet("%H:%M"));
    	    std::istringstream is(time_str);
    	    is.imbue(loc);
    	    boost::posix_time::ptime t;
    	    is >> t;

    	    return t;
    	}
};
