#ifndef __EDITTING_HXX__
#define __EDITTING_HXX__

#include "Structure.hxx"
#include "Utilities.hxx"
#include "Timetable.hxx"

namespace TTBParser
{
    class TTBBuilder
    {
	private:
		Timetable* _timetable = nullptr;
		void null_guard()
		{
			if(_timetable == nullptr)
			{
				throw std::runtime_error("TTBBuilder instance does not point to any Timetable object for editting.");
			}
		}
	public:
		TTBBuilder() {}
		TTBBuilder(Timetable* timetable) : _timetable(timetable) {}
		void setStartTime(std::string time);
		Entry& insertEntry(const Entry entry, const int pos_index=-1);
		Timetable* getTimetable() const {return _timetable;}
		void connectEvents(std::string entry_label_1, std::string entry_label_2);
		void insertEvent(std::string entry_label, std::string start_time, std::string end_time, std::string label, int index=-1);
		void insertEvent(std::string entry_label, std::string time, std::string label, int index=-1);
		void DeleteEntry(std::string label, int instance_num=-1);
		Entry& CloneEntry(std::string entry_label, const std::string new_headcode, const int n_mins_offset);
    };	
};

#endif
