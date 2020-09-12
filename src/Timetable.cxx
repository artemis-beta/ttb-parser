
#include "Timetable.hxx"

namespace TTBParser
{
    bool Timetable::sendToFile(const std::string file_name)
    {
        std::ofstream outs(file_name);

        outs << _send_to_string();

        outs.close();

        return true;
    }

    std::string Timetable::_send_to_string()
    {
        std::stringstream strs;
        for(int i{0}; i < entries.size(); ++i)
        {
            if(i > 0)
            {
                    strs << char(0);
            }
            std::pair<std::string, Entry> entry = getEntry(i);
            if(entry.first.substr(0,7) == "Comment")
            {
                strs << entry.second.description;
            }
            else if(entry.first == "Start Time")
            {
                strs << ptimeToString(entry.second.start_time);
            }
            else
            {
                strs << boost::algorithm::join(entry.second.asVector(), ",");
            }
        }

        return strs.str();
    }
};