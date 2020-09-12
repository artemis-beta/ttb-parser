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

        Entry getEntry(std::string key)
        {
            if(entries.find(key) == entries.end())
            {
                throw std::invalid_argument("Entry '"+key+"' does not exist in timetable.");
            }

            return entries[key];
        }

        std::pair<std::string, Entry> getEntry(const int i) const
        {
            for(auto& entry : entries)
            {
                if(entry.second.index == i)
                {
                    return {entry.first, entry.second};
                }
            }

            throw std::invalid_argument("Index '"+std::to_string(i)+"' is not within timetable scope.");
        }

        std::map<std::string, Entry> getEntries() const {return entries;}

        std::string _send_to_string();
        bool sendToFile(const std::string file_name);
    };
};

#endif