#include "gtest/gtest.h"
#include "TTBParser.hxx"
#include "Structure.hxx"
#include "Timetable.hxx"
#include "Editting.hxx"
#include "Utilities.hxx"

#ifndef ROOT_DIR
#error Root Directory Must be Set!
#endif

#include <string>

#include <boost/random.hpp>

TEST(TestTTBEditting, TestInsertEntry)
{
    TTBParser::Timetable* ttb = new TTBParser::Timetable;
    TTBParser::TTBBuilder* _builder = new TTBParser::TTBBuilder(ttb);
    TTBParser::Entry entry_a, entry_b;
    entry_a.headcode = "1V02";
    entry_b.headcode = "2V01"; 
    _builder->insertEntry(entry_a);
    EXPECT_NO_THROW(_builder->insertEntry(entry_b, 0));
    EXPECT_NO_FATAL_FAILURE(_builder->getTimetable()->getEntries()[entry_a.headcode+"-"+TTBParser::ptimeToString(entry_a.start_time)]);
}

TEST(TestTTBEditting, TestNewTimetable)
{
}

TEST(TestTTBEditting, TestCloneEntry)
{
}
