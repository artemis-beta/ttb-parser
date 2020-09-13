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

TEST(TestTTBEditting, TestEditNoTimetable)
{
    TTBParser::TTBBuilder* _builder = new TTBParser::TTBBuilder;
    EXPECT_ANY_THROW(_builder->setStartTime("11:23"));
}

TEST(TestTTBEditting, TestInsertEntry)
{
    TTBParser::Timetable* ttb = new TTBParser::Timetable;
    TTBParser::TTBBuilder* _builder = new TTBParser::TTBBuilder(ttb);
    TTBParser::Entry entry_a, entry_b, entry_c, entry_d, entry_e;
    entry_a.headcode = "2V00";
    entry_b.headcode = "2V01";
    entry_c.headcode = "2V02";
    entry_d.headcode = "2V03";
    entry_e.headcode = "2V04";
    _builder->insertEntry(entry_a);
    EXPECT_NO_FATAL_FAILURE(_builder->insertEntry(entry_b, 0));
    EXPECT_NO_FATAL_FAILURE(_builder->insertEntry(entry_c, 0));
    EXPECT_NO_FATAL_FAILURE(_builder->insertEntry(entry_d, 2));
    EXPECT_NO_FATAL_FAILURE(_builder->insertEntry(entry_a, 1));
    EXPECT_NO_FATAL_FAILURE(_builder->insertEntry(entry_e));
    EXPECT_NO_THROW(_builder->getTimetable()->getEntry(entry_a.headcode+"-"+TTBParser::ptimeToString(entry_a.start_time)));
}

TEST(TestTTBEditting, TestCloneEntry)
{
    TTBParser::Timetable* ttb = new TTBParser::Timetable;
    TTBParser::TTBBuilder* _builder = new TTBParser::TTBBuilder(ttb);

    TTBParser::Entry test_entry;
    test_entry.headcode = "2V03";
    test_entry.start_time = TTBParser::getTimeFromString("11:48");
    test_entry.finish_time = TTBParser::getTimeFromString("12:32");

    _builder->insertEntry(test_entry);

    TTBParser::Entry clone = _builder->CloneEntry("2V03", "2V13", 60);

    std::cout << clone.toString() << std::endl;
}

TEST(TestTTBEditting, TestDeleteEntry)
{
    TTBParser::Timetable* ttb = new TTBParser::Timetable;
    TTBParser::TTBBuilder* _builder = new TTBParser::TTBBuilder(ttb);
    TTBParser::Entry entry_a, entry_b, entry_c, entry_d, entry_e;
    entry_a.headcode = "2V00";
    entry_b.headcode = "2V01";
    entry_c.headcode = "2V02";
    entry_d.headcode = "2V03";
    entry_e.headcode = "2V04";
    _builder->insertEntry(entry_a);
    _builder->insertEntry(entry_b);
    _builder->insertEntry(entry_c);
    _builder->insertEntry(entry_d);
    _builder->insertEntry(entry_e);

    _builder->DeleteEntry("2V02");

    EXPECT_EQ(_builder->getTimetable()->getEntry("2V03").index, 2);
}