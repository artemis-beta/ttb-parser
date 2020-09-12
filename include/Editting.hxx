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
	public:
		TTBBuilder(Timetable* timetable) : _timetable(timetable) {}
		void setStartTime(std::string time);
		void insertEntry(const Entry entry, const int pos_index=-1); 
    };

    Entry Clone(const Entry entry, const std::string headcode, const int n_mins_offset);
};

#endif
