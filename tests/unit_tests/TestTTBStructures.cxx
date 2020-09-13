#include "gtest/gtest.h"
#include "Structure.hxx"
#include "Utilities.hxx"


TEST(TestTTBStructures, TestEntryShimmy)
{
    TTBParser::Entry entry;
    entry.headcode = "1V01";
    entry.description = "London Waterloo to Wimbledon";
    entry._add_single_event(TTBParser::getTimeFromString("11:49"), "Waterloo", -1);
    entry._add_duration_event(TTBParser::getTimeFromString("11:53"),
                           TTBParser::getTimeFromString("11:54"),
                           "Vauxhall", -1);
    entry._add_duration_event(TTBParser::getTimeFromString("11:57"), TTBParser::getTimeFromString("11:57"), "Earlsfield", -1);
    entry._add_duration_event(TTBParser::getTimeFromString("11:55"),
                           TTBParser::getTimeFromString("11:55"),
                           "Clapham Junction", 1);
    entry._add_single_event(TTBParser::getTimeFromString("12:03"), "Wimbledon", -1);
    entry.start_time = TTBParser::getTimeFromString("11:48");
    entry.finish_time = TTBParser::getTimeFromString("12:04");

    EXPECT_NO_FATAL_FAILURE(std::cout << entry.toString() << std::endl);
}

TEST(TestTTBStructures, TestTemperalOffset)
{
    TTBParser::Entry entry;
    entry.headcode = "1V01";
    entry.start_time = TTBParser::getTimeFromString("11:48");
    entry.finish_time = TTBParser::getTimeFromString("12:04");

    entry.TemperalOffset(60);

    EXPECT_EQ(TTBParser::ptimeToString(entry.start_time), "12:48");
    EXPECT_EQ(TTBParser::ptimeToString(entry.finish_time), "13:04");
}