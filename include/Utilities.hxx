#ifndef __UTILITIES_HXX__
#define __UTILITIES_HXX__

#include <string>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace TTBParser
{
	std::string ptimeToString(boost::posix_time::ptime time, const char* form="%H:%M");

        /**
         * @brief parse string to a boost posix_time::ptime object
         * 
         * @param time_str input string to parse
         * @return boost::posix_time::ptime 
         */
        boost::posix_time::ptime getTimeFromString(const std::string& time_str);
};

#endif
