#include "Editting.hxx"

namespace TTBParser
{
    void TTBBuilder::setStartTime(std::string time)
    {
        null_guard();
       _timetable->start = getTimeFromString(time);
    }

    void TTBBuilder::insertEvent(std::string entry_label, std::string start_time, std::string end_time, std::string label, int index)
    {
        null_guard();
        _timetable->getEntry(entry_label)._add_duration_event(getTimeFromString(start_time), getTimeFromString(end_time), label, index);
    }

    void TTBBuilder::insertEvent(std::string entry_label, std::string time, std::string label, int index)
    {
        null_guard();
        _timetable->getEntry(entry_label)._add_single_event(getTimeFromString(time), label, index);
    }

    void TTBBuilder::DeleteEntry(std::string label, int instance_num)
    {
        null_guard();
        auto loc = _timetable->getEntries().find(label);
        int counter = 0;
        if(loc == _timetable->getEntries().end())
        {
            for(auto& entry : _timetable->getEntries())
            {
                if(label+ptimeToString(entry.second.start_time) == entry.first)
                {
                    if(counter != instance_num) ++counter; continue;
                    loc = _timetable->getEntries().find(entry.first);
                }
            }
        }
        
        const int index = loc->second.index;

        for(int i{index+1}; i < _timetable->getEntries().size(); ++i)
        {
            _timetable->getEntry(i).second.index = i-1;
        }

        _timetable->getEntries().erase(loc);
    }

    Entry& TTBBuilder::insertEntry(const Entry entry, const int pos_index)
    {
        const std::map<std::string, Entry> entries(_timetable->entries);
        std::string key = entry.headcode;
        
        if(entries.size() > 0)
        {
            auto find_srv = entries.find(entry.headcode);
            if(find_srv != entries.end())
            {
                key += "-";
                key += ptimeToString(entry.start_time);
            }
        }
        
        Entry _entry_insert(entry);
        if(pos_index == -1)
        {
            _entry_insert.index = _timetable->entries.size();
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
        return _timetable->entries[key];
    }

    Entry& TTBBuilder::CloneEntry(std::string entry_label, const std::string new_headcode, const int n_mins_offset)
    {
      Entry _new = Entry(_timetable->getEntry(entry_label));
      _new.TemperalOffset(n_mins_offset);
      _new.headcode = new_headcode;
      insertEntry(_new);
      return _timetable->getEntry(_new.headcode);
    }

    void TTBBuilder::connectEvents(std::string entry_label_1, std::string entry_label_2)
    {
        Entry& first = _timetable->getEntry(entry_label_1);
        Entry& second = _timetable->getEntry(entry_label_2);

        first.daughter = &second;
        second.parent = &first;

        first.finish_as = TTBParser::FinishNewService;
        second.type = TTBParser::ServiceFromService;
    }
};
