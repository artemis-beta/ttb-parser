#include "Editting.hxx"

namespace TTBParser
{
    void TTBBuilder::setStartTime(std::string time)
    {
       _timetable->start = getTimeFromString(time);
    }

    void TTBBuilder::insertEntry(const Entry entry, const int pos_index)
    {
        const std::map<std::string, Entry> entries(_timetable->entries);
        std::string key = entry.headcode;
        
        if(entries.size() > 0)
        {
            auto find_srv = entries.find(entry.headcode);

            key += "-";
            key += ptimeToString(entry.start_time);
        }
        
        Entry _entry_insert(entry);
        if(pos_index == -1)
        {
            _entry_insert.index = _timetable->entries.size()-1;
            _timetable->entries[key] = _entry_insert;
        }
        else
        {
            _entry_insert.index = pos_index;
        for(auto& e : _timetable->entries)
        {
            if(e.second.index >= pos_index) ++e.second.index;
        }
        _timetable->entries[key] = _entry_insert;
        }
    }

    Entry Clone(const Entry entry, const std::string headcode, const int n_mins_offset)
    {
      Entry _new = Entry(entry);
      _new.headcode = headcode;
      _new.TemperalOffset(n_mins_offset);
      return _new;
    }
};
