#ifndef __TTB_TIMETABLE__
#define __TTB_TIMETABLE__

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/date_time.hpp>

#include <map>
#include <string>

#include "Structure.hxx"

namespace TTBParser
{
    struct Timetable
    {
        boost::posix_time::ptime start;
        std::map<std::string, Entry> entries = {};
        std::string route_name;
        std::string name;

        std::pair<std::string, Entry> getEntry(const int i) const
        {
            for(auto& entry : entries)
            {
                if(entry.second.index == i)
                {
                return {entry.first, entry.second};
                }
            }

            return {"",{}};
        }

        std::map<std::string, Entry> getEntries() const {return entries;}

        std::string _send_to_string();
        bool sendToFile(const std::string file_name);
    };
};

#endif